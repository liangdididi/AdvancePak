// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "AdvancePakType.h"
#include "CoreMinimal.h"

/**
 *
 */
class FAdvancePakThread final : public FRunnable
{
public:
	
	explicit FAdvancePakThread(const TCHAR* InThreadName, const FKeyChain& InKeyChain, TSharedPtr<FAdvancePakPatchInfo> InPatchInfo, const FPakCommandLineParameters& InCmdParams, const FString& InMergePakPath, const FString& InFormerPakPath, const FString& InFuturePakPath) :
	ThreadName(InThreadName), 
	KeyChain(InKeyChain),
	PatchInfo(InPatchInfo),
	CmdParams(InCmdParams),
	MergePakPath(InMergePakPath),
	FormerPakPath(InFormerPakPath),
	FuturePakPath(InFuturePakPath)
	{}

	void Execute();

	void Join();

	virtual void Stop() override;

	virtual void Exit() override;

	virtual uint32 Run() override;

public:

	DECLARE_DELEGATE(FAdvancePakThreadDelegate)

	FAdvancePakThreadDelegate ExtractFormerFailed;

	FAdvancePakThreadDelegate ExtractFutureFailed;

	FAdvancePakThreadDelegate CreateMergeFailed;

	FAdvancePakThreadDelegate CreateMergeSucceed;

private:

	FString ThreadName;
	TUniquePtr<FRunnableThread> WorkThread;

	FKeyChain KeyChain;
	TSharedPtr<FAdvancePakPatchInfo> PatchInfo;
	FPakCommandLineParameters CmdParams;
	FString MergePakPath;
	FString FormerPakPath;
	FString FuturePakPath;
};
