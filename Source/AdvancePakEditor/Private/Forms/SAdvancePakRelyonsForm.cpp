// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakRelyonsForm.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SHyperlink.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Editor.h"
#include "SAdvancePakAbsolveRegularsForm.h"
#include "Widgets/SOverlay.h"
#include "Raws/SAdvancePakRelyonsRaw.h"
#include "Widgets/Input/SComboBox.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "SAdvancePakInvolveRegularsForm.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakRelyonsForm::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;
	RelyonsList = InArgs._TreeItemsSource;

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
			SAssignNew(RelyonsForm, STreeView<TSharedPtr<FAdvancePakRelyonInfo>>)
			.HeaderRow(
				SNew(SHeaderRow)
				.Visibility(EVisibility::SelfHitTestInvisible)

				+ SHeaderRow::Column(AdvancePakCreator->PackageNameName)
				.DefaultLabel(OPTEXT("PackageName"))
				.FillWidth(6.0f)

				+ SHeaderRow::Column(AdvancePakCreator->PatchNameName)
				.DefaultLabel(OPTEXT("PatchName"))
				.FillWidth(0.7f)

				+ SHeaderRow::Column(AdvancePakCreator->RelyonTypeName)
				.DefaultLabel(OPTEXT("RelyonType"))
				.FillWidth(0.7f)

				+ SHeaderRow::Column(AdvancePakCreator->UnViewName)
				.DefaultLabel(OPTEXT("UnView"))
				.FillWidth(0.6f)

				+ SHeaderRow::Column(AdvancePakCreator->BrowseName)
				.DefaultLabel(OPTEXT("BrowseName"))
				.FillWidth(0.4f)
			)
		.ItemHeight(16.f)
		.TreeItemsSource(RelyonsList)
		.OnGenerateRow(this, &SAdvancePakRelyonsForm::DealwithResourceViewPromoteRow)
		.OnGetChildren(this, &SAdvancePakRelyonsForm::DealwithResourceViewGetChildren)
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
			.OnNavigate(this, &SAdvancePakRelyonsForm::DealwithSelectLinkwork, true)
		.Text(OPTEXT("All"))
		.ToolTipText(OPTEXT("SelectAllResources"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakRelyonsForm::DealwithSelectLinkwork, false)
		.Text(OPTEXT("None"))
		.ToolTipText(OPTEXT("SelectNoneResources"))
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SObjectPropertyEntryBox)
			.OnObjectChanged(this, &SAdvancePakRelyonsForm::DealwithRegularObjectChanged)
		.ObjectPath_Raw(this, &SAdvancePakRelyonsForm::DealwithRegularObjectPath)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakRelyonsForm::DealwithViewLinkwork)
		.Text(OPTEXT("Relyon"))
		.ToolTipText(OPTEXT("ViewResourcesRelyon"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakRelyonsForm::DealwithInvolveLinkwork)
		.Text(OPTEXT("Push"))
		.ToolTipText(OPTEXT("PushResourcesToPatch"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 8.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakRelyonsForm::DealwithInvolveLinkwork)
		.Text(OPTEXT("Remove"))
		.ToolTipText(OPTEXT("RemoveResourcesFromPatch"))
		]

		]
		]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAdvancePakRelyonsForm::DealwithResourceViewGetChildren(TSharedPtr<FAdvancePakRelyonInfo> RelyonInfo, TArray<TSharedPtr<FAdvancePakRelyonInfo>>& OutRelyons)
{
	if (RelyonInfo->RelyonType == EAdvancePakRelyonType::Nothing)
		return;

	TArray<FName> Relyons;
	UAdvancePakEditorLibrary::GetResourceRelyons(RelyonInfo->PackageName, RelyonInfo->RelyonType, Relyons);

	for (auto PackageName : Relyons)
	{
		if (RelyonInfo->Relyons.Contains(PackageName) || RelyonInfo->PackageName.IsEqual(PackageName))
			continue;
		TSharedPtr<FAdvancePakRelyonInfo> NewRelyonInfo = MakeShareable(new FAdvancePakRelyonInfo());
		NewRelyonInfo->PackageName = PackageName;
		NewRelyonInfo->PatchName = AdvancePakCreator->GetFutureRegularPatchName(PackageName);
		NewRelyonInfo->RelyonType = EAdvancePakRelyonType::Packages;
		NewRelyonInfo->IsTop = false;
		RelyonInfo->Relyons.Add(PackageName, NewRelyonInfo);
	}
	RelyonInfo->Relyons.GenerateValueArray(OutRelyons);
}

