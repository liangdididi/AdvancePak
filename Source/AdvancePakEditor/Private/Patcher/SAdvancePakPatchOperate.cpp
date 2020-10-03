// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdvancePakPatchOperate.h"
#include "SlateOptMacros.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Utility/AdvancePakProcThread.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Layout/SBox.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSeparator.h"
#include "Framework/Application/SlateApplication.h"
#include "Utility/AdvancePakCreator.h"
#include "EditorStyleSet.h"
#include "Framework/Docking/TabManager.h"
#include "Forms/SAdvancePakAccordsForm.h"
#include "AdvancePakType.h"


#define LOCTEXT_NAMESPACE "SAdvancePakPatchOperate"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakPatchOperate::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;
	AdvancePakCreator->OnActiveNameChanged.AddRaw(this, &SAdvancePakPatchOperate::OnConfiguresChanged);
	AdvancePakCreator->OnTotalConfiguresChanged.AddRaw(this, &SAdvancePakPatchOperate::OnConfiguresChanged);

	ChildSlot
	[
		SNew(SBorder)
		.Padding(4.f)
		[
			SNew(SVerticalBox)
			.IsEnabled_Lambda([this]() { return !AdvancePakCreator->IsPackaging && AdvancePakCreator->GetFuturePatch().IsValid(); })

	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(PickupOptionsEditor, SMultiLineEditableTextBox)
			.OnTextCommitted(this, &SAdvancePakPatchOperate::OnPickupOptionsCommitted)
		.AutoWrapText(true)
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("PakList"))
		.ToolTipText(OPTEXT("CreatePakListFile"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchOperate::DealwithPakListButtonClicked)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("ExportConfig"))
		.ToolTipText(OPTEXT("ExportPatchConfigure"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchOperate::DealwithExportButtonClicked)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("ReplicateConfig"))
		.ToolTipText(OPTEXT("ReplicateConfigureToPatch"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchOperate::DealwithExportButtonClicked)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("SaveConfig"))
		.ToolTipText(OPTEXT("SavePatchConfigure"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchOperate::DealwithSaveButtonClicked)
		]


	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("Pickup"))
		.ToolTipText(OPTEXT("PickupPatch"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchOperate::DealwithPickupButtonClicked)
		]

		]
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAdvancePakPatchOperate::OnConfiguresChanged()
{
	TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = AdvancePakCreator->GetFuturePatch();
	if (PatchConfigure.IsValid())
	{
		PickupOptionsEditor->SetText(FText::FromString(PatchConfigure->PakOptions));
	}
	else
	{
		PickupOptionsEditor->SetText(FText());
	}
}

void SAdvancePakPatchOperate::OnPickupOptionsCommitted(const FText& InText, ETextCommit::Type CommitInfo)
{
	AdvancePakCreator->GetFuturePatch()->PakOptions = InText.ToString();
}

FReply SAdvancePakPatchOperate::DealwithPakListButtonClicked()
{
	PromotePatchAccords();

	if (AdvancePakCreator->IsPatchAccordFaild)
	{
		RenovatePatchAccordsForm();
		UAdvancePakEditorLibrary::CreatePromptNotify(OPSTRING("PatchResourcesLacked"), false);
		return FReply::Handled();
	}

	if (!PromotePatchPakList())
	{
		UAdvancePakEditorLibrary::CreatePromptNotify(OPSTRING("NoResourcesToPak"), false);
		return FReply::Handled();
	}

	UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("SavePatchPakListFile"), PakListPath);

	return FReply::Handled();
}

FReply SAdvancePakPatchOperate::DealwithSaveButtonClicked()
{
	FString PublishConfigPath = AdvancePakCreator->GetFuturePublish()->ConfigDir / AdvancePakCreator->GetFuturePublish()->Version / UAdvancePakEditorLibrary::PublishConfigName;
	FString PublishContext = UAdvancePakEditorLibrary::AbsorbPublishConfigureToJson(AdvancePakCreator->GetFuturePublish());
	FFileHelper::SaveStringToFile(PublishContext, *PublishConfigPath);

	PromotePatchAccords();

	FString PatchConfigPath = AdvancePakCreator->GetFuturePublish()->ConfigDir / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->GetFuturePublish()->Version / (AdvancePakCreator->ActiveName.ToString() + TEXT("Config.json"));
	FString PatchContext = UAdvancePakEditorLibrary::AbsorbPatchConfigureToJson(AdvancePakCreator->GetFuturePatch());
	FFileHelper::SaveStringToFile(PatchContext, *PatchConfigPath);

	UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("SavePatchConfig") + AdvancePakCreator->ActiveName.ToString(), PatchConfigPath);
	UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("SavePublishConfig"), PublishConfigPath);

	return FReply::Handled();
}

