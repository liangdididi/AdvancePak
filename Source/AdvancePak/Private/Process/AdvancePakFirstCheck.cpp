#include "AdvancePakFirstCheck.h"
#include "AdvancePakWork.h"
#include "AdvancePakLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Online/HTTP/Public/HttpModule.h"
#include "AdvancePakType.h"
#include "AdvancePakAgency.h" 
#include "Misc/Paths.h"
#include "PakFile/Public/IPlatformFilePak.h"

void FAdvancePakFirstCheck::EnterProcess()
{
	if (!State->SavedVersion.IsEmpty())
	{
		TMap<FString, FAdvancePakFileInfo> SavedFiles;
		FAdvancePakFileInfoVisitor SavedFileInfosVisitor(SavedFiles, State->SavedVersion);
		PlatformFile->IterateDirectory(*UAdvancePakLibrary::SavedPaksPath, SavedFileInfosVisitor);

		TArray<FString> AbandonFiles;
		for (auto SavedFile : SavedFiles)
		{
			if (State->SavedFiles.Contains(SavedFile.Key))
			{
				if (!State->SavedFiles.Find(SavedFile.Key)->IsEqual(SavedFile.Value))
				{
					if (!SavedFile.Value.IsPak())
						UnMount(UAdvancePakLibrary::SavedPaksPath / SavedFile.Key);
					PlatformFile->DeleteFile(*(UAdvancePakLibrary::SavedPaksPath / SavedFile.Key));
					AbandonFiles.Add(SavedFile.Key);
				}
			}
			else
			{
				if (!SavedFile.Value.IsPak())
					UnMount(UAdvancePakLibrary::SavedPaksPath / SavedFile.Key);
				PlatformFile->DeleteFile(*(UAdvancePakLibrary::SavedPaksPath / SavedFile.Key));
				AbandonFiles.Add(SavedFile.Key);
			}
		}

		for (auto AbandonFile : AbandonFiles)
			SavedFiles.Remove(AbandonFile);

		if (State->SavedVersion.Equals(Work->LatestVersion))
		{
			if (SavedFiles.Num() == State->SavedFiles.Num())
			{
				bool IsSavedMatch = true;
				for (auto SavedFile : SavedFiles)
				{
					if (!(State->SavedFiles.Contains(SavedFile.Key) && State->SavedFiles.Find(SavedFile.Key)->IsEqual(SavedFile.Value)))
					{
						IsSavedMatch = false;
						break;
					}
				}
				if (IsSavedMatch)
				{
					Work->OnUpdateProgressNotify.Broadcast(1.f);
					Work->SwitchEnterProcess(EAdvancePakProcess::UpdateSucceed);
					return;
				}
			}
		}

		for (auto SaveFile : SavedFiles)
		{
			PlatformFile->MoveFile(*(UAdvancePakLibrary::StorePaksPath / SaveFile.Value.GetFormatName()), *(UAdvancePakLibrary::SavedPaksPath / SaveFile.Key));
		}
		SavedFiles.Reset();

		TMap<FString, FAdvancePakFileInfo> StoreFiles;
		FAdvancePakFileInfoVisitor StoreFileInfosVisitor(StoreFiles);
		PlatformFile->IterateDirectory(*UAdvancePakLibrary::StorePaksPath, StoreFileInfosVisitor);

		bool IsNeedOrigion = false;
		int32 MatchNum = 0;
		for (auto StoreFile : StoreFiles)
		{
			FString UnFormatName = GetUnFormatName(StoreFile.Key);
			if (State->SavedFiles.Contains(UnFormatName) && State->SavedFiles.Find(UnFormatName)->IsEqual(StoreFile.Value))
			{
				MatchNum++;
			}
			else
			{
				IsNeedOrigion = true;
				break;
			}
		}
		if (!IsNeedOrigion)
		{
			IsNeedOrigion = !(MatchNum == State->SavedFiles.Num());
		}

		if (IsNeedOrigion)
			Work->ExpectConfigMarks.AddUnique(FAdvancePakConfigMark(Config->Version, Config->Version));

		if (!Work->LatestVersion.Equals(Config->Version) && IsNeedOrigion)
			Work->ExpectConfigMarks.AddUnique(FAdvancePakConfigMark(Config->Version, Work->LatestVersion));

		if (!Work->LatestVersion.Equals(State->SavedVersion))
			Work->ExpectConfigMarks.AddUnique(FAdvancePakConfigMark(State->SavedVersion, Work->LatestVersion));

		check(Work->ExpectConfigMarks.Num() != 0);
	}
	else
	{
		FOnliePakPakSigVisitor SavedPakSigVisitor;
		PlatformFile->IterateDirectory(*UAdvancePakLibrary::SavedPaksPath, SavedPakSigVisitor);
		for (auto PakFilePath : SavedPakSigVisitor.PakFilePaths)
		{
			UnMount(PakFilePath);
			PlatformFile->DeleteFile(*PakFilePath);
		}
		for (auto SigFilePath : SavedPakSigVisitor.SigFilePaths)
		{
			PlatformFile->DeleteFile(*SigFilePath);
		}

		Work->ExpectConfigMarks.AddUnique(FAdvancePakConfigMark(Config->Version, Config->Version));

		if (!Work->LatestVersion.Equals(Config->Version))
			Work->ExpectConfigMarks.AddUnique(FAdvancePakConfigMark(Config->Version, Work->LatestVersion));
	}

	for (auto ConfigMark : Work->ExpectConfigMarks)
	{
		if (!Config->ConfigMarks.Contains(ConfigMark))
		{
			Work->RequestConfigMarks.Add(ConfigMark);
		}
	}

	if (Work->RequestConfigMarks.Num() > 0)
	{
		Work->SwitchEnterProcess(EAdvancePakProcess::LoadConfig);
	}
	else
	{
		Work->SwitchEnterProcess(EAdvancePakProcess::SecondCheck);
	}
}
