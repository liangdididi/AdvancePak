// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Utility/AdvancePakEditorType.h"

class FAdvancePakCreator;
class SAdvancePakPublishResourcesForm;
/**
 * 
 */
class SAdvancePakPublishResourcesRaw : public SMultiColumnTableRow<TSharedPtr<FAdvancePakPublishResourceInfo>>
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPublishResourcesRaw)
	{}
	SLATE_ARGUMENT(TSharedPtr<STableViewBase>, OwnerTableView)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakPublishResourceInfo>, ResourceInfo)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakCreator>, AdvancePakCreator)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, SAdvancePakPublishResourcesForm* InResourcesForm);

protected:

	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

	void DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState);

	ECheckBoxState DealwithCheckBoxIsChecked() const;

	void DealwithResourceBrowse();

	FSlateColor DealwithResourceColor() const;

protected:

	SAdvancePakPublishResourcesForm* PublishResourcesForm;

	TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;
};
