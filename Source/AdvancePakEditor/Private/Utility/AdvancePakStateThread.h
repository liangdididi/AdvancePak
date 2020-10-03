// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "CoreMinimal.h"

DECLARE_MULTICAST_DELEGATE(FStateThreadDelegate)

/**
 *
 */
class FAdvancePakStateThread final : public FRunnable
{
public:
	using FCallback = TFunction<void()>;

	explicit FAdvancePakStateThread(const TCHAR* InThreadName, const FCallback& InRunFunc) : ThreadName(InThreadName), CallBack(InRunFunc) {}

	virtual void Execute();

	virtual void Join();

	virtual void Stop() override;

	virtual void Exit() override;

	virtual uint32 Run() override;

public:

	FStateThreadDelegate StateRunDelagete;
	FStateThreadDelegate StateCompleteDelagete;
	FStateThreadDelegate StateStopDelagete;
	FStateThreadDelegate StateJoinDelagete;
	FStateThreadDelegate StateExitDelagete;

protected:

	FString ThreadName;
	FCallback CallBack;
	TUniquePtr<FRunnableThread> WorkThread;

private:
	FAdvancePakStateThread(const FAdvancePakStateThread&) = delete;
	FAdvancePakStateThread& operator=(const FAdvancePakStateThread&) = delete;
};
