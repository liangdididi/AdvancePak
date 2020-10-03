// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"

class SEditableTextBox;
class FAdvancePakCreator;
class SAdvancePakGathersRaw;

/**
 * 
 */
class SAdvancePakGathersForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakGathersForm)
	{}
	SLATE_ARGUMENT(const TArray<TSharedPtr<FAdvancePakGatherOption>>*, ListItemsSource)
		SLATE_ARGUMENT(bool, IsInvolve)
	SLATE_ARGUMENT(FText, Title)
	SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

		const TArray<FString>& GetSelectedGathers();

		void RemoveGather(const FString& GatherPath);

		void DemandViewRenovate();

	void AddGather(const FString& GatherPath);

	void DemandSelectRenovate();

public:


protected:

	TSharedRef<ITableRow> DealwithGatherViewPromoteRow(TSharedPtr<FAdvancePakGatherOption> GatherInfo, const TSharedRef<STableViewBase>& OwnerTable);

	void DealwithBrowseGathersLinkwork();

	void DealwithAddGathersLinkwork();

	void DealwithSelectLinkwork(bool IsAll);

	void DealwithRemoveGathersLinkwork();

	bool AbsorbFullToGather(FString FullPath, FString& GatherPath, bool& CanGatherRegular);

public:

	const TArray<TSharedPtr<FAdvancePakGatherOption>>* GathersList;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TSharedPtr<FAdvancePakGatherOption> GatherOption;

	bool IsInvolve;

	TArray<FString> SelectedGathers;

	TSharedPtr<SListView<TSharedPtr<FAdvancePakGatherOption>>> GathersListView;

	bool IsFormerPatch;

	TSharedPtr<SEditableTextBox> GatherPathEditor;

	FSimpleDelegate RenovateOne;

	FSimpleDelegate RenovateTwo;

	FSimpleDelegate RenovateThree;

	FSimpleDelegate RenovateFour;

	FSimpleDelegate RenovateFive;

	FSimpleDelegate RenovateSix;

	FSimpleDelegate PromoteOne;

	FSimpleDelegate PromoteTwo;

	FSimpleDelegate PromoteThree;

	FSimpleDelegate PromoteEight;

	FSimpleDelegate PromoteNine;

	FSimpleDelegate PromoteTen;
};
