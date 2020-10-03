// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/SaveGame.h"
#include "IPlatformFilePak.h"
#include "Misc/SecureHash.h"
#include "Math/BigInt.h"
#include "Misc/AES.h"
#include "Templates/UniquePtr.h"
#include "Serialization/LargeMemoryWriter.h"
#include "ProfilingDebugging/DiagnosticTable.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/Base64.h"
#include "Misc/Compression.h"
#include "Features/IModularFeatures.h"
#include "Misc/CoreDelegates.h"
#include "Misc/FileHelper.h"
#include "Misc/ConfigCacheIni.h"
#include "HAL/PlatformFilemanager.h"
#include "Async/ParallelFor.h"
#include "Async/AsyncWork.h"
#include "Modules/ModuleManager.h"
#include "Serialization/Archive.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "AdvancePakType.generated.h"

// #define PAKCOMPRESS_DERIVEDDATA_VER TEXT("9493D2AB515048658AF7BE1342EC21FC")

// #define SEEK_OPT_VERBOSITY Display

/**
 * Wrapper for writing and signing an archive
 */
class FSignedArchiveWriter : public FArchive
{
	/** Buffer to sign */
	TArray<uint8> Buffer;
	/** Buffer writer */
	FMemoryWriter BufferArchive;
	/** The actual pak archive */
	FArchive& PakWriter;
	/** The filename of the signature file that accompanies the pak */
	FString PakSignaturesFilename;
	/** Size of the archive on disk (including signatures) */
	int64 SizeOnDisk;
	/** Data size (excluding signatures) */
	int64 PakSize;
	/** Signing key */
	const FRSAKeyHandle SigningKey;
	/** Hashes */
	TArray<TPakChunkHash> ChunkHashes;

	/**
	 * Serializes and signs a buffer
	 */
	void SerializeBufferAndSign();

public:

	FSignedArchiveWriter(FArchive& InPak, const FString& InPakFilename, const FRSAKeyHandle InSigningKey);
	virtual ~FSignedArchiveWriter();

	// FArchive interface
	virtual bool Close() override;
	virtual void Serialize(void* Data, int64 Length) override;
	virtual int64 Tell() override;
	virtual int64 TotalSize() override;
	virtual void Seek(int64 InPos) override;
};


/**
 *
 */

struct FNamedAESKey
{
	FString Name;
	FGuid Guid;
	FAES::FAESKey Key;

	bool IsValid() const
	{
		return Key.IsValid();
	}
};

struct FKeyChain
{
	FRSAKeyHandle SigningKey = InvalidRSAKeyHandle;
	TMap<FGuid, FNamedAESKey> EncryptionKeys;
	const FNamedAESKey* MasterEncryptionKey = nullptr;
};


class FThreadLocalScratchSpace
{
public:
#define MAX_SCRATCHSPACE_THREADS 64
#define ALLOC_BUFFER_SIZE (256 * 1024 * 1024)
	static FThreadLocalScratchSpace& Get()
	{
		static FThreadLocalScratchSpace Result;
		return Result;
	}
	struct FScratchSpace
	{
	public:
		FScratchSpace()
		{
			Size = 0;
			Buffer = nullptr;
		}
		int64 Size;
		uint8* Buffer;
		TArray<uint8> Array;

		void Embiggen(int64 NewSize)
		{
			if (NewSize > Size)
			{
				Buffer = (uint8*)FMemory::Realloc(Buffer, NewSize);
				Size = NewSize;
			}
		}
		void CleanUp()
		{
			FMemory::Free(Buffer);
			Buffer = nullptr;
			Size = 0;
			Array.Empty();
		}
	};

	void GetScratchSpace(FScratchSpace*& ScratchSpace)
	{
		ScratchSpace = (FScratchSpace*)FPlatformTLS::GetTlsValue(TLSSlot);
		if (ScratchSpace == nullptr)
		{
			int32 MyScratchSpace = FPlatformAtomics::InterlockedIncrement(&ThreadCounter);
			check(MyScratchSpace < (MAX_SCRATCHSPACE_THREADS - 1));
			ScratchSpace = &ScratchSpaces[MyScratchSpace];
			FPlatformTLS::SetTlsValue(TLSSlot, (void*)ScratchSpace);
		}
	}

