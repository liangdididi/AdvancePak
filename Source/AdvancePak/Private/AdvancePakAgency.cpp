// Fill out your copyright notice in the Description page of Project Settings.
#include "AdvancePakAgency.h"
#include "Online/HTTP/Public/HttpModule.h"
#include "openssl/md5.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"


#if PLATFORM_APPLE
#if WITH_SSL
#include "Ssl.h"
#endif

/**
 * Apple Response Wrapper which will be used for it's delegates to receive responses.
 */
@interface FHttpResponseAppleWrapper : NSObject
{
	/** Holds the payload as we receive it. */
	TArray<uint8> Payload;
}
/** A handle for the response */
@property(retain) NSHTTPURLResponse* Response;
/** Flag whether the response is ready */
@property BOOL bIsReady;
/** When the response is complete, indicates whether the response was received without error. */
@property BOOL bHadError;
/** When the response is complete, indicates whether the response failed with an error specific to connecting to the host. */
@property BOOL bIsHostConnectionFailure;
/** The total number of bytes written out during the request/response */
@property int32 BytesWritten;

/** Delegate called when we send data. See Apple docs for when/how this should be used. */
-(void)connection:(NSURLConnection*)connection didSendBodyData : (NSInteger)bytesWritten totalBytesWritten : (NSInteger)totalBytesWritten totalBytesExpectedToWrite : (NSInteger)totalBytesExpectedToWrite;
/** Delegate called with we receive a response. See Apple docs for when/how this should be used. */
-(void)connection:(NSURLConnection*)connection didReceiveResponse : (NSURLResponse*)response;
/** Delegate called with we receive data. See Apple docs for when/how this should be used. */
-(void)connection:(NSURLConnection*)connection didReceiveData : (NSData*)data;
/** Delegate called with we complete with an error. See Apple docs for when/how this should be used. */
-(void)connection:(NSURLConnection*)connection didFailWithError : (NSError*)error;
/** Delegate called with we complete successfully. See Apple docs for when/how this should be used. */
-(void)connectionDidFinishLoading:(NSURLConnection*)connection;

#if WITH_SSL
/** Delegate called when the connection is about to validate an auth challenge. We only care about server trust. See Apple docs for when/how this should be used. */
- (void)connection:(NSURLConnection*)connection willSendRequestForAuthenticationChallenge : (NSURLAuthenticationChallenge*)challenge;
#endif

- (TArray<uint8>&)getPayload;
-(int32)getBytesWritten;
@end


/**
 * Apple implementation of an Http response
 */
	class FAdvancePakAgencyResponse : public IHttpResponse
{
public:
	// This is the NSHTTPURLResponse, all our functionality will deal with.
	FHttpResponseAppleWrapper* ResponseWrapper;
	/** Request that owns this response */
	void* Request;
	/** BYTE array to fill in as the response is read via didReceiveData */
	mutable TArray<uint8> Payload;
};

#else

class FAdvancePakAgencyResponse : public IHttpResponse
{
public:
	/** Request that owns this response */
	void* Request;
	/** BYTE array to fill in as the response is read via didReceiveData */
	TArray<uint8> Payload;
	/** Caches how many bytes of the response we've read so far */
	FThreadSafeCounter TotalBytesRead;
	/** Cached key/value header pairs. Parsed once request completes. Only accessible on the game thread. */
	TMap<FString, FString> Headers;
	/** Newly received headers we need to inform listeners about */
	TQueue<TPair<FString, FString>> NewlyReceivedHeaders;
	/** Cached code from completed response */
	int32 HttpCode;
	/** Cached content length from completed response */
	int32 ContentLength;
	/** True when the response has finished async processing */
	int32 volatile bIsReady;
	/** True if the response was successfully received/processed */
	int32 volatile bSucceeded;
};

#endif


