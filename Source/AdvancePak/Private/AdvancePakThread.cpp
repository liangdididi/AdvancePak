// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancePakThread.h"
#include "AdvancePakLibrary.h"

void FAdvancePakThread::Execute()
{
	WorkThread.Reset(FRunnableThread::Create(this, *ThreadName, 0));
}

void FAdvancePakThread::Join()
{

}

void FAdvancePakThread::Stop()
{

}

void FAdvancePakThread::Exit()
{

}

uint32 FAdvancePakThread::Run()
{
	TArray<FPakInputPair> FormerEntries;
	if (!FormerPakPath.IsEmpty())
	{
		if (!UAdvancePakLibrary::ExtractPakFile(*FormerPakPath, &FormerEntries, KeyChain))
		{
			ExtractFormerFailed.ExecuteIfBound();
			return 0;
		}
	}

	TArray<FPakInputPair> FutureEntries;
	if (!FuturePakPath.IsEmpty())
	{
		if (!UAdvancePakLibrary::ExtractPakFile(*FuturePakPath, &FutureEntries, KeyChain))
		{
			ExtractFutureFailed.ExecuteIfBound();
			return 0;
		}
	}

	if (PatchInfo.IsValid())
	{
		for (auto UpdateDest : PatchInfo->UpdateResources)
		{
			int Index = -1;
			for (int i = 0; i < FormerEntries.Num(); ++i)
			{
				if (FormerEntries[i].Dest.Equals(UpdateDest))
				{
					Index = i;
					break;
				}
			}
			if (Index >= 0)
				FormerEntries.RemoveAt(Index);
		}

		for (auto DeleteDest : PatchInfo->DeleteResources)
		{
			int Index = -1;
			for (int i = 0; i < FormerEntries.Num(); ++i)
			{
				if (FormerEntries[i].Dest.Equals(DeleteDest))
				{
					Index = i;
					break;
				}
			}
			if (Index >= 0)
				FormerEntries.RemoveAt(Index);
		}
	}

	FormerEntries.Append(FutureEntries);

	if (UAdvancePakLibrary::CreatePakFile(*MergePakPath, FormerEntries, CmdParams, KeyChain))
	{
		CreateMergeSucceed.ExecuteIfBound();
		return 0;
	}
	CreateMergeFailed.ExecuteIfBound();

	return 0;
}
