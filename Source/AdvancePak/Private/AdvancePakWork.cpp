// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "AdvancePakWork.h"
#include "PakFile/Public/IPlatformFilePak.h"
#include "Online/HTTP/Public/HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "AdvancePakLibrary.h"

#include "Process/AdvancePakLoadPlatform.h"
#include "Process/AdvancePakLoadConfig.h"
#include "Process/AdvancePakFirstCheck.h"
#include "Process/AdvancePakSecondCheck.h"
#include "Process/AdvancePakLoadPatchs.h"
#include "Process/AdvancePakMergeCheck.h"
#include "Process/AdvancePakMergePatchs.h"
#include "Process/AdvancePakUpdatePatchs.h"
#include "Process/AdvancePakUpdateSucceed.h"

UAdvancePakWork::UAdvancePakWork()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAdvancePakWork::BeginPlay()
{
	Super::BeginPlay();

	check(!PlatformURL.IsEmpty());

	PakFileMgr = (FPakPlatformFile*)(FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile")));

	AdvancePakRecorder = MakeShareable(new FAdvancePakRecorder());

	ProcessList.Add(EAdvancePakProcess::LoadPlatform, MakeShareable(new FAdvancePakLoadPlatform()));
	ProcessList.Add(EAdvancePakProcess::FirstCheck, MakeShareable(new FAdvancePakFirstCheck()));
	ProcessList.Add(EAdvancePakProcess::LoadConfig, MakeShareable(new FAdvancePakLoadConfig()));
	ProcessList.Add(EAdvancePakProcess::SecondCheck, MakeShareable(new FAdvancePakSecondCheck()));
	ProcessList.Add(EAdvancePakProcess::LoadPatchs, MakeShareable(new FAdvancePakLoadPatchs()));
	ProcessList.Add(EAdvancePakProcess::MergeCheck, MakeShareable(new FAdvancePakMergeCheck()));
	ProcessList.Add(EAdvancePakProcess::MergePatchs, MakeShareable(new FAdvancePakMergePatchs()));
	ProcessList.Add(EAdvancePakProcess::UpdatePatchs, MakeShareable(new FAdvancePakUpdatePatchs()));
	ProcessList.Add(EAdvancePakProcess::UpdateSucceed, MakeShareable(new FAdvancePakUpdateSucceed()));

	FNamedAESKey NewKey;
	NewKey.Name = TEXT("Default");
	NewKey.Guid = FGuid();
	FAES::FAESKey OutKey;
	FPakPlatformFile::GetPakEncryptionKey(NewKey.Key, NewKey.Guid);
	KeyChain.EncryptionKeys.Add(NewKey.Guid, NewKey);
	KeyChain.MasterEncryptionKey = KeyChain.EncryptionKeys.Find(NewKey.Guid);
	FCoreDelegates::FPakSigningKeysDelegate& Delegate = FCoreDelegates::GetPakSigningKeysDelegate();
	TArray<uint8> PublicExponent;
	TArray<uint8> Modulus;

	if (UGameplayStatics::DoesSaveGameExist(UAdvancePakLibrary::AdvancePakConfigName, 0))
	{
		AdvancePakConfig = Cast<UAdvancePakConfigSave>(UGameplayStatics::LoadGameFromSlot(UAdvancePakLibrary::AdvancePakConfigName, 0));

		if (Delegate.IsBound() && AdvancePakConfig->PrivateExponent.Num() != 0)
		{
			Delegate.Execute(PublicExponent, Modulus);
			KeyChain.SigningKey = FRSA::CreateKey(PublicExponent, AdvancePakConfig->PrivateExponent, Modulus);
		}
	}
	else
	{
		AdvancePakConfig = Cast<UAdvancePakConfigSave>(UGameplayStatics::CreateSaveGameObject(UAdvancePakConfigSave::StaticClass()));

		// get Origion Data
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		FAdvancePakContentPakVisitor ContentPakVisitor;
		PlatformFile.IterateDirectory(*UAdvancePakLibrary::ContentPaksPath, ContentPakVisitor);
		AdvancePakConfig->IsWithSig = ContentPakVisitor.IsWithSig;
		GConfig->GetString(TEXT("AdvancePak"), TEXT("Platform"), AdvancePakConfig->Platform, UAdvancePakLibrary::DefaultGameIniPath);
		FString PrivateExponentValue;
		GConfig->GetString(TEXT("AdvancePak"), TEXT("PrivateExponent"), PrivateExponentValue, UAdvancePakLibrary::DefaultGameIniPath);
		FBase64::Decode(PrivateExponentValue, AdvancePakConfig->PrivateExponent);
		if (Delegate.IsBound() && AdvancePakConfig->PrivateExponent.Num() != 0)
		{
			Delegate.Execute(PublicExponent, Modulus);
			KeyChain.SigningKey = FRSA::CreateKey(PublicExponent, AdvancePakConfig->PrivateExponent, Modulus);
		}

		bool IsInitOrigion = false;

		TArray<uint8> ConfigBuffer;
		if (UAdvancePakLibrary::ExtractSingleFile(*ContentPakVisitor.ContentPakPath, *UAdvancePakLibrary::DefaultGameIniPath, ConfigBuffer, KeyChain))
		{
			FString CacheGameIniPath = FPaths::ProjectSavedDir() / FString("CacheGame.ini");
			FFileHelper::SaveArrayToFile(ConfigBuffer, *CacheGameIniPath);
			IsInitOrigion = GConfig->GetString(TEXT("AdvancePak"), TEXT("Version"), AdvancePakConfig->Version, UAdvancePakLibrary::DefaultGameIniPath);
			PlatformFile.DeleteFile(*CacheGameIniPath);
		}

		if (!IsInitOrigion)
		{
			GConfig->GetString(TEXT("AdvancePak"), TEXT("Version"), AdvancePakConfig->Version, UAdvancePakLibrary::DefaultGameIniPath);
		}
	}

	CmdParams.bSign = AdvancePakConfig->IsWithSig;

	if (UGameplayStatics::DoesSaveGameExist(UAdvancePakLibrary::AdvancePakStateName, 0))
	{
		AdvancePakState = Cast<UAdvancePakStateSave>(UGameplayStatics::LoadGameFromSlot(UAdvancePakLibrary::AdvancePakStateName, 0));
	}
	else
	{
		AdvancePakState = Cast<UAdvancePakStateSave>(UGameplayStatics::CreateSaveGameObject(UAdvancePakStateSave::StaticClass()));
	}

	for (auto ProcessPtr : ProcessList)
		ProcessPtr.Value->InitializeProcess(this);
}

void UAdvancePakWork::RunProcess()
{
	SwitchEnterProcess(EAdvancePakProcess::LoadPlatform);
}

void UAdvancePakWork::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!AdvancePakRecorder.IsValid())
		return;

	AdvancePakRecorder->UpdateRecorder(DeltaTime);
	if (AdvancePakRecorder->HasProgressChanged())
		OnUpdateProgressNotify.Broadcast(AdvancePakRecorder->GetDownLoadProgress());
	if (AdvancePakRecorder->HasSpeedChanged())
		OnUpdateSpeedNotify.Broadcast(AdvancePakRecorder->GetDownLoadSpeed());
}

void UAdvancePakWork::SwitchEnterProcess(const EAdvancePakProcess& InstallProcess)
{
	if (InstallProcess == ActiveProcess)
		return;

	ActiveProcess = InstallProcess;
	OnProcessNotify.Broadcast(ActiveProcess);
	(*ProcessList.Find(ActiveProcess))->EnterProcess();
}



