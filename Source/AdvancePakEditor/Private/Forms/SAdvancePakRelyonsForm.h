// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STreeView.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Utility/AdvancePakEditorType.h"

class FAdvancePakCreator;
class SAdvancePakRelyonsRaw;
struct FAssetData;


/**
 *
 */
class SAdvancePakRelyonsForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakRelyonsForm)
	{}
	SLATE_ARGUMENT(const TArray<TSharedPtr<FAdvancePakRelyonInfo>>*, TreeItemsSource)
	SLATE_ARGUMENT(FText, Title)
	SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

	const TArray<FName>& GetSelectedInvolves();

	const TArray<FName>& GetSelectedAbsolves();

	void DealwithViewLinkwork();

	void DealwithInvolveLinkwork();

	void AddInvolve(const FName& PackageName);

	void RemoveInvolve(const FName& PackageName);

	void AddAbsolve(const FName& PackageName);

	void DealwithSelectRecursion(const TSharedPtr<FAdvancePakRelyonInfo>& RelyonInfo);

	void DemandViewRenovate();

	void DemandSelectRenovate();

	void RemoveAbsolve(const FName& PackageName);

	void DealwithSelectLinkwork(bool IsAll);

	void DealwithAbsolveLinkwork();

	void DealwithRegularObjectChanged(const FAssetData& InResourceData);

	FString DealwithRegularObjectPath() const;

protected:

	TSharedRef<ITableRow> DealwithResourceViewPromoteRow(TSharedPtr<FAdvancePakRelyonInfo> RelyonInfo, const TSharedRef<STableViewBase>& OwnerTable);

	void DealwithResourceViewGetChildren(TSharedPtr<FAdvancePakRelyonInfo> RelyonInfo, TArray<TSharedPtr<FAdvancePakRelyonInfo>>& OutRelyons);

	bool ResourceExistRecursion(const TSharedPtr<FAdvancePakRelyonInfo>& RelyonInfo, const FName& PackageName);

public:

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	const TArray<TSharedPtr<FAdvancePakRelyonInfo>>* RelyonsList;

	TArray<FName> SelectedAbsolves;

	FAssetData ResourceData;

	TSharedPtr<STreeView<TSharedPtr<FAdvancePakRelyonInfo>>> RelyonsForm;

	TArray<FName> SelectedInvolves;

	FSimpleDelegate RenovateTwo;

	FSimpleDelegate RenovateThree;

	FSimpleDelegate RenovateFour;

	FSimpleDelegate PromoteFive;

	FSimpleDelegate PromoteSeven;

	FSimpleDelegate PromoteTen;
};
