#include "AdvancePakMergeCheck.h"
#include "AdvancePakWork.h"
#include "AdvancePakLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Online/HTTP/Public/HttpModule.h"
#include "AdvancePakType.h"
#include "AdvancePakAgency.h" 
#include "Misc/Paths.h"
#include "PakFile/Public/IPlatformFilePak.h"

void FAdvancePakMergeCheck::EnterProcess()
{
	TMap<FString, FAdvancePakFileInfo> StoreFiles;
	FAdvancePakFileInfoVisitor StoreFileInfosVisitor(StoreFiles);
	PlatformFile->IterateDirectory(*UAdvancePakLibrary::StorePaksPath, StoreFileInfosVisitor);

	bool IsNeedCheck = ((Work->ExpectConfigMarks.Num() == 1) && !(Work->ExpectConfigMarks[0] == FAdvancePakConfigMark(Config->Version, Config->Version))) || (Work->ExpectConfigMarks.Num() == 3);
	Work->RequestLoadInfos.Reset();
	for (auto LoadInfo : Work->ExpectLoadInfos)
	{
		if (!(StoreFiles.Contains(LoadInfo.Key)
			&& StoreFiles.Find(LoadInfo.Key)->IsEqual(LoadInfo.Value)))
		{
			Work->RequestLoadInfos.Add(LoadInfo.Key, LoadInfo.Value);
			
			if (IsNeedCheck && LoadInfo.Value.Former.Equals(State->SavedVersion) && LoadInfo.Value.Future.Equals(State->SavedVersion))
			{
				Work->ExpectLoadInfos.Reset();
				Work->RequestLoadInfos.Reset();
				Work->RequestMergeInfos.Reset();
				Work->ExpectConfigMarks.Reset();
				Work->RequestConfigMarks.Reset();
				Work->VersionConfigs.Reset();
				Work->ExpectPatchs.Reset();
				Work->ExpectMarks.Reset();
				Work->SwitchEnterProcess(EAdvancePakProcess::FirstCheck);
				return;
			}
		}
	}

	if (Work->RequestLoadInfos.Num() > 0)
	{
		Work->SwitchEnterProcess(EAdvancePakProcess::LoadPatchs);
		return;
	}

	if (Work->ExpectConfigMarks.Num() == 1
		&& Work->ExpectConfigMarks[0] == FAdvancePakConfigMark(Config->Version, Config->Version))
	{
		Work->SwitchEnterProcess(EAdvancePakProcess::UpdatePatchs);
		return;
	}

	TMap<FString, FAdvancePakFileInfo> MergeFiles;
	FAdvancePakFileInfoVisitor MergeFileInfosVisitor(MergeFiles);
	PlatformFile->IterateDirectory(*UAdvancePakLibrary::MergePaksPath, MergeFileInfosVisitor);

	TArray<FString> AbandonFiles;
	for (auto MergeFile : MergeFiles)
	{
		if (MergeFile.Value.Former.Equals(Work->LatestVersion)
			&& MergeFile.Value.Future.Equals(Work->LatestVersion)
			&& State->MergeFiles.Contains(MergeFile.Key)
			&& State->MergeFiles.Find(MergeFile.Key)->IsEqual(MergeFile.Value))
		{
			if (!Config->IsWithSig)
			{
				continue;
			}
			else
			{
				FString MateFileName = FPaths::GetBaseFilename(MergeFile.Key) + (MergeFile.Value.IsPak() ? UAdvancePakLibrary::SigSuffixName : UAdvancePakLibrary::PakSuffixName);
				if (MergeFiles.Contains(MateFileName)
					&& State->MergeFiles.Contains(MateFileName)
					&& State->MergeFiles.Find(MateFileName)->IsEqual(*MergeFiles.Find(MateFileName))
					)
				{
					continue;
				}
			}
		}
		PlatformFile->DeleteFile(*(UAdvancePakLibrary::MergePaksPath / MergeFile.Key));
		AbandonFiles.Add(MergeFile.Key);
	}

	for (auto AbandonFile : AbandonFiles)
	{
		MergeFiles.Remove(AbandonFile);
	}

	State->MergeFiles.Reset();
	for (auto MergeFile : MergeFiles)
	{
		State->MergeFiles.Add(MergeFile.Key, MergeFile.Value);
	}
	SaveState();

	TArray<FString> LackPatchs;
	for (auto PatchName : Work->ExpectPatchs)
	{
		bool IsLack = true;
		for (auto MergeFile : State->MergeFiles)
		{
			if (MergeFile.Value.GetPatchName().Equals(PatchName))
			{
				IsLack = false;
				break;
			}
		}
		if (IsLack)
		{
			LackPatchs.Add(PatchName);
		}
	}

	Work->RequestMergeInfos.Reset();

	if (Work->ExpectConfigMarks.Num() == 1)
	{
		for (auto PatchName : LackPatchs)
		{
			FAdvancePakMergeInfo MergeInfo;
			MergeInfo.Name = PatchName;
			MergeInfo.Latest = Work->LatestVersion;
			for (auto LoadInfo : Work->ExpectLoadInfos)
			{
				if (LoadInfo.Value.GetPatchName().Equals(PatchName))
				{
					if (LoadInfo.Value.Future.Equals(Work->LatestVersion))
					{
						MergeInfo.NexFormer = LoadInfo.Value.Former;
						MergeInfo.NexFuture = LoadInfo.Value.Future;
						MergeInfo.NexSize = LoadInfo.Value.Size;
						MergeInfo.NexHash = LoadInfo.Value.Hash;
					}
					else
					{
						MergeInfo.PreFormer = LoadInfo.Value.Former;
						MergeInfo.PreFuture = LoadInfo.Value.Future;
						MergeInfo.PreSize = LoadInfo.Value.Size;
						MergeInfo.PreHash = LoadInfo.Value.Hash;
					}
				}
			}
			
			if (Work->LatSavConfig->UpdatePatchs.Contains(PatchName))
			{
				MergeInfo.PatchInfo = *Work->LatSavConfig->UpdatePatchs.Find(PatchName);
			}
			Work->RequestMergeInfos.Add(MergeInfo);
		}
	}
	else if (Work->ExpectConfigMarks.Num() == 2)
	{
		for (auto PatchName : LackPatchs)
		{
			FAdvancePakMergeInfo MergeInfo;
			MergeInfo.Name = PatchName;
			MergeInfo.Latest = Work->LatestVersion;
			for (auto LoadInfo : Work->ExpectLoadInfos)
			{
				if (LoadInfo.Value.GetPatchName().Equals(PatchName))
				{
					if (LoadInfo.Value.Future.Equals(Work->LatestVersion))
					{
						MergeInfo.NexFormer = LoadInfo.Value.Former;
						MergeInfo.NexFuture = LoadInfo.Value.Future;
						MergeInfo.NexSize = LoadInfo.Value.Size;
						MergeInfo.NexHash = LoadInfo.Value.Hash;
					}
					else
					{
						MergeInfo.PreFormer = LoadInfo.Value.Former;
						MergeInfo.PreFuture = LoadInfo.Value.Future;
						MergeInfo.PreSize = LoadInfo.Value.Size;
						MergeInfo.PreHash = LoadInfo.Value.Hash;
					}
				}
			}
			
			if (Work->LatOriConfig->UpdatePatchs.Contains(PatchName))
			{
				MergeInfo.PatchInfo = *Work->LatOriConfig->UpdatePatchs.Find(PatchName);
			}
			Work->RequestMergeInfos.Add(MergeInfo);
		}

	}
	else if (Work->ExpectConfigMarks.Num() == 3)
	{
		for (auto PatchName : LackPatchs)
		{
			FAdvancePakMergeInfo MergeInfo;
			MergeInfo.Name = PatchName;
			MergeInfo.Latest = Work->LatestVersion;
			for (auto LoadInfo : Work->ExpectLoadInfos)
			{
				if (LoadInfo.Value.GetPatchName().Equals(PatchName))
				{
					if (LoadInfo.Value.Future.Equals(Work->LatestVersion))
					{
						MergeInfo.NexFormer = LoadInfo.Value.Former;
						MergeInfo.NexFuture = LoadInfo.Value.Future;
						MergeInfo.NexSize = LoadInfo.Value.Size;
						MergeInfo.NexHash = LoadInfo.Value.Hash;
					}
					else
					{
						MergeInfo.PreFormer = LoadInfo.Value.Former;
						MergeInfo.PreFuture = LoadInfo.Value.Future;
						MergeInfo.PreSize = LoadInfo.Value.Size;
						MergeInfo.PreHash = LoadInfo.Value.Hash;
					}
				}
			}
			if (MergeInfo.NexFormer.Equals(Config->Version) && Work->LatOriConfig->UpdatePatchs.Contains(PatchName))
			{
				MergeInfo.PatchInfo = *Work->LatOriConfig->UpdatePatchs.Find(PatchName);
			}
			else if (MergeInfo.NexFormer.Equals(State->SavedVersion) && Work->LatSavConfig->UpdatePatchs.Contains(PatchName))
			{
				MergeInfo.PatchInfo = *Work->LatSavConfig->UpdatePatchs.Find(PatchName);
			}
			Work->RequestMergeInfos.Add(MergeInfo);
		}
	}

	if (Work->RequestMergeInfos.Num() > 0)
	{
		Work->SwitchEnterProcess(EAdvancePakProcess::MergePatchs);
	}
	else
	{
		Work->SwitchEnterProcess(EAdvancePakProcess::UpdatePatchs);
	}
}
