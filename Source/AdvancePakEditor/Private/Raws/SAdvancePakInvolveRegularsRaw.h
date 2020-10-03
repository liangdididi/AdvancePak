// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Utility/AdvancePakEditorType.h"

class FAdvancePakCreator;
class SAdvancePakInvolveRegularsForm;

/**
 * 
 */
class SAdvancePakInvolveRegularsRaw : public SMultiColumnTableRow<TSharedPtr<FAdvancePakRegularOption>>
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakInvolveRegularsRaw)
	{}
	SLATE_ARGUMENT(TSharedPtr<STableViewBase>, OwnerTableView)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakRegularOption>, ResourceInfo)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakCreator>, AdvancePakCreator)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, SAdvancePakInvolveRegularsForm* InInvolveRegularsForm);

protected:

	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

	void DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState);

	ECheckBoxState DealwithCheckBoxIsChecked() const;

	TSharedRef<SWidget> MakeComboItem(TSharedPtr<FString> ComboItem);

	void OnRelyonTypeChanged(TSharedPtr<FString> ComboItem, ESelectInfo::Type SelectInfo);

	FText GetRelyonTypeText() const;

	void DealwithResourceBrowse();

	void OnGatherModeChanged(TSharedPtr<FString> ComboItem, ESelectInfo::Type SelectInfo);

	FText GetGatherModeText() const;

	void RePromoteResources();

protected:

	SAdvancePakInvolveRegularsForm* InvolveRegularsForm;

	TSharedPtr<FAdvancePakRegularOption> ResourceInfo;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;
};
