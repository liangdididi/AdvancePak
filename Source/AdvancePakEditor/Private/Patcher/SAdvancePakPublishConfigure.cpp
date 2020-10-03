// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakPublishConfigure.h"
#include "SlateOptMacros.h"
#include "Utility/AdvancePakCreator.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Framework/Application/SlateApplication.h"

#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SBox.h"
#include "Forms/SAdvancePakAccordsForm.h"
#include "Forms/SAdvancePakPublishResourcesForm.h"

#include "Patcher/SAdvancePakPatchBar.h"

#include "Utility/AdvancePakProcThread.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "EditorStyleSet.h"
#include "Framework/Docking/TabManager.h"
#include "Misc/SecureHash.h"

#include "AdvancePakType.h"


#define LOCTEXT_NAMESPACE "SAdvancePakPublishConfigure"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakPublishConfigure::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;
	ScrollWidth = 1815.f;

	SAssignNew(AdvancePakCreator->FormerRegularsForm, SAdvancePakPublishResourcesForm, InModel)
		.ListItemsSource(&AdvancePakCreator->FormerRegulars)
		.Title(OPTEXT("FormerPublishRegulars"))
		.IsPublish(true)
		.IsFormer(true)
		.IsRegular(true);
	SAssignNew(AdvancePakCreator->FormerInformalsForm, SAdvancePakPublishResourcesForm, InModel)
		.ListItemsSource(&AdvancePakCreator->FormerInformals)
		.Title(OPTEXT("FormerPublishInformals"))
		.IsPublish(true)
		.IsFormer(true)
		.IsRegular(false);
	SAssignNew(AdvancePakCreator->FutureRegularsForm, SAdvancePakPublishResourcesForm, InModel)
		.ListItemsSource(&AdvancePakCreator->PublishFutureRegulars)
		.Title(OPTEXT("FuturePublishRegulars"))
		.IsPublish(true)
		.IsFormer(false)
		.IsRegular(true);
	SAssignNew(AdvancePakCreator->FutureInformalsForm, SAdvancePakPublishResourcesForm, InModel)
		.ListItemsSource(&AdvancePakCreator->PublishFutureInformals)
		.Title(OPTEXT("FuturePublishInformals"))
		.IsPublish(true)
		.IsFormer(false)
		.IsRegular(false);
	SAssignNew(AdvancePakCreator->PublishAccordsForm, SAdvancePakAccordsForm, InModel)
		.ListItemsSource(&AdvancePakCreator->PublishAccords)
		.Title(OPTEXT("AccordForm"))
		.IsPublish(true);

	ChildSlot
		[

			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("FaceOne"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPublishConfigure::DealwithInterfaceChanged, 0)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("FaceTwo"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPublishConfigure::DealwithInterfaceChanged, 1)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("FaceAll"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPublishConfigure::DealwithInterfaceChanged, 2)

		]
		]

	+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[

			SNew(SScrollBox)
			.Orientation(EOrientation::Orient_Horizontal)

		+ SScrollBox::Slot()
		.VAlign(VAlign_Fill)
		[
			SNew(SBox)
			.WidthOverride_Raw(this, &SAdvancePakPublishConfigure::DealwithScrollWidth)
		[
			SAssignNew(ConfigureGrid, SGridPanel)
		]
		]
		]
		];

	DealwithInterfaceChanged(0);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


FOptionalSize SAdvancePakPublishConfigure::DealwithScrollWidth() const
{
	return ScrollWidth;
}

FReply SAdvancePakPublishConfigure::DealwithInterfaceChanged(int32 FaceIndex)
{
	switch (FaceIndex)
	{
	case 0:
		ScrollWidth = 1815.f;
		ConfigureGrid->ClearFill();
		ConfigureGrid->ClearChildren();
		ConfigureGrid->SetColumnFill(0, 1.f);
		ConfigureGrid->SetColumnFill(1, 1.f);
		ConfigureGrid->SetRowFill(0, 1.f);
		ConfigureGrid->SetRowFill(1, 1.f);

		ConfigureGrid->AddSlot(0, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FormerRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(0, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FormerInformalsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(1, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FutureRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(1, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FutureInformalsForm.ToSharedRef()];
		break;
	case 1:
		ScrollWidth = 1815.f;
		ConfigureGrid->ClearFill();
		ConfigureGrid->ClearChildren();
		ConfigureGrid->SetColumnFill(0, 1.f);
		ConfigureGrid->SetColumnFill(1, 1.f);
		ConfigureGrid->SetRowFill(0, 1.f);
		ConfigureGrid->SetRowFill(1, 1.f);
		ConfigureGrid->AddSlot(0, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FutureRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(0, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FutureInformalsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(1, 0).Padding(0.f, 0.f, 4.f, 0.f).RowSpan(2)[AdvancePakCreator->PublishAccordsForm.ToSharedRef()];
		break;
	case 2:
		ScrollWidth = 2840.f;
		ConfigureGrid->ClearFill();
		ConfigureGrid->ClearChildren();
		ConfigureGrid->SetColumnFill(0, 1.f);
		ConfigureGrid->SetColumnFill(1, 1.f);
		ConfigureGrid->SetColumnFill(2, 1.f);
		ConfigureGrid->SetRowFill(0, 1.f);
		ConfigureGrid->SetRowFill(1, 1.f);
		ConfigureGrid->AddSlot(0, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FormerRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(0, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FormerInformalsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(1, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FutureRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(1, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FutureInformalsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(2, 0).Padding(0.f, 0.f, 4.f, 0.f).RowSpan(2)[AdvancePakCreator->PublishAccordsForm.ToSharedRef()];
		break;
	}
	return FReply::Handled();
}



#undef LOCTEXT_NAMESPACE