// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Utility/AdvancePakEditorType.h"

class FAdvancePakCreator;
class SAdvancePakPatchResourcesForm;
/**
 * 
 */
class SAdvancePakPatchResourcesRaw : public SMultiColumnTableRow<TSharedPtr<FAdvancePakPatchResourceInfo>>
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPatchResourcesRaw)
	{}
	SLATE_ARGUMENT(TSharedPtr<STableViewBase>, OwnerTableView)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakPatchResourceInfo>, ResourceInfo)
		SLATE_ARGUMENT(TSharedPtr<FAdvancePakCreator>, AdvancePakCreator)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, SAdvancePakPatchResourcesForm* InResourcesForm);

protected:

	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

	void DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState);

	ECheckBoxState DealwithCheckBoxIsChecked() const;

	void DealwithResourceBrowse();

	FSlateColor DealwithResourceColor() const;

protected:

	SAdvancePakPatchResourcesForm* PatchResourcesForm;

	TSharedPtr<FAdvancePakPatchResourceInfo> ResourceInfo;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;
};
