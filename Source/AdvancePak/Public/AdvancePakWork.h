// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "UObject/NoExportTypes.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AdvancePakType.h"
#include "Interfaces/IHttpRequest.h"
#include "AdvancePakWork.generated.h"

class FPakPlatformFile;
class FAdvancePakProcess;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ADVANCEPAK_API UAdvancePakWork : public USceneComponent
{
	GENERATED_BODY()

public:

	UAdvancePakWork();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SwitchEnterProcess(const EAdvancePakProcess& InstallProcess);

	UFUNCTION(BlueprintCallable, Category = "AdvancePak")
		void RunProcess();

public:

	UPROPERTY(EditAnywhere, Category = "AdvancePak")
		FString PlatformURL;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAdvancePakEventNotify);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdvancePakNumberNotify, float, InProgress);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdvancePakProcessNotify, EAdvancePakProcess, InProcess);

	UPROPERTY(BlueprintAssignable)
		FAdvancePakEventNotify OnLoadPlatformInfoFailed;

	UPROPERTY(BlueprintAssignable)
		FAdvancePakEventNotify OnLoadVersionInfoFailed;

	UPROPERTY(BlueprintAssignable)
		FAdvancePakEventNotify OnLoadConfigFailed;

	UPROPERTY(BlueprintAssignable)
		FAdvancePakNumberNotify OnDiskFreeSpaceLacked;

	UPROPERTY(BlueprintAssignable)
		FAdvancePakEventNotify OnLoadPatchsFailed;

	UPROPERTY(BlueprintAssignable)
		FAdvancePakEventNotify OnMergeFailed;

	UPROPERTY(BlueprintAssignable)
		FAdvancePakEventNotify OnUpdateSucceedNotify;

	UPROPERTY(BlueprintAssignable)
		FAdvancePakNumberNotify OnUpdateProgressNotify;

	UPROPERTY(BlueprintAssignable)
		FAdvancePakNumberNotify OnUpdateSpeedNotify;

	UPROPERTY(BlueprintAssignable)
		FAdvancePakProcessNotify OnProcessNotify;

	TSharedPtr<FAdvancePakRecorder> AdvancePakRecorder;

	TMap<FString, FAdvancePakFileInfo> ExpectLoadInfos;

	TMap<FString, FAdvancePakFileInfo> RequestLoadInfos;

	TArray<FAdvancePakMergeInfo> RequestMergeInfos;

	TArray<FAdvancePakConfigMark> ExpectConfigMarks;

	TArray<FAdvancePakConfigMark> RequestConfigMarks;

	TMap<FAdvancePakConfigMark, TSharedPtr<FAdvancePakVersionConfig>> VersionConfigs;

	TArray<FString> ExpectPatchs;

	TArray<int32> ExpectMarks;

	FKeyChain KeyChain;

	FPakPlatformFile* PakFileMgr;

	FString LatestVersion;

	FPakCommandLineParameters CmdParams;

	TSharedPtr<FAdvancePakVersionConfig> OriOriConfig;
	TSharedPtr<FAdvancePakVersionConfig> LatOriConfig;
	TSharedPtr<FAdvancePakVersionConfig> LatSavConfig;

	UPROPERTY()
		UAdvancePakConfigSave* AdvancePakConfig;

	UPROPERTY()
		UAdvancePakStateSave* AdvancePakState;

protected:

	virtual void BeginPlay() override;

protected:

	TMap<EAdvancePakProcess, TSharedPtr<FAdvancePakProcess>> ProcessList;

	EAdvancePakProcess ActiveProcess;

};
