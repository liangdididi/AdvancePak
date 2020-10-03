// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "Containers/Array.h"
#include "Delegates/Delegate.h"
#include "GameProjectHelper.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/App.h"
#include "Templates/SharedPointer.h"
#include "ILauncher.h"
#include "ILauncherProfileManager.h"
#include "AssetRegistryModule.h"
#include "Misc/Paths.h"
#include "AdvancePakEditorType.h"
#include "Utility/AdvancePakEditorLibrary.h"

class FAdvancePakStateThread;

class FAdvancePakCreator
{

public:

	FAdvancePakCreator(const TSharedRef<ILauncherProfileManager>& ProfileManager);

	~FAdvancePakCreator() {}

	bool IsActivedPatch() const;

	TSharedPtr<FAdvancePakPatchConfigure> GetFormerPatch(const FName& PatchName = NAME_None);

	TSharedPtr<FAdvancePakPatchConfigure> GetFuturePatch(const FName& PatchName = NAME_None);

	FName GetFutureRegularPatchName(const FName& PackageName);

	const TSharedPtr<FAdvancePakPublishConfigure> GetFuturePublish() const;

	bool CheckRegularExist(const FName& PackageName);

	EAdvancePakResourceState GetInformalResourceState(const FName& InformalName, const FString& InformalPath);

	EAdvancePakResourceState GetRegularResourceState(const FName& PackageName);

public:

	DECLARE_MULTICAST_DELEGATE(FAdvancePakStatusChanged)

	bool IsPackaging;

	bool IsFormerPublishDirty;

	FAdvancePakStatusChanged OnActiveNameChanged;

	bool IsPublishFormerRegularsStateRunning;

	bool IsFormerPatchDirty;

	TSharedPtr<ILauncherProfile> Profile;

	mutable TSharedPtr<FAdvancePakStateThread> PatchFormerPatchRegularsStateThread;

	bool IsBuilding;

	bool IsCooking;

	FName ActiveName;

	TMap<FName, bool> RegularsVaildMap;

	mutable TSharedPtr<FAdvancePakStateThread> PatchFormerPatchInformalsStateThread;

	TSharedPtr<FAdvancePakPublishConfigure> FormerPublish;

	TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>> FormerPatchs;

	bool IsPatchFormerPublishRegularsStateRunning;

	bool IsFuturePublishDirty;

	bool IsPatchFormerPublishInformalsStateRunning;

	TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>> FuturePatchs;

	mutable TSharedPtr<FAdvancePakStateThread> PatchFormerPublishInformalsStateThread;

	mutable TSharedPtr<FAdvancePakStateThread> PublishFormerRegularsStateThread;

	bool IsPatchFormerPatchInformalsStateRunning;

	TSharedPtr<FAdvancePakPublishConfigure> FuturePublish;

	mutable TSharedPtr<FAdvancePakStateThread> PatchFormerPublishRegularsStateThread;

	FAdvancePakStatusChanged OnTotalConfiguresChanged;

	mutable TSharedPtr<FAdvancePakStateThread> PublishFormerInformalsStateThread;

	bool IsPatchFormerPatchRegularsStateRunning;

	bool IsPublishFormerInformalsStateRunning;

	TSharedPtr<class SAdvancePakPublishResourcesForm> FormerRegularsForm;

	TSharedPtr<class SAdvancePakPublishResourcesForm> FormerInformalsForm;

	TSharedPtr<class SAdvancePakPublishResourcesForm> FutureRegularsForm;

	TSharedPtr<class SAdvancePakPublishResourcesForm> FutureInformalsForm;

	TSharedPtr<class SAdvancePakAccordsForm> PublishAccordsForm;

	TSharedPtr<class SAdvancePakAccordsForm> PatchAccordsForm;

	TSharedPtr<class SAdvancePakPatchResourcesForm> FuturePatchRegularsForm;

	TSharedPtr<class SAdvancePakPatchResourcesForm> FuturePatchInformalsForm;

	TSharedPtr<class SAdvancePakGathersForm> InvolveGathersForm;

	TSharedPtr<class SAdvancePakAbsolveRegularsForm> AbsolveRegularsForm;

	TSharedPtr<class SAdvancePakInformalsForm> InvolveInformalsForm;

	TSharedPtr<class SAdvancePakInformalsForm> AbsolveInformalsForm;

	TSharedPtr<class SAdvancePakRelyonsForm> RelyonsForm;

	TSharedPtr<class SAdvancePakPublishResourcesForm> FormerPublishInformalsForm;

	TSharedPtr<class SAdvancePakPatchResourcesForm> FormerPatchRegularsForm;

	TSharedPtr<class SAdvancePakPatchResourcesForm> FormerPatchInformalsForm;

	TSharedPtr<class SAdvancePakInvolveRegularsForm> InvolveRegularsForm;

	TSharedPtr<class SAdvancePakGathersForm> AbsolveGathersForm;

	TSharedPtr<class SAdvancePakPublishResourcesForm> FormerPublishRegularsForm;

	bool IsPublishReduced;

	bool IsPublishAccordFaild;

	FAssetRegistryModule* AssetRegistryModule;

	TArray<TSharedPtr<FAdvancePakAccordInfo>> PublishAccords;

	TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> PublishFutureInformals;

	TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> PublishFutureRegulars;

	TArray<FName> CacheFormerRegulars;

	TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> FormerRegulars;

	TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> FormerInformals;

	TArray<FName> CacheFormerInformals;

	TMap<FName, FString> PatchFutureInformals;

	TArray<FName> PatchFutureRegulars;

	bool IsPatchAccordFaild;

	bool IsPatchReduced;

	TArray<TSharedPtr<FAdvancePakAccordInfo>> PatchAccords;

	TArray<TSharedPtr<FAdvancePakPatchResourceInfo>> FuturePatchInformals;

	TArray<TSharedPtr<FAdvancePakGatherOption>> InvolveGathers;

	TArray<TSharedPtr<FName>> AbsolveRegulars;

	TArray<TSharedPtr<FAdvancePakInformalInfo>> InvolveInformals;

	TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> FormerPublishRegulars;

	TMap<FName, FString> AbsolveGathersInformals;

	TArray<TSharedPtr<FAdvancePakPatchResourceInfo>> FormerPatchRegulars;

	TArray<FName> AbsolveGathersRegulars;

	TArray<TSharedPtr<FAdvancePakPatchResourceInfo>> FormerPatchInformals;

	TArray<TSharedPtr<FAdvancePakPatchResourceInfo>> FuturePatchRegulars;

	TArray<TSharedPtr<FAdvancePakGatherOption>> AbsolveGathers;

	TArray<TSharedPtr<FAdvancePakRegularOption>> InvolveRegulars;

	TArray<TSharedPtr<FAdvancePakInformalInfo>> AbsolveInformals;

	TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> FormerPublishInformals;

	TArray<TSharedPtr<FAdvancePakRelyonInfo>> Relyons;

	TArray<FName> InvolveGathersRegulars;

	TArray<FName> InvolveRegularsRegulars;

	TArray<FName> AbsolveRegularsRegulars;

	TMap<FName, FString> InvolveInformalsInformals;

	TMap<FName, FString> AbsolveInformalsInformals;

	TMap<FName, FString> InvolveGathersInformals;

	FName ResourceNameName;

	FName GatherTypeName;

	FName PublishName;

	FName PatchNameName;

	FName GatherPathName;

	FName GatherModeName;

	FName UnViewName;

	FName BrowseName;

	FName PackageNameName;

	FName RelyonTypeName;
};