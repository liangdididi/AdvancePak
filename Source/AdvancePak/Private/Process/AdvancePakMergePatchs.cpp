#include "AdvancePakMergePatchs.h"
#include "AdvancePakWork.h"
#include "AdvancePakLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Online/HTTP/Public/HttpModule.h"
#include "AdvancePakType.h"
#include "AdvancePakAgency.h" 
#include "Misc/Paths.h"
#include "PakFile/Public/IPlatformFilePak.h"
#include "Misc/SecureHash.h"
#include "AdvancePakThread.h"

void FAdvancePakMergePatchs::EnterProcess()
{
	int32 TotalMerge = Work->RequestMergeInfos.Num();
	MergeIndex = 0;

	if (!CheckMerge())
	{
		Work->SwitchEnterProcess(EAdvancePakProcess::MergeCheck);
		return;
	}

	MergeThread = MakeShareable(new FAdvancePakThread(
		TEXT("MergeThread"),
		Work->KeyChain,
		Work->RequestMergeInfos[MergeIndex].PatchInfo,
		Work->CmdParams,
		Work->RequestMergeInfos[MergeIndex].GetMergePath(),
		Work->RequestMergeInfos[MergeIndex].GetFormorPath(),
		Work->RequestMergeInfos[MergeIndex].GetFuturePath()
	));

	MergeThread->ExtractFormerFailed.BindRaw(this, &FAdvancePakMergePatchs::OnExtractFormerFailed);
	MergeThread->ExtractFutureFailed.BindRaw(this, &FAdvancePakMergePatchs::OnExtractFutureFailed);
	MergeThread->CreateMergeFailed.BindRaw(this, &FAdvancePakMergePatchs::OnCreateMergeFailed);
	MergeThread->CreateMergeSucceed.BindRaw(this, &FAdvancePakMergePatchs::OnCreateMergeSucceed);

	MergeThread->Execute();

	Work->AdvancePakRecorder->EnterRecorder(TotalMerge);
}

void FAdvancePakMergePatchs::OnExtractFormerFailed()
{
	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		UE_LOG(LogTemp, Log, TEXT("ExtractFormerFailed Former[%s] Future[%s] Merge[%s]"), *Work->RequestMergeInfos[MergeIndex].GetFormorPath(), *Work->RequestMergeInfos[MergeIndex].GetFuturePath(), *Work->RequestMergeInfos[MergeIndex].GetMergePath());
		Work->OnMergeFailed.Broadcast();
	}, TStatId(), NULL, ENamedThreads::GameThread);
}

void FAdvancePakMergePatchs::OnExtractFutureFailed()
{
	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		UE_LOG(LogTemp, Log, TEXT("ExtractFutureFailed Former[%s] Future[%s] Merge[%s]"), *Work->RequestMergeInfos[MergeIndex].GetFormorPath(), *Work->RequestMergeInfos[MergeIndex].GetFuturePath(), *Work->RequestMergeInfos[MergeIndex].GetMergePath());
		Work->OnMergeFailed.Broadcast();
	}, TStatId(), NULL, ENamedThreads::GameThread);
}

void FAdvancePakMergePatchs::OnCreateMergeFailed()
{
	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		UE_LOG(LogTemp, Log, TEXT("CreateMergeFailed Former[%s] Future[%s] Merge[%s]"), *Work->RequestMergeInfos[MergeIndex].GetFormorPath(), *Work->RequestMergeInfos[MergeIndex].GetFuturePath(), *Work->RequestMergeInfos[MergeIndex].GetMergePath());
		Work->OnMergeFailed.Broadcast();
	}, TStatId(), NULL, ENamedThreads::GameThread);
}

