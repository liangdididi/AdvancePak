// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakInvolveRegularsForm.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SHyperlink.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SCheckBox.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "Raws/SAdvancePakInvolveRegularsRaw.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SComboBox.h"
#include "SAdvancePakRelyonsForm.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakInvolveRegularsForm::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;
	ResourcesList = InArgs._ListItemsSource;

	ChildSlot
		[

			SNew(SBorder)
			.Padding(4.f)
		[

			SNew(SVerticalBox)
			.IsEnabled_Lambda([this]() { return !AdvancePakCreator->IsPackaging; })

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(InArgs._Title)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		.ColorAndOpacity(FLinearColor(0.f, 1.f, 0.f))
		]
		]

	+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(ResourcesListView, SListView<TSharedPtr<FAdvancePakRegularOption>>)
			.HeaderRow(
				SNew(SHeaderRow)
				.Visibility(EVisibility::SelfHitTestInvisible)

				+ SHeaderRow::Column(AdvancePakCreator->PackageNameName)
				.DefaultLabel(OPTEXT("PackageName"))
				.FillWidth(6.0f)

				+ SHeaderRow::Column(AdvancePakCreator->RelyonTypeName)
				.DefaultLabel(OPTEXT("RelyonType"))
				.FillWidth(1.0f)

				+ SHeaderRow::Column(AdvancePakCreator->GatherModeName)
				.DefaultLabel(OPTEXT("GatherMode"))
				.FillWidth(1.0f)

				+ SHeaderRow::Column(AdvancePakCreator->BrowseName)
				.DefaultLabel(OPTEXT("BrowseName"))
				.FillWidth(0.4f)
			)
		.ItemHeight(16.f)
		.ListItemsSource(ResourcesList)
		.OnGenerateRow(this, &SAdvancePakInvolveRegularsForm::DealwithResourceViewPromoteRow)
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 6.0f, 0.0f, 4.0f)
		[
			SNew(SSeparator)
			.Orientation(Orient_Horizontal)
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Center)
		[
			SNew(SHorizontalBox)
			.IsEnabled_Lambda([this]() { return AdvancePakCreator->GetFuturePatch().IsValid(); })

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(STextBlock)
			.Text(OPTEXT("Select"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakInvolveRegularsForm::DealwithSelectLinkwork, true)
		.Text(OPTEXT("All"))
		.ToolTipText(OPTEXT("SelectAllResources"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakInvolveRegularsForm::DealwithSelectLinkwork, false)
		.Text(OPTEXT("None"))
		.ToolTipText(OPTEXT("SelectNoneResources"))
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SObjectPropertyEntryBox)
			.OnObjectChanged(this, &SAdvancePakInvolveRegularsForm::DealwithRegularObjectChanged)
		.ObjectPath_Raw(this, &SAdvancePakInvolveRegularsForm::DealwithRegularObjectPath)
		]


	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakInvolveRegularsForm::DealwithViewResourcesLinkwork)
		.Text(OPTEXT("Relyon"))
		.ToolTipText(OPTEXT("ViewResourcesRelyon"))
		]


	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakInvolveRegularsForm::DealwithAddResourcesLinkwork)
		.Text(OPTEXT("Push"))
		.ToolTipText(OPTEXT("PushResourcesToPatch"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 8.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakInvolveRegularsForm::DealwithRemoveResourcesLinkwork)
		.Text(OPTEXT("Remove"))
		.ToolTipText(OPTEXT("RemoveResourcesFromPatch"))
		]

		]
		]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAdvancePakInvolveRegularsForm::DealwithViewResourcesLinkwork()
{
	if (SelectedResources.Num() == 0)
		return;

	ViewRelyons(SelectedResources);
}

void SAdvancePakInvolveRegularsForm::ViewRelyons(const TArray<FName>& PackageNames)
{
	bool IsChanged = false;
	for (auto PackageName : PackageNames)
	{
		bool IsExisted = false;
		for (auto Relyon : AdvancePakCreator->Relyons)
		{
			if (Relyon->PackageName.IsEqual(PackageName))
			{
				IsExisted = true;
				break;
			}
		}
		if (!IsExisted)
		{
			TSharedPtr<FAdvancePakRelyonInfo> RelyonInfo = MakeShareable(new FAdvancePakRelyonInfo());
			RelyonInfo->PackageName = PackageName;
			RelyonInfo->PatchName = AdvancePakCreator->GetFutureRegularPatchName(PackageName);
			RelyonInfo->RelyonType = EAdvancePakRelyonType::Packages;
			RelyonInfo->IsTop = true;
			AdvancePakCreator->Relyons.Add(RelyonInfo);
			IsChanged = true;
		}
	}
	if (IsChanged)
	{
		AdvancePakCreator->RelyonsForm->DemandViewRenovate();
	}
}

