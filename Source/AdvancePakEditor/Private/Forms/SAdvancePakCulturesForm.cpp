// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakCulturesForm.h"
#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "AdvancePakLibrary.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Interfaces/ITargetPlatformManagerModule.h"
#include "Interfaces/ITargetPlatform.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SHyperlink.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SEditableTextBox.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakCulturesForm::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{

	AdvancePakCreator = InModel;

	FInternationalization::Get().GetCultureNames(CulturesList);

	ChildSlot
		[
			SNew(SBorder)
			.Padding(4.f)
		[

			SNew(SExpandableArea)
			.AreaTitle(OPTEXT("SelectCultures"))
		.AreaTitleFont(FCoreStyle::GetDefaultFontStyle("Bold", 8))
		.InitiallyCollapsed(true)
		.Padding(8.0f)
		.BodyContent()
		[

			SNew(SVerticalBox)
			.IsEnabled_Lambda([this]() { return !AdvancePakCreator->IsCooking; })


	+ SVerticalBox::Slot()
		.AutoHeight()
		.MaxHeight(200.f)
		[
			SNew(SScrollBox)
			.Orientation(EOrientation::Orient_Vertical)

		+ SScrollBox::Slot()
		.VAlign(VAlign_Fill)
		[
			SAssignNew(CultureGridPanel, SGridPanel)
		]
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

			+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			SNullWidget::NullWidget
		]

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
			.OnNavigate(this, &SAdvancePakCulturesForm::DealwithSelectLinkwork, true)
		.Text(OPTEXT("All"))
		.ToolTipText(OPTEXT("SelectAllResources"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 8.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakCulturesForm::DealwithSelectLinkwork, false)
		.Text(OPTEXT("None"))
		.ToolTipText(OPTEXT("SelectNoneResources"))
		]

		]

		]
		]
		];

	for (int i = 0; i < CulturesList.Num(); ++i)
	{
		CultureGridPanel->AddSlot(i % 9, i / 9)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SCheckBox)
				.IsChecked(this, &SAdvancePakCulturesForm::DealwithCheckBoxIsChecked, i)
			.OnCheckStateChanged(this, &SAdvancePakCulturesForm::DealwithCheckBoxCheckStateChanged, i)
			.Padding(FMargin(6.0, 2.0))
			[
				SNew(STextBlock)
				.Text(FText::FromString(*CulturesList[i]))
			]
			];
	}

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

const TArray<FString>& SAdvancePakCulturesForm::GetAllSelectCultures()
{
	return SelectCultures;
}

void SAdvancePakCulturesForm::DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState, int Index)
{
	if (NewState == ECheckBoxState::Checked)
	{
		if (!SelectCultures.Contains(CulturesList[Index]))
			SelectCultures.Add(CulturesList[Index]);
	}
	else
	{
		if (SelectCultures.Contains(CulturesList[Index]))
			SelectCultures.Remove(CulturesList[Index]);
	}
}

ECheckBoxState SAdvancePakCulturesForm::DealwithCheckBoxIsChecked(int Index) const
{
	if (SelectCultures.Contains(CulturesList[Index]))
		return ECheckBoxState::Checked;
	return ECheckBoxState::Unchecked;
}

void SAdvancePakCulturesForm::DealwithSelectLinkwork(bool IsAll)
{
	SelectCultures.Reset();
	if (IsAll)
	{
		SelectCultures.Append(CulturesList);
	}
}
