// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "GenericPlatform/GenericPlatformProcess.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FProcOutputDelegate, FString)
DECLARE_MULTICAST_DELEGATE(FProcStateDelegate)

/**
 *
 */
class FAdvancePakProcThread final : public FRunnable
{
public:
	explicit FAdvancePakProcThread(const TCHAR* InThreadName, const FString& InProcURL, const FString& InProcParms) : ThreadName(InThreadName), ProcURL(InProcURL), ProcParams(InProcParms), IsCancel(false){
		UE_LOG(AdvancePakLog, Log, TEXT("%s %s"), *InProcURL, *InProcParms);
	}

	void Execute();

	void Cancel();

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

public:

	FProcOutputDelegate ProcOutputDelegate;
	FProcStateDelegate ProcBeginDelegate;
	FProcStateDelegate ProcSucceedDelegate;
	FProcStateDelegate ProcFailedDelegate;

private:


	FString ThreadName;
	TUniquePtr<FRunnableThread> WorkThread;

	FString ProcURL;
	FString ProcParams;
	void* ReadPipe;
	void* WritePipe;
	uint32 ProcId;
	FProcHandle ProcHandle;
	FString OutputBuffer;
	bool IsCancel;
};
