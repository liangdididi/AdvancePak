// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"

class UAdvancePakWork;
class FAdvancePakAgency;
class UAdvancePakConfigSave;
class UAdvancePakStateSave;
class IPlatformFile;
class IFileManager;

class FAdvancePakProcess
{
public:

	FAdvancePakProcess() {};

	virtual ~FAdvancePakProcess() {}

	void InitializeProcess(UAdvancePakWork* InAdvancePakWork);

	virtual void ProcessTick(float DeltaTime) {}

	virtual void EnterProcess() {}

	void SaveConfig();

	void SaveState();

	bool Mount(const FString& PakFile, uint32 PakOrder = 0);

	bool UnMount(const FString& PakFile);

	FString GetFormatName(const FString& FileName, const FString& Former, const FString& Future) const;

	FString GetUnFormatName(const FString& FileName) const;

	FString GetPatchName(const FString& FileName) const;

	void DeleteFilsInDirectory(const FString& Directory);

	void DeleteFilsInSaved();

	// 检查Merge文件夹与最新版本, 保存State数据
	void CheckMergeState();

	UAdvancePakWork* Work;

	UAdvancePakConfigSave* Config;

	UAdvancePakStateSave* State;

	IPlatformFile* PlatformFile;
};

