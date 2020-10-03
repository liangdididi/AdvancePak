// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"

class FAdvancePakCreator;
class SAdvancePakInformalsForm;

/**
 * 
 */
class SAdvancePakInformalsRaw : public SMultiColumnTableRow<TSharedPtr<FAdvancePakInformalInfo>>
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakInformalsRaw)
	{}
	SLATE_ARGUMENT(TSharedPtr<STableViewBase>, OwnerTableView)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakInformalInfo>, Informal)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakCreator>, AdvancePakCreator)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, SAdvancePakInformalsForm* InInformalsForm);

protected:

	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

	void DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState);

	ECheckBoxState DealwithCheckBoxIsChecked() const;

	void DealwithInformalBrowse();

protected:

	SAdvancePakInformalsForm* InformalsForm;

	TSharedPtr<FAdvancePakInformalInfo> Informal;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;
};
