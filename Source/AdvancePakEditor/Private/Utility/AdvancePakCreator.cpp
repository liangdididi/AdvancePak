// Fill out your copyright notice in the Futurecription page of Project Configures.

#include "AdvancePakCreator.h"
#include "Forms/SAdvancePakPublishResourcesForm.h"
#include "Forms/SAdvancePakAccordsForm.h"
#include "Forms/SAdvancePakInvolveRegularsForm.h"
#include "Forms/SAdvancePakPatchResourcesForm.h"
#include "Forms/SAdvancePakInformalsForm.h"
#include "Utility/AdvancePakStateThread.h"
#include "Forms/SAdvancePakGathersForm.h"
#include "Forms/SAdvancePakRelyonsForm.h"
#include "Forms/SAdvancePakAbsolveRegularsForm.h"

FAdvancePakCreator::FAdvancePakCreator(const TSharedRef<ILauncherProfileManager>& ProfileManager)
{
	Profile = ProfileManager->AddNewProfile();
	ProfileManager->RemoveProfile(Profile.ToSharedRef());
	UAdvancePakEditorLibrary::InitializeLibrary();
	IsFuturePublishDirty = true;
	IsFormerPublishDirty = false;
	IsFormerPatchDirty = false;
	IsPackaging = false;
	IsBuilding = false;
	IsCooking = false;
	AssetRegistryModule = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IsPublishFormerRegularsStateRunning = false;
	IsPublishFormerInformalsStateRunning = false;
	IsPatchFormerPublishRegularsStateRunning = false;
	IsPatchFormerPublishInformalsStateRunning = false;
	IsPatchFormerPatchRegularsStateRunning = false;
	IsPatchFormerPatchInformalsStateRunning = false;
	PublishName = FName(TEXT("Publish"));
	ResourceNameName = FName(TEXT("ResourceName"));
	GatherPathName = FName(TEXT("GatherPath"));
	GatherModeName = FName(TEXT("GatherMode"));
	PackageNameName = FName(TEXT("PackageName"));
	PatchNameName = FName(TEXT("Patch"));
	UnViewName = FName(TEXT("UnView"));
	RelyonTypeName = FName(TEXT("Relyon"));
	GatherTypeName = FName(TEXT("GatherType"));
	BrowseName = FName(TEXT("Bw"));
}

FName FAdvancePakCreator::GetFutureRegularPatchName(const FName& PackageName)
{
	if (!FuturePublish.IsValid())
	{
		return NAME_None;
	}
	for (auto Regular : FuturePublish->Regulars)
	{
		if (Regular.IsEqual(PackageName))
		{
			return PublishName;
		}
	}
	for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(FuturePatchs); It; ++It)
	{
		for (auto Regular : It.Value()->Regulars)
		{
			if (Regular.IsEqual(PackageName))
			{
				return It.Key();
			}
		}
	}
	return NAME_None;
}

EAdvancePakResourceState FAdvancePakCreator::GetRegularResourceState(const FName& PackageName)
{
	if (!CheckRegularExist(PackageName))
	{
		return EAdvancePakResourceState::InValid;
	}
	if (!GetFutureRegularPatchName(PackageName).IsNone())
	{
		return EAdvancePakResourceState::Allot;
	}
	return EAdvancePakResourceState::Free;
}

bool FAdvancePakCreator::CheckRegularExist(const FName& PackageName)
{
	if (RegularsVaildMap.Contains(PackageName))
	{
		return *RegularsVaildMap.Find(PackageName);
	}

	TArray<FAssetData> ResourceDatas;
	if (AssetRegistryModule->Get().GetAssetsByPackageName(PackageName, ResourceDatas) && ResourceDatas.Num() > 0 && !ResourceDatas[0].IsRedirector())
	{
		RegularsVaildMap.Add(PackageName, true);
		return true;
	}

	RegularsVaildMap.Add(PackageName, false);
	return false;
}

const TSharedPtr<FAdvancePakPublishConfigure> FAdvancePakCreator::GetFuturePublish() const
{
	return FuturePublish;
}

bool FAdvancePakCreator::IsActivedPatch() const
{
	return !ActiveName.IsNone() && !ActiveName.IsEqual(PublishName);
}

TSharedPtr<FAdvancePakPatchConfigure> FAdvancePakCreator::GetFormerPatch(const FName& PatchName /*= NAME_None*/)
{
	if (FormerPatchs.Contains(PatchName.IsNone() ? ActiveName : PatchName))
	{
		return *FormerPatchs.Find(PatchName.IsNone() ? ActiveName : PatchName);
	}
	return TSharedPtr<FAdvancePakPatchConfigure>();
}

TSharedPtr<FAdvancePakPatchConfigure> FAdvancePakCreator::GetFuturePatch(const FName& PatchName)
{
	if (FuturePatchs.Contains(PatchName.IsNone() ? ActiveName : PatchName))
	{
		return *FuturePatchs.Find(PatchName.IsNone() ? ActiveName : PatchName);
	}
	return TSharedPtr<FAdvancePakPatchConfigure>();
}

EAdvancePakResourceState FAdvancePakCreator::GetInformalResourceState(const FName& InformalName, const FString& InformalPath)
{
	if (!IFileManager::Get().FileExists(*InformalPath))
	{
		return EAdvancePakResourceState::InValid;
	}

	FName TempName = NAME_None;
	if (FuturePublish.IsValid())
	{
		for (auto Informal : FuturePublish->Informals)
		{
			if (Informal->InformalName.IsEqual(InformalName))
			{
				TempName = PublishName;
				break;
			}
		}
		if (TempName.IsNone())
		{
			for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(FuturePatchs); It; ++It)
			{
				for (auto Informal : It.Value()->Informals)
				{
					if (Informal->InformalName.IsEqual(InformalName))
					{
						TempName = It.Key();
						break;
					}
				}
			}
		}
	}

	if (!TempName.IsNone())
	{
		return EAdvancePakResourceState::Allot;
	}
	return EAdvancePakResourceState::Free;
}

