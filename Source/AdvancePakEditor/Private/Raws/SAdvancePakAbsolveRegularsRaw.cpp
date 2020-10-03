// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdvancePakAbsolveRegularsRaw.h"
#include "SlateOptMacros.h"
#include "Forms/SAdvancePakAbsolveRegularsForm.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SHyperlink.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SComboBox.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakAbsolveRegularsRaw::Construct(const FArguments& InArgs, SAdvancePakAbsolveRegularsForm* InAbsolveRegularsForm)
{
	PackageName = InArgs._PackageName;
	AdvancePakCreator = InArgs._AdvancePakCreator;
	AbsolveRegularsForm = InAbsolveRegularsForm;
	SMultiColumnTableRow<TSharedPtr<FName>>::Construct(FSuperRowType::FArguments(), InArgs._OwnerTableView.ToSharedRef());
}

TSharedRef<SWidget> SAdvancePakAbsolveRegularsRaw::GenerateWidgetForColumn(const FName& ColumnName)
{
	if (ColumnName.IsEqual(AdvancePakCreator->PackageNameName))
	{
		return SNew(SCheckBox)
			.IsChecked(this, &SAdvancePakAbsolveRegularsRaw::DealwithCheckBoxIsChecked)
			.OnCheckStateChanged(this, &SAdvancePakAbsolveRegularsRaw::DealwithCheckBoxCheckStateChanged)
			[
				SNew(STextBlock)
				.Text(FText::FromName(*PackageName))
			.ToolTipText(FText::FromName(*PackageName))
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
					FSimpleDelegate::CreateRaw(this, &SAdvancePakAbsolveRegularsRaw::DealwithResourceBrowse)
				)
			];
	}
	return SNullWidget::NullWidget;
}

void SAdvancePakAbsolveRegularsRaw::DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState)
{
	if (NewState == ECheckBoxState::Checked)
	{
		AbsolveRegularsForm->AddResource(*PackageName);
	}
	else
	{
		AbsolveRegularsForm->RemoveResource(*PackageName);
	}
}

ECheckBoxState SAdvancePakAbsolveRegularsRaw::DealwithCheckBoxIsChecked() const
{
	if (AbsolveRegularsForm->GetSelectedResources().Contains(*PackageName))
		return ECheckBoxState::Checked;

	return ECheckBoxState::Unchecked;
}

void SAdvancePakAbsolveRegularsRaw::DealwithResourceBrowse()
{
	TArray<FAssetData> ResourceDatas;
	UAdvancePakEditorLibrary::GetResourceDatasByPackageName(*PackageName, ResourceDatas);
	if (ResourceDatas.Num() > 0)
		GEditor->SyncBrowserToObjects(ResourceDatas);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
