// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdvancePakPublishResourcesRaw.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SHyperlink.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Layout/SBorder.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "Forms/SAdvancePakPublishResourcesForm.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakPublishResourcesRaw::Construct(const FArguments& InArgs, SAdvancePakPublishResourcesForm* InResourcesForm)
{
	ResourceInfo = InArgs._ResourceInfo;
	AdvancePakCreator = InArgs._AdvancePakCreator;
	PublishResourcesForm = InResourcesForm;
	SMultiColumnTableRow<TSharedPtr<FAdvancePakPublishResourceInfo>>::Construct(FSuperRowType::FArguments(), InArgs._OwnerTableView.ToSharedRef());
}

TSharedRef<SWidget> SAdvancePakPublishResourcesRaw::GenerateWidgetForColumn(const FName& ColumnName)
{
	if (ColumnName.IsEqual(AdvancePakCreator->ResourceNameName))
	{
		return SNew(SOverlay)

			+ SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return FText::FromName(ResourceInfo->ResourceName); })
			.ToolTipText_Lambda([this]() { return FText::FromName(ResourceInfo->ResourceName); })
			.ColorAndOpacity(this, &SAdvancePakPublishResourcesRaw::DealwithResourceColor)
			.Visibility_Lambda([this]()->EVisibility { return PublishResourcesForm->IsPublish ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed; })
			]

		+ SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			[
				SNew(SCheckBox)
				.IsChecked(this, &SAdvancePakPublishResourcesRaw::DealwithCheckBoxIsChecked)
			.OnCheckStateChanged(this, &SAdvancePakPublishResourcesRaw::DealwithCheckBoxCheckStateChanged)
			.Visibility_Lambda([this]()->EVisibility { return !PublishResourcesForm->IsPublish && ResourceInfo->ResourceState == EAdvancePakResourceState::Free ? EVisibility::Visible : EVisibility::Collapsed; })
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return FText::FromName(ResourceInfo->ResourceName); })
			.ToolTipText_Lambda([this]() { return FText::FromName(ResourceInfo->ResourceName); })
			.ColorAndOpacity(this, &SAdvancePakPublishResourcesRaw::DealwithResourceColor)
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
			.ColorAndOpacity(this, &SAdvancePakPublishResourcesRaw::DealwithResourceColor)
			.Visibility_Lambda([this]()->EVisibility { return !PublishResourcesForm->IsPublish && ResourceInfo->ResourceState != EAdvancePakResourceState::Free ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed; })
			];
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->PatchNameName))
	{
		return SNew(STextBlock)
			.Text_Lambda([this]() { return FText::FromName(ResourceInfo->PatchName); })
			.ColorAndOpacity(this, &SAdvancePakPublishResourcesRaw::DealwithResourceColor);
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
					FSimpleDelegate::CreateRaw(this, &SAdvancePakPublishResourcesRaw::DealwithResourceBrowse)
				)
			];
	}

	return SNullWidget::NullWidget;
}

void SAdvancePakPublishResourcesRaw::DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState)
{
	if (NewState == ECheckBoxState::Checked)
	{
		PublishResourcesForm->AddResource(ResourceInfo->ResourceName);
	}
	else
	{
		PublishResourcesForm->RemoveResource(ResourceInfo->ResourceName);
	}
}

ECheckBoxState SAdvancePakPublishResourcesRaw::DealwithCheckBoxIsChecked() const
{
	if (PublishResourcesForm->GetSelectedResources().Contains(ResourceInfo->ResourceName))
		return ECheckBoxState::Checked;

	return ECheckBoxState::Unchecked;
}

void SAdvancePakPublishResourcesRaw::DealwithResourceBrowse()
{
	if (PublishResourcesForm->IsRegular)
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

FSlateColor SAdvancePakPublishResourcesRaw::DealwithResourceColor() const
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
