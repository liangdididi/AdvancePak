// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdvancePakInvolveRegularsRaw.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SHyperlink.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SComboBox.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "Forms/SAdvancePakInvolveRegularsForm.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakInvolveRegularsRaw::Construct(const FArguments& InArgs, SAdvancePakInvolveRegularsForm* InInvolveRegularsForm)
{
	ResourceInfo = InArgs._ResourceInfo;
	AdvancePakCreator = InArgs._AdvancePakCreator;
	InvolveRegularsForm = InInvolveRegularsForm;
	SMultiColumnTableRow<TSharedPtr<FAdvancePakRegularOption>>::Construct(FSuperRowType::FArguments(), InArgs._OwnerTableView.ToSharedRef());
}

TSharedRef<SWidget> SAdvancePakInvolveRegularsRaw::GenerateWidgetForColumn(const FName& ColumnName)
{
	if (ColumnName.IsEqual(AdvancePakCreator->PackageNameName))
	{
		return SNew(SCheckBox)
			.IsChecked(this, &SAdvancePakInvolveRegularsRaw::DealwithCheckBoxIsChecked)
			.OnCheckStateChanged(this, &SAdvancePakInvolveRegularsRaw::DealwithCheckBoxCheckStateChanged)
			[
				SNew(STextBlock)
				.Text(FText::FromName(ResourceInfo->PackageName))
			.ToolTipText(FText::FromName(ResourceInfo->PackageName))
			];
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->RelyonTypeName))
	{
		TSharedPtr<SComboBox<TSharedPtr<FString>>> RelyonTypeCombo;
		SAssignNew(RelyonTypeCombo, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&UAdvancePakEditorLibrary::RelyonTypeOptions)
			.OnGenerateWidget(this, &SAdvancePakInvolveRegularsRaw::MakeComboItem)
			.OnSelectionChanged(this, &SAdvancePakInvolveRegularsRaw::OnRelyonTypeChanged)
			[
				SNew(STextBlock)
				.Text(this, &SAdvancePakInvolveRegularsRaw::GetRelyonTypeText)
			];
		RelyonTypeCombo->RefreshOptions();
		return RelyonTypeCombo.ToSharedRef();
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->GatherModeName))
	{
		TSharedPtr<SComboBox<TSharedPtr<FString>>> GatherModeCombo;
		SAssignNew(GatherModeCombo, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&UAdvancePakEditorLibrary::GatherModeOptions)
			.OnGenerateWidget(this, &SAdvancePakInvolveRegularsRaw::MakeComboItem)
			.OnSelectionChanged(this, &SAdvancePakInvolveRegularsRaw::OnGatherModeChanged)
			[
				SNew(STextBlock)
				.Text(this, &SAdvancePakInvolveRegularsRaw::GetGatherModeText)
			];
		GatherModeCombo->RefreshOptions();
		return GatherModeCombo.ToSharedRef();
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->BrowseName))
	{
		return SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.FillWidth(1.f)
			[
				PropertyCustomizationHelpers::MakeBrowseButton(
					FSimpleDelegate::CreateRaw(this, &SAdvancePakInvolveRegularsRaw::DealwithResourceBrowse)
				)
			];
	}
	return SNullWidget::NullWidget;
}

void SAdvancePakInvolveRegularsRaw::DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState)
{
	if (NewState == ECheckBoxState::Checked)
	{
		InvolveRegularsForm->AddResource(ResourceInfo->PackageName);
	}
	else
	{
		InvolveRegularsForm->RemoveResource(ResourceInfo->PackageName);
	}
}

ECheckBoxState SAdvancePakInvolveRegularsRaw::DealwithCheckBoxIsChecked() const
{
	if (InvolveRegularsForm->GetSelectedResources().Contains(ResourceInfo->PackageName))
		return ECheckBoxState::Checked;

	return ECheckBoxState::Unchecked;
}

TSharedRef<SWidget> SAdvancePakInvolveRegularsRaw::MakeComboItem(TSharedPtr<FString> ComboItem)
{
	check(ComboItem.IsValid());
	return	SNew(STextBlock)
		.Text(FText::FromString(*ComboItem));
}

void SAdvancePakInvolveRegularsRaw::OnRelyonTypeChanged(TSharedPtr<FString> ComboItem, ESelectInfo::Type SelectInfo)
{
	check(ComboItem.IsValid());
	ResourceInfo->RelyonType = UAdvancePakEditorLibrary::AbsorbStringToRelyonType(*ComboItem);

	RePromoteResources();
}

FText SAdvancePakInvolveRegularsRaw::GetRelyonTypeText() const
{
	return FText::FromString(*UAdvancePakEditorLibrary::RelyonTypeOptions[(int32)ResourceInfo->RelyonType]);
}

void SAdvancePakInvolveRegularsRaw::DealwithResourceBrowse()
{
	TArray<FAssetData> ResourceDatas;
	UAdvancePakEditorLibrary::GetResourceDatasByPackageName(ResourceInfo->PackageName, ResourceDatas);
	if (ResourceDatas.Num() > 0)
		GEditor->SyncBrowserToObjects(ResourceDatas);
}

void SAdvancePakInvolveRegularsRaw::OnGatherModeChanged(TSharedPtr<FString> ComboItem, ESelectInfo::Type SelectInfo)
{
	check(ComboItem.IsValid());
	ResourceInfo->GatherMode = UAdvancePakEditorLibrary::AbsorbStringToGatherMode(*ComboItem);

	RePromoteResources();
}

FText SAdvancePakInvolveRegularsRaw::GetGatherModeText() const
{
	return FText::FromString(*UAdvancePakEditorLibrary::GatherModeOptions[(int32)ResourceInfo->GatherMode]);
}

void SAdvancePakInvolveRegularsRaw::RePromoteResources()
{
	InvolveRegularsForm->PromoteFive.ExecuteIfBound();
	InvolveRegularsForm->PromoteTen.ExecuteIfBound();
	InvolveRegularsForm->RenovateFour.ExecuteIfBound();
	InvolveRegularsForm->RenovateTwo.ExecuteIfBound();
	InvolveRegularsForm->RenovateThree.ExecuteIfBound();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
