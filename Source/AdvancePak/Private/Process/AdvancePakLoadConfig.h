#pragma once
#include "CoreMinimal.h"
#include "AdvancePakProcess.h"


class FAdvancePakLoadConfig : public FAdvancePakProcess
{
public:

	virtual void EnterProcess() override;

	void RequestConfigCompleted(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucceed);

	int32 RequestIndex;
};