	void CleanUp()
	{
		for (int32 I = 0; I < MAX_SCRATCHSPACE_THREADS; ++I)
		{
			ScratchSpaces[I].CleanUp();
		}
		ThreadCounter = 0;
	}
private:

	FThreadLocalScratchSpace()
	{
		TLSSlot = FPlatformTLS::AllocTlsSlot();
		ThreadCounter = 0;
	}

	uint32 TLSSlot;
	volatile int32 ThreadCounter;
	FScratchSpace ScratchSpaces[MAX_SCRATCHSPACE_THREADS];
};

class FMemoryCompressor;

/**
* AsyncTask for FMemoryCompressor
* Compress a memory block asynchronously
 */
class FBlockCompressTask : public FNonAbandonableTask
{
public:
	friend class FAsyncTask<FBlockCompressTask>;
	friend class FMemoryCompressor;
	FBlockCompressTask(void* InUncompressedBuffer, int32 InUncompressedSize, FName InFormat, int32 InBlockSize) :
		UncompressedBuffer(InUncompressedBuffer),
		UncompressedSize(InUncompressedSize),
		Format(InFormat),
		BlockSize(InBlockSize),
		Result(false)
	{
		// Store buffer size.
		CompressedSize = FCompression::CompressMemoryBound(Format, BlockSize);
		CompressedBuffer = FMemory::Malloc(CompressedSize);
	}

	~FBlockCompressTask()
	{
		FMemory::Free(CompressedBuffer);
	}

	/** Do compress */
	void DoWork()
	{
		// Compress memory block.
		// Actual size will be stored to CompressedSize.
		Result = FCompression::CompressMemory(Format, CompressedBuffer, CompressedSize, UncompressedBuffer, UncompressedSize);
	}


	FORCEINLINE TStatId GetStatId() const { RETURN_QUICK_DECLARE_CYCLE_STAT(ExampleAsyncTask, STATGROUP_ThreadPoolAsyncTasks); }

private:
	// Source buffer
	void* UncompressedBuffer;
	int32 UncompressedSize;

	// Compress parameters
	FName Format;
	int32 BlockSize;
	int32 BitWindow;

	// Compressed result
	void* CompressedBuffer;
	int32 CompressedSize;
	bool Result;
};

/**
* asynchronous memory compressor
*/
class FMemoryCompressor
{
public:
	/** Divide into blocks and start compress asynchronously */
	FMemoryCompressor(uint8* UncompressedBuffer, int32 UncompressedSize, FName Format, int32 CompressionBlockSize) :
		Index(0)
	{
		// Divide into blocks and start compression async tasks.
		// These blocks must be as same as followed CompressMemory callings.
		int64 UncompressedBytes = 0;
		while (UncompressedSize)
		{
			int32 BlockSize = (int32)FMath::Min<int64>(UncompressedSize, CompressionBlockSize);
			auto* AsyncTask = new FAsyncTask<FBlockCompressTask>(UncompressedBuffer + UncompressedBytes, BlockSize, Format, BlockSize);
			AsyncTask->StartBackgroundTask();
			BlockCompressAsyncTasks.Add(AsyncTask);
			UncompressedSize -= BlockSize;
			UncompressedBytes += BlockSize;
		}
	}

	~FMemoryCompressor()
	{
		for (auto* AsyncTask : BlockCompressAsyncTasks)
		{
			if (!AsyncTask->Cancel())
			{
				AsyncTask->EnsureCompletion();
			}
			delete AsyncTask;
		}
	}


	/** Fetch compressed result. Returns true and store CompressedSize if succeeded */
	bool CompressMemory(FName Format, void* CompressedBuffer, int32& CompressedSize, const void* UncompressedBuffer, int32 UncompressedSize)
	{
		// Fetch compressed result from task.
		// We assume this is called only once, same order, same parameters for
		// each task.
		auto* AsyncTask = BlockCompressAsyncTasks[Index++];
		AsyncTask->EnsureCompletion();

		FBlockCompressTask& Task = AsyncTask->GetTask();
		check(Task.Format == Format);
		check(Task.UncompressedBuffer == UncompressedBuffer);
		check(Task.UncompressedSize == UncompressedSize);
		check(CompressedSize >= Task.CompressedSize);
		if (!Task.Result)
		{
			return false;
		}
		FMemory::Memcpy(CompressedBuffer, Task.CompressedBuffer, Task.CompressedSize);
		CompressedSize = Task.CompressedSize;

		return true;
	}

private:
	TArray<FAsyncTask<FBlockCompressTask>*> BlockCompressAsyncTasks;

