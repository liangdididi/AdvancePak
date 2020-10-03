// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Utility/AdvancePakEditorType.h"

class SAdvancePakAbsolveRegularsRaw;
struct FAssetData;
class FAdvancePakCreator;

/**
 * 
 */
class SAdvancePakAbsolveRegularsForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakAbsolveRegularsForm)
	{}
	SLATE_ARGUMENT(const TArray<TSharedPtr<FName>>*, ListItemsSource)
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

	void DealwithSelectLinkwork(bool IsAll);

	void DealwithViewResourcesLinkwork();

	TSharedRef<ITableRow> DealwithResourceViewPromoteRow(TSharedPtr<FName> PackageName, const TSharedRef<STableViewBase>& OwnerTable);

	void DealwithRegularObjectChanged(const FAssetData& InResourceData);

	FString DealwithRegularObjectPath() const;

	void DealwithRemoveResourcesLinkwork();

	void DealwithAddResourcesLinkwork();

	void ViewRelyons(const TArray<FName>& PackageNames);

public:

	/** Holds the map list. */
	const TArray<TSharedPtr<FName>>* ResourcesList;

	/** Holds the map list view. */
	TSharedPtr<SListView<TSharedPtr<FName>>> ResourcesListView;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TArray<FName> SelectedResources;

	FAssetData ResourceData;

	FSimpleDelegate RenovateTwo;

	FSimpleDelegate RenovateThree;

	FSimpleDelegate RenovateFour;

	FSimpleDelegate PromoteSeven;

	FSimpleDelegate PromoteTen;
};
