// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class FAdvancePakCreator;
class SEditableTextBox;
class SMultiLineEditableTextBox;
class FAdvancePakProcThread;
class SNotificationItem;
/**
 * 
 */
class SAdvancePakPatchOperate : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPatchOperate)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

protected:

	FReply DealwithReplicateButtonClicked();

	FReply DealwithSaveButtonClicked();

	void DealwithPickupCancelButtonClicked();

	void DealwithDismissButtonClicked();

	FString PromotePickupOptions();

	FReply DealwithPickupButtonClicked();

	void OnPickupTaskOutput(FString OutputLog);

	void OnConfiguresChanged();

	void OnPickupOptionsCommitted(const FText& InText, ETextCommit::Type CommitInfo);

	FReply DealwithPakListButtonClicked();

	FReply DealwithExportButtonClicked();

	void OnPickupTaskBegin();

	void OnPickupTaskSucceed();

	void OnPickupTaskFailed();

	void DealwithPickupLinkwork();

	void RenovatePatchAccordsForm();

	void PromotePatchAccords(const FName& PatchName = NAME_None);

	bool PromotePatchPakList(const FName& PatchName = NAME_None);

protected:

	FString PakFilePath;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TWeakPtr<SNotificationItem> NotificationItemPtr;

	FString PakListPath;

	TSharedPtr<SMultiLineEditableTextBox> PickupOptionsEditor;

	mutable TSharedPtr<FAdvancePakProcThread> PickupTaskThread;

};
