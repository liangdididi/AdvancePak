// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdvancePakGathersRaw.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SHyperlink.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SComboBox.h"
#include "Utility/AdvancePakCreator.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "Forms/SAdvancePakGathersForm.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakGathersRaw::Construct(const FArguments& InArgs, SAdvancePakGathersForm* InGathersForm)
{
	GatherInfo = InArgs._GatherInfo;
	AdvancePakCreator = InArgs._AdvancePakCreator;
	GathersForm = InGathersForm;
	SMultiColumnTableRow<TSharedPtr<FAdvancePakGatherOption>>::Construct(FSuperRowType::FArguments(), InArgs._OwnerTableView.ToSharedRef());
}

TSharedRef<SWidget> SAdvancePakGathersRaw::GenerateWidgetForColumn(const FName& ColumnName)
{
	if (ColumnName.IsEqual(AdvancePakCreator->GatherPathName))
	{
		return SNew(SCheckBox)
			.IsChecked(this, &SAdvancePakGathersRaw::DealwithCheckBoxIsChecked)
			.OnCheckStateChanged(this, &SAdvancePakGathersRaw::DealwithCheckBoxCheckStateChanged)
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return FText::FromString(GatherInfo->GatherPath); })
			.ToolTipText_Lambda([this]() { return FText::FromString(GatherInfo->FullPath); })
			];
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->GatherTypeName))
	{
		TSharedPtr<SComboBox<TSharedPtr<FString>>> GatherTypeCombo;
		SAssignNew(GatherTypeCombo, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&UAdvancePakEditorLibrary::GatherTypeOptions)
			.OnGenerateWidget(this, &SAdvancePakGathersRaw::MakeComboItem)
			.OnSelectionChanged(this, &SAdvancePakGathersRaw::OnGatherTypeChanged)
			.IsEnabled_Lambda([this]() ->bool { return GatherInfo->CanGatherRegular; })
			[
				SNew(STextBlock)
				.Text(this, &SAdvancePakGathersRaw::GetFilterTypeText)
			];
		GatherTypeCombo->RefreshOptions();
		return GatherTypeCombo.ToSharedRef();
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->RelyonTypeName))
	{
		TSharedPtr<SComboBox<TSharedPtr<FString>>> RelyonTypeCombo;
		SAssignNew(RelyonTypeCombo, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&UAdvancePakEditorLibrary::RelyonTypeOptions)
			.OnGenerateWidget(this, &SAdvancePakGathersRaw::MakeComboItem)
			.OnSelectionChanged(this, &SAdvancePakGathersRaw::OnRelyonTypeChanged)
			.IsEnabled_Lambda([this]() ->bool { return GatherInfo->GatherType != EAdvancePakGatherType::Informal; })
			[
				SNew(STextBlock)
				.Text(this, &SAdvancePakGathersRaw::GetRelyonTypeText)
			];
		RelyonTypeCombo->RefreshOptions();
		return RelyonTypeCombo.ToSharedRef();
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->GatherModeName))
	{
		TSharedPtr<SComboBox<TSharedPtr<FString>>> GatherModeCombo;
		SAssignNew(GatherModeCombo, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&UAdvancePakEditorLibrary::GatherModeOptions)
			.OnGenerateWidget(this, &SAdvancePakGathersRaw::MakeComboItem)
			.OnSelectionChanged(this, &SAdvancePakGathersRaw::OnGatherModeChanged)
			[
				SNew(STextBlock)
				.Text(this, &SAdvancePakGathersRaw::GetGatherModeText)
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
					FSimpleDelegate::CreateRaw(this, &SAdvancePakGathersRaw::DealwithGatherBrowse)
				)
			];
	}
	return SNullWidget::NullWidget;
}

void SAdvancePakGathersRaw::DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState)
{
	if (NewState == ECheckBoxState::Checked)
	{
		GathersForm->AddGather(GatherInfo->GatherPath);
	}
	else
	{
		GathersForm->RemoveGather(GatherInfo->GatherPath);
	}
}

ECheckBoxState SAdvancePakGathersRaw::DealwithCheckBoxIsChecked() const
{
	if (GathersForm->GetSelectedGathers().Contains(GatherInfo->GatherPath))
		return ECheckBoxState::Checked;

	return ECheckBoxState::Unchecked;
}

TSharedRef<SWidget> SAdvancePakGathersRaw::MakeComboItem(TSharedPtr<FString> ComboItem)
{
	check(ComboItem.IsValid());
	return	SNew(STextBlock)
		.Text(FText::FromString(*ComboItem));
}

void SAdvancePakGathersRaw::OnGatherTypeChanged(TSharedPtr<FString> ComboItem, ESelectInfo::Type SelectInfo)
{
	check(ComboItem.IsValid());
	GatherInfo->GatherType = *UAdvancePakEditorLibrary::GatherTypeMap.Find(*ComboItem);

	RePromoteResources();
}

void SAdvancePakGathersRaw::OnRelyonTypeChanged(TSharedPtr<FString> ComboItem, ESelectInfo::Type SelectInfo)
{
	check(ComboItem.IsValid());
	GatherInfo->RelyonType = UAdvancePakEditorLibrary::AbsorbStringToRelyonType(*ComboItem);

	RePromoteResources();
}

void SAdvancePakGathersRaw::OnGatherModeChanged(TSharedPtr<FString> ComboItem, ESelectInfo::Type SelectInfo)
{
	check(ComboItem.IsValid());
	GatherInfo->GatherMode = UAdvancePakEditorLibrary::AbsorbStringToGatherMode(*ComboItem);

	RePromoteResources();
}

FText SAdvancePakGathersRaw::GetFilterTypeText() const
{
	return FText::FromString(*UAdvancePakEditorLibrary::GatherTypeOptions[(int32)GatherInfo->GatherType]);
}

FText SAdvancePakGathersRaw::GetRelyonTypeText() const
{
	return FText::FromString(*UAdvancePakEditorLibrary::RelyonTypeOptions[(int32)GatherInfo->RelyonType]);
}

FText SAdvancePakGathersRaw::GetGatherModeText() const
{
	return FText::FromString(*UAdvancePakEditorLibrary::GatherModeOptions[(int32)GatherInfo->GatherMode]);
}

void SAdvancePakGathersRaw::DealwithGatherBrowse()
{
	FPlatformProcess::ExploreFolder(*GatherInfo->FullPath);
}

void SAdvancePakGathersRaw::RePromoteResources()
{
	if (GathersForm->IsInvolve)
	{
		GathersForm->PromoteOne.ExecuteIfBound();
		GathersForm->PromoteTwo.ExecuteIfBound();
	}
	else
	{
		GathersForm->PromoteNine.ExecuteIfBound();
		GathersForm->PromoteThree.ExecuteIfBound();
	}
	GathersForm->PromoteTen.ExecuteIfBound();
	GathersForm->PromoteEight.ExecuteIfBound();
	GathersForm->RenovateFour.ExecuteIfBound();
	GathersForm->RenovateFive.ExecuteIfBound();
	GathersForm->RenovateOne.ExecuteIfBound();
	GathersForm->RenovateTwo.ExecuteIfBound();
	GathersForm->RenovateThree.ExecuteIfBound();
	GathersForm->RenovateSix.ExecuteIfBound();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
