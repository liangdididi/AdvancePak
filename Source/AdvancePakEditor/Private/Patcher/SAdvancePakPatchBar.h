// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"


class SAdvancePakPackageBar;
class FAdvancePakCreator;
class SEditableTextBox;
class SMultiLineEditableTextBox;
class IDetailsView;
class STextBlock;
class SScrollBox;
class SAdvancePakPatchOperate;
class SAdvancePakPatchLump;
class SAdvancePakPublishOperate;
class SAdvancePakPatchBar;

class SAdvancePakPatchItem : public STableRow<TSharedPtr<FAdvancePakPatchItem>>
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPatchItem)
	{}
	SLATE_ARGUMENT(TSharedPtr<STableViewBase>, OwnerTableView)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakPatchItem>, PatchItem)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakCreator>, AdvancePakCreator)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, SAdvancePakPatchBar* InPatchBar);

protected:

	FSlateColor DealwithItemColor() const;

public:

	TSharedPtr<FAdvancePakPatchItem> PatchItem;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TSharedPtr<SAdvancePakPatchLump> PatchLump;

	SAdvancePakPatchBar* PatchBar;
};


/**
 *
 */
class SAdvancePakPatchBar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPatchBar)
	{}
	SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

	bool LoadConfigConfigures(FString ConfigPath, bool IsFormerConfig);

protected:

	bool DealwithButtonEnabled() const;

	FReply DealwithFormerButtonClicked();

	FReply DealwithFutureButtonClicked();

	void OnTotalConfiguresChanged();

	TSharedRef<ITableRow> DealwithPatchViewPromoteRow(TSharedPtr<FAdvancePakPatchItem> PatchItem, const TSharedRef<STableViewBase>& OwnerTable);

	void DealwithPatchViewSelectionChanged(TSharedPtr<FAdvancePakPatchItem> PatchItem, ESelectInfo::Type SelectInfo);

	void ResetActiveName(const FName& NewName = NAME_None);

	void ResetCookedPath();

	void RenovatePatchFutureViews();

	void RenovatePublishFutureViews();

	void RenovatePatchFormerViews();

	void RenovatePublishFormerViews();

public:

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TSharedPtr<SEditableTextBox> FormerConfigDirEditor;

	TSharedPtr<SEditableTextBox> FutureConfigDirEditor;

	TSharedPtr<STextBlock> FormerVersionText;

	TSharedPtr<STextBlock> FutureVersionText;

	TSharedPtr<STextBlock> FormerPlatformText;

	TSharedPtr<STextBlock> FuturePlatformText;

	TSharedPtr<SAdvancePakPublishOperate> PublishOperate;

	TSharedPtr<SAdvancePakPatchOperate> PatchOperate;

	TArray<TSharedPtr<FAdvancePakPatchItem>> PatchItems;

	TSharedPtr<SListView<TSharedPtr<FAdvancePakPatchItem>>> PatchView;

	FSimpleDelegate RenovateOne;

	FSimpleDelegate RenovateTwo;

	FSimpleDelegate RenovateFour;

	FSimpleDelegate RenovateFive;

	FSimpleDelegate PromoteOne;

	FSimpleDelegate PromoteTwo;

	FSimpleDelegate PromoteThree;

	FSimpleDelegate PromoteFour;

	FSimpleDelegate PromoteFive;

	FSimpleDelegate PromoteSix;

	FSimpleDelegate PromoteSeven;

	FSimpleDelegate PromoteEight;

	FSimpleDelegate PromoteNine;

	FSimpleDelegate PromoteTen;
};
