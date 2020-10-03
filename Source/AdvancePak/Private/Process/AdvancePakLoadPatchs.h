#pragma once
#include "CoreMinimal.h"
#include "AdvancePakProcess.h"


class FAdvancePakLoadPatchs : public FAdvancePakProcess
{
public:

	virtual void EnterProcess() override;

	void RequestPatchsCompleted(bool IsSucceed);

	void RequestPatchsProgress(int32 BytesReceived);

	int32 RequestIndex;

	TSharedPtr<FAdvancePakAgency> AdvancePakAgency;

	TArray<FString> RequestList;
};