void SAdvancePakRelyonsForm::DealwithViewLinkwork()
{
	if (!ResourceData.IsValid() || ResourceData.IsRedirector())
	{
		ResourceData = FAssetData();
		return;
	}

	for (auto Relyon : AdvancePakCreator->Relyons)
	{
		if (Relyon->PackageName.IsEqual(ResourceData.PackageName))
			return;
	}
	TSharedPtr<FAdvancePakRelyonInfo> RelyonInfo = MakeShareable(new FAdvancePakRelyonInfo());
	RelyonInfo->PackageName = ResourceData.PackageName;
	RelyonInfo->PatchName = AdvancePakCreator->GetFutureRegularPatchName(ResourceData.PackageName);
	RelyonInfo->RelyonType = EAdvancePakRelyonType::Packages;
	RelyonInfo->IsTop = true;
	AdvancePakCreator->Relyons.Add(RelyonInfo);
	AdvancePakCreator->RelyonsForm->DemandViewRenovate();
}

void SAdvancePakRelyonsForm::DealwithInvolveLinkwork()
{
	if (SelectedInvolves.Num() == 0)
		return;

	TSharedPtr<FAdvancePakPatchConfigure> FuturePatch = AdvancePakCreator->GetFuturePatch();
	bool IsChanged = false;
	for (auto PackageName : SelectedInvolves)
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
		AdvancePakCreator->InvolveRegularsForm->DemandViewRenovate();

	AdvancePakCreator->IsFuturePublishDirty = true;
	PromoteFive.ExecuteIfBound();
	PromoteTen.ExecuteIfBound();
	RenovateFour.ExecuteIfBound();
	RenovateTwo.ExecuteIfBound();
	RenovateThree.ExecuteIfBound();
}

const TArray<FName>& SAdvancePakRelyonsForm::GetSelectedInvolves()
{
	return SelectedInvolves;
}

void SAdvancePakRelyonsForm::DealwithSelectLinkwork(bool IsAll)
{
	if (IsAll)
	{
		for (auto RelyonInfo : *RelyonsList)
		{
			DealwithSelectRecursion(RelyonInfo);
		}
	}
	else
	{
		SelectedInvolves.Reset();
		SelectedAbsolves.Reset();
	}
}

const TArray<FName>& SAdvancePakRelyonsForm::GetSelectedAbsolves()
{
	return SelectedAbsolves;
}

void SAdvancePakRelyonsForm::AddInvolve(const FName& PackageName)
{
	if (!SelectedInvolves.Contains(PackageName))
		SelectedInvolves.Add(PackageName);
}

void SAdvancePakRelyonsForm::RemoveInvolve(const FName& PackageName)
{
	if (SelectedInvolves.Contains(PackageName))
		SelectedInvolves.Remove(PackageName);
}

TSharedRef<ITableRow> SAdvancePakRelyonsForm::DealwithResourceViewPromoteRow(TSharedPtr<FAdvancePakRelyonInfo> RelyonInfo, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SAdvancePakRelyonsRaw, this)
		.OwnerTableView(OwnerTable)
		.AdvancePakCreator(AdvancePakCreator)
		.RelyonInfo(RelyonInfo)
		;
}

void SAdvancePakRelyonsForm::AddAbsolve(const FName& PackageName)
{
	if (!SelectedAbsolves.Contains(PackageName))
		SelectedAbsolves.Add(PackageName);
}

void SAdvancePakRelyonsForm::DemandViewRenovate()
{
	SelectedInvolves.Reset();
	SelectedAbsolves.Reset();
	RelyonsForm->RequestTreeRefresh();
}

