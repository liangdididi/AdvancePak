// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Utility/AdvancePakEditorType.h"

class FAdvancePakCreator;
class SAdvancePakPublishResourcesRaw;

/**
 *
 */
class SAdvancePakPublishResourcesForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPublishResourcesForm)
	{}
	SLATE_ARGUMENT(const TArray<TSharedPtr<FAdvancePakPublishResourceInfo>>*, ListItemsSource)
	SLATE_ARGUMENT(FText, Title)
		SLATE_ARGUMENT(bool, IsPublish)
		SLATE_ARGUMENT(bool, IsFormer)
		SLATE_ARGUMENT(bool, IsRegular)
	SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

	const TArray<FName>& GetSelectedResources();

	void DemandViewRenovate();

	void DemandSelectRenovate();

	void AddResource(const FName& PackageName);

	void RemoveResource(const FName& PackageName);

public:


protected:

	TSharedRef<ITableRow> DealwithResourceViewPromoteRow(TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo, const TSharedRef<STableViewBase>& OwnerTable);

	void DealwithViewResourcesLinkwork();

	void DealwithSelectLinkwork(bool IsAll);

	void DealwithAddResourcesLinkwork();

	void ViewRelyons(const TArray<FName>& PackageNames);

public:

	const TArray<TSharedPtr<FAdvancePakPublishResourceInfo>>* ResourcesList;

	TArray<FName> SelectedResources;

	bool IsPublish;

	TSharedPtr<SListView<TSharedPtr<FAdvancePakPublishResourceInfo>>> ResourcesListView;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

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

	FSimpleDelegate PromoteEight;

	FSimpleDelegate PromoteTen;
};
