#pragma once
#include "CoreMinimal.h"
#include "AdvancePakProcess.h"


class FAdvancePakLoadPlatform : public FAdvancePakProcess
{
public:

	virtual void EnterProcess() override;

	void RequestPlatformInfoCompleted(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucceed);

	void RequestVersionInfoCompleted(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucceed);
};