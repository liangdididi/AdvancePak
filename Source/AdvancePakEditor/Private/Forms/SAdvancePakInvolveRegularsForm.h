// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Utility/AdvancePakEditorType.h"

struct FAssetData;
class FAdvancePakCreator;
class SAdvancePakInvolveRegularsRaw;

/**
 *
 */
class SAdvancePakInvolveRegularsForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakInvolveRegularsForm)
	{}
	SLATE_ARGUMENT(const TArray<TSharedPtr<FAdvancePakRegularOption>>*, ListItemsSource)
	SLATE_ARGUMENT(FText, Title)
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

	const TArray<FName>& GetSelectedResources();

	void AddResource(const FName& ResourceName);

	void RemoveResource(const FName& ResourceName);

	void DemandViewRenovate();

	void DemandSelectRenovate();

protected:

	TSharedRef<ITableRow> DealwithResourceViewPromoteRow(TSharedPtr<FAdvancePakRegularOption> ResourceInfo, const TSharedRef<STableViewBase>& OwnerTable);

	void DealwithRemoveResourcesLinkwork();

	void DealwithAddResourcesLinkwork();

	void DealwithSelectLinkwork(bool IsAll);

	void DealwithRegularObjectChanged(const FAssetData& InResourceData);

	FString DealwithRegularObjectPath() const;

	void DealwithViewResourcesLinkwork();

	void ViewRelyons(const TArray<FName>& PackageNames);

public:

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TArray<FName> SelectedResources;

	const TArray<TSharedPtr<FAdvancePakRegularOption>>* ResourcesList;

	TSharedPtr<SListView<TSharedPtr<FAdvancePakRegularOption>>> ResourcesListView;

	FAssetData ResourceData;

	FSimpleDelegate RenovateTwo;

	FSimpleDelegate RenovateThree;

	FSimpleDelegate RenovateFour;

	FSimpleDelegate PromoteFive;

	FSimpleDelegate PromoteTen;
};
