#include "AdvancePakSecondCheck.h"
#include "AdvancePakWork.h"
#include "AdvancePakLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Online/HTTP/Public/HttpModule.h"
#include "AdvancePakType.h"
#include "AdvancePakAgency.h" 
#include "Misc/Paths.h"
#include "PakFile/Public/IPlatformFilePak.h"

void FAdvancePakSecondCheck::EnterProcess()
{
	for (auto ConfigMask : Work->ExpectConfigMarks)
	{
		Work->VersionConfigs.Add(ConfigMask, UAdvancePakLibrary::AbsorbJsonToVersionConfig(*Config->ConfigMarks.Find(ConfigMask)));
	}

	if (Work->ExpectConfigMarks.Num() == 1)
	{
		if (Work->ExpectConfigMarks[0] == FAdvancePakConfigMark(Config->Version, Config->Version))
		{
			Work->OriOriConfig = *Work->VersionConfigs.Find(Work->ExpectConfigMarks[0]);

			Work->ExpectPatchs.Append(Work->OriOriConfig->AddPatchs);

			for (auto PatchProp : Work->OriOriConfig->PatchProps)
			{
				FAdvancePakFileInfo ExpectLoadInfo;
				ExpectLoadInfo.Name = PatchProp.Key;
				ExpectLoadInfo.Size = PatchProp.Value->Size;
				ExpectLoadInfo.Hash = PatchProp.Value->Hash;
				ExpectLoadInfo.Former = Config->Version;
				ExpectLoadInfo.Future = Config->Version;
				Work->ExpectLoadInfos.Add(ExpectLoadInfo.GetFormatName(), ExpectLoadInfo);
			}

			TMap<FString, FAdvancePakFileInfo> StoreFiles;
			FAdvancePakFileInfoVisitor StoreFileInfosVisitor(StoreFiles);
			PlatformFile->IterateDirectory(*UAdvancePakLibrary::StorePaksPath, StoreFileInfosVisitor);

			TArray<FString> AbandonFiles;
			for (auto StoreFile : StoreFiles)
			{
				if (!(Work->ExpectLoadInfos.Contains(StoreFile.Key) && Work->ExpectLoadInfos.Find(StoreFile.Key)->IsEqual(StoreFile.Value)))
				{
					PlatformFile->DeleteFile(*(UAdvancePakLibrary::StorePaksPath / StoreFile.Key));
					AbandonFiles.Add(StoreFile.Key);
				}
			}

			for (auto AbandonFile : AbandonFiles)
				StoreFiles.Remove(AbandonFile);

			for (auto LoadInfo : Work->ExpectLoadInfos)
			{
				if (!StoreFiles.Contains(LoadInfo.Key))
				{
					Work->RequestLoadInfos.Add(LoadInfo.Key, LoadInfo.Value);
				}
			}

			if (Work->RequestLoadInfos.Num() > 0)
			{
				Work->SwitchEnterProcess(EAdvancePakProcess::LoadPatchs);
			}
			else
			{
				Work->SwitchEnterProcess(EAdvancePakProcess::UpdatePatchs);
			}

			return;
		}
		else
		{
			Work->LatSavConfig = *Work->VersionConfigs.Find(Work->ExpectConfigMarks[0]);
			UAdvancePakLibrary::ProcessCommandLine(*Work->LatSavConfig->CmdLine, Work->CmdParams);

			Work->ExpectPatchs.AddUnique(UAdvancePakLibrary::PublishName);
			for (auto SavedFile : State->SavedFiles)
			{
				Work->ExpectPatchs.AddUnique(GetPatchName(SavedFile.Key));
			}
			for (auto AddPatch : Work->LatSavConfig->AddPatchs)
			{
				Work->ExpectPatchs.AddUnique(AddPatch);
			}
			for (auto DeletePatch : Work->LatSavConfig->DeletePatchs)
			{
				Work->ExpectPatchs.Remove(DeletePatch);
			}

			TArray<FString> AbandonFiles;
			for (auto SavedFile : State->SavedFiles)
			{
				if (SavedFile.Value.IsPak() && Work->ExpectPatchs.Contains(SavedFile.Value.GetPatchName()))
				{
					Work->ExpectLoadInfos.Add(SavedFile.Value.GetFormatName(), SavedFile.Value);
				}
				else
				{
					AbandonFiles.Add(SavedFile.Key);
				}
			}

			for (auto AbandonFile : AbandonFiles)
				State->SavedFiles.Remove(AbandonFile);
			SaveState();

			for (auto PatchProp : Work->LatSavConfig->PatchProps)
			{
				FAdvancePakFileInfo ExpectLoadInfo;
				ExpectLoadInfo.Name = PatchProp.Key;
				ExpectLoadInfo.Size = PatchProp.Value->Size;
				ExpectLoadInfo.Hash = PatchProp.Value->Hash;
				ExpectLoadInfo.Former = State->SavedVersion;
				ExpectLoadInfo.Future = Work->LatestVersion;
				Work->ExpectLoadInfos.Add(ExpectLoadInfo.GetFormatName(), ExpectLoadInfo);
			}

			TMap<FString, FAdvancePakFileInfo> StoreFiles;
			FAdvancePakFileInfoVisitor StoreFileInfosVisitor(StoreFiles);
			PlatformFile->IterateDirectory(*UAdvancePakLibrary::StorePaksPath, StoreFileInfosVisitor);

			AbandonFiles.Reset();
			for (auto StoreFile : StoreFiles)
			{
				if (!(Work->ExpectLoadInfos.Contains(StoreFile.Key) && Work->ExpectLoadInfos.Find(StoreFile.Key)->IsEqual(StoreFile.Value)))
				{
					PlatformFile->DeleteFile(*(UAdvancePakLibrary::StorePaksPath / StoreFile.Key));
					AbandonFiles.Add(StoreFile.Key);
				}
			}

			for (auto AbandonFile : AbandonFiles)
				StoreFiles.Remove(AbandonFile);

			for (auto LoadInfo : Work->ExpectLoadInfos)
			{
				if (!StoreFiles.Contains(LoadInfo.Key))
				{
					Work->RequestLoadInfos.Add(LoadInfo.Key, LoadInfo.Value);
				}
			}

			if (Work->RequestLoadInfos.Num() > 0)
			{
				Work->SwitchEnterProcess(EAdvancePakProcess::LoadPatchs);
			}
			else
			{
				Work->SwitchEnterProcess(EAdvancePakProcess::MergeCheck);
			}
		}
	}
	else if (Work->ExpectConfigMarks.Num() == 2)
	{
		Work->OriOriConfig = *Work->VersionConfigs.Find(Work->ExpectConfigMarks[0]);
		Work->LatOriConfig = *Work->VersionConfigs.Find(Work->ExpectConfigMarks[1]);
		UAdvancePakLibrary::ProcessCommandLine(*Work->LatOriConfig->CmdLine, Work->CmdParams);

		Work->ExpectPatchs.AddUnique(UAdvancePakLibrary::PublishName);
		Work->ExpectPatchs.Append(Work->OriOriConfig->AddPatchs);
		for (auto AddPatch : Work->LatOriConfig->AddPatchs)
		{
			Work->ExpectPatchs.AddUnique(AddPatch);
		}
		for (auto DeletePatch : Work->LatOriConfig->DeletePatchs)
		{
			Work->ExpectPatchs.Remove(DeletePatch);
		}

		for (auto PatchProp : Work->OriOriConfig->PatchProps)
		{
			if (PatchProp.Key.EndsWith(UAdvancePakLibrary::PakSuffixName) && Work->ExpectPatchs.Contains(GetPatchName(PatchProp.Key)))
			{
				FAdvancePakFileInfo ExpectLoadInfo;
				ExpectLoadInfo.Name = PatchProp.Key;
				ExpectLoadInfo.Size = PatchProp.Value->Size;
				ExpectLoadInfo.Hash = PatchProp.Value->Hash;
				ExpectLoadInfo.Former = Config->Version;
				ExpectLoadInfo.Future = Config->Version;
				Work->ExpectLoadInfos.Add(ExpectLoadInfo.GetFormatName(), ExpectLoadInfo);
			}
			else
			{
				State->SavedFiles.Remove(PatchProp.Key);
			}
		}
		SaveState();

		for (auto PatchProp : Work->LatOriConfig->PatchProps)
		{
			FAdvancePakFileInfo ExpectLoadInfo;
			ExpectLoadInfo.Name = PatchProp.Key;
			ExpectLoadInfo.Size = PatchProp.Value->Size;
			ExpectLoadInfo.Hash = PatchProp.Value->Hash;
			ExpectLoadInfo.Former = Config->Version;
			ExpectLoadInfo.Future = Work->LatestVersion;
			Work->ExpectLoadInfos.Add(ExpectLoadInfo.GetFormatName(), ExpectLoadInfo);
		}

		TMap<FString, FAdvancePakFileInfo> StoreFiles;
		FAdvancePakFileInfoVisitor StoreFileInfosVisitor(StoreFiles);
		PlatformFile->IterateDirectory(*UAdvancePakLibrary::StorePaksPath, StoreFileInfosVisitor);

		TArray<FString> AbandonFiles;
		for (auto StoreFile : StoreFiles)
		{
			if (!(Work->ExpectLoadInfos.Contains(StoreFile.Key) && Work->ExpectLoadInfos.Find(StoreFile.Key)->IsEqual(StoreFile.Value)))
			{
				PlatformFile->DeleteFile(*(UAdvancePakLibrary::StorePaksPath / StoreFile.Key));
				AbandonFiles.Add(StoreFile.Key);
			}
		}

		for (auto AbandonFile : AbandonFiles)
			StoreFiles.Remove(AbandonFile);

		for (auto LoadInfo : Work->ExpectLoadInfos)
		{
			if (!StoreFiles.Contains(LoadInfo.Key))
			{
				Work->RequestLoadInfos.Add(LoadInfo.Key, LoadInfo.Value);
			}
		}

	}
	else if (Work->ExpectConfigMarks.Num() == 3)
	{
		Work->OriOriConfig = *Work->VersionConfigs.Find(Work->ExpectConfigMarks[0]);
		Work->LatOriConfig = *Work->VersionConfigs.Find(Work->ExpectConfigMarks[1]);
		Work->LatSavConfig = *Work->VersionConfigs.Find(Work->ExpectConfigMarks[2]);
		UAdvancePakLibrary::ProcessCommandLine(*Work->LatSavConfig->CmdLine, Work->CmdParams);

		Work->ExpectPatchs.AddUnique(UAdvancePakLibrary::PublishName);
		Work->ExpectPatchs.Append(Work->OriOriConfig->AddPatchs);
		for (auto AddPatch : Work->LatOriConfig->AddPatchs)
		{
			Work->ExpectPatchs.AddUnique(AddPatch);
		}
		for (auto DeletePatch : Work->LatOriConfig->DeletePatchs)
		{
			Work->ExpectPatchs.Remove(DeletePatch);
		}
		for (auto AddPatch : Work->LatSavConfig->AddPatchs)
		{
			Work->ExpectPatchs.AddUnique(AddPatch);
		}
		for (auto DeletePatch : Work->LatSavConfig->DeletePatchs)
		{
			Work->ExpectPatchs.Remove(DeletePatch);
		}

		TMap<FString, FAdvancePakFileInfo> StoreFiles;
		FAdvancePakFileInfoVisitor StoreFileInfosVisitor(StoreFiles);
		PlatformFile->IterateDirectory(*UAdvancePakLibrary::StorePaksPath, StoreFileInfosVisitor);

		TMap<FString, FAdvancePakFileInfo> OriOriFiles;
		TMap<FString, FAdvancePakFileInfo> SavSavFiles;
		TMap<FString, FAdvancePakFileInfo> LatSavFiles;
		TMap<FString, FAdvancePakFileInfo> LatOriFiles;

		TArray<FString> OriOriPatchs;
		TArray<FString> SavSavPatchs;
		TArray<FString> LatSavPatchs;
		TArray<FString> LatOriPatchs;

		TArray<FString> AbandonFiles;
		for (auto StoreFile : StoreFiles)
		{
			if (StoreFile.Value.Former.Equals(Config->Version) && StoreFile.Value.Future.Equals(Config->Version) && StoreFile.Value.IsPak())
			{
				if (Work->OriOriConfig->PatchProps.Contains(StoreFile.Value.Name)
					&& (*Work->OriOriConfig->PatchProps.Find(StoreFile.Value.Name))->Size == StoreFile.Value.Size
					&& (*Work->OriOriConfig->PatchProps.Find(StoreFile.Value.Name))->Hash.Equals(StoreFile.Value.Hash)
					&& Work->ExpectPatchs.Contains(StoreFile.Value.GetPatchName()))
				{
					OriOriFiles.Add(StoreFile.Key, StoreFile.Value);
					OriOriPatchs.Add(StoreFile.Value.GetPatchName());
					continue;
				}
			}
			else if (StoreFile.Value.Former.Equals(State->SavedVersion) && StoreFile.Value.Future.Equals(State->SavedVersion) && StoreFile.Value.IsPak())
			{
				if (State->SavedFiles.Contains(GetUnFormatName(StoreFile.Key))
					&& State->SavedFiles.Find(GetUnFormatName(StoreFile.Key))->Size == StoreFile.Value.Size
					&& State->SavedFiles.Find(GetUnFormatName(StoreFile.Key))->Hash.Equals(StoreFile.Value.Hash)
					&& Work->ExpectPatchs.Contains(StoreFile.Value.GetPatchName()))
				{
					SavSavFiles.Add(StoreFile.Key, StoreFile.Value);
					SavSavPatchs.Add(StoreFile.Value.GetPatchName());
					continue;
				}
			}
			else if (StoreFile.Value.Former.Equals(State->SavedVersion) && StoreFile.Value.Future.Equals(Work->LatestVersion) && StoreFile.Value.IsPak())
			{
				if (Work->LatSavConfig->PatchProps.Contains(StoreFile.Value.Name)
					&& (*Work->LatSavConfig->PatchProps.Find(StoreFile.Value.Name))->Size == StoreFile.Value.Size
					&& (*Work->LatSavConfig->PatchProps.Find(StoreFile.Value.Name))->Hash.Equals(StoreFile.Value.Hash)
					&& Work->ExpectPatchs.Contains(StoreFile.Value.GetPatchName()))
				{
					LatSavFiles.Add(StoreFile.Key, StoreFile.Value);
					LatSavPatchs.Add(StoreFile.Value.GetPatchName());
					continue;
				}
			}
			else if (StoreFile.Value.Former.Equals(Config->Version) && StoreFile.Value.Future.Equals(Work->LatestVersion) && StoreFile.Value.IsPak())
			{
				if (Work->LatOriConfig->PatchProps.Contains(StoreFile.Value.Name)
					&& (*Work->LatOriConfig->PatchProps.Find(StoreFile.Value.Name))->Size == StoreFile.Value.Size
					&& (*Work->LatOriConfig->PatchProps.Find(StoreFile.Value.Name))->Hash.Equals(StoreFile.Value.Hash)
					&& Work->ExpectPatchs.Contains(StoreFile.Value.GetPatchName()))
				{
					LatOriFiles.Add(StoreFile.Key, StoreFile.Value);
					LatOriPatchs.Add(StoreFile.Value.GetPatchName());
					continue;
				}
			}
			PlatformFile->DeleteFile(*(UAdvancePakLibrary::StorePaksPath / StoreFile.Key));
			AbandonFiles.Add(StoreFile.Key);
		}

		for (auto AbandonFile : AbandonFiles)
		{
			StoreFiles.Remove(AbandonFile);
		}

		State->SavedFiles.Reset();
		for (auto SavSavFile : SavSavFiles)
		{
			State->SavedFiles.Add(GetUnFormatName(SavSavFile.Key), SavSavFile.Value);
		}
		SaveState();
		
		Work->ExpectLoadInfos = SavSavFiles;
		Work->ExpectMarks.Add(Work->ExpectLoadInfos.Num());

		int Index = 0;
		for (auto PatchName : SavSavPatchs)
		{
			if (Work->LatSavConfig->PatchProps.Contains(SavSavPatchs[Index]))
			{
				TSharedPtr<FAdvancePakPatchProp> PatchProp = *Work->LatSavConfig->PatchProps.Find(SavSavPatchs[Index] + UAdvancePakLibrary::PakSuffixName);
				FAdvancePakFileInfo ExpectLoadInfo;
				ExpectLoadInfo.Name = SavSavPatchs[Index] + UAdvancePakLibrary::PakSuffixName;
				ExpectLoadInfo.Size = PatchProp->Size;
				ExpectLoadInfo.Hash = PatchProp->Hash;
				ExpectLoadInfo.Former = State->SavedVersion;
				ExpectLoadInfo.Future = Work->LatestVersion;
				Work->ExpectLoadInfos.Add(ExpectLoadInfo.GetFormatName(), ExpectLoadInfo);
			}
			Index++;
		}
		
		for (auto LatSaveAdd : Work->LatSavConfig->AddPatchs)
		{
			TSharedPtr<FAdvancePakPatchProp> PatchProp = *Work->LatSavConfig->PatchProps.Find(LatSaveAdd + UAdvancePakLibrary::PakSuffixName);
			FAdvancePakFileInfo ExpectLoadInfo;
			ExpectLoadInfo.Name = LatSaveAdd + UAdvancePakLibrary::PakSuffixName;
			ExpectLoadInfo.Size = PatchProp->Size;
			ExpectLoadInfo.Hash = PatchProp->Hash;
			ExpectLoadInfo.Former = State->SavedVersion;
			ExpectLoadInfo.Future = Work->LatestVersion;
			Work->ExpectLoadInfos.Add(ExpectLoadInfo.GetFormatName(), ExpectLoadInfo);
		}
		Work->ExpectMarks.Add(Work->ExpectLoadInfos.Num());

		AbandonFiles.Reset();
		for (auto LatSavFile : LatSavFiles)
		{
			if (!SavSavPatchs.Contains(LatSavFile.Value.GetPatchName()) && !Work->LatSavConfig->AddPatchs.Contains(LatSavFile.Value.GetPatchName()))
			{
				PlatformFile->DeleteFile(*(UAdvancePakLibrary::StorePaksPath / LatSavFile.Key));
				AbandonFiles.Add(LatSavFile.Key);
				
				LatSavPatchs.Remove(LatSavFile.Value.GetPatchName());
			}
		}
		for (auto AbandonFile : AbandonFiles)
			LatSavFiles.Remove(AbandonFile);

		TArray<FString> LackPatchs;
		for (auto PatchName : Work->ExpectPatchs)
		{
			if (!SavSavPatchs.Contains(PatchName) && Work->LatSavConfig->AddPatchs.Contains(PatchName))
			{
				LackPatchs.Add(PatchName);
			}
		}

		for (auto PatchName : LackPatchs)
		{
			if (Work->OriOriConfig->PatchProps.Contains(PatchName + UAdvancePakLibrary::PakSuffixName))
			{
				TSharedPtr<FAdvancePakPatchProp> PatchProp = *Work->OriOriConfig->PatchProps.Find(PatchName + UAdvancePakLibrary::PakSuffixName);
				FAdvancePakFileInfo ExpectLoadInfo;
				ExpectLoadInfo.Name = PatchName + UAdvancePakLibrary::PakSuffixName;
				ExpectLoadInfo.Size = PatchProp->Size;
				ExpectLoadInfo.Hash = PatchProp->Hash;
				ExpectLoadInfo.Former = Config->Version;
				ExpectLoadInfo.Future = Config->Version;
				Work->ExpectLoadInfos.Add(ExpectLoadInfo.GetFormatName(), ExpectLoadInfo);
			}
		}
		Work->ExpectMarks.Add(Work->ExpectLoadInfos.Num());

		AbandonFiles.Reset();
		for (auto OriOriFile : OriOriFiles)
		{
			if (!LackPatchs.Contains(OriOriFile.Value.GetPatchName()))
			{
				PlatformFile->DeleteFile(*(UAdvancePakLibrary::StorePaksPath / OriOriFile.Key));
				AbandonFiles.Add(OriOriFile.Key);
				
				OriOriPatchs.Remove(OriOriFile.Value.GetPatchName());
			}
		}
		for (auto AbandonFile : AbandonFiles)
			OriOriFiles.Remove(AbandonFile);

		for (auto PatchName : LackPatchs)
		{
			if (Work->LatOriConfig->PatchProps.Contains(PatchName + UAdvancePakLibrary::PakSuffixName))
			{
				TSharedPtr<FAdvancePakPatchProp> PatchProp = *Work->LatOriConfig->PatchProps.Find(PatchName + UAdvancePakLibrary::PakSuffixName);
				FAdvancePakFileInfo ExpectLoadInfo;
				ExpectLoadInfo.Name = PatchName + UAdvancePakLibrary::PakSuffixName;
				ExpectLoadInfo.Size = PatchProp->Size;
				ExpectLoadInfo.Hash = PatchProp->Hash;
				ExpectLoadInfo.Former = Config->Version;
				ExpectLoadInfo.Future = Work->LatestVersion;
				Work->ExpectLoadInfos.Add(ExpectLoadInfo.GetFormatName(), ExpectLoadInfo);
			}
		}

		AbandonFiles.Reset();
		for (auto LatOriFile : LatOriFiles)
		{
			if (!LackPatchs.Contains(LatOriFile.Value.GetPatchName()))
			{
				PlatformFile->DeleteFile(*(UAdvancePakLibrary::StorePaksPath / LatOriFile.Key));
				AbandonFiles.Add(LatOriFile.Key);
				
				LatOriPatchs.Remove(LatOriFile.Value.GetPatchName());
			}
		}
		for (auto AbandonFile : AbandonFiles)
			LatOriFiles.Remove(AbandonFile);

		Index = 0;
		for (auto LoadInfo : Work->ExpectLoadInfos)
		{
			if (Index < Work->ExpectMarks[0])
			{
				
			}
			else if (Index < Work->ExpectMarks[1])
			{
				if (!LatSavFiles.Contains(LoadInfo.Key))
				{
					Work->RequestLoadInfos.Add(LoadInfo.Key, LoadInfo.Value);
				}
			}
			else if (Index < Work->ExpectMarks[2])
			{
				if (!OriOriFiles.Contains(LoadInfo.Key))
				{
					Work->RequestLoadInfos.Add(LoadInfo.Key, LoadInfo.Value);
				}
			}
			else
			{
				if (!LatOriFiles.Contains(LoadInfo.Key))
				{
					Work->RequestLoadInfos.Add(LoadInfo.Key, LoadInfo.Value);
				}
			}

			Index++;
		}

	}

	if (Work->RequestLoadInfos.Num() > 0)
	{
		Work->SwitchEnterProcess(EAdvancePakProcess::LoadPatchs);
	}
	else
	{
		Work->SwitchEnterProcess(EAdvancePakProcess::MergeCheck);
	}
}