	// Fetched task index
	int32 Index;
};

/**
* Defines the order mapping for files within a pak
 */
class FPakOrderMap
{
public:
	FPakOrderMap()
		: MaxPrimaryOrderIndex(MAX_uint64)
	{}

	int32 Num() const
	{
		return OrderMap.Num();
	}

	void Add(const FString& Filename, uint64 Index)
	{
		OrderMap.Add(Filename, Index);
	}

	bool ProcessOrderFile(const TCHAR* ResponseFile, bool bSecondaryOrderFile = false)
	{
		int32 OrderOffset = 0;
		if (bSecondaryOrderFile)
		{
			OrderOffset = Num();
			MaxPrimaryOrderIndex = OrderOffset;
		}
		// List of all items to add to pak file
		FString Text;
		UE_LOG(LogPakFile, Display, TEXT("Loading pak order file %s..."), ResponseFile);
		if (FFileHelper::LoadFileToString(Text, ResponseFile))
		{
			// Read all lines
			TArray<FString> Lines;
			Text.ParseIntoArray(Lines, TEXT("\n"), true);
			for (int32 EntryIndex = 0; EntryIndex < Lines.Num(); EntryIndex++)
			{
				Lines[EntryIndex].ReplaceInline(TEXT("\r"), TEXT(""));
				Lines[EntryIndex].ReplaceInline(TEXT("\n"), TEXT(""));
				int32 OpenOrderNumber = EntryIndex;
				if (Lines[EntryIndex].FindLastChar('"', OpenOrderNumber))
				{
					FString ReadNum = Lines[EntryIndex].RightChop(OpenOrderNumber + 1);
					Lines[EntryIndex] = Lines[EntryIndex].Left(OpenOrderNumber + 1);
					ReadNum.TrimStartInline();
					if (ReadNum.IsNumeric())
					{
						OpenOrderNumber = FCString::Atoi(*ReadNum);
					}
				}
				Lines[EntryIndex] = Lines[EntryIndex].TrimQuotes();
				FString Path = FString::Printf(TEXT("%s"), *Lines[EntryIndex]);
				FPaths::NormalizeFilename(Path);
				Path = Path.ToLower();
				if (bSecondaryOrderFile && OrderMap.Contains(Path))
				{
					continue;
				}
				OrderMap.Add(Path, OpenOrderNumber + OrderOffset);
			}
			UE_LOG(LogPakFile, Display, TEXT("Finished loading pak order file %s."), ResponseFile);
			return true;
		}
		else
		{
			UE_LOG(LogPakFile, Error, TEXT("Unable to load pak order file %s."), ResponseFile);
			return false;
		}
	}

	uint64 GetFileOrder(const FString& Path, bool bAllowUexpUBulkFallback, bool* OutIsPrimary = nullptr) const
	{
		FString RegionStr;
		FString NewPath = RemapLocalizationPathIfNeeded(Path.ToLower(), RegionStr);
		const uint64* FoundOrder = OrderMap.Find(NewPath);
		uint64 ReturnOrder = MAX_uint64;
		if (FoundOrder != nullptr)
		{
			ReturnOrder = *FoundOrder;
			if (OutIsPrimary)
			{
				*OutIsPrimary = (ReturnOrder < MaxPrimaryOrderIndex);
			}
		}
		else if (bAllowUexpUBulkFallback)
		{
			// if this is a cook order or an old order it will not have uexp files in it, so we put those in the same relative order after all of the normal files, but before any ubulk files
			if (Path.EndsWith(TEXT("uexp")) || Path.EndsWith(TEXT("ubulk")))
			{
				uint64 CounterpartOrder = GetFileOrder(FPaths::GetBaseFilename(Path, false) + TEXT(".uasset"), false);
				if (CounterpartOrder == MAX_uint64)
				{
					CounterpartOrder = GetFileOrder(FPaths::GetBaseFilename(Path, false) + TEXT(".umap"), false);
				}
				if (CounterpartOrder != MAX_uint64)
				{
					if (Path.EndsWith(TEXT("uexp")))
					{
						ReturnOrder = CounterpartOrder | (1 << 29);
					}
					else
					{
						ReturnOrder = CounterpartOrder | (1 << 30);
					}
				}
			}
		}
		return ReturnOrder;
	}

