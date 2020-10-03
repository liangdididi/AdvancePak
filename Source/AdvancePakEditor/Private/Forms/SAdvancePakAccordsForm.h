// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Utility/AdvancePakEditorType.h"

class SEditableTextBox;
class STextBlock;
class FAdvancePakCreator;
class SAdvancePakAccordsRaw;

/**
 * 
 */
class SAdvancePakAccordsForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakAccordsForm)
	{}
	SLATE_ARGUMENT(const TArray<TSharedPtr<FAdvancePakAccordInfo>>*, ListItemsSource)
	SLATE_ARGUMENT(FText, Title)
		SLATE_ARGUMENT(bool, IsPublish)
	SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

	void DemandViewRenovate(bool IsFailed = false);

protected:

	TSharedRef<ITableRow> DealwithAccordViewPromoteRow(TSharedPtr<FAdvancePakAccordInfo> AccordInfo, const TSharedRef<STableViewBase>& OwnerTable);

	EVisibility DealwithPromptVisibility() const;

	void DealwithAccordPublishLinkwork();

	void DealwithAccordAllLinkwork();

	void DealwithAccordPatchLinkwork();

	void PromotePublishAccords();

	void RenovatePublishAccordsForm();

	void RenovatePatchAccordsForm();

	void PromotePatchAccords(const FName& PatchName = NAME_None);

public:

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TSharedPtr<STextBlock> AccordPromptText;

	bool IsAccordFailed;

	const TArray<TSharedPtr<FAdvancePakAccordInfo>>* AccordsList;

	TSharedPtr<SListView<TSharedPtr<FAdvancePakAccordInfo>>> AccordsListView;

	bool IsPublish;
};
