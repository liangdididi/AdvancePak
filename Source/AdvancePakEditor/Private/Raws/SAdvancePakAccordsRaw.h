// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Utility/AdvancePakEditorType.h"

class FAdvancePakCreator;
class SAdvancePakAccordsForm;

/**
 * 
 */
class SAdvancePakAccordsRaw : public SMultiColumnTableRow<TSharedPtr<FAdvancePakAccordInfo>>
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakAccordsRaw)
	{}
	SLATE_ARGUMENT(TSharedPtr<STableViewBase>, OwnerTableView)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakAccordInfo>, AccordInfo)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakCreator>, AdvancePakCreator)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, SAdvancePakAccordsForm* InAccordsForm);

protected:

	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

	void DealwithResourceBrowse();

	FSlateColor DealwithResourceNameColor() const;

protected:

	SAdvancePakAccordsForm* AccordsForm;

	TSharedPtr<FAdvancePakAccordInfo> AccordInfo;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;
};