void FAdvancePakMergePatchs::OnCreateMergeSucceed()
{
	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		FAdvancePakFileInfo MergePakInfo;
		MergePakInfo.Name = Work->RequestMergeInfos[MergeIndex].Name + UAdvancePakLibrary::PakSuffixName;

		FString MergePakPath = Work->RequestMergeInfos[MergeIndex].GetMergePath();
		MergePakInfo.Size = PlatformFile->FileSize(*MergePakPath);
		MergePakInfo.Hash = LexToString(FMD5Hash::HashFile(*MergePakPath));
		MergePakInfo.Former = Work->LatestVersion;
		MergePakInfo.Future = Work->LatestVersion;

		State->MergeFiles.Add(MergePakInfo.GetFormatName(), MergePakInfo);

		if (Config->IsWithSig)
		{
			FAdvancePakFileInfo MergeSigInfo;
			MergeSigInfo.Name = Work->RequestMergeInfos[MergeIndex].Name + UAdvancePakLibrary::SigSuffixName;

			FString MergeSigPath = Work->RequestMergeInfos[MergeIndex].GetMergePath(true);
			MergeSigInfo.Size = PlatformFile->FileSize(*MergeSigPath);
			MergeSigInfo.Hash = LexToString(FMD5Hash::HashFile(*MergeSigPath));
			MergeSigInfo.Former = Work->LatestVersion;
			MergeSigInfo.Future = Work->LatestVersion;

			State->MergeFiles.Add(MergeSigInfo.GetFormatName(), MergeSigInfo);
		}

		SaveState();

		Work->AdvancePakRecorder->PeddingReceived(1);

		MergeIndex++;
		if (MergeIndex < Work->RequestMergeInfos.Num())
		{
			MergeThread = MakeShareable(new FAdvancePakThread(
				TEXT("MergeThread"),
				Work->KeyChain,
				Work->RequestMergeInfos[MergeIndex].PatchInfo,
				Work->CmdParams,
				Work->RequestMergeInfos[MergeIndex].GetMergePath(),
				Work->RequestMergeInfos[MergeIndex].GetFormorPath(),
				Work->RequestMergeInfos[MergeIndex].GetFuturePath()
			));

			MergeThread->ExtractFormerFailed.BindRaw(this, &FAdvancePakMergePatchs::OnExtractFormerFailed);
			MergeThread->ExtractFutureFailed.BindRaw(this, &FAdvancePakMergePatchs::OnExtractFutureFailed);
			MergeThread->CreateMergeFailed.BindRaw(this, &FAdvancePakMergePatchs::OnCreateMergeFailed);
			MergeThread->CreateMergeSucceed.BindRaw(this, &FAdvancePakMergePatchs::OnCreateMergeSucceed);

			MergeThread->Execute();
		}
		else
		{
			DeleteFilsInDirectory(UAdvancePakLibrary::StorePaksPath);
			Work->AdvancePakRecorder->ExitRecorder();
			Work->OnUpdateProgressNotify.Broadcast(1.f);
			Work->SwitchEnterProcess(EAdvancePakProcess::UpdatePatchs);
		}
	}, TStatId(), NULL, ENamedThreads::GameThread);
}

bool FAdvancePakMergePatchs::CheckMerge()
{
	FAdvancePakMergeInfo& MergeInfo = Work->RequestMergeInfos[MergeIndex];

	FString FormerPakPath = MergeInfo.GetFormorPath();

	if (!FormerPakPath.IsEmpty())
	{
		if (PlatformFile->FileExists(*FormerPakPath))
		{

			if (PlatformFile->FileSize(*FormerPakPath) != MergeInfo.PreSize
				|| !LexToString(FMD5Hash::HashFile(*FormerPakPath)).Equals(MergeInfo.PreHash))
			{
				return false;
			}
		}
	}

	FString FuturePakPath = MergeInfo.GetFuturePath();

	if (!FuturePakPath.IsEmpty())
	{
		if (PlatformFile->FileExists(*FuturePakPath))
		{
			if (PlatformFile->FileSize(*FuturePakPath) != MergeInfo.NexSize
				|| !LexToString(FMD5Hash::HashFile(*FuturePakPath)).Equals(MergeInfo.NexHash))
			{
				return false;
			}
		}
	}

	return true;
}
