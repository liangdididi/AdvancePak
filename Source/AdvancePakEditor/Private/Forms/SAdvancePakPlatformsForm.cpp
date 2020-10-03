// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakPlatformsForm.h"
#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "AdvancePakLibrary.h"
#include "Interfaces/ITargetPlatformManagerModule.h"
#include "Interfaces/ITargetPlatform.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Input/SHyperlink.h"
#include "Widgets/Layout/SExpandableArea.h"



BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakPlatformsForm::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;

	IsSingleMode = InArgs._IsSingleMode.Get();
	OnPlatformSelected = InArgs._OnPlatformSelected;

	if (IsSingleMode)
	{
#if PLATFORM_WINDOWS
		PlatformList.Add(FString("Android_ASTC"));
		PlatformList.Add(FString("Android_ATC"));
		PlatformList.Add(FString("Android_DXT"));
		PlatformList.Add(FString("Android_ETC1"));
		PlatformList.Add(FString("Android_ETC1a"));
		PlatformList.Add(FString("Android_ETC2"));
		PlatformList.Add(FString("Android_PVRTC"));
		PlatformList.Add(FString("WindowsNoEditor"));
#endif

#if PLATFORM_MAC
		PlatformList.Add(new FString("Android_ASTC"));
		PlatformList.Add(Mnew FString("Android_ATC"));
		PlatformList.Add(new FString("Android_DXT"));
		PlatformList.Add(new FString("Android_ETC1"));
		PlatformList.Add(new FString("Android_ETC1a"));
		PlatformList.Add(new FString("Android_ETC2"));
		PlatformList.Add(new FString("Android_PVRTC"));
		PlatformList.Add(new FString("IOS"));
		PlatformList.Add(new FString("MacNoEditor"));
#endif
	}
	else
	{
		TArray<ITargetPlatform*> AllPlatforms = GetTargetPlatformManager()->GetTargetPlatforms();
		if (AllPlatforms.Num() > 0)
		{
			PlatformList.Reset();
			for (auto Platform : AllPlatforms)
			{
				PlatformList.Add(FString(Platform->PlatformName()));
			}
		}
	}

	if (IsSingleMode)
	{
		ChildSlot
			[
				SNew(SBorder)
				.Padding(4.f)
			[
				SAssignNew(PlatformGridPanel, SGridPanel)
			]
			];
	}
	else
	{
		ChildSlot
			[

				SNew(SBorder)
				.Padding(4.f)
			[

				SNew(SExpandableArea)
				.AreaTitle(OPTEXT("SelectPlatforms"))
			.AreaTitleFont(FCoreStyle::GetDefaultFontStyle("Bold", 10))
			.InitiallyCollapsed(false)
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
				SAssignNew(PlatformGridPanel, SGridPanel)
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
				.OnNavigate(this, &SAdvancePakPlatformsForm::DealwithSelectLinkwork, true)
			.Text(OPTEXT("All"))
			.ToolTipText(OPTEXT("SelectAllPlatforms"))
			]

		+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8.f, 0.f, 8.f, 0.f)
			[
				SNew(SHyperlink)
				.OnNavigate(this, &SAdvancePakPlatformsForm::DealwithSelectLinkwork, false)
			.Text(OPTEXT("None"))
			.ToolTipText(OPTEXT("SelectNonePlatforms"))
			]

			]

			]
			]
			];
	}


	for (int i = 0; i < PlatformList.Num(); ++i)
	{
		PlatformGridPanel->AddSlot(i % 5, i / 5)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SCheckBox)
				.IsChecked(this, &SAdvancePakPlatformsForm::DealwithCheckBoxIsChecked, i)
			.OnCheckStateChanged(this, &SAdvancePakPlatformsForm::DealwithCheckBoxCheckStateChanged, i)
			.Padding(FMargin(6.0, 2.0))
			[
				SNew(STextBlock)
				.Text(FText::FromString(*PlatformList[i]))
			]
			];
	}
}

void SAdvancePakPlatformsForm::AddSelectPlatform(const FString& InPlatform)
{
	if (IsSingleMode)
	{
		SelectPlatforms.Reset();
		SelectPlatforms.Add(InPlatform);
	}
	else
	{
		if (!SelectPlatforms.Contains(InPlatform))
			SelectPlatforms.Add(InPlatform);
	}
	OnPlatformSelected.ExecuteIfBound();
}

void SAdvancePakPlatformsForm::RemoveSelectPlatform(const FString& InPlatform)
{
	if (SelectPlatforms.Contains(InPlatform))
		SelectPlatforms.Remove(InPlatform);
	OnPlatformSelected.ExecuteIfBound();
}

const TArray<FString>& SAdvancePakPlatformsForm::GetAllSelectPlatforms()
{
	return SelectPlatforms;
}

FString SAdvancePakPlatformsForm::GetSelectPlatform()
{
	if (SelectPlatforms.Num() > 0)
		return SelectPlatforms[0];
	return FString();
}

void SAdvancePakPlatformsForm::DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState, int Index)
{
	if (NewState == ECheckBoxState::Checked)
	{
		if (IsSingleMode)
		{
			SelectPlatforms.Reset();
			SelectPlatforms.Add(PlatformList[Index]);
		}
		else
		{
			if (!SelectPlatforms.Contains(PlatformList[Index]))
				SelectPlatforms.Add(PlatformList[Index]);
		}
	}
	else
	{
		if (SelectPlatforms.Contains(PlatformList[Index]))
			SelectPlatforms.Remove(PlatformList[Index]);
	}
	OnPlatformSelected.ExecuteIfBound();
}

ECheckBoxState SAdvancePakPlatformsForm::DealwithCheckBoxIsChecked(int Index) const
{
	if (SelectPlatforms.Contains(PlatformList[Index]))
		return ECheckBoxState::Checked;
	return ECheckBoxState::Unchecked;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SAdvancePakPlatformsForm::DealwithSelectLinkwork(bool IsAll)
{
	SelectPlatforms.Reset();
	if (IsAll)
	{
		SelectPlatforms.Append(PlatformList);
	}
}


