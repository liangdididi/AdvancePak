#include "AdvancePakLoadConfig.h"
#include "AdvancePakWork.h"
#include "AdvancePakLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Online/HTTP/Public/HttpModule.h"
#include "AdvancePakType.h"
#include "AdvancePakAgency.h" 
#include "Misc/Paths.h"
#include "PakFile/Public/IPlatformFilePak.h"

void FAdvancePakLoadConfig::EnterProcess()
{
	RequestIndex = 0;
	
	FHttpRequestPtr NewHttpRequest = FHttpModule::Get().CreateRequest();
	NewHttpRequest->SetURL(Work->RequestConfigMarks[RequestIndex].GetRequestURL());
	NewHttpRequest->SetVerb(TEXT("GET"));
	NewHttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	NewHttpRequest->OnProcessRequestComplete().BindRaw(this, &FAdvancePakLoadConfig::RequestConfigCompleted);
	NewHttpRequest->ProcessRequest();
}

void FAdvancePakLoadConfig::RequestConfigCompleted(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucceed)
{
	if (!IsSucceed)
	{
		Work->OnLoadConfigFailed.Broadcast();
		return;
	}

	Config->ConfigMarks.Add(Work->RequestConfigMarks[RequestIndex], HttpResponse->GetContentAsString());

	SaveConfig();

	RequestIndex++;

	if (RequestIndex < Work->RequestConfigMarks.Num())
	{
		FHttpRequestPtr NewHttpRequest = FHttpModule::Get().CreateRequest();
		NewHttpRequest->SetURL(Work->RequestConfigMarks[RequestIndex].GetRequestURL());
		NewHttpRequest->SetVerb(TEXT("GET"));
		NewHttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		NewHttpRequest->OnProcessRequestComplete().BindRaw(this, &FAdvancePakLoadConfig::RequestConfigCompleted);
		NewHttpRequest->ProcessRequest();
	}
	else
	{
		Work->SwitchEnterProcess(EAdvancePakProcess::SecondCheck);
	}
}
