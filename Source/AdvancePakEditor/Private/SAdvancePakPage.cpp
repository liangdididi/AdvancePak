// Copyright Epic Games, Inc. All Rights Reserved.

#include "SAdvancePakPage.h"

#include "EditorStyleSet.h"
#include "Styling/CoreStyle.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/MultiBox/MultiBoxDefs.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Package/SAdvancePakPackageWork.h"
#include "Widgets/Layout/SBox.h"
#include "Package/SAdvancePakEditorLog.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Utility/AdvancePakCreator.h"
#include "Package/SAdvancePakCookPage.h"
#include "Package/SAdvancePakBuildPage.h"

class FAdvancePakEditorLogHistory : public FOutputDevice
{
public:

	FAdvancePakEditorLogHistory()
	{
		GLog->AddOutputDevice(this);
		GLog->SerializeBacklog(this);
	}

	~FAdvancePakEditorLogHistory()
	{
		if (GLog != NULL)
		{
			GLog->RemoveOutputDevice(this);
		}
	}

	const TArray< TSharedPtr<FAdvancePakEditorLogMessage> >& GetMessages() const
	{
		return Messages;
	}

protected:

	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const class FName& Category) override
	{
		SAdvancePakEditorLog::CreateLogMessages(V, Verbosity, Category, Messages);
	}

private:

	TArray< TSharedPtr<FAdvancePakEditorLogMessage> > Messages;
};

static TSharedPtr<FAdvancePakEditorLogHistory> AdvancePakEditorLogHistory;


#define LOCTEXT_NAMESPACE "SAdvancePakPage"

SAdvancePakPage::SAdvancePakPage()
{ }


SAdvancePakPage::~SAdvancePakPage()
{
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakPage::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	Model = InModel;

	OnCloseClicked = InArgs._OnCloseClicked;

	AdvancePakEditorLogHistory = MakeShareable(new FAdvancePakEditorLogHistory);

	ChildSlot
		[

			SNew(SOverlay)

			+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.Padding(20.f)
		[

			SNew(SScrollBox)

			+ SScrollBox::Slot()
		.Padding(0.0f, 0.0f, 0.0f, 0.0f)
		[
			SNew(SGridPanel)
			.FillColumn(0, 1.0f)

		+ SGridPanel::Slot(0, 0)
		.Padding(0.0f, 16.0f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SSeparator)
			.Orientation(Orient_Horizontal)
		]


	+ SGridPanel::Slot(0, 1)
		.Padding(8.0f, 0.0f, 8.0f, 0.0f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			SNew(SAdvancePakCookPage, InModel)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(20.f, 8.f, 20.f, 8.f)
		[
			SNew(SSeparator)
			.Orientation(Orient_Vertical)
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			SNew(SAdvancePakBuildPage, InModel)
		]

		]

	+ SGridPanel::Slot(0, 2)
		.Padding(0.0f, 16.0f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SSeparator)
			.Orientation(Orient_Horizontal)
		]


	+ SGridPanel::Slot(0, 3)
		.Padding(8.0f, 0.0f, 8.0f, 0.0f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SAdvancePakPackageWork, InModel)
		]

	+ SGridPanel::Slot(0, 4)
		.Padding(0.0f, 16.0f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SSeparator)
			.Orientation(Orient_Horizontal)
		]

	+ SGridPanel::Slot(0, 5)
		.Padding(8.0f, 0.0f, 8.0f, 0.0f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBox)
			.HeightOverride(600.f)
		[
			SNew(SAdvancePakEditorLog)
			.Messages(AdvancePakEditorLogHistory->GetMessages())
		]
		]
		]
		]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SAdvancePakPage::EnterEditMode()
{
	if (NameEditBox.IsValid())
	{
		NameEditBox->EnterEditingMode();
	}
}

#undef LOCTEXT_NAMESPACE
