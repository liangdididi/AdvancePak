#include "AdvancePakLoadPatchs.h"
#include "AdvancePakWork.h"
#include "AdvancePakLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Online/HTTP/Public/HttpModule.h"
#include "AdvancePakType.h"
#include "AdvancePakAgency.h" 
#include "Misc/Paths.h"
#include "PakFile/Public/IPlatformFilePak.h"

void FAdvancePakLoadPatchs::EnterProcess()
{
	uint64 TotalSize = 0;

	TMap<FString, FAdvancePakFileInfo> BufferFiles;
	FAdvancePakFileInfoVisitor BufferFileInfosVisitor(BufferFiles);
	PlatformFile->IterateDirectory(*UAdvancePakLibrary::BufferPaksPath, BufferFileInfosVisitor);

	if (BufferFiles.Num() > 1)
	{
		DeleteFilsInDirectory(UAdvancePakLibrary::BufferPaksPath);
		BufferFiles.Reset();
	}
	else if (BufferFiles.Num() == 1)
	{
		if (Work->RequestLoadInfos.Contains(State->LoadingInfo.GetFormatName())
			&& Work->RequestLoadInfos.Find(State->LoadingInfo.GetFormatName())->IsEqual(State->LoadingInfo)
			&& BufferFiles.Contains(State->LoadingInfo.GetFormatName())
			&& BufferFiles.begin().Value().Former.Equals(State->LoadingInfo.Former)
			&& BufferFiles.begin().Value().Future.Equals(State->LoadingInfo.Future))
		{
			if (BufferFiles.begin().Value().Size == State->LoadingInfo.Size)
			{
				PlatformFile->MoveFile(*(UAdvancePakLibrary::BufferPaksPath / State->LoadingInfo.GetFormatName()), *(UAdvancePakLibrary::StorePaksPath / State->LoadingInfo.GetFormatName()));
				Work->RequestLoadInfos.Remove(State->LoadingInfo.GetFormatName());
			}
			else if (BufferFiles.begin().Value().Size < State->LoadingInfo.Size)
			{
				TotalSize += State->LoadingInfo.Size - BufferFiles.begin().Value().Size;
				RequestList.Add(State->LoadingInfo.GetFormatName());
			}
			else
			{
				DeleteFilsInDirectory(UAdvancePakLibrary::BufferPaksPath);
				BufferFiles.Reset();
			}
		}
		else
		{
			DeleteFilsInDirectory(UAdvancePakLibrary::BufferPaksPath);
			BufferFiles.Reset();
		}
	}

	if (RequestList.Num() == 0)
	{
		for (auto LoadInfo : Work->RequestLoadInfos)
		{
			TotalSize += LoadInfo.Value.Size;
			RequestList.Add(LoadInfo.Key);
		}
	}
	else
	{
		FString BufferFile = RequestList[0];
		for (auto LoadInfo : Work->RequestLoadInfos)
		{
			if (!LoadInfo.Key.Equals(BufferFile))
			{
				TotalSize += LoadInfo.Value.Size;
				RequestList.Add(LoadInfo.Key);
			}
		}
	}

	uint64 MergeSize = 0;
	for (auto LoadInfo : Work->ExpectLoadInfos)
	{
		MergeSize += LoadInfo.Value.Size;
	}
	uint64 TotalDiskFreeSpace = UAdvancePakLibrary::GetDiskFreeSpace();
	if (TotalSize + MergeSize > TotalDiskFreeSpace)
	{
		Work->OnDiskFreeSpaceLacked.Broadcast((float)FUnitConversion::Convert((double)(TotalSize + MergeSize - TotalDiskFreeSpace), EUnit::Bytes, EUnit::Megabytes));
		return;
	}

	if (RequestList.Num() == 0)
	{
		if (Work->ExpectConfigMarks.Num() == 1 && Work->ExpectConfigMarks[0] == FAdvancePakConfigMark(Config->Version, Config->Version))
		{
			Work->SwitchEnterProcess(EAdvancePakProcess::UpdatePatchs);
		}
		else
		{
			Work->SwitchEnterProcess(EAdvancePakProcess::MergeCheck);
		}
	}
	else
	{
		RequestIndex = 0;

		State->LoadingInfo = *Work->RequestLoadInfos.Find(RequestList[RequestIndex]);
		SaveState();

		AdvancePakAgency = MakeShareable(new FAdvancePakAgency());
		AdvancePakAgency->SetVerb(TEXT("GET"));
		AdvancePakAgency->SetCachePath(UAdvancePakLibrary::BufferPaksPath / RequestList[RequestIndex]);
		AdvancePakAgency->SetStoragePath(UAdvancePakLibrary::StorePaksPath / RequestList[RequestIndex]);
		AdvancePakAgency->SetTotalFileSize(State->LoadingInfo.Size);
		AdvancePakAgency->SetURL(State->LoadingInfo.GetRequestURL());
		AdvancePakAgency->OnProgressDelegate.BindRaw(this, &FAdvancePakLoadPatchs::RequestPatchsProgress);
		AdvancePakAgency->OnRequestComplete.BindRaw(this, &FAdvancePakLoadPatchs::RequestPatchsCompleted);
		AdvancePakAgency->ProcessRequest();

		Work->AdvancePakRecorder->EnterRecorder(TotalSize);
	}
}

void FAdvancePakLoadPatchs::RequestPatchsCompleted(bool IsSucceed)
{
	if (!IsSucceed)
	{
		Work->OnLoadPatchsFailed.Broadcast();
		return;
	}

	RequestIndex++;
	if (RequestIndex < RequestList.Num())
	{
		State->LoadingInfo = *Work->RequestLoadInfos.Find(RequestList[RequestIndex]);
		SaveState();

		AdvancePakAgency = MakeShareable(new FAdvancePakAgency());
		AdvancePakAgency->SetVerb(TEXT("GET"));
		AdvancePakAgency->SetCachePath(UAdvancePakLibrary::BufferPaksPath / RequestList[RequestIndex]);
		AdvancePakAgency->SetStoragePath(UAdvancePakLibrary::StorePaksPath / RequestList[RequestIndex]);
		AdvancePakAgency->SetTotalFileSize(State->LoadingInfo.Size);
		AdvancePakAgency->SetURL(State->LoadingInfo.GetRequestURL());
		AdvancePakAgency->OnProgressDelegate.BindRaw(this, &FAdvancePakLoadPatchs::RequestPatchsProgress);
		AdvancePakAgency->OnRequestComplete.BindRaw(this, &FAdvancePakLoadPatchs::RequestPatchsCompleted);
		AdvancePakAgency->ProcessRequest();
	}
	else
	{
		Work->AdvancePakRecorder->ExitRecorder();
		Work->OnUpdateProgressNotify.Broadcast(1.f);

		if (Work->ExpectConfigMarks.Num() == 1 && Work->ExpectConfigMarks[0] == FAdvancePakConfigMark(Config->Version, Config->Version))
		{
			Work->SwitchEnterProcess(EAdvancePakProcess::UpdatePatchs);
		}
		else
		{
			Work->SwitchEnterProcess(EAdvancePakProcess::MergeCheck);
		}
	}
}

void FAdvancePakLoadPatchs::RequestPatchsProgress(int32 BytesReceived)
{
	Work->AdvancePakRecorder->PeddingReceived(BytesReceived);
}