static TArray<uint8>& GetAgencyResponsePayload(FHttpRequestPtr HttpRequest)
{
	union FAdvancePakResponseUnion {
		IHttpResponse* Former;
		FAdvancePakAgencyResponse* Future;
	};
	FAdvancePakResponseUnion ResponseUnion;
	ResponseUnion.Former = HttpRequest->GetResponse().Get();

#if PLATFORM_APPLE
	FHttpResponseAppleWrapper* ResponseWrapper = ResponseUnion.Future->ResponseWrapper;
	TArray<uint8>& Payload = [ResponseWrapper getPayload];
#else
	TArray<uint8>& Payload = ResponseUnion.Future->Payload;
#endif

	return Payload;
}

FAdvancePakAgency::FAdvancePakAgency()
{
	TotalFileSize = 0;
	LastReceived = 0;

	HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->OnRequestProgress().BindRaw(this, &FAdvancePakAgency::OnProcessRequestProgress);
	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FAdvancePakAgency::OnProcessRequestComplete);
}

FString FAdvancePakAgency::GetURL()
{
	return HttpRequest->GetURL();
}

FString FAdvancePakAgency::GetURLParameter(const FString& ParameterName)
{
	return HttpRequest->GetURLParameter(ParameterName);
}

FString FAdvancePakAgency::GetHeader(const FString& HeaderName)
{
	return HttpRequest->GetHeader(HeaderName);
}

TArray<FString> FAdvancePakAgency::GetAllHeaders()
{
	return HttpRequest->GetAllHeaders();
}

FString FAdvancePakAgency::GetContentType()
{
	return HttpRequest->GetContentType();
}

int32 FAdvancePakAgency::GetContentLength()
{
	return HttpRequest->GetContentLength();
}

TArray<uint8>& FAdvancePakAgency::GetContent()
{
	return GetAgencyResponsePayload(HttpRequest);
}

FString FAdvancePakAgency::GetVerb()
{
	return HttpRequest->GetVerb();
}

void FAdvancePakAgency::SetVerb(const FString& Verb)
{
	HttpRequest->SetVerb(Verb);
}

void FAdvancePakAgency::SetURL(const FString& URL)
{
	HttpRequest->SetURL(URL);
}

void FAdvancePakAgency::SetContent(const TArray<uint8>& ContentPayload)
{
	HttpRequest->SetContent(ContentPayload);
}

void FAdvancePakAgency::SetContentAsString(const FString& ContentString)
{
	HttpRequest->SetContentAsString(ContentString);
}

void FAdvancePakAgency::SetContentAsStreamedFile(const FString& Filename)
{
	HttpRequest->SetContentAsStreamedFile(Filename);
}

void FAdvancePakAgency::SetHeader(const FString& HeaderName, const FString& HeaderValue)
{
	HttpRequest->SetHeader(HeaderName, HeaderValue);
}

void FAdvancePakAgency::AppendToHeader(const FString& HeaderName, const FString& AdditionalHeaderValue)
{
	HttpRequest->AppendToHeader(HeaderName, AdditionalHeaderValue);
}

bool FAdvancePakAgency::ProcessRequest()
{
	if (TotalFileSize > 0)
	{
		return DoProcessRequest();
	}

	TSharedRef<IHttpRequest> HttpHeadRequest = FHttpModule::Get().CreateRequest();
	HttpHeadRequest->OnHeaderReceived().BindRaw(this, &FAdvancePakAgency::OnProcessHeaderReceived);
	HttpHeadRequest->SetURL(HttpRequest->GetURL());
	HttpHeadRequest->SetVerb(TEXT("HEAD"));
	return HttpHeadRequest->ProcessRequest();
}

void FAdvancePakAgency::CancelRequest()
{
	HttpRequest->CancelRequest();
}

float FAdvancePakAgency::GetElapsedTime()
{
	return HttpRequest->GetElapsedTime();
}

EHttpRequestStatus::Type FAdvancePakAgency::GetStatus()
{
	return HttpRequest->GetStatus();
}

void FAdvancePakAgency::SetStoragePath(const FString& FilePath)
{
	StoragePath = FilePath;
}

FString FAdvancePakAgency::GetStoragePath()
{
	return StoragePath;
}

void FAdvancePakAgency::SetCachePath(const FString& FilePath)
{
	CachePath = FilePath;
}

