// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"

class FAdvancePakCreator;
class SAdvancePakInformalsRaw;
class SEditableTextBox;

/**
 * 
 */
class SAdvancePakInformalsForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakInformalsForm)
	{}
	SLATE_ARGUMENT(const TArray<TSharedPtr<FAdvancePakInformalInfo>>*, ListItemsSource)
		SLATE_ARGUMENT(bool, IsInvolve)
	SLATE_ARGUMENT(FText, Title)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

	void RemoveInformal(const FName& Informal);

	const TArray<FName>& GetSelectedInformals();

	void DemandViewRenovate();

	void DemandSelectRenovate();

	void AddInformal(const FName& Informal);

protected:

	TSharedRef<ITableRow> DealwithInformalViewPromoteRow(TSharedPtr<FAdvancePakInformalInfo> Informal, const TSharedRef<STableViewBase>& OwnerTable);

	void DealwithBrowseInformalsLinkwork();

	void DealwithAddInformalsLinkwork();

	void DealwithSelectLinkwork(bool IsAll);

	void DealwithRemoveInformalsLinkwork();

public:

	const TArray<TSharedPtr<FAdvancePakInformalInfo>>* InformalsList;

	TArray<FName> SelectedInformals;

	TSharedPtr<SEditableTextBox> InformalEditor;

	TArray<TSharedPtr<FAdvancePakInformalInfo>> InformalInfos;

	bool IsInvolve;

	TSharedPtr<SListView<TSharedPtr<FAdvancePakInformalInfo>>> InformalsListView;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	FSimpleDelegate RenovateOne;

	FSimpleDelegate RenovateFive;

	FSimpleDelegate RenovateSix;

	FSimpleDelegate PromoteFour;

	FSimpleDelegate PromoteSix;

	FSimpleDelegate PromoteEight;
};