	void WriteOpenOrder(FArchive* Ar)
	{
		for (const auto& It : OrderMap)
		{
			Ar->Logf(TEXT("\"%s\" %d"), *It.Key, It.Value);
		}
	}

private:
	FString RemapLocalizationPathIfNeeded(const FString& PathLower, FString& OutRegion) const
	{
		static const TCHAR* L10NPrefix = (const TCHAR*)TEXT("/content/l10n/");
		static const int32 L10NPrefixLength = FCString::Strlen(L10NPrefix);
		int32 FoundIndex = PathLower.Find(L10NPrefix, ESearchCase::CaseSensitive);
		if (FoundIndex > 0)
		{
			// Validate the content index is the first one
			int32 ContentIndex = PathLower.Find(TEXT("/content/"), ESearchCase::CaseSensitive);
			if (ContentIndex == FoundIndex)
			{
				int32 EndL10NOffset = ContentIndex + L10NPrefixLength;
				int32 NextSlashIndex = PathLower.Find(TEXT("/"), ESearchCase::CaseSensitive, ESearchDir::FromStart, EndL10NOffset);
				int32 RegionLength = NextSlashIndex - EndL10NOffset;
				if (RegionLength >= 2)
				{
					FString NonLocalizedPath = PathLower.Mid(0, ContentIndex) + TEXT("/content") + PathLower.Mid(NextSlashIndex);
					OutRegion = PathLower.Mid(EndL10NOffset, RegionLength);
					return NonLocalizedPath;
				}
			}
		}
		return PathLower;
	}

	TMap<FString, uint64> OrderMap;
	uint64 MaxPrimaryOrderIndex;
};


enum class ESeekOptMode : uint8
{
	None = 0,
	OnePass = 1,
	Incremental = 2,
	Incremental_OnlyPrimaryOrder = 3,
	Incremental_PrimaryThenSecondary = 4,
	COUNT
};

struct FPatchSeekOptParams
{
	FPatchSeekOptParams()
		: MaxGapSize(0)
		, MaxInflationPercent(0.0f)
		, Mode(ESeekOptMode::None)
		, MaxAdjacentOrderDiff(128)
	{}
	int64 MaxGapSize;
	float MaxInflationPercent; // For Incremental_ modes only
	ESeekOptMode Mode;
	int32 MaxAdjacentOrderDiff;
};

struct FPakCommandLineParameters
{
	FPakCommandLineParameters()
		: CompressionBlockSize(64 * 1024)
		, FileSystemBlockSize(0)
		, PatchFilePadAlign(0)
		, AlignForMemoryMapping(0)
		, GeneratePatch(false)
		, EncryptIndex(false)
		, UseCustomCompressor(false)
		, bSign(false)
		, bPatchCompatibilityMode421(false)
		, bFallbackOrderForNonUassetFiles(false)
		, bAsyncCompression(false)
		, bAlignFilesLargerThanBlock(false)
	{
	}

	TArray<FName> CompressionFormats;
	FPatchSeekOptParams SeekOptParams;
	int32  CompressionBlockSize;
	int64  FileSystemBlockSize;
	int64  PatchFilePadAlign;
	int64  AlignForMemoryMapping;
	bool   GeneratePatch;
	FString SourcePatchPakFilename;
	FString SourcePatchDiffDirectory;
	FString InputFinalPakFilename; // This is the resulting pak file we want to end up with after we generate the pak patch.  This is used instead of passing in the raw content.
	FString ChangedFilesOutputFilename;
	bool EncryptIndex;
	bool UseCustomCompressor;
	FGuid EncryptionKeyGuid;
	bool bSign;
	bool bPatchCompatibilityMode421;
	bool bFallbackOrderForNonUassetFiles;
	bool bAsyncCompression;
	bool bAlignFilesLargerThanBlock;	// Align files that are larger than block size
};

struct FPakEntryPair
{
	FString Filename;
	FPakEntry Info;
};

struct FPakInputPair
{
	FString Source;
	FString Dest;
	uint64 SuggestedOrder;
	bool bNeedsCompression;
	bool bNeedEncryption;
	bool bIsDeleteRecord;	// This is used for patch PAKs when a file is deleted from one patch to the next
	bool bIsInPrimaryOrder;
	TArray<uint8> Bytes;

