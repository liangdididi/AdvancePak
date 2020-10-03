// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "AdvancePakType.h"
#include "AdvancePakLibrary.h"
#include "HAL/FileManager.h"

FSignedArchiveWriter::FSignedArchiveWriter(FArchive& InPak, const FString& InPakFilename, const FRSAKeyHandle InSigningKey)
	: BufferArchive(Buffer)
	, PakWriter(InPak)
	, PakSignaturesFilename(FPaths::ChangeExtension(InPakFilename, TEXT("sig")))
	, SizeOnDisk(0)
	, PakSize(0)
	, SigningKey(InSigningKey)
{
	Buffer.Reserve(FPakInfo::MaxChunkDataSize);
}

FSignedArchiveWriter::~FSignedArchiveWriter()
{
	if (BufferArchive.Tell() > 0)
	{
		SerializeBufferAndSign();
	}
	delete& PakWriter;
}

void FSignedArchiveWriter::SerializeBufferAndSign()
{
	// Compute a hash for this buffer data
	ChunkHashes.Add(ComputePakChunkHash(&Buffer[0], Buffer.Num()));

	// Flush the buffer
	PakWriter.Serialize(&Buffer[0], Buffer.Num());
	BufferArchive.Seek(0);
	Buffer.Empty(FPakInfo::MaxChunkDataSize);
}

bool FSignedArchiveWriter::Close()
{
	if (BufferArchive.Tell() > 0)
	{
		SerializeBufferAndSign();
	}

	FArchive* SignatureWriter = IFileManager::Get().CreateFileWriter(*PakSignaturesFilename);
	FPakSignatureFile SignatureFile;
	SignatureFile.SetChunkHashesAndSign(ChunkHashes, SigningKey);
	SignatureFile.Serialize(*SignatureWriter);
	delete SignatureWriter;

	return FArchive::Close();
}

void FSignedArchiveWriter::Serialize(void* Data, int64 Length)
{
	// Serialize data to a buffer. When the max buffer size is reached, the buffer is signed and
	// serialized to disk with its signature
	uint8* DataToWrite = (uint8*)Data;
	int64 RemainingSize = Length;
	while (RemainingSize > 0)
	{
		int64 BufferPos = BufferArchive.Tell();
		int64 SizeToWrite = RemainingSize;
		if (BufferPos + SizeToWrite > FPakInfo::MaxChunkDataSize)
		{
			SizeToWrite = FPakInfo::MaxChunkDataSize - BufferPos;
		}

		BufferArchive.Serialize(DataToWrite, SizeToWrite);
		if (BufferArchive.Tell() == FPakInfo::MaxChunkDataSize)
		{
			SerializeBufferAndSign();
		}

		SizeOnDisk += SizeToWrite;
		PakSize += SizeToWrite;

		RemainingSize -= SizeToWrite;
		DataToWrite += SizeToWrite;
	}
}

int64 FSignedArchiveWriter::Tell()
{
	return PakSize;
}

int64 FSignedArchiveWriter::TotalSize()
{
	return PakSize;
}

void FSignedArchiveWriter::Seek(int64 InPos)
{
	UE_LOG(LogPakFile, Fatal, TEXT("Seek is not supported in FSignedArchiveWriter."));
}

