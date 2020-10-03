// Copyright Epic Games, Inc. All Rights Reserved.

#include "SAdvancePakCookPage.h"
#include "DesktopPlatformModule.h"
#include "EditorStyleSet.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Forms/SAdvancePakCulturesForm.h"
#include "Forms/SAdvancePakMapsForm.h"
#include "Editor.h"
#include "EditorAnalytics.h"
#include "AdvancePakEditorStyle.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Views/SListView.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "Utility/AdvancePakEditorType.h"
#include "AdvancePakLibrary.h"
#include "Utility/AdvancePakProcThread.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "SlateOptMacros.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Utility/AdvancePakEditorLibrary.h"
#include "Forms/SAdvancePakPlatformsForm.h"
#include "Textures/SlateIcon.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SHyperlink.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "IDetailsView.h"


#define LOCTEXT_NAMESPACE "SAdvancePakCookPage"

SAdvancePakCookPage::~SAdvancePakCookPage()
{
}

void SAdvancePakCookPage::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;

	CreateAdvancedConfigureDetailView();

	ChildSlot
		[

			SNew(SBorder)
			.Padding(4.f)
		[

			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0, 4.0, 0.0, 0.0)
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.HAlign(HAlign_Center)
.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(OPTEXT("CookPage"))
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		.ColorAndOpacity(FLinearColor(1.f, 1.f, 0.f))
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0, 4.0, 0.0, 0.0)
		[
			SAssignNew(PlatformsView, SAdvancePakPlatformsForm, InModel)
			.IsSingleMode(false)
		]

#if 0

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0, 8.0, 0.0, 0.0)
		[
			SAssignNew(CulturesView, SAdvancePakCulturesForm, InModel)
		]

#endif

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0, 8.0, 0.0, 0.0)
		[
			SAssignNew(MapsView, SAdvancePakMapsForm, InModel)
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0, 8.0, 0.0, 0.0)
		[
			SNew(SBorder)
			.Padding(4.f)
		[
			AdvancedConfiguresView->AsShared()
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 8.0f, 8.0f, 4.0f)
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Right)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("StartCook"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakCookPage::StartCookResources)
		.IsEnabled_Lambda([this]() { return StartCookResourcesEnabled(); })
		]
		]

		]

		];


	AdvancedConfiguresView->SetObject(CookAdvancedConfigures);
}


void SAdvancePakCookPage::CreateAdvancedConfigureDetailView()
{
	CookAdvancedConfigures = GetMutableDefault<UCookAdvancedConfigures>();

	FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = true;
	DetailsViewArgs.bLockable = true;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::ComponentsAndActorsUseNameArea;
	DetailsViewArgs.bCustomNameAreaLocation = false;
	DetailsViewArgs.bCustomFilterAreaLocation = true;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;

	AdvancedConfiguresView = EditModule.CreateDetailView(DetailsViewArgs);;
}

FReply SAdvancePakCookPage::StartCookResources()
{
	FString FullCommandParams = FString::Printf(TEXT("%s -run=cook -targetplatform"), *FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()));

	TArray<FString> TargetPlatforms = PlatformsView->GetAllSelectPlatforms();
	for (int i = 0; i < TargetPlatforms.Num(); ++i)
	{
		FullCommandParams += i == 0 ? FString::Printf(TEXT("=%s"), *TargetPlatforms[i]) : FString::Printf(TEXT("+%s"), *TargetPlatforms[i]);
	}

	if (!CookAdvancedConfigures->bCookAll)
	{
		FullCommandParams += TEXT(" -map");
		TArray<FString> CookedMaps = MapsView->GetAllSelectMaps();
		for (int i = 0; i < CookedMaps.Num(); ++i)
		{
			FullCommandParams += i == 0 ? FString::Printf(TEXT("=%s"), *CookedMaps[i]) : FString::Printf(TEXT("+%s"), *CookedMaps[i]);
		}
	}

	if (CookAdvancedConfigures->bCookIterate)
	{
		FullCommandParams += TEXT(" -iterate");
	}

	if (CookAdvancedConfigures->bCookAll)
	{
		FullCommandParams += TEXT(" -CookAll");
	}

	if (CookAdvancedConfigures->bCompressed)
	{
		FullCommandParams += TEXT(" -Compressed");
	}

	if (TargetPlatforms.Num() == 1 && !CookAdvancedConfigures->CookOutputDirectory.Path.IsEmpty())
	{
		FullCommandParams += FString::Printf(TEXT(" OutputDir=%s/%s"), *CookAdvancedConfigures->CookOutputDirectory.Path, *TargetPlatforms[0]);
	}

	if (!CookAdvancedConfigures->AdvancedOptions.IsEmpty())
	{
		FullCommandParams += FString::Printf(TEXT(" %s"), *CookAdvancedConfigures->AdvancedOptions.TrimStart());
	}

	AdvancePakCreator->IsCooking = true;

	CookProcTaskThread = MakeShareable(new FAdvancePakProcThread(TEXT("CookTask"), UAdvancePakEditorLibrary::UnrealCmdPath, FullCommandParams));
	CookProcTaskThread->ProcBeginDelegate.AddRaw(this, &SAdvancePakCookPage::DealwithCookProcBegin);
	CookProcTaskThread->ProcSucceedDelegate.AddRaw(this, &SAdvancePakCookPage::DealwithCookProcSucceed);
	CookProcTaskThread->ProcFailedDelegate.AddRaw(this, &SAdvancePakCookPage::DealwithCookProcFailed);
	CookProcTaskThread->ProcOutputDelegate.AddRaw(this, &SAdvancePakCookPage::DealwithCookProcResult);

	CookProcTaskThread->Execute();

	return FReply::Handled();
}