FReply SAdvancePakPatchOperate::DealwithExportButtonClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
		void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

		FString ExportPath;

		const bool bFolderSelected = DesktopPlatform->OpenDirectoryDialog(
			ParentWindowHandle,
			FString("Choose a directory"),
			AdvancePakCreator->GetFuturePublish()->ConfigDir,
			ExportPath
		);

		if (bFolderSelected)
		{
			ExportPath = FPaths::ConvertRelativePathToFull(ExportPath);

			FString PatchContext = UAdvancePakEditorLibrary::AbsorbPatchConfigureToJson(AdvancePakCreator->GetFuturePatch());
			FFileHelper::SaveStringToFile(PatchContext, *(ExportPath / FString::Printf(TEXT("%sConfig.json"), *AdvancePakCreator->ActiveName.ToString())));

			UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("ExportPatch"), *ExportPath);
		}
	}
	return FReply::Handled();
}

FReply SAdvancePakPatchOperate::DealwithReplicateButtonClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
		void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

		TArray<FString> SelectedFiles;

		const bool bFileSelected = DesktopPlatform->OpenFileDialog(
			ParentWindowHandle,
			FString("Choose Build Config Json"),
			AdvancePakCreator->GetFuturePublish()->ConfigDir,
			TEXT(""),
			TEXT("AdvancePak json (*.json)|*.json"),
			EFileDialogFlags::None,
			SelectedFiles
		);

		if (bFileSelected && SelectedFiles.Num() > 0)
		{
			FString ConfigPath = FPaths::ConvertRelativePathToFull(SelectedFiles[0]);
			if (FPaths::GetCleanFilename(ConfigPath).Equals(UAdvancePakEditorLibrary::PublishConfigName))
			{
				FString JsonContext;
				FFileHelper::LoadFileToString(JsonContext, *ConfigPath);
				TSharedPtr<FAdvancePakPublishConfigure> PublishConfigure = UAdvancePakEditorLibrary::AbsorbJsonToPublishConfigure(JsonContext);

				if (!PublishConfigure->Patchs.Contains(AdvancePakCreator->ActiveName))
				{
					PublishConfigure->Patchs.Add(AdvancePakCreator->ActiveName);

					FString PublishContext = UAdvancePakEditorLibrary::AbsorbPublishConfigureToJson(PublishConfigure);
					FFileHelper::SaveStringToFile(PublishContext, *ConfigPath);

					UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("UpdatePublishConfig"), *ConfigPath);
				}

				FString PatchContext = UAdvancePakEditorLibrary::AbsorbPatchConfigureToJson(AdvancePakCreator->GetFuturePatch());
				FFileHelper::SaveStringToFile(PatchContext, *(PublishConfigure->ConfigDir / PublishConfigure->Version / PublishConfigure->Version / FString::Printf(TEXT("%sConfig.json"), *AdvancePakCreator->ActiveName.ToString())));

				UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("ReplicatePatchConfig"), *(PublishConfigure->ConfigDir / PublishConfigure->Version / PublishConfigure->Version));
			}
		}
	}

	return FReply::Handled();
}

FReply SAdvancePakPatchOperate::DealwithPickupButtonClicked()
{
	PromotePatchAccords();

	if (AdvancePakCreator->IsPatchAccordFaild)
	{
		RenovatePatchAccordsForm();
		UAdvancePakEditorLibrary::CreatePromptNotify(OPSTRING("PatchResourcesLacked"), false);
		return FReply::Handled();
	}

	if (!PromotePatchPakList())
	{
		UAdvancePakEditorLibrary::CreatePromptNotify(OPSTRING("NoResourcesToPak"), false);
		return FReply::Handled();
	}

	PickupTaskThread = MakeShareable(new FAdvancePakProcThread(
		TEXT("PickupTask"),
		UAdvancePakEditorLibrary::UnrealPakPath,
		PromotePickupOptions()
	));

	PickupTaskThread->ProcOutputDelegate.AddRaw(this, &SAdvancePakPatchOperate::OnPickupTaskOutput);
	PickupTaskThread->ProcBeginDelegate.AddRaw(this, &SAdvancePakPatchOperate::OnPickupTaskBegin);
	PickupTaskThread->ProcSucceedDelegate.AddRaw(this, &SAdvancePakPatchOperate::OnPickupTaskSucceed);
	PickupTaskThread->ProcFailedDelegate.AddRaw(this, &SAdvancePakPatchOperate::OnPickupTaskFailed);
	PickupTaskThread->Execute();

	AdvancePakCreator->IsPackaging = true;

	return FReply::Handled();
}

