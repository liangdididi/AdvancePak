// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class FAdvancePakCreator;
/**
 * 
 */
class SAdvancePakPublishOperate : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPublishOperate)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

	void SetPatchBox(SAdvancePakPatchBar* InPatchBox);

protected:

	void OnPackageDirCommitted(const FText& InText, ETextCommit::Type CommitInfo);

	FReply DealwithPackageDirButtonClicked();

	FReply DealwithPakListButtonClicked();

	FReply DealwithPickupPublishButtonClicked();

	FString PromotePickupOptions();

	void SaveVersionConfig();

	FReply DealwithExportButtonClicked();

	void OnConfiguresChanged();

	void OnPickupOptionsCommitted(const FText& InText, ETextCommit::Type CommitInfo);

	FReply DealwithReplicateButtonClicked();

	void OnPickupAllTaskSucceed();

	void OnPickupTaskOutput(FString OutputLog);

	void OnPickupTaskBegin();

	FReply DealwithPickupAllButtonClicked();

	void OnPickupTaskFailed();

	void DealwithPickupLinkwork();

	void DealwithPickupCancelButtonClicked();

	void DealwithDismissButtonClicked();

	void OnPickupPublishTaskSucceed();

	FReply DealwithSaveButtonClicked();

	void RecursionPickupVersion();

	void PromotePublishAccords();

	void RenovatePublishAccordsForm();

	void PromotePatchAccords(const FName& PatchName = NAME_None);

	bool PromotePatchPakList(const FName& PatchName = NAME_None);

	bool PromotePublishPakList();

	FString AbsorbOptionsToCmdLine(const FString& PakCommamd);

protected:

	TSharedPtr<SCheckBox> RecursionCheckBox;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TSharedPtr<SEditableTextBox> PackageDirEditor;

	TSharedPtr<SMultiLineEditableTextBox> PickupOptionsEditor;

	mutable TSharedPtr<FAdvancePakProcThread> PickupTaskThread;

	TWeakPtr<SNotificationItem> NotificationItemPtr;

	FString PakListPath;

	FString PakFilePath;

	int32 PickupIndex;

	TSharedPtr<FAdvancePakVersionConfig> VersionConfig;

	SAdvancePakPatchBar* PatchBox;
};
