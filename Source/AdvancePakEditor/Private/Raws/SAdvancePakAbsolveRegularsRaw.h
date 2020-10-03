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
class SAdvancePakAbsolveRegularsRaw : public SMultiColumnTableRow<TSharedPtr<FName>>
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakAbsolveRegularsRaw)
	{}
	SLATE_ARGUMENT(TSharedPtr<STableViewBase>, OwnerTableView)
		SLATE_ARGUMENT(TSharedPtr<FName>, PackageName)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakCreator>, AdvancePakCreator)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, SAdvancePakAbsolveRegularsForm* InAbsolveRegularsForm);

protected:

	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

	void DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState);

	ECheckBoxState DealwithCheckBoxIsChecked() const;

	void DealwithResourceBrowse();

protected:

	SAdvancePakAbsolveRegularsForm* AbsolveRegularsForm;

	TSharedPtr<FName> PackageName;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;
};
