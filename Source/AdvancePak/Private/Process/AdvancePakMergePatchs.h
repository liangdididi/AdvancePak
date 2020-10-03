#pragma once
#include "CoreMinimal.h"
#include "AdvancePakProcess.h"

class FAdvancePakThread;

class FAdvancePakMergePatchs : public FAdvancePakProcess
{
public:

	virtual void EnterProcess() override;

	void OnExtractFormerFailed();

	void OnExtractFutureFailed();

	void OnCreateMergeFailed();

	void OnCreateMergeSucceed();

	bool CheckMerge();

	int32 MergeIndex;

	mutable TSharedPtr<FAdvancePakThread> MergeThread;
};