FString FAdvancePakAgency::GetCachePath()
{
	return CachePath;
}


void FAdvancePakAgency::SetTotalFileSize(uint64 FileSize)
{
	TotalFileSize = FileSize;
}

FHttpRequestPtr FAdvancePakAgency::GetHttpRequest()
{
	return HttpRequest;
}

void FAdvancePakAgency::OnProcessHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
{
	if (HttpRequest->GetStatus() == EHttpRequestStatus::Processing)
		return;

	OnHeaderReceived.ExecuteIfBound(HeaderName, NewHeaderValue);

	if (HeaderName.Equals(TEXT("Content-Length")))
	{
		TotalFileSize = FCString::Strtoui64(*NewHeaderValue, nullptr, 10);

		if (!DoProcessRequest())
			OnRequestComplete.ExecuteIfBound(false);
	}
}

void FAdvancePakAgency::OnProcessRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	TArray<uint8>& ResponseContent = GetAgencyResponsePayload(HttpRequest);

	unsigned char* PaddingContent = const_cast<unsigned char*>(ResponseContent.GetData() + LastReceived);

	if (FFileHelper::SaveArrayToFile(TArrayView<const uint8>(PaddingContent, BytesReceived - LastReceived), *CachePath, &IFileManager::Get(), EFileWrite::FILEWRITE_Append | EFileWrite::FILEWRITE_AllowRead | EFileWrite::FILEWRITE_EvenIfReadOnly))
	{
		OnProgressDelegate.ExecuteIfBound(BytesReceived - LastReceived);

		LastReceived = BytesReceived;
	}
}

bool FAdvancePakAgency::DoProcessRequest()
{
	if (IFileManager::Get().FileExists(*CachePath))
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		uint64 ExsitFileSize = (uint64)PlatformFile.FileSize(*CachePath);

		if (ExsitFileSize == TotalFileSize)
		{
			IFileManager::Get().Move(*StoragePath, *CachePath);
			OnRequestComplete.ExecuteIfBound(true);
			return true;
		}
		else if (ExsitFileSize < TotalFileSize)
		{
			FString RangeArgs = FString::Printf(TEXT("bytes=%llu-%llu"), ExsitFileSize, TotalFileSize - 1);
			HttpRequest->SetHeader(FString("Range"), RangeArgs);
			return HttpRequest->ProcessRequest();
		}
		else if (ExsitFileSize > TotalFileSize)
		{
			IFileManager::Get().Delete(*CachePath);
			ExsitFileSize = 0;
			return HttpRequest->ProcessRequest();
		}
	}

	return HttpRequest->ProcessRequest();
}

void FAdvancePakAgency::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	uint64 CacheFileSize = (uint64)PlatformFile.FileSize(*CachePath);
	if (TotalFileSize > CacheFileSize && Response->GetContent().Num() > LastReceived)
	{
		int32 SaveCounter = 0;
		unsigned char* PaddingContent = const_cast<unsigned char*>(Response->GetContent().GetData() + LastReceived);

		while (!FFileHelper::SaveArrayToFile(TArrayView<const uint8>(PaddingContent, Response->GetContent().Num() - LastReceived), *CachePath, &IFileManager::Get(), EFileWrite::FILEWRITE_Append | EFileWrite::FILEWRITE_AllowRead | EFileWrite::FILEWRITE_EvenIfReadOnly))
		{
			SaveCounter++;
			if (SaveCounter > 3)
			{
				OnRequestComplete.ExecuteIfBound(false);
				return;
			}
		}

		OnProgressDelegate.ExecuteIfBound(Response->GetContent().Num() - LastReceived);

		IFileManager::Get().Move(*StoragePath, *CachePath);
		OnRequestComplete.ExecuteIfBound(true);

		return;
	}
	else if (TotalFileSize == CacheFileSize && Response->GetContent().Num() == LastReceived)
	{
		IFileManager::Get().Move(*StoragePath, *CachePath);
		OnRequestComplete.ExecuteIfBound(true);
		return;
	}

	OnRequestComplete.ExecuteIfBound(false);
}