	FPakInputPair()
		: SuggestedOrder(MAX_uint64)
		, bNeedsCompression(false)
		, bNeedEncryption(false)
		, bIsDeleteRecord(false)
		, bIsInPrimaryOrder(false)
	{
	}

	FPakInputPair(const FString& InSource, const FString& InDest)
		: Source(InSource)
		, Dest(InDest)
		, bNeedsCompression(false)
		, bNeedEncryption(false)
		, bIsDeleteRecord(false)
	{

	}

	FORCEINLINE bool operator==(const FPakInputPair& Other) const
	{
		return Source == Other.Source;
	}
};


struct FFileInfo
{
	uint64 FileSize;
	int32 PatchIndex;
	bool bIsDeleteRecord;
	bool bForceInclude;
	uint8 Hash[16];
};


struct FCompressedFileBuffer
{
	FCompressedFileBuffer()
		: OriginalSize(0)
		, TotalCompressedSize(0)
		, FileCompressionBlockSize(0)
		, CompressedBufferSize(0)
	{

	}

	void Reinitialize(FArchive* File, FName CompressionMethod, int64 CompressionBlockSize)
	{
		OriginalSize = File->TotalSize();
		TotalCompressedSize = 0;
		FileCompressionBlockSize = 0;
		FileCompressionMethod = CompressionMethod;
		CompressedBlocks.Reset();
		CompressedBlocks.AddUninitialized((OriginalSize + CompressionBlockSize - 1) / CompressionBlockSize);
	}

	void Empty()
	{
		OriginalSize = 0;
		TotalCompressedSize = 0;
		FileCompressionBlockSize = 0;
		FileCompressionMethod = NAME_None;
		CompressedBuffer = nullptr;
		CompressedBufferSize = 0;
		CompressedBlocks.Empty();
	}

	void EnsureBufferSpace(int64 RequiredSpace)
	{
		if (RequiredSpace > CompressedBufferSize)
		{
			TUniquePtr<uint8[]> NewCompressedBuffer = MakeUnique<uint8[]>(RequiredSpace);
			FMemory::Memcpy(NewCompressedBuffer.Get(), CompressedBuffer.Get(), CompressedBufferSize);
			CompressedBuffer = MoveTemp(NewCompressedBuffer);
			CompressedBufferSize = RequiredSpace;
		}
	}

	bool CompressFileToWorkingBuffer(const FPakInputPair& InFile, FName CompressionMethod, const int32 CompressionBlockSize);

	void SerializeDDCData(FArchive& Ar)
	{
		Ar << OriginalSize;
		Ar << TotalCompressedSize;
		Ar << FileCompressionBlockSize;
		Ar << FileCompressionMethod;
		Ar << CompressedBlocks;
		if (Ar.IsLoading())
		{
			EnsureBufferSpace(TotalCompressedSize);
		}
		Ar.Serialize(CompressedBuffer.Get(), TotalCompressedSize);
	}

	int64 GetSerializedSizeEstimate() const
	{
		int64 Size = 0;
		Size += sizeof(*this);
		Size += CompressedBlocks.Num() * sizeof(FPakCompressedBlock);
		Size += CompressedBufferSize;
		return Size;
	}

	FString GetDDCKeyString(const uint8* UncompressedFile, const int64& UncompressedFileSize, FName CompressionFormat, const int64& BlockSize);

	int64				OriginalSize;
	int64				TotalCompressedSize;
	int32				FileCompressionBlockSize;
	FName				FileCompressionMethod;
	TArray<FPakCompressedBlock>	CompressedBlocks;
	int64				CompressedBufferSize;
	TUniquePtr<uint8[]>		CompressedBuffer;
};


UENUM(BlueprintType)
enum class EAdvancePakProcess : uint8
{
	None = 0,
	LoadPlatform,
	FirstCheck,
	LoadConfig,
	SecondCheck,
	LoadPatchs,
	MergeCheck,
	MergePatchs,
	UpdatePatchs,
	UpdateSucceed
};


class FAdvancePakRecorder
{
public:

	FAdvancePakRecorder();

	bool HasProgressChanged() const;

	bool HasSpeedChanged() const;

	void PeddingReceived(int32 BytesReceived);

	float GetDownLoadSpeed();

	void ExitRecorder();

	void UpdateRecorder(float DeltaTime);

	float GetDownLoadProgress();

	void EnterRecorder(uint32 InTotalSize);

	float GetPredictTime();

protected:

	float LoadSpeed;