bool SAdvancePakCookPage::StartCookResourcesEnabled()
{
	if (CookProcTaskThread.IsValid())
		return false;

	if (PlatformsView->GetAllSelectPlatforms().Num() == 0)
		return false;

	if (PlatformsView->GetAllSelectPlatforms().Num() > 1 && !CookAdvancedConfigures->CookOutputDirectory.Path.IsEmpty())
		return false;

	if (MapsView->GetAllSelectMaps().Num() == 0 && !CookAdvancedConfigures->bCookAll)
		return false;

	return true;
}

void SAdvancePakCookPage::DealwithCookProcResult(FString OutputLog)
{
	if (OutputLog.Contains(TEXT("Error:")))
	{
		UE_LOG(AdvancePakLog, Error, TEXT("%s"), *OutputLog);
	}
	else if (OutputLog.Contains(TEXT("Warning:")))
	{
		UE_LOG(AdvancePakLog, Warning, TEXT("%s"), *OutputLog);
	}
	else
	{
		UE_LOG(AdvancePakLog, Log, TEXT("%s"), *OutputLog);
	}
}

void SAdvancePakCookPage::DealwithCookProcBegin()
{
	AsyncTask(ENamedThreads::GameThread, [this]() {

		FString TargetPlatforms;
		for (int i = 0; i < PlatformsView->GetAllSelectPlatforms().Num(); ++i)
		{
			TargetPlatforms += i == 0 ? PlatformsView->GetAllSelectPlatforms()[0] : FString::Printf(TEXT("+%s"), *PlatformsView->GetAllSelectPlatforms()[i]);
		}

		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("Platform"), FText::FromString(TargetPlatforms));
		Arguments.Add(TEXT("TaskName"), OPTEXT("CookTask"));
		FNotificationInfo Info(FText::Format(LOCTEXT("UatTaskInProgressNotification", "{TaskName} for {Platform}..."), Arguments));

		Info.Image = FEditorStyle::GetBrush(TEXT("MainFrame.CookContent"));
		Info.bFireAndForget = false;
		Info.FadeOutDuration = 0.0f;
		Info.ExpireDuration = 0.0f;
		Info.Hyperlink = FSimpleDelegate::CreateRaw(this, &SAdvancePakCookPage::DealwithUatLinkwork);
		Info.HyperlinkText = OPTEXT("ShowOutputLog");
		Info.ButtonDetails.Add(
			FNotificationButtonInfo(
				OPTEXT("Cancel"),
				OPTEXT("CancelTask"),
				FSimpleDelegate::CreateRaw(this, &SAdvancePakCookPage::DealwithUatCancelButtonClicked),
				SNotificationItem::CS_Pending
			)
		);
		Info.ButtonDetails.Add(
			FNotificationButtonInfo(
				OPTEXT("Dismiss"),
				FText(),
				FSimpleDelegate::CreateRaw(this, &SAdvancePakCookPage::DealwithDismissButtonClicked),
				SNotificationItem::CS_Fail
			)
		);

		TSharedPtr<SNotificationItem> NotificationItem = FSlateNotificationManager::Get().AddNotification(Info);
		if (NotificationItemPtr.IsValid())
		{
			NotificationItemPtr.Pin().Get()->Fadeout();
		}

		if (!NotificationItem.IsValid())
		{
			return;
		}

		FEditorAnalytics::ReportEvent(TEXT("Editor.Cook.Start"), PlatformsView->GetAllSelectPlatforms()[0], false);

		NotificationItem->SetCompletionState(SNotificationItem::CS_Pending);
		NotificationItem->SetFadeOutDuration(5.f);

		NotificationItemPtr = NotificationItem;

		GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileStart_Cue.CompileStart_Cue"));

		UE_LOG(AdvancePakLog, Warning, TEXT("Cook Begin"));

		}
	);
}

void SAdvancePakCookPage::DealwithCookProcSucceed()
{
	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			TSharedPtr<SNotificationItem> NotificationItem = NotificationItemPtr.Pin();

			if (NotificationItem.IsValid())
			{
				NotificationItem->SetText(LOCTEXT("CookSuccessedNotification", "Cook Finished!"));
				NotificationItem->SetCompletionState(SNotificationItem::CS_Success);
				NotificationItem->ExpireAndFadeout();

				NotificationItemPtr.Reset();
			}

			AdvancePakCreator->IsCooking = false;

			CookProcTaskThread.Reset();

			GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));

			UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("CookResourceSucceed"), FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir()));

			UE_LOG(AdvancePakLog, Warning, TEXT("Cook Succeed"));
		}
	);
}

void SAdvancePakCookPage::DealwithCookProcFailed()
{
	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			TSharedPtr<SNotificationItem> NotificationItem = NotificationItemPtr.Pin();

			if (NotificationItem.IsValid())
			{
				NotificationItem->SetText(LOCTEXT("CookFaildNotification", "Cook Faild!"));
				NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
				NotificationItem->ExpireAndFadeout();

				NotificationItemPtr.Reset();
			}

			AdvancePakCreator->IsCooking = false;

			CookProcTaskThread.Reset();

			GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));

			UE_LOG(AdvancePakLog, Error, TEXT("Cook Failed"));
		}
	);
}

void SAdvancePakCookPage::DealwithUatLinkwork()
{
	FGlobalTabmanager::Get()->InvokeTab(FName("OutputLog"));
}

void SAdvancePakCookPage::DealwithUatCancelButtonClicked()
{
	if (CookProcTaskThread.IsValid())
	{
		CookProcTaskThread->Cancel();

		AdvancePakCreator->IsCooking = false;
	}
}

void SAdvancePakCookPage::DealwithDismissButtonClicked()
{

}

/* SAdvancePakCookPage callbacks
 *****************************************************************************/



#undef LOCTEXT_NAMESPACE