void SAdvancePakPatchOperate::OnPickupTaskFailed()
{
	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{

		AdvancePakCreator->IsPackaging = false;

		TSharedPtr<SNotificationItem> NotificationItem = NotificationItemPtr.Pin();

		if (NotificationItem.IsValid())
		{
			NotificationItem->SetText(OPTEXT("PickupFailed"));
			NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
			NotificationItem->ExpireAndFadeout();

			NotificationItemPtr.Reset();
		}

		PickupTaskThread.Reset();

		GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));

	}, TStatId(), NULL, ENamedThreads::GameThread);
}

void SAdvancePakPatchOperate::DealwithPickupLinkwork()
{
	FGlobalTabmanager::Get()->InvokeTab(FName("OutputLog"));
}

void SAdvancePakPatchOperate::RenovatePatchAccordsForm()
{
	if (!AdvancePakCreator->IsActivedPatch())
		return;

	if (!AdvancePakCreator->FuturePublish.IsValid())
	{
		AdvancePakCreator->PatchAccords.Reset();
		AdvancePakCreator->PatchAccordsForm->DemandViewRenovate();
		return;
	}

	AdvancePakCreator->PatchAccordsForm->DemandViewRenovate(AdvancePakCreator->IsPatchAccordFaild);
}

void SAdvancePakPatchOperate::PromotePatchAccords(const FName& PatchName /*= NAME_None*/)
{
	TSharedPtr<FAdvancePakPatchConfigure> FormerPatchConfigure = AdvancePakCreator->GetFormerPatch(PatchName);
	TSharedPtr<FAdvancePakPatchConfigure> FuturePatchConfigure = AdvancePakCreator->GetFuturePatch(PatchName);

	AdvancePakCreator->PatchAccords.Reset();
	AdvancePakCreator->IsPatchAccordFaild = false;
	AdvancePakCreator->IsPatchReduced = false;

	FuturePatchConfigure->PakFileList.Reset();

	TArray<FName> LackRegulars;
	UAdvancePakEditorLibrary::AbsorbRegularsToPakFileList(FuturePatchConfigure->Regulars, FuturePatchConfigure->PakFileList, LackRegulars);

	TMap<FName, FString> LackInformals;
	UAdvancePakEditorLibrary::AbsorbInformalsToPakFileList(FuturePatchConfigure->Informals, FuturePatchConfigure->PakFileList, LackInformals);

	if (LackRegulars.Num() + LackInformals.Num() > 0)
	{
		for (auto PackageName : LackRegulars)
		{
			TSharedPtr<FAdvancePakAccordInfo> AccordInfo = MakeShareable(new FAdvancePakAccordInfo());
			AccordInfo->Name = PackageName.ToString();
			AccordInfo->AccordType = EAdvancePakAccordType::Delete;
			AccordInfo->IsRegular = true;
			AdvancePakCreator->PatchAccords.Add(AccordInfo);
		}

		for (TMap<FName, FString>::TIterator It(LackInformals); It; ++It)
		{
			TSharedPtr<FAdvancePakAccordInfo> AccordInfo = MakeShareable(new FAdvancePakAccordInfo());
			AccordInfo->Name = It.Key().ToString();
			AccordInfo->Path = It.Value();
			AccordInfo->AccordType = EAdvancePakAccordType::Delete;
			AccordInfo->IsRegular = false;
			AdvancePakCreator->PatchAccords.Add(AccordInfo);
		}

		AdvancePakCreator->IsPatchAccordFaild = true;
	}
	else
	{
		if (FormerPatchConfigure.IsValid())
		{
			TArray<TSharedPtr<FAdvancePakAccordInfo>> AddAccords;
			TArray<TSharedPtr<FAdvancePakAccordInfo>> UpdateAccords;
			TArray<TSharedPtr<FAdvancePakAccordInfo>> KeepAccords;
			TArray<TSharedPtr<FAdvancePakAccordInfo>> DeleteAccords;
			for (auto FuturePakLine : FuturePatchConfigure->PakFileList)
			{
				EAdvancePakAccordType AccordType = EAdvancePakAccordType::Add;
				for (auto FormerPakLine : FormerPatchConfigure->PakFileList)
				{
					if (FormerPakLine->MountPath.Equals(FuturePakLine->MountPath))
					{
						if (FormerPakLine->Hash.Equals(FuturePakLine->Hash))
						{
							AccordType = EAdvancePakAccordType::Keep;
							break;
						}
						AccordType = EAdvancePakAccordType::Update;
						break;
					}
				}
				TSharedPtr<FAdvancePakAccordInfo> AccordInfo = MakeShareable(new FAdvancePakAccordInfo());
				AccordInfo->Name = FuturePakLine->MountPath;
				AccordInfo->Path = FuturePakLine->FilePath;
				AccordInfo->AccordType = AccordType;
				switch (AccordType)
				{
				case EAdvancePakAccordType::Add:
					AddAccords.Add(AccordInfo);
					break;
				case EAdvancePakAccordType::Update:
					UpdateAccords.Add(AccordInfo);
					break;
				case EAdvancePakAccordType::Keep:
					KeepAccords.Add(AccordInfo);
					break;
				}
			}
			for (auto FormerPakLine : FormerPatchConfigure->PakFileList)
			{
				bool IsDeleted = true;
				for (auto FuturePakLine : FuturePatchConfigure->PakFileList)
				{
					if (FuturePakLine->MountPath.Equals(FormerPakLine->MountPath))
					{
						IsDeleted = false;
						break;
					}
				}
				if (IsDeleted)
				{
					TSharedPtr<FAdvancePakAccordInfo> AccordInfo = MakeShareable(new FAdvancePakAccordInfo());
					AccordInfo->Name = FormerPakLine->MountPath;
					AccordInfo->Path = FormerPakLine->FilePath;
					AccordInfo->AccordType = EAdvancePakAccordType::Delete;
					DeleteAccords.Add(AccordInfo);
				}
			}

			AdvancePakCreator->IsPatchReduced = DeleteAccords.Num() > 0;

			AdvancePakCreator->PatchAccords.Append(AddAccords);
			AdvancePakCreator->PatchAccords.Append(UpdateAccords);
			AdvancePakCreator->PatchAccords.Append(KeepAccords);
			AdvancePakCreator->PatchAccords.Append(DeleteAccords);
		}
		else
		{
			for (auto PakLine : FuturePatchConfigure->PakFileList)
			{
				TSharedPtr<FAdvancePakAccordInfo> AccordInfo = MakeShareable(new FAdvancePakAccordInfo());
				AccordInfo->Name = PakLine->MountPath;
				AccordInfo->Path = PakLine->FilePath;
				AccordInfo->AccordType = EAdvancePakAccordType::Add;
				AdvancePakCreator->PatchAccords.Add(AccordInfo);
			}
		}
	}
}

bool SAdvancePakPatchOperate::PromotePatchPakList(const FName& PatchName /*= NAME_None*/)
{
	TSharedPtr<FAdvancePakPatchConfigure> FuturePatchConfigure = AdvancePakCreator->GetFuturePatch(PatchName);

	FString PakListContext;

	if (AdvancePakCreator->FormerPublish.IsValid())
	{
		PakListPath = AdvancePakCreator->FuturePublish->ConfigDir / AdvancePakCreator->FuturePublish->Version / AdvancePakCreator->FormerPublish->Version / ((PatchName.IsNone() ? AdvancePakCreator->ActiveName.ToString() : PatchName.ToString()) + FString("PakList.txt"));
		PakFilePath = (AdvancePakCreator->FuturePublish->PackageDir.IsEmpty() ? AdvancePakCreator->FuturePublish->ConfigDir : AdvancePakCreator->FuturePublish->PackageDir) / AdvancePakCreator->FuturePublish->Version / AdvancePakCreator->FormerPublish->Version / ((PatchName.IsNone() ? AdvancePakCreator->ActiveName.ToString() : PatchName.ToString()) + FString(".pak"));
		for (auto AccordInfo : AdvancePakCreator->PatchAccords)
		{
			if (AccordInfo->AccordType == EAdvancePakAccordType::Add || AccordInfo->AccordType == EAdvancePakAccordType::Update)
			{
				PakListContext += FString::Printf(TEXT("\"%s\" \"%s\" %s\n"), *AccordInfo->Path, *AccordInfo->Name, *AdvancePakCreator->FuturePublish->LineOptions);
			}
		}
	}
	else
	{
		PakListPath = AdvancePakCreator->FuturePublish->ConfigDir / AdvancePakCreator->FuturePublish->Version / AdvancePakCreator->FuturePublish->Version / ((PatchName.IsNone() ? AdvancePakCreator->ActiveName.ToString() : PatchName.ToString()) + FString("PakList.txt"));
		PakFilePath = (AdvancePakCreator->FuturePublish->PackageDir.IsEmpty() ? AdvancePakCreator->FuturePublish->ConfigDir : AdvancePakCreator->FuturePublish->PackageDir) / AdvancePakCreator->FuturePublish->Version / AdvancePakCreator->FuturePublish->Version / ((PatchName.IsNone() ? AdvancePakCreator->ActiveName.ToString() : PatchName.ToString()) + FString(".pak"));
		for (auto PakLine : FuturePatchConfigure->PakFileList)
		{
			PakListContext += FString::Printf(TEXT("\"%s\" \"%s\" %s\n"), *PakLine->FilePath, *PakLine->MountPath, *AdvancePakCreator->FuturePublish->LineOptions);
		}
	}

	if (PakListContext.IsEmpty())
	{
		return false;
	}

	return FFileHelper::SaveStringToFile(PakListContext, *PakListPath);
}

