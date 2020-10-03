// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Utility/AdvancePakEditorType.h"

class FAdvancePakCreator;
class SEditableTextBox;
class SCheckBox;

namespace EPatchEditType
{
	enum Type
	{
		None,
		Rename,
		Delete,
		Copy,
		Add
	};
}

/**
 *
 */
class SAdvancePakPatchLump : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPatchLump)
	{}
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakPatchItem>, PatchItem)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakCreator>, AdvancePakCreator)
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs);

protected:

	bool IsEditable() const;

	FReply DealwithCopyClicked();

	FReply DealwithAddClicked();

	FReply DealwithRenameClicked();

	FReply DealwithDeleteClicked();

	FReply DealwithSureClicked();

	FReply DealwithCancelClicked();

	void RenovatePatchFutureViews();

	void RenovatePublishFutureViews();

	void RenovatePatchFormerViews();

	void RenovatePublishFormerViews();

	TSharedPtr<FAdvancePakPatchConfigure> CreatePatchConfigureCopy(const TSharedPtr<FAdvancePakPatchConfigure>& PatchConfigure);

public:

	TSharedPtr<FAdvancePakPatchItem> PatchItem;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	EPatchEditType::Type PatchEditType;

	TSharedPtr<SCheckBox> CopyCheckBox;

	TSharedPtr<SEditableTextBox> NameEditor;

	static FName PublishName;

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
