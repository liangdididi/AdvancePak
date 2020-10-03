// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"

class FAdvancePakCreator;
class SAdvancePakGathersForm;
/**
 * 
 */
class SAdvancePakGathersRaw : public SMultiColumnTableRow<TSharedPtr<FAdvancePakGatherOption>>
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakGathersRaw)
	{}
	SLATE_ARGUMENT(TSharedPtr<STableViewBase>, OwnerTableView)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakGatherOption>, GatherInfo)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakCreator>, AdvancePakCreator)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, SAdvancePakGathersForm* InGathersForm);

protected:

	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

	void DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState);

	ECheckBoxState DealwithCheckBoxIsChecked() const;

	TSharedRef<SWidget> MakeComboItem(TSharedPtr<FString> ComboItem);

	void OnGatherTypeChanged(TSharedPtr<FString> ComboItem, ESelectInfo::Type SelectInfo);

	void OnRelyonTypeChanged(TSharedPtr<FString> ComboItem, ESelectInfo::Type SelectInfo);

	void OnGatherModeChanged(TSharedPtr<FString> ComboItem, ESelectInfo::Type SelectInfo);

	FText GetFilterTypeText() const;

	FText GetRelyonTypeText() const;

	FText GetGatherModeText() const;

	void DealwithGatherBrowse();

	void RePromoteResources();

protected:

	SAdvancePakGathersForm* GathersForm;

	TSharedPtr<FAdvancePakGatherOption> GatherInfo;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;
};