bool FCompressedFileBuffer::CompressFileToWorkingBuffer(const FPakInputPair& InFile, FName CompressionMethod, const int32 CompressionBlockSize)
{
	FThreadLocalScratchSpace::FScratchSpace* ScratchSpace = nullptr;
	FThreadLocalScratchSpace::Get().GetScratchSpace(ScratchSpace);
	check(ScratchSpace);

	//TUniquePtr<FArchive> FileHandle(IFileManager::Get().CreateFileReader(*InFile.Source));
	TUniquePtr<FArchive> FileHandle = MakeUnique<FMemoryReader>(InFile.Bytes);
	if (!FileHandle)
	{
		TotalCompressedSize = 0;
		return false;
	}

	Reinitialize(FileHandle.Get(), CompressionMethod, CompressionBlockSize);
	const int64 FileSize = OriginalSize;
	const int64 PaddedEncryptedFileSize = Align(FileSize, FAES::AESBlockSize);
	check(PaddedEncryptedFileSize >= FileSize);
	if (ScratchSpace->Size < PaddedEncryptedFileSize)
	{
		ScratchSpace->Embiggen(PaddedEncryptedFileSize);
	}


	uint8*& InOutPersistentBuffer = ScratchSpace->Buffer;
	int64& InOutBufferSize = ScratchSpace->Size;

	// Load to buffer
	FileHandle->Serialize(InOutPersistentBuffer, FileSize);


	FString DDCKey;
	TArray<uint8>& GetData = ScratchSpace->Array;

	{
		// Start parallel compress
		FMemoryCompressor MemoryCompressor(InOutPersistentBuffer, FileSize, CompressionMethod, CompressionBlockSize);

		// Build buffers for working
		int64 UncompressedSize = FileSize;
		int32 CompressionBufferSize = Align(FCompression::CompressMemoryBound(CompressionMethod, CompressionBlockSize, COMPRESS_NoFlags), FAES::AESBlockSize);
		EnsureBufferSpace(Align(FCompression::CompressMemoryBound(CompressionMethod, FileSize, COMPRESS_NoFlags), FAES::AESBlockSize));

		TotalCompressedSize = 0;
		int64 UncompressedBytes = 0;
		int32 CurrentBlock = 0;
		while (UncompressedSize)
		{
			int32 BlockSize = (int32)FMath::Min<int64>(UncompressedSize, CompressionBlockSize);
			int32 MaxCompressedBlockSize = FCompression::CompressMemoryBound(CompressionMethod, BlockSize, COMPRESS_NoFlags);
			int32 CompressedBlockSize = FMath::Max<int32>(CompressionBufferSize, MaxCompressedBlockSize);
			FileCompressionBlockSize = FMath::Max<uint32>(BlockSize, FileCompressionBlockSize);
			EnsureBufferSpace(Align(TotalCompressedSize + CompressedBlockSize, FAES::AESBlockSize));
			if (!MemoryCompressor.CompressMemory(CompressionMethod, CompressedBuffer.Get() + TotalCompressedSize, CompressedBlockSize, InOutPersistentBuffer + UncompressedBytes, BlockSize))
			{
				return false;
			}
			UncompressedSize -= BlockSize;
			UncompressedBytes += BlockSize;

			CompressedBlocks[CurrentBlock].CompressedStart = TotalCompressedSize;
			CompressedBlocks[CurrentBlock].CompressedEnd = TotalCompressedSize + CompressedBlockSize;
			++CurrentBlock;

			TotalCompressedSize += CompressedBlockSize;

			if (InFile.bNeedEncryption)
			{
				int32 EncryptionBlockPadding = Align(TotalCompressedSize, FAES::AESBlockSize);
				for (int64 FillIndex = TotalCompressedSize; FillIndex < EncryptionBlockPadding; ++FillIndex)
				{
					// Fill the trailing buffer with bytes from file. Note that this is now from a fixed location
					// rather than a random one so that we produce deterministic results
					CompressedBuffer.Get()[FillIndex] = CompressedBuffer.Get()[FillIndex % TotalCompressedSize];
				}
				TotalCompressedSize += EncryptionBlockPadding - TotalCompressedSize;
			}
		}

	}


	return true;
}

FString FCompressedFileBuffer::GetDDCKeyString(const uint8* UncompressedFile, const int64& UncompressedFileSize, FName CompressionFormat, const int64& BlockSize)
{
	FString KeyString;

	KeyString += FString::Printf(TEXT("_F:%s_C:%s_B:%d_"), *CompressionFormat.ToString(), *FCompression::GetCompressorDDCSuffix(CompressionFormat), BlockSize);

	FSHA1 HashState;
	HashState.Update(UncompressedFile, UncompressedFileSize);
	HashState.Final();
	FSHAHash FinalHash;
	HashState.GetHash(FinalHash.Hash);
	KeyString += FinalHash.ToString();

	return UAdvancePakLibrary::SanitizeCacheKey(TEXT("PAKCOMPRESS_"), TEXT("9493D2AB515048658AF7BE1342EC21FC"), *KeyString);
}

FAdvancePakRecorder::FAdvancePakRecorder()
	: LoadSpeed(0.f)
	, IsProgressChanged(false)
	, PeddingTime(0.f)
	, PeddingSize(0)
	, CurrentSize(0)
	, LastTime(0.f)
	, IsDownLoading(false)
	, TotalSize(0)
	, IsSpeedChanged(false)
{

}

void FAdvancePakRecorder::EnterRecorder(uint32 InTotalSize)
{
	LoadSpeed = 0.f;
	IsProgressChanged = false;
	PeddingSize = 0;
	IsSpeedChanged = false;
	CurrentSize = 0;
	LastTime = 0.f;
	PeddingTime = 0.f;
	IsDownLoading = true;
	TotalSize = InTotalSize;
}

float FAdvancePakRecorder::GetDownLoadProgress()
{
	if (!IsDownLoading)
		return 0.f;

	IsProgressChanged = false;
	return (float)((double)(CurrentSize + PeddingSize) / (double)TotalSize);
}

