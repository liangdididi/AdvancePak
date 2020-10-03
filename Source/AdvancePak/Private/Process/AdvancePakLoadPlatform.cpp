#include "AdvancePakLoadPlatform.h"
#include "AdvancePakWork.h"
#include "AdvancePakLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Online/HTTP/Public/HttpModule.h"
#include "AdvancePakType.h"
#include "AdvancePakAgency.h" 
#include "Misc/Paths.h"
#include "PakFile/Public/IPlatformFilePak.h"

void FAdvancePakLoadPlatform::EnterProcess()
{
	FHttpRequestPtr NewHttpRequest = FHttpModule::Get().CreateRequest();
	NewHttpRequest->SetURL(Work->PlatformURL);
	NewHttpRequest->SetVerb(TEXT("GET"));
	NewHttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	NewHttpRequest->OnProcessRequestComplete().BindRaw(this, &FAdvancePakLoadPlatform::RequestPlatformInfoCompleted);
	NewHttpRequest->ProcessRequest();
}

void FAdvancePakLoadPlatform::RequestPlatformInfoCompleted(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucceed)
{
	if (!IsSucceed)
	{
		Work->OnLoadPlatformInfoFailed.Broadcast();
		return;
	}

	FString VersionURL = UAdvancePakLibrary::GetVersionURL(HttpResponse->GetContentAsString(), Config->Platform);
	if (VersionURL.IsEmpty())
	{
		Work->OnLoadPlatformInfoFailed.Broadcast();
		return;
	}
	UAdvancePakLibrary::VersionRootURL = FPaths::GetPath(VersionURL);

	FHttpRequestPtr NewHttpRequest = FHttpModule::Get().CreateRequest();
	NewHttpRequest->SetURL(VersionURL);
	NewHttpRequest->SetVerb(TEXT("GET"));
	NewHttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	NewHttpRequest->OnProcessRequestComplete().BindRaw(this, &FAdvancePakLoadPlatform::RequestVersionInfoCompleted);
	NewHttpRequest->ProcessRequest();
}

void FAdvancePakLoadPlatform::RequestVersionInfoCompleted(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucceed)
{
	if (!IsSucceed)
	{
		Work->OnLoadVersionInfoFailed.Broadcast();
		return;
	}

	FString VersionValue = HttpResponse->GetContentAsString();
	TArray<FString> VersionList;
	VersionValue.ParseIntoArray(VersionList, TEXT("\n"));

	for (int i = 0; i < VersionList.Num(); ++i)
	{
		VersionList[i].RemoveFromEnd(FString("\r"));
	}

	if (VersionList.Num() == 0 || !VersionList.Contains(Config->Version))
	{
		Work->OnLoadVersionInfoFailed.Broadcast();
		return;
	}

	Work->LatestVersion = VersionList[0];

	Work->SwitchEnterProcess(EAdvancePakProcess::FirstCheck);
}