void SAdvancePakRelyonsForm::DemandSelectRenovate()
{
	TArray<FName> AbandonInvolves;
	TArray<FName> AbandonAbsolves;

	for (auto PackageName : SelectedInvolves)
	{
		bool IsAbandon = true;
		for (auto Relyon : *RelyonsList)
		{
			if (ResourceExistRecursion(Relyon, PackageName))
			{
				IsAbandon = false;
				break;
			}
		}
		if (IsAbandon)
		{
			AbandonInvolves.Add(PackageName);
		}
	}

	for (auto PackageName : SelectedAbsolves)
	{
		bool IsAbandon = true;
		for (auto Relyon : *RelyonsList)
		{
			if (ResourceExistRecursion(Relyon, PackageName))
			{
				IsAbandon = false;
				break;
			}
		}
		if (IsAbandon)
		{
			AbandonAbsolves.Add(PackageName);
		}
	}

	for (auto PackageName : AbandonInvolves)
		SelectedInvolves.Remove(PackageName);

	for (auto PackageName : AbandonAbsolves)
		SelectedAbsolves.Remove(PackageName);
}

bool SAdvancePakRelyonsForm::ResourceExistRecursion(const TSharedPtr<FAdvancePakRelyonInfo>& RelyonInfo, const FName& PackageName)
{
	if (RelyonInfo->PackageName.IsEqual(PackageName))
		return true;

	for (TMap<FName, TSharedPtr<FAdvancePakRelyonInfo>>::TIterator It(RelyonInfo->Relyons); It; ++It)
		return ResourceExistRecursion(RelyonInfo, PackageName);

	return false;
}

void SAdvancePakRelyonsForm::RemoveAbsolve(const FName& PackageName)
{
	if (SelectedAbsolves.Contains(PackageName))
		SelectedAbsolves.Remove(PackageName);
}

void SAdvancePakRelyonsForm::DealwithSelectRecursion(const TSharedPtr<FAdvancePakRelyonInfo>& RelyonInfo)
{
	if (RelyonInfo->PatchName.IsNone())
	{
		SelectedInvolves.AddUnique(RelyonInfo->PackageName);
	}
	else if (RelyonInfo->PatchName.IsEqual(AdvancePakCreator->ActiveName))
	{
		SelectedAbsolves.AddUnique(RelyonInfo->PackageName);
	}
	for (TMap<FName, TSharedPtr<FAdvancePakRelyonInfo>>::TIterator It(RelyonInfo->Relyons); It; ++It)
	{
		DealwithSelectRecursion(It.Value());
	}
}

FString SAdvancePakRelyonsForm::DealwithRegularObjectPath() const
{
	return ResourceData.ObjectPath.ToString();
}

void SAdvancePakRelyonsForm::DealwithAbsolveLinkwork()
{
	if (SelectedAbsolves.Num() == 0)
		return;

	TSharedPtr<FAdvancePakPatchConfigure> FuturePatch = AdvancePakCreator->GetFuturePatch();
	bool IsChanged = false;
	for (auto PackageName : SelectedAbsolves)
	{
		bool IsExisted = false;
		for (auto AbsolveRegular : AdvancePakCreator->AbsolveRegulars)
		{
			if (AbsolveRegular->IsEqual(PackageName))
			{
				IsExisted = true;
				break;
			}
		}
		if (!IsExisted)
		{
			FuturePatch->AbsolveRegulars.Add(PackageName);
			AdvancePakCreator->AbsolveRegulars.Add(MakeShareable(new FName(PackageName)));
			IsChanged = true;
		}
	}
	if (IsChanged)
		AdvancePakCreator->AbsolveRegularsForm->DemandViewRenovate();

	AdvancePakCreator->IsFuturePublishDirty = true;
	PromoteSeven.ExecuteIfBound();
	PromoteTen.ExecuteIfBound();
	RenovateFour.ExecuteIfBound();
	RenovateTwo.ExecuteIfBound();
	RenovateThree.ExecuteIfBound();
}

void SAdvancePakRelyonsForm::DealwithRegularObjectChanged(const FAssetData& InResourceData)
{
	if (InResourceData.PackageName.ToString().StartsWith(UAdvancePakEditorLibrary::ScriptStartPath))
	{
		ResourceData = FAssetData();
		return;
	}
	ResourceData = InResourceData;
}