void FAdvancePakRecorder::ExitRecorder()
{
	PeddingTime = 0.f;
	LoadSpeed = 0.f;
	IsProgressChanged = false;
	PeddingSize = 0;
	IsSpeedChanged = false;
	IsDownLoading = false;
	TotalSize = 0;
	CurrentSize = 0;
	LastTime = 0.f;
}

void FAdvancePakRecorder::PeddingReceived(int32 BytesReceived)
{
	PeddingSize += BytesReceived;
	IsProgressChanged = true;
}

float FAdvancePakRecorder::GetDownLoadSpeed()
{
	if (!IsDownLoading)
		return 0.f;

	IsSpeedChanged = false;
	return LoadSpeed;
}

float FAdvancePakRecorder::GetPredictTime()
{
	if (!IsDownLoading)
		return 0.f;

	float AverageSpeed = (double)(CurrentSize + PeddingSize) / (LastTime + PeddingTime);
	return (float)((double)(TotalSize - CurrentSize - PeddingSize) / AverageSpeed);
}

bool FAdvancePakRecorder::HasProgressChanged() const
{
	if (!IsDownLoading)
		return false;

	return IsProgressChanged;
}

void FAdvancePakRecorder::UpdateRecorder(float DeltaTime)
{
	if (!IsDownLoading)
		return;

	PeddingTime += DeltaTime;
	if (PeddingTime > 1.f)
	{
		LastTime += PeddingTime;
		LoadSpeed = (float)FUnitConversion::Convert((double)(PeddingSize / PeddingTime), EUnit::Bytes, EUnit::Kilobytes);
		PeddingTime = 0.f;
		IsSpeedChanged = true;

		CurrentSize += PeddingSize;
		PeddingSize = 0;
	}
}

bool FAdvancePakRecorder::HasSpeedChanged() const
{
	if (!IsDownLoading)
		return false;

	return IsSpeedChanged;
}

FString FAdvancePakMergeInfo::GetFormorPath()
{
	if(PreFormer.IsEmpty() || PreFuture.IsEmpty())
		return FString();
	return UAdvancePakLibrary::StorePaksPath / (FString::Printf(TEXT("%s_%s_%s"), *Name, *PreFormer, *PreFuture) + UAdvancePakLibrary::PakSuffixName);
}

FString FAdvancePakMergeInfo::GetFuturePath()
{
	if (NexFormer.IsEmpty() || NexFuture.IsEmpty())
		return FString();
	return UAdvancePakLibrary::StorePaksPath / (FString::Printf(TEXT("%s_%s_%s"), *Name, *NexFormer, *NexFuture) + UAdvancePakLibrary::PakSuffixName);
}

FString FAdvancePakMergeInfo::GetMergePath(bool IsSig /*= false*/)
{
	return UAdvancePakLibrary::MergePaksPath / ((FString::Printf(TEXT("%s_%s_%s"), *Name, *Latest, *Latest)) + (IsSig ? UAdvancePakLibrary::SigSuffixName : UAdvancePakLibrary::PakSuffixName));
}

FAdvancePakFileInfo::FAdvancePakFileInfo()
{
	Size = 0;
}

FAdvancePakFileInfo::FAdvancePakFileInfo(const FString& InName, const uint64& InSize, const FString& InHash, const FString& InFormer, const FString& InFuture)
{
		Name = InName;
		Size = InSize;
		Hash = InHash;
		Former = InFormer;
		Future = InFuture;
}

bool FAdvancePakFileInfo::IsPak() const
{
	return Name.EndsWith(UAdvancePakLibrary::PakSuffixName);
}

bool FAdvancePakFileInfo::IsEqual(const FAdvancePakFileInfo& Other) const
{
	return Former.Equals(Other.Former) && Future.Equals(Other.Future) && Size == Other.Size && Hash.Equals(Other.Hash);
}

FString FAdvancePakFileInfo::GetFormatName() const
{
	return FString::Printf(TEXT("%s_%s_%s"), *FPaths::GetBaseFilename(Name), *Former, *Future) + FPaths::GetExtension(Name, true);
}

FString FAdvancePakFileInfo::GetRequestURL() const
{
	return UAdvancePakLibrary::VersionRootURL / Future / Former / Name;
}

FString FAdvancePakFileInfo::GetPatchName() const
{
	return FPaths::GetBaseFilename(Name);
}

FString FAdvancePakConfigMark::GetRequestURL() const
{
	return UAdvancePakLibrary::VersionRootURL / Future / Former / UAdvancePakLibrary::VersionConfigName;
}
