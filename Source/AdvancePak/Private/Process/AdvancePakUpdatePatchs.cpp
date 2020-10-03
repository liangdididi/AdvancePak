#include "AdvancePakUpdatePatchs.h"
#include "AdvancePakWork.h"
#include "AdvancePakLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Online/HTTP/Public/HttpModule.h"
#include "AdvancePakType.h"
#include "AdvancePakAgency.h" 
#include "Misc/Paths.h"
#include "PakFile/Public/IPlatformFilePak.h"

void FAdvancePakUpdatePatchs::EnterProcess()
{
	if (Work->ExpectConfigMarks.Num() == 1 && Work->ExpectConfigMarks[0] == FAdvancePakConfigMark(Config->Version, Config->Version))
	{

		TMap<FString, FAdvancePakFileInfo> StoreFiles;
		FAdvancePakFileInfoVisitor StoreFileInfosVisitor(StoreFiles);
		PlatformFile->IterateDirectory(*UAdvancePakLibrary::StorePaksPath, StoreFileInfosVisitor);

		Work->RequestLoadInfos.Reset();

		for (auto LoadInfo : Work->ExpectLoadInfos)
		{
			if (!StoreFiles.Contains(LoadInfo.Key))
			{
				Work->RequestLoadInfos.Add(LoadInfo.Key, LoadInfo.Value);
			}
			else
			{
				if (!StoreFiles.Find(LoadInfo.Key)->IsEqual(LoadInfo.Value))
				{
					Work->RequestLoadInfos.Add(LoadInfo.Key, LoadInfo.Value);
				}
			}
		}

		if (Work->RequestLoadInfos.Num() > 0)
		{
			Work->SwitchEnterProcess(EAdvancePakProcess::LoadPatchs);
		}
		else
		{

			DeleteFilsInSaved();

			TArray<FString> PakPaths;
			State->SavedFiles.Reset();
			for (auto LoadInfo : Work->ExpectLoadInfos)
			{
				PlatformFile->MoveFile(*(UAdvancePakLibrary::SavedPaksPath / LoadInfo.Value.Name), *(UAdvancePakLibrary::StorePaksPath / LoadInfo.Key));
				State->SavedFiles.Add(LoadInfo.Value.Name, LoadInfo.Value);
				if (LoadInfo.Value.IsPak())
					PakPaths.Add(UAdvancePakLibrary::SavedPaksPath / LoadInfo.Value.Name);
			}
			for (auto PakPath : PakPaths)
			{
				Mount(PakPath, 1);
			}


			State->SavedVersion = Config->Version;
			State->MergeFiles.Reset();
			SaveState();

			Work->SwitchEnterProcess(EAdvancePakProcess::UpdateSucceed);

		}

		return;
	}


	TMap<FString, FAdvancePakFileInfo> MergeFiles;
	FAdvancePakFileInfoVisitor MergeFileInfosVisitor(MergeFiles);
	PlatformFile->IterateDirectory(*UAdvancePakLibrary::MergePaksPath, MergeFileInfosVisitor);

	for (auto MergeInfo : State->MergeFiles)
	{
		if (!(MergeFiles.Contains(MergeInfo.Key)
			&& MergeFiles.Find(MergeInfo.Key)->IsEqual(MergeInfo.Value)))
		{

			Work->SwitchEnterProcess(EAdvancePakProcess::MergeCheck);
			return;
		}
	}

	State->SavedFiles.Reset();
	TArray<FString> PakPaths;
	for (auto MergeInfo : State->MergeFiles)
	{

		PlatformFile->MoveFile(*(UAdvancePakLibrary::SavedPaksPath / MergeInfo.Value.Name), *(UAdvancePakLibrary::MergePaksPath / MergeInfo.Key));

		State->SavedFiles.Add(MergeInfo.Value.Name, MergeInfo.Value);

		if(MergeInfo.Value.IsPak())
			PakPaths.Add(UAdvancePakLibrary::SavedPaksPath / MergeInfo.Value.Name);
	}

	for (auto PakPath : PakPaths)
	{
		Mount(PakPath, 1);
	}

	State->SavedVersion = Work->LatestVersion;
	State->MergeFiles.Reset();
	SaveState();

	Work->SwitchEnterProcess(EAdvancePakProcess::UpdateSucceed);
}