	bool IsProgressChanged;

	float PeddingTime;

	uint32 PeddingSize;

	uint32 CurrentSize;

	float LastTime;

	bool IsDownLoading;

	uint32 TotalSize;

	bool IsSpeedChanged;

};

class FAdvancePakPatchInfo
{
public:
	TArray<FString> DeleteResources;
	TArray<FString> AddResources;
	TArray<FString> UpdateResources;
};

class FAdvancePakPatchProp
{
public:
	uint64 Size;
	FString Hash;
};

class FAdvancePakVersionConfig
{
public:
	FString CmdLine;
	TArray<FString> KeepPatchs;
	TArray<FString> AddPatchs;
	TArray<FString> DeletePatchs;
	TMap<FString, TSharedPtr<FAdvancePakPatchInfo>> UpdatePatchs;
	TMap<FString, TSharedPtr<FAdvancePakPatchProp>> PatchProps;
};

USTRUCT()
struct FAdvancePakConfigMark
{
	GENERATED_BODY()

public:

	FAdvancePakConfigMark() {}

	FAdvancePakConfigMark(const FString& InFormer, const FString& InFuture) : Former(InFormer), Future(InFuture) {}

	UPROPERTY()
		FString Former;

	UPROPERTY()
		FString Future;

	friend inline bool operator==(const FAdvancePakConfigMark& A, const FAdvancePakConfigMark& B)
	{
		return A.Former.Equals(B.Former) && A.Future.Equals(B.Future);
	}

	friend inline uint32 GetTypeHash(const FAdvancePakConfigMark& Key)
	{
		uint32 Hash = 0;
		Hash = HashCombine(Hash, GetTypeHash(Key.Former));
		Hash = HashCombine(Hash, GetTypeHash(Key.Future));
		return Hash;
	}

	FString GetRequestURL() const;
};

USTRUCT()
struct FAdvancePakMergeInfo
{
	GENERATED_BODY()

public:

	// Patch Name
	UPROPERTY()
		FString Name;
	UPROPERTY()
		FString PreFormer;
	UPROPERTY()
		FString PreFuture;
	UPROPERTY()
		FString NexFormer;
	UPROPERTY()
		FString NexFuture;
	UPROPERTY()
		uint64 PreSize;
	UPROPERTY()
		FString PreHash;
	UPROPERTY()
		uint64 NexSize;
	UPROPERTY()
		FString NexHash;
	UPROPERTY()
		FString Latest;

	TSharedPtr<FAdvancePakPatchInfo> PatchInfo;

	FString GetFormorPath();
	FString GetFuturePath();
	FString GetMergePath(bool IsSig = false);
};

USTRUCT()
struct FAdvancePakFileInfo
{
	GENERATED_BODY()

public:

	FAdvancePakFileInfo();

	FAdvancePakFileInfo(const FString& InName, const uint64& InSize, const FString& InHash, const FString& InFormer, const FString& InFuture);

	bool IsPak() const;
	bool IsEqual(const FAdvancePakFileInfo& Other) const;
	FString GetFormatName() const;
	FString GetRequestURL() const;
	FString GetPatchName() const;

	UPROPERTY()
		FString Name;

	UPROPERTY()
		uint64 Size;

	UPROPERTY()
		FString Hash;

	UPROPERTY()
		FString Former;

	UPROPERTY()
		FString Future;
};

UCLASS()
class ADVANCEPAK_API UAdvancePakStateSave : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FString SavedVersion;

	UPROPERTY()
		FString MergeVersion;

	UPROPERTY()
		TMap<FString, FAdvancePakFileInfo> MergeFiles;

	UPROPERTY()
		TMap<FString, FAdvancePakFileInfo> SavedFiles;

	UPROPERTY()
		FAdvancePakFileInfo LoadingInfo;

	UPROPERTY()
		FAdvancePakMergeInfo MergeingInfo;

};

UCLASS()
class ADVANCEPAK_API UAdvancePakConfigSave : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
		bool IsWithSig;

	UPROPERTY()
		FString Platform;

	UPROPERTY()
		FString Version;

	UPROPERTY()
		TArray<uint8> PrivateExponent;

	UPROPERTY()
		TMap<FAdvancePakConfigMark, FString> ConfigMarks;

};

UCLASS()
class ADVANCEPAK_API UAdvancePakType : public UObject
{
	GENERATED_BODY()

public:

};