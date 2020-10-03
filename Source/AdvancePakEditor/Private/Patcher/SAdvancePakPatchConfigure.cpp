// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakPatchConfigure.h"
#include "SlateOptMacros.h"

#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SBox.h"

#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Framework/Application/SlateApplication.h"

#include "Utility/AdvancePakCreator.h"
#include "Forms/SAdvancePakPublishResourcesForm.h"
#include "Forms/SAdvancePakPatchResourcesForm.h"
#include "Forms/SAdvancePakGathersForm.h"
#include "Forms/SAdvancePakInvolveRegularsForm.h"
#include "Forms/SAdvancePakAbsolveRegularsForm.h"
#include "Forms/SAdvancePakInformalsForm.h"
#include "Forms/SAdvancePakRelyonsForm.h"
#include "Forms/SAdvancePakAccordsForm.h"
#include "Utility/AdvancePakProcThread.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "EditorStyleSet.h"
#include "Framework/Docking/TabManager.h"
#include "AdvancePakType.h"

#define LOCTEXT_NAMESPACE "SAdvancePakPatchConfigure"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakPatchConfigure::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;
	ScrollWidth = 1815.f;

	SAssignNew(AdvancePakCreator->FormerPublishRegularsForm, SAdvancePakPublishResourcesForm, InModel)
		.ListItemsSource(&AdvancePakCreator->FormerPublishRegulars)
		.Title(OPTEXT("FormerPublishRegulars"))
		.IsPublish(false)
		.IsFormer(true)
		.IsRegular(true);
	SAssignNew(AdvancePakCreator->FormerPatchRegularsForm, SAdvancePakPatchResourcesForm, InModel)
		.ListItemsSource(&AdvancePakCreator->FormerPatchRegulars)
		.Title(OPTEXT("FormerPatchRegulars"))
		.IsFormer(true)
		.IsRegular(true);
	SAssignNew(AdvancePakCreator->FuturePatchRegularsForm, SAdvancePakPatchResourcesForm, InModel)
		.ListItemsSource(&AdvancePakCreator->FuturePatchRegulars)
		.Title(OPTEXT("FuturePatchRegulars"))
		.IsFormer(false)
		.IsRegular(true);
	SAssignNew(AdvancePakCreator->FormerPublishInformalsForm, SAdvancePakPublishResourcesForm, InModel)
		.ListItemsSource(&AdvancePakCreator->FormerPublishInformals)
		.Title(OPTEXT("FormerPublishInformals"))
		.IsPublish(false)
		.IsFormer(true)
		.IsRegular(false);
	SAssignNew(AdvancePakCreator->FormerPatchInformalsForm, SAdvancePakPatchResourcesForm, InModel)
		.ListItemsSource(&AdvancePakCreator->FormerPatchInformals)
		.Title(OPTEXT("FormerPatchInformals"))
		.IsFormer(true)
		.IsRegular(false);
	SAssignNew(AdvancePakCreator->FuturePatchInformalsForm, SAdvancePakPatchResourcesForm, InModel)
		.ListItemsSource(&AdvancePakCreator->FuturePatchInformals)
		.Title(OPTEXT("FuturePatchInformals"))
		.IsFormer(false)
		.IsRegular(false);
	SAssignNew(AdvancePakCreator->InvolveGathersForm, SAdvancePakGathersForm, InModel)
		.ListItemsSource(&AdvancePakCreator->InvolveGathers)
		.IsInvolve(true)
		.Title(OPTEXT("InvolveGathers"));
	SAssignNew(AdvancePakCreator->InvolveRegularsForm, SAdvancePakInvolveRegularsForm, InModel)
		.ListItemsSource(&AdvancePakCreator->InvolveRegulars)
		.Title(OPTEXT("InvolveRegulars"));
	SAssignNew(AdvancePakCreator->InvolveInformalsForm, SAdvancePakInformalsForm, InModel)
		.ListItemsSource(&AdvancePakCreator->InvolveInformals)
		.IsInvolve(true)
		.Title(OPTEXT("InvolveInformals"));
	SAssignNew(AdvancePakCreator->AbsolveGathersForm, SAdvancePakGathersForm, InModel)
		.ListItemsSource(&AdvancePakCreator->AbsolveGathers)
		.IsInvolve(false)
		.Title(OPTEXT("AbsolveGathers"));
	SAssignNew(AdvancePakCreator->AbsolveRegularsForm, SAdvancePakAbsolveRegularsForm, InModel)
		.ListItemsSource(&AdvancePakCreator->AbsolveRegulars)
		.Title(OPTEXT("AbsolveRegulars"));
	SAssignNew(AdvancePakCreator->AbsolveInformalsForm, SAdvancePakInformalsForm, InModel)
		.ListItemsSource(&AdvancePakCreator->AbsolveInformals)
		.IsInvolve(false)
		.Title(OPTEXT("AbsolveInformals"));
	SAssignNew(AdvancePakCreator->RelyonsForm, SAdvancePakRelyonsForm, InModel)
		.TreeItemsSource(&AdvancePakCreator->Relyons)
		.Title(OPTEXT("RelyonForm"));
	SAssignNew(AdvancePakCreator->PatchAccordsForm, SAdvancePakAccordsForm, InModel)
		.ListItemsSource(&AdvancePakCreator->PatchAccords)
		.Title(OPTEXT("AccordForm"))
		.IsPublish(false);


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
		.OnClicked(this, &SAdvancePakPatchConfigure::DealwithInterfaceChanged, 0)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("FaceTwo"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchConfigure::DealwithInterfaceChanged, 1)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("FaceThree"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchConfigure::DealwithInterfaceChanged, 2)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("FaceFour"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchConfigure::DealwithInterfaceChanged, 3)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("FaceFive"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchConfigure::DealwithInterfaceChanged, 4)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("FaceAllTwo"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchConfigure::DealwithInterfaceChanged, 5)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("FaceAllThree"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchConfigure::DealwithInterfaceChanged, 6)
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
			.WidthOverride_Raw(this, &SAdvancePakPatchConfigure::DealwithScrollWidth)
		[
			SAssignNew(ConfigureGrid, SGridPanel)
		]
		]
		]
		];

	DealwithInterfaceChanged(0);

}

