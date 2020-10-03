// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdvancePakAccordsRaw.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SHyperlink.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Images/SImage.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "Forms/SAdvancePakAccordsForm.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakAccordsRaw::Construct(const FArguments& InArgs, SAdvancePakAccordsForm* InAccordsForm)
{
	AccordInfo = InArgs._AccordInfo;
	AdvancePakCreator = InArgs._AdvancePakCreator;
	AccordsForm = InAccordsForm;
	SMultiColumnTableRow<TSharedPtr<FAdvancePakAccordInfo>>::Construct(FSuperRowType::FArguments(), InArgs._OwnerTableView.ToSharedRef());
}

TSharedRef<SWidget> SAdvancePakAccordsRaw::GenerateWidgetForColumn(const FName& ColumnName)
{
	if (ColumnName.IsEqual(AdvancePakCreator->ResourceNameName))
	{
		return SNew(STextBlock)
			.Text(FText::FromString(!AccordsForm->IsAccordFailed ? AccordInfo->Name.Right(AccordInfo->Name.Len() - 8) : AccordInfo->Name))
			.ToolTipText(FText::FromString(!AccordsForm->IsAccordFailed ? AccordInfo->Name.Right(AccordInfo->Name.Len() - 8) : AccordInfo->Name))
			.ColorAndOpacity(this, &SAdvancePakAccordsRaw::DealwithResourceNameColor)
			;
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->BrowseName))
	{
		return SNew(SHorizontalBox)
			.Visibility_Lambda([this]() -> EVisibility { return AccordsForm->IsAccordFailed || (!AccordsForm->IsAccordFailed && AccordInfo->AccordType != EAdvancePakAccordType::Delete) ? EVisibility::Visible : EVisibility::Hidden; })

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.FillWidth(1.f)
			[
				PropertyCustomizationHelpers::MakeBrowseButton(
					FSimpleDelegate::CreateRaw(this, &SAdvancePakAccordsRaw::DealwithResourceBrowse)
				)
			];
	}
	return SNullWidget::NullWidget;
}

void SAdvancePakAccordsRaw::DealwithResourceBrowse()
{
	if (AccordsForm->IsAccordFailed)
	{
		if (AccordInfo->IsRegular)
		{
			TArray<FAssetData> ResourceDatas;
			UAdvancePakEditorLibrary::GetResourceDatasByPackageName(FName(*AccordInfo->Name), ResourceDatas);
			if (ResourceDatas.Num() > 0)
				GEditor->SyncBrowserToObjects(ResourceDatas);
		}
		else
		{
			FPlatformProcess::ExploreFolder(*AccordInfo->Path);
		}
	}
	else
	{
		FPlatformProcess::ExploreFolder(*AccordInfo->Path);
	}
}

FSlateColor SAdvancePakAccordsRaw::DealwithResourceNameColor() const
{
	switch (AccordInfo->AccordType)
	{
	case EAdvancePakAccordType::Add:
		return FLinearColor::Green;
	case EAdvancePakAccordType::Update:
		return FLinearColor::Yellow;
	case EAdvancePakAccordType::Keep:
		return FLinearColor::White;
	case EAdvancePakAccordType::Delete:
		return FLinearColor::Red;
	}
	return FLinearColor::White;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
