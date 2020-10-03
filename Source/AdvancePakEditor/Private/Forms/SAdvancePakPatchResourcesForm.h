// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Utility/AdvancePakEditorType.h"

class FAdvancePakCreator;
class SAdvancePakPatchResourcesRaw;


/**
 * 
 */
class SAdvancePakPatchResourcesForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPatchResourcesForm)
	{}
	SLATE_ARGUMENT(const TArray<TSharedPtr<FAdvancePakPatchResourceInfo>>*, ListItemsSource)
	SLATE_ARGUMENT(FText, Title)
		SLATE_ARGUMENT(bool, IsFormer)
		SLATE_ARGUMENT(bool, IsRegular)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

	void DemandViewRenovate();

	void AddResource(const FName& ResourceName);

	void DemandSelectRenovate();

	const TArray<FName>& GetSelectedResources();

	void RemoveResource(const FName& ResourceName);

public:


protected:

	void DealwithAddResourcesLinkwork();

	void DealwithRemoveResourcesLinkwork();

	void DealwithSelectLinkwork(bool IsAll);

	void DealwithViewResourcesLinkwork();

	TSharedRef<ITableRow> DealwithResourceViewPromoteRow(TSharedPtr<FAdvancePakPatchResourceInfo> ResourceInfo, const TSharedRef<STableViewBase>& OwnerTable);

	void ViewRelyons(const TArray<FName>& PackageNames);

public:

	/** Holds the map list. */
	const TArray<TSharedPtr<FAdvancePakPatchResourceInfo>>* ResourcesList;

	/** Holds the map list view. */
	TSharedPtr<SListView<TSharedPtr<FAdvancePakPatchResourceInfo>>> ResourcesListView;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TArray<FName> SelectedResources;

	bool IsFormer;

	bool IsRegular;

	FSimpleDelegate RenovateOne;

	FSimpleDelegate RenovateTwo;

	FSimpleDelegate RenovateThree;

	FSimpleDelegate RenovateFour;

	FSimpleDelegate RenovateFive;

	FSimpleDelegate RenovateSix;

	FSimpleDelegate PromoteFour;

	FSimpleDelegate PromoteFive;

	FSimpleDelegate PromoteSix;

	FSimpleDelegate PromoteSeven;

	FSimpleDelegate PromoteEight;

	FSimpleDelegate PromoteTen;
};
