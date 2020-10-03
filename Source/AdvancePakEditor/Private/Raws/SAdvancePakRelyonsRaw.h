// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STreeView.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Utility/AdvancePakEditorType.h"

class FAdvancePakCreator;
class SAdvancePakRelyonsForm;

/**
 * 
 */
class SAdvancePakRelyonsRaw : public SMultiColumnTableRow<TSharedPtr<FAdvancePakRelyonInfo>>
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakRelyonsRaw)
	{}
	SLATE_ARGUMENT(TSharedPtr<STableViewBase>, OwnerTableView)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakRelyonInfo>, RelyonInfo)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakCreator>, AdvancePakCreator)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, SAdvancePakRelyonsForm* InRelyonsForm);

protected:

	TSharedRef<SWidget> CreatePackageNameWidget();

	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

	void DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState);

	ECheckBoxState DealwithCheckBoxIsChecked() const;

	TSharedRef<SWidget> MakeComboItem(TSharedPtr<FString> ComboItem);

	void OnRelyonTypeChanged(TSharedPtr<FString> ComboItem, ESelectInfo::Type SelectInfo);

	FText GetRelyonTypeText() const;

	void DealwithResourceBrowse();

	FSlateColor DealwithRowColor() const;

	FReply DealwithUnViewButtonClicked();

protected:

	SAdvancePakRelyonsForm* RelyonsForm;

	TSharedPtr<FAdvancePakRelyonInfo> RelyonInfo;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;
};
