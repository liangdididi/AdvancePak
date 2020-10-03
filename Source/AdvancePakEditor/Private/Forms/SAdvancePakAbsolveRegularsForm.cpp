// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakAbsolveRegularsForm.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SComboBox.h"
#include "Raws/SAdvancePakAbsolveRegularsRaw.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SHyperlink.h"
#include "Utility/AdvancePakCreator.h"
#include "Editor.h"
#include "SAdvancePakRelyonsForm.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakAbsolveRegularsForm::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
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
			SAssignNew(ResourcesListView, SListView<TSharedPtr<FName>>)
			.HeaderRow(
				SNew(SHeaderRow)
				.Visibility(EVisibility::SelfHitTestInvisible)

				+ SHeaderRow::Column(AdvancePakCreator->PackageNameName)
				.DefaultLabel(OPTEXT("PackageName"))
				.FillWidth(8.0f)

				+ SHeaderRow::Column(AdvancePakCreator->BrowseName)
				.DefaultLabel(OPTEXT("BrowseName"))
				.FillWidth(0.4f)
			)
		.ItemHeight(16.f)
		.ListItemsSource(ResourcesList)
		.OnGenerateRow(this, &SAdvancePakAbsolveRegularsForm::DealwithResourceViewPromoteRow)
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
			.OnNavigate(this, &SAdvancePakAbsolveRegularsForm::DealwithSelectLinkwork, true)
		.Text(OPTEXT("All"))
		.ToolTipText(OPTEXT("SelectAllResources"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakAbsolveRegularsForm::DealwithSelectLinkwork, false)
		.Text(OPTEXT("None"))
		.ToolTipText(OPTEXT("SelectNoneResources"))
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SObjectPropertyEntryBox)
			.OnObjectChanged(this, &SAdvancePakAbsolveRegularsForm::DealwithRegularObjectChanged)
		.ObjectPath_Raw(this, &SAdvancePakAbsolveRegularsForm::DealwithRegularObjectPath)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakAbsolveRegularsForm::DealwithViewResourcesLinkwork)
		.Text(OPTEXT("Relyon"))
		.ToolTipText(OPTEXT("ViewResourcesRelyon"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakAbsolveRegularsForm::DealwithAddResourcesLinkwork)
		.Text(OPTEXT("Push"))
		.ToolTipText(OPTEXT("PushResourcesToPatch"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 8.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakAbsolveRegularsForm::DealwithRemoveResourcesLinkwork)
		.Text(OPTEXT("Remove"))
		.ToolTipText(OPTEXT("RemoveResourcesFromPatch"))
		]

		]
		]
		];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAdvancePakAbsolveRegularsForm::DealwithRegularObjectChanged(const FAssetData& InResourceData)
{
	if (InResourceData.PackageName.ToString().StartsWith(UAdvancePakEditorLibrary::ScriptStartPath))
	{
		ResourceData = FAssetData();
		return;
	}

	ResourceData = InResourceData;
}

void SAdvancePakAbsolveRegularsForm::DealwithViewResourcesLinkwork()
{
	if (SelectedResources.Num() == 0)
		return;

	ViewRelyons(SelectedResources);
}

FString SAdvancePakAbsolveRegularsForm::DealwithRegularObjectPath() const
{
	return ResourceData.ObjectPath.ToString();
}

void SAdvancePakAbsolveRegularsForm::RemoveResource(const FName& ResourceName)
{
	if (SelectedResources.Contains(ResourceName))
		SelectedResources.Remove(ResourceName);
}

void SAdvancePakAbsolveRegularsForm::DemandViewRenovate()
{
	DemandSelectRenovate();
	ResourcesListView->RequestListRefresh();
}

void SAdvancePakAbsolveRegularsForm::AddResource(const FName& ResourceName)
{
	if (!SelectedResources.Contains(ResourceName))
		SelectedResources.Add(ResourceName);
}

void SAdvancePakAbsolveRegularsForm::DealwithSelectLinkwork(bool IsAll)
{
	if (IsAll)
	{
		for (auto PackageName : *ResourcesList)
		{
			SelectedResources.AddUnique(*PackageName);
		}
	}
	else
	{
		SelectedResources.Reset();
	}
}

void SAdvancePakAbsolveRegularsForm::DealwithRemoveResourcesLinkwork()
{
	if (SelectedResources.Num() == 0)
		return;

	for (auto PackageName : SelectedResources)
	{
		int32 RemoveIndex = -1;
		for (int i = 0; i < AdvancePakCreator->AbsolveRegulars.Num(); ++i)
		{
			if (AdvancePakCreator->AbsolveRegulars[i]->IsEqual(PackageName))
			{
				RemoveIndex = i;
				break;
			}
		}
		if (RemoveIndex >= 0)
		{
			AdvancePakCreator->GetFuturePatch()->AbsolveRegulars.Remove(PackageName);
			AdvancePakCreator->AbsolveRegulars.RemoveAt(RemoveIndex);
		}
	}
	AdvancePakCreator->AbsolveRegularsForm->DemandViewRenovate();

	PromoteSeven.ExecuteIfBound();
	RenovateFour.ExecuteIfBound();
	RenovateTwo.ExecuteIfBound();
	RenovateThree.ExecuteIfBound();

	SelectedResources.Reset();
}

void SAdvancePakAbsolveRegularsForm::DealwithAddResourcesLinkwork()
{
	if (!ResourceData.IsValid() || ResourceData.IsRedirector())
	{
		ResourceData = FAssetData();
		return;
	}

	TSharedPtr<FAdvancePakPatchConfigure> FuturePatch = AdvancePakCreator->GetFuturePatch();

	if (FuturePatch->AbsolveRegulars.Contains(ResourceData.PackageName))
		return;

	FuturePatch->AbsolveRegulars.Add(ResourceData.PackageName);
	AdvancePakCreator->AbsolveRegulars.Add(MakeShareable(new FName(ResourceData.PackageName)));
	AdvancePakCreator->AbsolveRegularsForm->DemandViewRenovate();

	AdvancePakCreator->IsFuturePublishDirty = true;
	PromoteSeven.ExecuteIfBound();
	PromoteTen.ExecuteIfBound();
	RenovateFour.ExecuteIfBound();
	RenovateTwo.ExecuteIfBound();
	RenovateThree.ExecuteIfBound();
}

void SAdvancePakAbsolveRegularsForm::ViewRelyons(const TArray<FName>& PackageNames)
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

TSharedRef<ITableRow> SAdvancePakAbsolveRegularsForm::DealwithResourceViewPromoteRow(TSharedPtr<FName> PackageName, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SAdvancePakAbsolveRegularsRaw, this)
		.OwnerTableView(OwnerTable)
		.AdvancePakCreator(AdvancePakCreator)
		.PackageName(PackageName)
		;
}

void SAdvancePakAbsolveRegularsForm::DemandSelectRenovate()
{
	TArray<FName> AbandonResources;

	for (auto PackageName : SelectedResources)
	{
		bool IsAbandon = true;
		for (auto Regular : *ResourcesList)
		{
			if (Regular->IsEqual(PackageName))
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

const TArray<FName>& SAdvancePakAbsolveRegularsForm::GetSelectedResources()
{
	return SelectedResources;
}