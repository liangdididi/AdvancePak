// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdvancePakPatchResourcesRaw.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SHyperlink.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Layout/SBorder.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "Forms/SAdvancePakPatchResourcesForm.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakPatchResourcesRaw::Construct(const FArguments& InArgs, SAdvancePakPatchResourcesForm* InResourcesForm)
{
	ResourceInfo = InArgs._ResourceInfo;
	AdvancePakCreator = InArgs._AdvancePakCreator;
	PatchResourcesForm = InResourcesForm;
	SMultiColumnTableRow<TSharedPtr<FAdvancePakPatchResourceInfo>>::Construct(FSuperRowType::FArguments(), InArgs._OwnerTableView.ToSharedRef());
}

TSharedRef<SWidget> SAdvancePakPatchResourcesRaw::GenerateWidgetForColumn(const FName& ColumnName)
{
	if (ColumnName.IsEqual(AdvancePakCreator->ResourceNameName))
	{
		return SNew(SOverlay)

			+ SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			[
				SNew(SCheckBox)
				.IsChecked(this, &SAdvancePakPatchResourcesRaw::DealwithCheckBoxIsChecked)
			.OnCheckStateChanged(this, &SAdvancePakPatchResourcesRaw::DealwithCheckBoxCheckStateChanged)
			.Visibility_Lambda([this]()->EVisibility { return ResourceInfo->ResourceState == EAdvancePakResourceState::Free ? EVisibility::Visible : EVisibility::Collapsed; })
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return FText::FromName(ResourceInfo->ResourceName); })
			.ToolTipText_Lambda([this]() { return FText::FromName(ResourceInfo->ResourceName); })
			.ColorAndOpacity(this, &SAdvancePakPatchResourcesRaw::DealwithResourceColor)
			]
			]

		+ SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(18.f, 0.f, 0.f, 0.f)
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return FText::FromName(ResourceInfo->ResourceName); })
			.ToolTipText_Lambda([this]() { return FText::FromName(ResourceInfo->ResourceName); })
			.ColorAndOpacity(this, &SAdvancePakPatchResourcesRaw::DealwithResourceColor)
			.Visibility_Lambda([this]()->EVisibility { return ResourceInfo->ResourceState != EAdvancePakResourceState::Free ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed; })
			];
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->BrowseName))
	{
		return SNew(SHorizontalBox)
			.Visibility_Lambda([this]() -> EVisibility { return ResourceInfo->ResourceState == EAdvancePakResourceState::InValid ? EVisibility::Hidden : EVisibility::Visible; })

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.FillWidth(1.f)
			[
				PropertyCustomizationHelpers::MakeBrowseButton(
					FSimpleDelegate::CreateRaw(this, &SAdvancePakPatchResourcesRaw::DealwithResourceBrowse)
				)
			];
	}
	return SNullWidget::NullWidget;
}

void SAdvancePakPatchResourcesRaw::DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState)
{
	if (NewState == ECheckBoxState::Checked)
	{
		PatchResourcesForm->AddResource(ResourceInfo->ResourceName);
	}
	else
	{
		PatchResourcesForm->RemoveResource(ResourceInfo->ResourceName);
	}
}

ECheckBoxState SAdvancePakPatchResourcesRaw::DealwithCheckBoxIsChecked() const
{
	if (PatchResourcesForm->GetSelectedResources().Contains(ResourceInfo->ResourceName))
		return ECheckBoxState::Checked;

	return ECheckBoxState::Unchecked;
}

void SAdvancePakPatchResourcesRaw::DealwithResourceBrowse()
{
	if (PatchResourcesForm->IsRegular)
	{
		TArray<FAssetData> ResourceDatas;
		UAdvancePakEditorLibrary::GetResourceDatasByPackageName(ResourceInfo->ResourceName, ResourceDatas);
		if (ResourceDatas.Num() > 0)
			GEditor->SyncBrowserToObjects(ResourceDatas);
	}
	else
	{
		FPlatformProcess::ExploreFolder(*ResourceInfo->ResourcePath);
	}
}

FSlateColor SAdvancePakPatchResourcesRaw::DealwithResourceColor() const
{
	switch (ResourceInfo->ResourceState)
	{
	case EAdvancePakResourceState::Free:
		return FLinearColor::Green;
	case EAdvancePakResourceState::Allot:
		return FLinearColor::Yellow;
	case EAdvancePakResourceState::InValid:
		return FLinearColor::Red;
	}
	return FLinearColor::White;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
