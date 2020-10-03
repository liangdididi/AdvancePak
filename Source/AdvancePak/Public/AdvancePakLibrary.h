// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AdvancePakType.h"
#include "Misc/Paths.h"
#include "Misc/App.h"
#include "AssetRegistryModule.h"
#include "Misc/SecureHash.h"
#include "AdvancePakLibrary.generated.h"


UCLASS()
class ADVANCEPAK_API UAdvancePakLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void InitializeLibrary();

	static bool CreatePakFile(const TCHAR* Filename, TArray<FPakInputPair>& FilesToAdd, const FPakCommandLineParameters& CmdLineParameters, const FKeyChain& InKeyChain);

	static bool ExtractPakFile(const TCHAR* InPakFilename, TArray<FPakInputPair>* OutEntries, const FKeyChain& InKeyChain);

	static bool ExtractSingleFile(const TCHAR* InPakFilename, const TCHAR* InSingleFilename, TArray<uint8>& FileData, const FKeyChain& InKeyChain);

	static void LoadKeyChainFromFile(const FString& InFilename, FKeyChain& OutCryptoSettings);

	static void ApplyEncryptionKeys(const FKeyChain& KeyChain);

	static FString GetLongestPath(TArray<FPakInputPair>& FilesToAdd);

	static FString GetCommonRootPath(TArray<FPakInputPair>& FilesToAdd);

	static FString SanitizeCacheKey(const TCHAR* PluginName, const TCHAR* VersionString, const TCHAR* PluginSpecificCacheKeySuffix);

	static bool PrepareCopyFileToPak(const FString& InMountPoint, const FPakInputPair& InFile, uint8*& InOutPersistentBuffer, int64& InOutBufferSize, FPakEntryPair& OutNewEntry, uint8*& OutDataToWrite, int64& OutSizeToWrite, const FKeyChain& InKeyChain);

	static void FinalizeCopyCompressedFileToPak(FPakInfo& InPakInfo, const FCompressedFileBuffer& CompressedFile, FPakEntryPair& OutNewEntry);

	static bool PrepareCopyCompressedFileToPak(const FString& InMountPoint, FPakInfo& Info, const FPakInputPair& InFile, const FCompressedFileBuffer& CompressedFile, FPakEntryPair& OutNewEntry, uint8*& OutDataToWrite, int64& OutSizeToWrite, const FKeyChain& InKeyChain);

	static void PrepareDeleteRecordForPak(const FString& InMountPoint, const FPakInputPair InDeletedFile, FPakEntryPair& OutNewEntry);

	static bool BufferedCopyFile(FArchive& Dest, FArchive& Source, const FPakFile& PakFile, const FPakEntry& Entry, void* Buffer, int64 BufferSize, const FKeyChain& InKeyChain);

	static bool UncompressCopyFile(FArchive& Dest, FArchive& Source, const FPakEntry& Entry, uint8*& PersistentBuffer, int64& BufferSize, const FKeyChain& InKeyChain, const FPakFile& PakFile);

	static FRSAKeyHandle ParseRSAKeyFromJson(TSharedPtr<FJsonObject> InObj);

	static FArchive* CreatePakWriter(const TCHAR* Filename, const FKeyChain& InKeyChain, bool bSign);

	static bool ReadSizeParam(const TCHAR* CmdLine, const TCHAR* ParamStr, int64& SizeOut);

	static void ProcessCommandLine(const TCHAR* CmdLine, FPakCommandLineParameters& CmdLineParameters);

	static FString AbsorbVersionConfigToJson(const TSharedPtr<FAdvancePakVersionConfig>& HotConfigure);

	static TSharedPtr<FAdvancePakVersionConfig> AbsorbJsonToVersionConfig(const FString& JsonContext);

	static FString ContentPaksPath;

	static FString VersionRootURL;

	static FString SavedPaksPath;

	static FString BufferPaksPath;

	static FString StorePaksPath;

	static FString MergePaksPath;

	static float GetDiskFreeSpaceMB();

	static FString CmdLineName;

	static uint64 GetDiskFreeSpace();

	static FString UpdatePatchsName;

	static FString ReducePatchsName;

	static FString PatchPropsName;

	static FString HashName;

	static FString SizeName;

	static FString AddResourcesName;

	static FString UpdateResourcesName;

	static FString AdvancePakConfigName;

	static FString AdvancePakStateName;

	static FString VersionConfigName;

	static FString AddPatchsName;

	static FString KeepPatchsName;

	static FString DeletePatchsName;

	static FString DeleteResourcesName;

	static FString PatchNameName;

	static FString PublishName;

	static FString PakSuffixName;

	static FString SigSuffixName;

	static FString GetVersionURL(const FString& JsonContext, const FString& Platform);

	static FString DefaultGameIniPath;

	static FString CachePaksPath;

	static FString DefaultCryptoPath;
};

class FAdvancePakPatchsVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	TArray<FString> FileNames;
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{
		FString FileName = FPaths::GetCleanFilename(FilenameOrDirectory);
		if (!bIsDirectory && (FileName.EndsWith(UAdvancePakLibrary::PakSuffixName) || FileName.EndsWith(UAdvancePakLibrary::SigSuffixName)))
		{
			FileNames.Add(FileName);
		}
		return true;
	}
};

class FAdvancePakDeleteVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	TArray<FString> FilterNames;
	TArray<FString> FilePaths;
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{
		FString FileName = FPaths::GetCleanFilename(FilenameOrDirectory);
		if (!bIsDirectory && (FileName.EndsWith(UAdvancePakLibrary::PakSuffixName) || FileName.EndsWith(UAdvancePakLibrary::SigSuffixName)))
		{
			FString PatchName;
			if (!FileName.Split(FString("_"), &PatchName, NULL))
			{
				PatchName = FPaths::GetBaseFilename(FileName);
			}
			if (FilterNames.Contains(PatchName))
			{
				FilePaths.Add(FString(FilenameOrDirectory));
			}
		}
		return true;
	}
};

class FOnliePakPakSigVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	TArray<FString> PakFilePaths;
	TArray<FString> SigFilePaths;
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{
		FString FileName = FilenameOrDirectory;
		if (!bIsDirectory)
		{
			if (FileName.EndsWith(UAdvancePakLibrary::PakSuffixName))
			{
				PakFilePaths.Add(FileName);
			}
			else if (FileName.EndsWith(UAdvancePakLibrary::SigSuffixName))
			{
				SigFilePaths.Add(FileName);
			}
			else
			{
				IFileManager::Get().Delete(FilenameOrDirectory);
			}
		}
		return true;
	}
};

class FAdvancePakFileInfoVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	TMap<FString, FAdvancePakFileInfo>* FileInfos;
	FString Version;
	FAdvancePakFileInfoVisitor(TMap<FString, FAdvancePakFileInfo>& InFileInfos, const FString& InVersion = FString())
	{
		FileInfos = &InFileInfos;
		Version = InVersion;
	}
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{
		FString FileName = FPaths::GetCleanFilename(FilenameOrDirectory);
		if (!bIsDirectory && (FileName.EndsWith(UAdvancePakLibrary::PakSuffixName) || FileName.EndsWith(UAdvancePakLibrary::SigSuffixName)))
		{
			if (Version.IsEmpty())
			{
				TArray<FString> CacheArray;
				FPaths::GetBaseFilename(FileName).ParseIntoArray(CacheArray, TEXT("_"));
				if (CacheArray.Num() == 3)
				{
					FileInfos->Add(FileName, FAdvancePakFileInfo(CacheArray[0] + FPaths::GetExtension(FileName, true), (uint64)IFileManager::Get().FileSize(FilenameOrDirectory), LexToString(FMD5Hash::HashFile(FilenameOrDirectory)), CacheArray[1], CacheArray[2]));
				}
				else
				{
					IFileManager::Get().Delete(FilenameOrDirectory);
				}
			}
			else
			{
				FileInfos->Add(FileName, FAdvancePakFileInfo(FileName, (uint64)IFileManager::Get().FileSize(FilenameOrDirectory), LexToString(FMD5Hash::HashFile(FilenameOrDirectory)), Version, Version));
			}
		}
		return true;
	}
};

class FAdvancePakContentPakVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	bool IsWithSig;
	FString ContentPakPath;
	FAdvancePakContentPakVisitor() { IsWithSig = false; }
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{
		if (!bIsDirectory && FString(FilenameOrDirectory).EndsWith(UAdvancePakLibrary::SigSuffixName))
		{
			IsWithSig = true;
		}
		if (!bIsDirectory && FString(FilenameOrDirectory).EndsWith(UAdvancePakLibrary::PakSuffixName))
		{
			ContentPakPath = FilenameOrDirectory;
		}
		return true;
	}
};