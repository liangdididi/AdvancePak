// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"

class FAdvancePakAgency
{
public:
 
	FAdvancePakAgency();

	FString GetURL();

	FString GetURLParameter(const FString& ParameterName);

	FString GetHeader(const FString& HeaderName);

	TArray<FString> GetAllHeaders();

	FString GetContentType();

	int32 GetContentLength();

	TArray<uint8>& GetContent();

	FString GetVerb();

	void SetVerb(const FString& Verb);

	void SetURL(const FString& URL);

	void SetContent(const TArray<uint8>& ContentPayload);

	void SetContentAsString(const FString& ContentString);

	void SetContentAsStreamedFile(const FString& Filename);

	void SetHeader(const FString& HeaderName, const FString& HeaderValue);

	void AppendToHeader(const FString& HeaderName, const FString& AdditionalHeaderValue);

	bool ProcessRequest();

	void CancelRequest();

	float GetElapsedTime();

	EHttpRequestStatus::Type GetStatus();

	void SetStoragePath(const FString& FilePath);

	FString GetStoragePath();

	void SetCachePath(const FString& FilePath);

	FString GetCachePath();

	void SetTotalFileSize(uint64 FileSize);

	FHttpRequestPtr GetHttpRequest();

public:

	DECLARE_DELEGATE_OneParam(FFAdvancePakAgencyCompleteDelegate, bool);

	DECLARE_DELEGATE_TwoParams(FFAdvancePakAgencyHeaderReceivedDelegate, FString, FString);

	DECLARE_DELEGATE_OneParam(FFAdvancePakAgencyProgressDelegate, int32);

	FFAdvancePakAgencyCompleteDelegate OnRequestComplete;

	FFAdvancePakAgencyHeaderReceivedDelegate OnHeaderReceived;

	FFAdvancePakAgencyProgressDelegate OnProgressDelegate;

protected:

	bool DoProcessRequest();

	void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void OnProcessRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);

	void OnProcessHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue);

protected:

	FHttpRequestPtr HttpRequest;

	FString StoragePath;

	FString CachePath;

	uint64 TotalFileSize;

	int32 LastReceived;
};
