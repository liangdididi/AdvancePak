// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdvancePakInformalsRaw.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SHyperlink.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SComboBox.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "Forms/SAdvancePakInformalsForm.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakInformalsRaw::Construct(const FArguments& InArgs, SAdvancePakInformalsForm* InInformalsForm)
{
	Informal = InArgs._Informal;
	AdvancePakCreator = InArgs._AdvancePakCreator;
	InformalsForm = InInformalsForm;
	SMultiColumnTableRow<TSharedPtr<FAdvancePakInformalInfo>>::Construct(FSuperRowType::FArguments(), InArgs._OwnerTableView.ToSharedRef());
}

TSharedRef<SWidget> SAdvancePakInformalsRaw::GenerateWidgetForColumn(const FName& ColumnName)
{
	if (ColumnName.IsEqual(AdvancePakCreator->ResourceNameName))
	{
		return SNew(SCheckBox)
			.IsChecked(this, &SAdvancePakInformalsRaw::DealwithCheckBoxIsChecked)
			.OnCheckStateChanged(this, &SAdvancePakInformalsRaw::DealwithCheckBoxCheckStateChanged)
			[
				SNew(STextBlock)
				.Text(FText::FromName(Informal->InformalName))
			.ToolTipText(FText::FromName(Informal->InformalName))
			];
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->BrowseName))
	{
		return SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.FillWidth(1.f)
			[
				PropertyCustomizationHelpers::MakeBrowseButton(
					FSimpleDelegate::CreateRaw(this, &SAdvancePakInformalsRaw::DealwithInformalBrowse)
				)
			];
	}
	return SNullWidget::NullWidget;
}

void SAdvancePakInformalsRaw::DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState)
{
	if (NewState == ECheckBoxState::Checked)
	{
		InformalsForm->AddInformal(Informal->InformalName);
	}
	else
	{
		InformalsForm->RemoveInformal(Informal->InformalName);
	}
}

ECheckBoxState SAdvancePakInformalsRaw::DealwithCheckBoxIsChecked() const
{
	if (InformalsForm->GetSelectedInformals().Contains(Informal->InformalName))
		return ECheckBoxState::Checked;

	return ECheckBoxState::Unchecked;
}

void SAdvancePakInformalsRaw::DealwithInformalBrowse()
{
	FPlatformProcess::ExploreFolder(*Informal->InformalPath);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