void SAdvancePakPatchOperate::DealwithPickupCancelButtonClicked()
{
	if (PickupTaskThread.IsValid())
	{
		PickupTaskThread->Cancel();
	}
	AdvancePakCreator->IsPackaging = false;
}

void SAdvancePakPatchOperate::DealwithDismissButtonClicked()
{
	TSharedPtr<SNotificationItem> NotificationItem = NotificationItemPtr.Pin();

	if (NotificationItem.IsValid())
	{
		NotificationItem->ExpireAndFadeout();
		NotificationItemPtr.Reset();
	}
}

void SAdvancePakPatchOperate::OnPickupTaskOutput(FString OutputLog)
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

void SAdvancePakPatchOperate::OnPickupTaskBegin()
{
	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("Platform"), FText::FromString(AdvancePakCreator->GetFuturePublish()->Platform));
		Arguments.Add(TEXT("TaskName"), OPTEXT("PickupTask"));
		FNotificationInfo Info(FText::Format(LOCTEXT("PickupTaskInProgressNotification", "{TaskName} for {Platform}..."), Arguments));

		Info.Image = FEditorStyle::GetBrush(TEXT("MainFrame.CookContent"));
		Info.bFireAndForget = false;
		Info.FadeOutDuration = 0.0f;
		Info.ExpireDuration = 0.0f;
		Info.Hyperlink = FSimpleDelegate::CreateRaw(this, &SAdvancePakPatchOperate::DealwithPickupLinkwork);
		Info.HyperlinkText = OPTEXT("ShowOutputLog");
		Info.ButtonDetails.Add(
			FNotificationButtonInfo(
				OPTEXT("Cancel"),
				OPTEXT("CancelTask"),
				FSimpleDelegate::CreateRaw(this, &SAdvancePakPatchOperate::DealwithPickupCancelButtonClicked),
				SNotificationItem::CS_Pending
			)
		);
		Info.ButtonDetails.Add(
			FNotificationButtonInfo(
				OPTEXT("Dismiss"),
				FText(),
				FSimpleDelegate::CreateRaw(this, &SAdvancePakPatchOperate::DealwithDismissButtonClicked),
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

		NotificationItem->SetCompletionState(SNotificationItem::CS_Pending);
		NotificationItem->SetFadeOutDuration(5.f);

		NotificationItemPtr = NotificationItem;

		GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileStart_Cue.CompileStart_Cue"));
	}, TStatId(), NULL, ENamedThreads::GameThread);
}

void SAdvancePakPatchOperate::OnPickupTaskSucceed()
{
	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		AdvancePakCreator->IsPackaging = false;

		TSharedPtr<SNotificationItem> NotificationItem = NotificationItemPtr.Pin();

		if (NotificationItem.IsValid())
		{
			NotificationItem->SetText(OPTEXT("PickupSucceed"));
			NotificationItem->SetCompletionState(SNotificationItem::CS_Success);
			NotificationItem->ExpireAndFadeout();

			NotificationItemPtr.Reset();
		}

		PickupTaskThread.Reset();

		GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));

		UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("ExportPakFile"), PakFilePath);
		UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("ExportListFile"), PakListPath);

	}, TStatId(), NULL, ENamedThreads::GameThread);
}

FString SAdvancePakPatchOperate::PromotePickupOptions()
{
	return FString::Printf(TEXT("%s -create=\"%s\" %s"), *PakFilePath, *PakListPath, *PickupOptionsEditor->GetText().ToString());
}

#undef LOCTEXT_NAMESPACE