// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakMapsForm.h"
#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "AdvancePakLibrary.h"
#include "Interfaces/ITargetPlatformManagerModule.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SBox.h"
#include "Interfaces/ITargetPlatform.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Input/SHyperlink.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "EditorStyleSet.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakMapsForm::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;

	ShowMapsChoice = EShowMapsChoices::ShowAllMaps;

	ChildSlot
		[
			SNew(SBorder)
			.Padding(4.f)
		[

			SNew(SExpandableArea)
			.AreaTitle(OPTEXT("SelectMaps"))
		.AreaTitleFont(FCoreStyle::GetDefaultFontStyle("Bold", 10))
		.InitiallyCollapsed(false)
		.Padding(8.0f)
		.BodyContent()
		[

			SNew(SVerticalBox)
			.IsEnabled_Lambda([this]() { return !AdvancePakCreator->IsCooking; })


		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 4.0f)
		.VAlign(VAlign_Center)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SCheckBox)
			.IsChecked(this, &SAdvancePakMapsForm::DealwithShowCheckBoxIsChecked, EShowMapsChoices::ShowAllMaps)
		.OnCheckStateChanged(this, &SAdvancePakMapsForm::DealwithShowCheckBoxCheckStateChanged, EShowMapsChoices::ShowAllMaps)
		.Style(FEditorStyle::Get(), "RadioButton")
		[
			SNew(STextBlock)
			.Text(OPTEXT("ShowAll"))
		]
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(8.0f, 0.0f, 0.0f, 0.0f)
		[
			SNew(SCheckBox)
			.IsChecked(this, &SAdvancePakMapsForm::DealwithShowCheckBoxIsChecked, EShowMapsChoices::ShowCookedMaps)
		.OnCheckStateChanged(this, &SAdvancePakMapsForm::DealwithShowCheckBoxCheckStateChanged, EShowMapsChoices::ShowCookedMaps)
		.Style(FEditorStyle::Get(), "RadioButton")
		[
			SNew(STextBlock)
			.Text(OPTEXT("ShowCooked"))
		]
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.MaxHeight(200.f)
		[
			SNew(SScrollBox)
			.Orientation(EOrientation::Orient_Vertical)

		+ SScrollBox::Slot()
		.VAlign(VAlign_Fill)
		[
			SAssignNew(MapGridPanel, SGridPanel)
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
			.OnNavigate(this, &SAdvancePakMapsForm::DealwithSelectLinkwork, true)
		.Text(OPTEXT("All"))
		.ToolTipText(OPTEXT("SelectAllMaps"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 8.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakMapsForm::DealwithSelectLinkwork, false)
		.Text(OPTEXT("None"))
		.ToolTipText(OPTEXT("SelectNoneMaps"))
		]

		]

		]
		]
		];

	RenovateMapList();

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

const TArray<FString>& SAdvancePakMapsForm::GetAllSelectMaps()
{
	return SelectMaps;
}

void SAdvancePakMapsForm::DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState, int Index)
{
	if (NewState == ECheckBoxState::Checked)
	{
		if (!SelectMaps.Contains(MapList[Index]))
			SelectMaps.Add(MapList[Index]);
	}
	else
	{
		if (SelectMaps.Contains(MapList[Index]))
			SelectMaps.Remove(MapList[Index]);
	}
}

ECheckBoxState SAdvancePakMapsForm::DealwithCheckBoxIsChecked(int Index) const
{
	if (SelectMaps.Contains(MapList[Index]))
		return ECheckBoxState::Checked;
	return ECheckBoxState::Unchecked;
}

void SAdvancePakMapsForm::DealwithSelectLinkwork(bool IsAll)
{
	SelectMaps.Reset();
	if (IsAll)
	{
		SelectMaps.Append(MapList);
	}
}

void SAdvancePakMapsForm::DealwithShowCheckBoxCheckStateChanged(ECheckBoxState NewState, EShowMapsChoices::Type Choice)
{
	if (NewState == ECheckBoxState::Checked)
	{
		ShowMapsChoice = Choice;
		RenovateMapList();
	}
}

ECheckBoxState SAdvancePakMapsForm::DealwithShowCheckBoxIsChecked(EShowMapsChoices::Type Choice) const
{
	if (ShowMapsChoice == Choice)
	{
		return ECheckBoxState::Checked;
	}

	return ECheckBoxState::Unchecked;
}

void SAdvancePakMapsForm::RenovateMapList()
{
	SelectMaps.Reset();
	MapList.Reset();

	ILauncherProfilePtr SelectedProfile = AdvancePakCreator->Profile;
	TArray<FString> AvailableMaps = FGameProjectHelper::GetAvailableMaps(SelectedProfile->GetProjectBasePath(), SelectedProfile->SupportsEngineMaps(), true);

	for (int32 AvailableMapIndex = 0; AvailableMapIndex < AvailableMaps.Num(); ++AvailableMapIndex)
	{
		FString& Map = AvailableMaps[AvailableMapIndex];

		if ((ShowMapsChoice == EShowMapsChoices::ShowAllMaps) || SelectedProfile->GetCookedMaps().Contains(Map))
		{
			MapList.Add(Map);
		}
	}

	MapGridPanel->ClearChildren();

	for (int i = 0; i < MapList.Num(); ++i)
	{
		MapGridPanel->AddSlot(i % 6, i / 6)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SCheckBox)
				.IsChecked(this, &SAdvancePakMapsForm::DealwithCheckBoxIsChecked, i)
			.OnCheckStateChanged(this, &SAdvancePakMapsForm::DealwithCheckBoxCheckStateChanged, i)
			.Padding(FMargin(6.0, 2.0))
			[
				SNew(STextBlock)
				.Text(FText::FromString(*MapList[i]))
			]
			];
	}
}
