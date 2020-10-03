// Fill out your copyright notice in the Description page of Project Settings.

#include "AdvancePakProcess.h"
#include "AdvancePakWork.h"
#include "AdvancePakLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Online/HTTP/Public/HttpModule.h"
#include "AdvancePakType.h"
#include "AdvancePakAgency.h" 
#include "Misc/Paths.h"
#include "PakFile/Public/IPlatformFilePak.h"

void FAdvancePakProcess::InitializeProcess(UAdvancePakWork* InAdvancePakWork)
{
	Work = InAdvancePakWork;
	Config = Work->AdvancePakConfig;
	State = Work->AdvancePakState;
	PlatformFile = &FPlatformFileManager::Get().GetPlatformFile();
}

void FAdvancePakProcess::SaveConfig()
{
	UGameplayStatics::SaveGameToSlot(Config, UAdvancePakLibrary::AdvancePakConfigName, 0);
}

void FAdvancePakProcess::SaveState()
{
	UGameplayStatics::SaveGameToSlot(State, UAdvancePakLibrary::AdvancePakStateName, 0);
}

bool FAdvancePakProcess::Mount(const FString& PakFile, uint32 PakOrder)
{
	return Work->PakFileMgr->Mount(*PakFile, PakOrder);
}

bool FAdvancePakProcess::UnMount(const FString& PakFile)
{
	return Work->PakFileMgr->Unmount(*PakFile);
}

FString FAdvancePakProcess::GetFormatName(const FString& FileName, const FString& Former, const FString& Future) const
{
	return FString::Printf(TEXT("%s_%s_%s"), *FPaths::GetBaseFilename(FileName), *Former, *Future) + FPaths::GetExtension(FileName, true);
}

FString FAdvancePakProcess::GetUnFormatName(const FString& FileName) const
{
	TArray<FString> CacheArray;
	if (FileName.Contains(TEXT("_")))
	{
		FileName.ParseIntoArray(CacheArray, TEXT("_"));
		return CacheArray[0] + FPaths::GetExtension(FileName, true);
	}
	return FileName;
}

FString FAdvancePakProcess::GetPatchName(const FString& FileName) const
{
	if (FileName.Contains(TEXT("_")))
	{
		FString PatchName;
		FileName.Split(FString("_"), &PatchName, NULL);
		return PatchName;
	}
	return FPaths::GetBaseFilename(FileName);
}

void FAdvancePakProcess::DeleteFilsInDirectory(const FString& Directory)
{
	TArray<FString> TotalFiles;
	PlatformFile->FindFiles(TotalFiles, *Directory, nullptr);
	for (auto FileName : TotalFiles)
		PlatformFile->DeleteFile(*FileName);
}

void FAdvancePakProcess::DeleteFilsInSaved()
{
	FOnliePakPakSigVisitor SavedPakSigVisitor;
	PlatformFile->IterateDirectory(*UAdvancePakLibrary::SavedPaksPath, SavedPakSigVisitor);
	for (auto PakFile : SavedPakSigVisitor.PakFilePaths)
	{
		UnMount(*PakFile);
		PlatformFile->DeleteFile(*PakFile);
	}
	for (auto SigFile : SavedPakSigVisitor.SigFilePaths)
	{
		PlatformFile->DeleteFile(*SigFile);
	}
}

void FAdvancePakProcess::CheckMergeState()
{
	TMap<FString, FAdvancePakFileInfo> MergeFiles;
	FAdvancePakFileInfoVisitor MergeFileInfosVisitor(MergeFiles);
	PlatformFile->IterateDirectory(*UAdvancePakLibrary::MergePaksPath, MergeFileInfosVisitor);

	TArray<FString> AbandonFiles;
	for (auto MergeFile : MergeFiles)
	{
		if (MergeFile.Value.Former.Equals(Work->LatestVersion) && MergeFile.Value.Future.Equals(Work->LatestVersion))
		{
			if (!(State->MergeFiles.Contains(MergeFile.Key) && State->MergeFiles.Find(MergeFile.Key)->IsEqual(MergeFile.Value)))
			{
				PlatformFile->DeleteFile(*(UAdvancePakLibrary::MergePaksPath / MergeFile.Key));
				AbandonFiles.Add(MergeFile.Key);
			}
		}
		else
		{
			PlatformFile->DeleteFile(*(UAdvancePakLibrary::MergePaksPath / MergeFile.Key));
			AbandonFiles.Add(MergeFile.Key);
		}
	}

	for (auto AbandonFile : AbandonFiles)
		MergeFiles.Remove(AbandonFile);

	State->MergeFiles = MergeFiles;
	SaveState();
}