FOptionalSize SAdvancePakPatchConfigure::DealwithScrollWidth() const
{
	return ScrollWidth;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


FReply SAdvancePakPatchConfigure::DealwithInterfaceChanged(int32 FaceIndex)
{
	switch (FaceIndex)
	{
	case 0:
		ScrollWidth = 1815.f;
		ConfigureGrid->ClearFill();
		ConfigureGrid->ClearChildren();
		ConfigureGrid->SetColumnFill(0, 1.f);
		ConfigureGrid->SetColumnFill(1, 1.f);
		ConfigureGrid->SetColumnFill(2, 1.f);
		ConfigureGrid->SetRowFill(0, 1.f);
		ConfigureGrid->SetRowFill(1, 1.f);

		ConfigureGrid->AddSlot(0, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FormerPublishRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(0, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FormerPublishInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(1, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FormerPatchRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(1, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FormerPatchInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(2, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FuturePatchRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(2, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FuturePatchInformalsForm.ToSharedRef()];

		break;
	case 1:
		ScrollWidth = 1815.f;
		ConfigureGrid->ClearFill();
		ConfigureGrid->ClearChildren();
		ConfigureGrid->SetColumnFill(0, 1.f);
		ConfigureGrid->SetColumnFill(1, 1.f);
		ConfigureGrid->SetColumnFill(2, 1.f);
		ConfigureGrid->SetRowFill(0, 1.f);
		ConfigureGrid->SetRowFill(1, 1.f);

		ConfigureGrid->AddSlot(0, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FormerPatchRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(0, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FormerPatchInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(1, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FuturePatchRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(1, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FuturePatchInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(2, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveGathersForm.ToSharedRef()];
		ConfigureGrid->AddSlot(2, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveGathersForm.ToSharedRef()];

		break;
	case 2:
		ScrollWidth = 1815.f;
		ConfigureGrid->ClearFill();
		ConfigureGrid->ClearChildren();
		ConfigureGrid->SetColumnFill(0, 1.f);
		ConfigureGrid->SetColumnFill(1, 1.f);
		ConfigureGrid->SetColumnFill(2, 1.f);
		ConfigureGrid->SetRowFill(0, 1.f);
		ConfigureGrid->SetRowFill(1, 1.f);

		ConfigureGrid->AddSlot(0, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FuturePatchRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(0, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FuturePatchInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(1, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveGathersForm.ToSharedRef()];
		ConfigureGrid->AddSlot(1, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveGathersForm.ToSharedRef()];

		ConfigureGrid->AddSlot(2, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(2, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveRegularsForm.ToSharedRef()];

		break;
	case 3:
		ScrollWidth = 1815.f;
		ConfigureGrid->ClearFill();
		ConfigureGrid->ClearChildren();
		ConfigureGrid->SetColumnFill(0, 1.f);
		ConfigureGrid->SetColumnFill(1, 1.f);
		ConfigureGrid->SetColumnFill(2, 1.f);
		ConfigureGrid->SetRowFill(0, 1.f);
		ConfigureGrid->SetRowFill(1, 1.f);

		ConfigureGrid->AddSlot(0, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveGathersForm.ToSharedRef()];
		ConfigureGrid->AddSlot(0, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveGathersForm.ToSharedRef()];

		ConfigureGrid->AddSlot(1, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(1, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveRegularsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(2, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveInformalsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(2, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveInformalsForm.ToSharedRef()];

		break;
	case 4:
		ScrollWidth = 1815.f;
		ConfigureGrid->ClearFill();
		ConfigureGrid->ClearChildren();
		ConfigureGrid->SetColumnFill(0, 1.f);
		ConfigureGrid->SetColumnFill(1, 1.f);
		ConfigureGrid->SetColumnFill(2, 1.f);
		ConfigureGrid->SetRowFill(0, 1.f);
		ConfigureGrid->SetRowFill(1, 1.f);

		ConfigureGrid->AddSlot(0, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(0, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveRegularsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(1, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveInformalsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(1, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(2, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->PatchAccordsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(2, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->RelyonsForm.ToSharedRef()];

		break;
	case 5:
		ScrollWidth = 3730.f;
		ConfigureGrid->ClearFill();
		ConfigureGrid->ClearChildren();
		ConfigureGrid->SetColumnFill(0, 1.f);
		ConfigureGrid->SetColumnFill(1, 1.f);
		ConfigureGrid->SetColumnFill(2, 1.f);
		ConfigureGrid->SetColumnFill(3, 1.f);
		ConfigureGrid->SetColumnFill(4, 1.f);
		ConfigureGrid->SetColumnFill(5, 1.f);
		ConfigureGrid->SetColumnFill(6, 1.f);
		ConfigureGrid->SetRowFill(0, 1.f);
		ConfigureGrid->SetRowFill(1, 1.f);

		ConfigureGrid->AddSlot(0, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FormerPublishRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(0, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FormerPublishInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(1, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FormerPatchRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(1, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FormerPatchInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(2, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FuturePatchRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(2, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FuturePatchInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(3, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveGathersForm.ToSharedRef()];
		ConfigureGrid->AddSlot(3, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveGathersForm.ToSharedRef()];

		ConfigureGrid->AddSlot(4, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(4, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveRegularsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(5, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveInformalsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(5, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(6, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->PatchAccordsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(6, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->RelyonsForm.ToSharedRef()];

		break;
	case 6:
		ScrollWidth = 5600.f;
		ConfigureGrid->ClearFill();
		ConfigureGrid->ClearChildren();
		ConfigureGrid->SetColumnFill(0, 1.f);
		ConfigureGrid->SetColumnFill(1, 1.f);
		ConfigureGrid->SetColumnFill(2, 1.f);
		ConfigureGrid->SetColumnFill(3, 1.f);
		ConfigureGrid->SetColumnFill(4, 1.f);
		ConfigureGrid->SetColumnFill(5, 1.f);
		ConfigureGrid->SetColumnFill(6, 1.f);
		ConfigureGrid->SetRowFill(0, 1.f);
		ConfigureGrid->SetRowFill(1, 1.f);

		ConfigureGrid->AddSlot(0, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FormerPublishRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(0, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FormerPublishInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(1, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FormerPatchRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(1, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FormerPatchInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(2, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->FuturePatchRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(2, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->FuturePatchInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(3, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveGathersForm.ToSharedRef()];
		ConfigureGrid->AddSlot(3, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveGathersForm.ToSharedRef()];

		ConfigureGrid->AddSlot(4, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveRegularsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(4, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveRegularsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(5, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->InvolveInformalsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(5, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->AbsolveInformalsForm.ToSharedRef()];

		ConfigureGrid->AddSlot(6, 0).Padding(0.f, 0.f, 4.f, 0.f)[AdvancePakCreator->PatchAccordsForm.ToSharedRef()];
		ConfigureGrid->AddSlot(6, 1).Padding(0.f, 4.f, 4.f, 0.f)[AdvancePakCreator->RelyonsForm.ToSharedRef()];

		break;
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE