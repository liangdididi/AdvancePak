// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakPublishResourcesForm.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Raws/SAdvancePakPublishResourcesRaw.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SHyperlink.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "Utility/AdvancePakCreator.h"
#include "SAdvancePakInvolveRegularsForm.h"
#include "SAdvancePakInformalsForm.h"
#include "SAdvancePakRelyonsForm.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakPublishResourcesForm::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;
	ResourcesList = InArgs._ListItemsSource;
	IsFormer = InArgs._IsFormer;
	IsRegular = InArgs._IsRegular;
	IsPublish = InArgs._IsPublish;

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
		.ColorAndOpacity(IsFormer ? FLinearColor(1.f, 1.f, 0.f) : FLinearColor(0.f, 1.f, 0.f))
		]
		]

	+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(ResourcesListView, SListView<TSharedPtr<FAdvancePakPublishResourceInfo>>)
			.HeaderRow(
				SNew(SHeaderRow)
				.Visibility(EVisibility::SelfHitTestInvisible)

				+ SHeaderRow::Column(AdvancePakCreator->ResourceNameName)
				.DefaultLabel(OPTEXT("ResourceName"))
				.FillWidth(7.0f)

				+ SHeaderRow::Column(AdvancePakCreator->PatchNameName)
				.DefaultLabel(OPTEXT("PatchName"))
				.FillWidth(1.0f)

				+ SHeaderRow::Column(AdvancePakCreator->BrowseName)
				.DefaultLabel(OPTEXT("BrowseName"))
				.FillWidth(0.4f)
			)
		.ItemHeight(16.f)
		.ListItemsSource(ResourcesList)
		.OnGenerateRow(this, &SAdvancePakPublishResourcesForm::DealwithResourceViewPromoteRow)
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 6.0f, 0.0f, 4.0f)
		[
			SNew(SSeparator)
			.Orientation(Orient_Horizontal)
		.Visibility_Lambda([this]()->EVisibility {return !IsPublish ?
			EVisibility::Visible : EVisibility::Collapsed; })
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Center)
		[
			SNew(SHorizontalBox)
			.Visibility_Lambda([this]()->EVisibility {return !IsPublish ?
				EVisibility::Visible : EVisibility::Collapsed; })
		.IsEnabled_Lambda([this]() { return AdvancePakCreator->GetFuturePatch().IsValid() && ((IsRegular && !AdvancePakCreator->IsPatchFormerPublishRegularsStateRunning) || (!IsRegular && !AdvancePakCreator->IsPatchFormerPublishInformalsStateRunning)); })

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
			.OnNavigate(this, &SAdvancePakPublishResourcesForm::DealwithSelectLinkwork, true)
		.Text(OPTEXT("All"))
		.ToolTipText(OPTEXT("SelectAllResources"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakPublishResourcesForm::DealwithSelectLinkwork, false)
		.Text(OPTEXT("None"))
		.ToolTipText(OPTEXT("SelectNoneResources"))
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			SNullWidget::NullWidget
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0.f, 0.f, 8.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakPublishResourcesForm::DealwithViewResourcesLinkwork)
		.Text(OPTEXT("Relyon"))
		.ToolTipText(OPTEXT("ViewResourcesRelyon"))
		.Visibility_Lambda([this]()->EVisibility { return !IsPublish && IsRegular ? EVisibility::Visible : EVisibility::Collapsed; })
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0.f, 0.f, 8.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakPublishResourcesForm::DealwithAddResourcesLinkwork)
		.Text(OPTEXT("Push"))
		.ToolTipText(OPTEXT("PushResourcesToPatch"))
		.Visibility_Lambda([this]()->EVisibility { return !IsPublish ? EVisibility::Visible : EVisibility::Collapsed; })
		]

		]
		]
		];
}



END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAdvancePakPublishResourcesForm::DealwithSelectLinkwork(bool IsAll)
{
	SelectedResources.Reset();
	if (IsAll)
	{
		for (auto ResourceInfo : *ResourcesList)
		{
			if (ResourceInfo->ResourceState == EAdvancePakResourceState::Free)
				SelectedResources.AddUnique(ResourceInfo->ResourceName);
		}
	}
}

void SAdvancePakPublishResourcesForm::DealwithAddResourcesLinkwork()
{
	if (SelectedResources.Num() == 0)
		return;

	if (IsRegular)
	{
		TSharedPtr<FAdvancePakPatchConfigure> FuturePatch = AdvancePakCreator->GetFuturePatch();
		bool IsChanged = false;
		for (auto PackageName : SelectedResources)
		{
			bool IsExisted = false;
			for (auto ResourceInfo : AdvancePakCreator->InvolveRegulars)
			{
				if (ResourceInfo->PackageName.IsEqual(PackageName))
				{
					IsExisted = true;
					break;
				}
			}
			if (!IsExisted)
			{
				TSharedPtr<FAdvancePakRegularOption> ResourceInfo = MakeShareable(new FAdvancePakRegularOption());
				ResourceInfo->PackageName = PackageName;
				ResourceInfo->RelyonType = EAdvancePakRelyonType::Nothing;
				ResourceInfo->GatherMode = EAdvancePakGatherMode::Local;
				FuturePatch->InvolveRegulars.Add(ResourceInfo);
				AdvancePakCreator->InvolveRegulars.Add(ResourceInfo);
				IsChanged = true;
			}
		}
		if (IsChanged)
		{
			AdvancePakCreator->InvolveRegularsForm->DemandViewRenovate();
		}

		AdvancePakCreator->IsFuturePublishDirty = true;
		PromoteFive.ExecuteIfBound();
		PromoteTen.ExecuteIfBound();
		RenovateFour.ExecuteIfBound();
		RenovateTwo.ExecuteIfBound();
		RenovateThree.ExecuteIfBound();
	}
	else
	{
		TArray<TSharedPtr<FAdvancePakInformalInfo>> SelectedInformals;
		for (auto InformalName : SelectedResources)
		{
			TSharedPtr<FAdvancePakInformalInfo> InformalInfo = MakeShareable(new FAdvancePakInformalInfo());
			InformalInfo->InformalName = InformalName;
			for (auto ResourceInfo : *ResourcesList)
			{
				if (ResourceInfo->ResourceName.IsEqual(InformalName))
				{
					InformalInfo->InformalPath = ResourceInfo->ResourcePath;
					break;
				}
			}
			SelectedInformals.Add(InformalInfo);
		}

		TSharedPtr<FAdvancePakPatchConfigure> FuturePatch = AdvancePakCreator->GetFuturePatch();

		bool IsChanged = false;
		for (auto InformalInfo : SelectedInformals)
		{
			bool IsExisted = false;
			for (auto Informal : AdvancePakCreator->InvolveInformals)
			{
				if (Informal->InformalName.IsEqual(InformalInfo->InformalName))
				{
					IsExisted = true;
					break;
				}
			}

			if (!IsExisted)
			{
				IsChanged = true;
				FuturePatch->InvolveInformals.Add(InformalInfo);
				AdvancePakCreator->InvolveInformals.Add(InformalInfo);
			}
		}

		if (IsChanged)
		{
			AdvancePakCreator->InvolveInformalsForm->DemandViewRenovate();
		}

		PromoteFour.ExecuteIfBound();
		PromoteEight.ExecuteIfBound();
		RenovateFive.ExecuteIfBound();
		RenovateOne.ExecuteIfBound();
		RenovateSix.ExecuteIfBound();
	}
}


void SAdvancePakPublishResourcesForm::ViewRelyons(const TArray<FName>& PackageNames)
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

void SAdvancePakPublishResourcesForm::DealwithViewResourcesLinkwork()
{
	if (SelectedResources.Num() == 0)
		return;

	ViewRelyons(SelectedResources);
}

void SAdvancePakPublishResourcesForm::AddResource(const FName& PackageName)
{
	if (!SelectedResources.Contains(PackageName))
		SelectedResources.Add(PackageName);
}

const TArray<FName>& SAdvancePakPublishResourcesForm::GetSelectedResources()
{
	return SelectedResources;
}

void SAdvancePakPublishResourcesForm::RemoveResource(const FName& PackageName)
{
	if (SelectedResources.Contains(PackageName))
		SelectedResources.Remove(PackageName);
}

TSharedRef<ITableRow> SAdvancePakPublishResourcesForm::DealwithResourceViewPromoteRow(TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SAdvancePakPublishResourcesRaw, this)
		.OwnerTableView(OwnerTable)
		.AdvancePakCreator(AdvancePakCreator)
		.ResourceInfo(ResourceInfo)
		;
}

void SAdvancePakPublishResourcesForm::DemandViewRenovate()
{
	SelectedResources.Reset();
	ResourcesListView->RequestListRefresh();
}

void SAdvancePakPublishResourcesForm::DemandSelectRenovate()
{
	TArray<FName> AbandonResources;

	for (auto PackageName : SelectedResources)
	{
		bool IsAbandon = true;
		for (auto ResourceInfo : *ResourcesList)
		{
			if (ResourceInfo->ResourceName.IsEqual(PackageName) && ResourceInfo->ResourceState == EAdvancePakResourceState::Free)
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