void SAdvancePakInvolveRegularsForm::DealwithRemoveResourcesLinkwork()
{
	if (SelectedResources.Num() == 0)
		return;

	for (auto PackageName : SelectedResources)
	{
		int32 RemoveIndex = -1;
		for (int i = 0; i < AdvancePakCreator->InvolveRegulars.Num(); ++i)
		{
			if (AdvancePakCreator->InvolveRegulars[i]->PackageName.IsEqual(PackageName))
			{
				RemoveIndex = i;
				break;
			}
		}
		if (RemoveIndex >= 0)
		{
			AdvancePakCreator->GetFuturePatch()->InvolveRegulars.RemoveAt(RemoveIndex);
			AdvancePakCreator->InvolveRegulars.RemoveAt(RemoveIndex);
		}
	}
	AdvancePakCreator->InvolveRegularsForm->DemandViewRenovate();

	AdvancePakCreator->IsFuturePublishDirty = true;
	PromoteFive.ExecuteIfBound();
	PromoteTen.ExecuteIfBound();
	RenovateFour.ExecuteIfBound();
	RenovateTwo.ExecuteIfBound();
	RenovateThree.ExecuteIfBound();

	SelectedResources.Reset();
}

void SAdvancePakInvolveRegularsForm::RemoveResource(const FName& ResourceName)
{
	if (SelectedResources.Contains(ResourceName))
		SelectedResources.Remove(ResourceName);
}

void SAdvancePakInvolveRegularsForm::DemandViewRenovate()
{
	DemandSelectRenovate();
	ResourcesListView->RequestListRefresh();
}

void SAdvancePakInvolveRegularsForm::DemandSelectRenovate()
{
	TArray<FName> AbandonResources;

	for (auto PackageName : SelectedResources)
	{
		bool IsAbandon = true;
		for (auto Regular : *ResourcesList)
		{
			if (Regular->PackageName.IsEqual(PackageName))
			{
				IsAbandon = false;
				break;
			}
		}
		if (IsAbandon)
			AbandonResources.Add(PackageName);
	}

	for (auto PackageName : AbandonResources)
		SelectedResources.Remove(PackageName);
}

TSharedRef<ITableRow> SAdvancePakInvolveRegularsForm::DealwithResourceViewPromoteRow(TSharedPtr<FAdvancePakRegularOption> ResourceInfo, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SAdvancePakInvolveRegularsRaw, this)
		.OwnerTableView(OwnerTable)
		.AdvancePakCreator(AdvancePakCreator)
		.ResourceInfo(ResourceInfo)
		;
}

const TArray<FName>& SAdvancePakInvolveRegularsForm::GetSelectedResources()
{
	return SelectedResources;
}

void SAdvancePakInvolveRegularsForm::DealwithRegularObjectChanged(const FAssetData& InResourceData)
{
	if (InResourceData.PackageName.ToString().StartsWith(UAdvancePakEditorLibrary::ScriptStartPath))
	{
		ResourceData = FAssetData();
		return;
	}

	ResourceData = InResourceData;
}

FString SAdvancePakInvolveRegularsForm::DealwithRegularObjectPath() const
{
	return ResourceData.ObjectPath.ToString();
}

void SAdvancePakInvolveRegularsForm::AddResource(const FName& ResourceName)
{
	if (!SelectedResources.Contains(ResourceName))
		SelectedResources.Add(ResourceName);
}

void SAdvancePakInvolveRegularsForm::DealwithSelectLinkwork(bool IsAll)
{
	if (IsAll)
	{
		for (auto ResourceInfo : *ResourcesList)
		{
			SelectedResources.AddUnique(ResourceInfo->PackageName);
		}
	}
	else
	{
		SelectedResources.Reset();
	}
}

void SAdvancePakInvolveRegularsForm::DealwithAddResourcesLinkwork()
{
	if (!ResourceData.IsValid() || ResourceData.IsRedirector())
	{
		ResourceData = FAssetData();
		return;
	}

	for (auto ResourceInfo : AdvancePakCreator->InvolveRegulars)
	{
		if (ResourceInfo->PackageName.IsEqual(ResourceData.PackageName))
			return;
	}

	TSharedPtr<FAdvancePakRegularOption> ResourceInfo = MakeShareable(new FAdvancePakRegularOption());
	ResourceInfo->PackageName = ResourceData.PackageName;
	ResourceInfo->RelyonType = EAdvancePakRelyonType::Packages;
	ResourceInfo->GatherMode = EAdvancePakGatherMode::Local;

	AdvancePakCreator->GetFuturePatch()->InvolveRegulars.Add(ResourceInfo);
	AdvancePakCreator->InvolveRegulars.Add(ResourceInfo);
	AdvancePakCreator->InvolveRegularsForm->DemandViewRenovate();

	AdvancePakCreator->IsFuturePublishDirty = true;
	PromoteFive.ExecuteIfBound();
	PromoteTen.ExecuteIfBound();
	RenovateFour.ExecuteIfBound();
	RenovateTwo.ExecuteIfBound();
	RenovateThree.ExecuteIfBound();
}


