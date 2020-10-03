// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakBuildPage.h"
#include "SlateOptMacros.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "LauncherServices/Public/ILauncherProfile.h"
#include "Widgets/Text/STextBlock.h"
#include "Utility/AdvancePakCreator.h"
#include "Framework/Application/SlateApplication.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "Forms/SAdvancePakPlatformsForm.h"
#include "Utility/AdvancePakProcThread.h"
#include "AdvancePakLibrary.h"
#include "Utility/AdvancePakEditorLibrary.h"
#include "Editor/UnrealEd/Classes/Settings/ProjectPackagingSettings.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Misc/SecureHash.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include <string>

#define LOCTEXT_NAMESPACE "SAdvancePakBuildPage"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakBuildPage::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;

	CreateBuildConfiguresView();

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
		[
			SNew(STextBlock)
			.Text(OPTEXT("BuildPage"))
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		.ColorAndOpacity(FLinearColor(0.f, 1.f, 1.f))
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0, 4.0, 0.0, 0.0)
		[
			SNew(SBorder)
			.Padding(4.f)
		[

			BuildConfiguresView->AsShared()
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0, 8.0, 0.0, 0.0)
		[
			SNew(SBorder)
			.Padding(4.f)
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.Image(FCoreStyle::Get().GetBrush("ExpandableArea.Border"))
		.ColorAndOpacity(FLinearColor::White)
		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(4.f)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SNew(STextBlock)
			.Text(OPTEXT("PleaseSelectOutputDirectory"))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)

		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.FillWidth(1.0)
		.MaxWidth(500.f)
		.Padding(4.0, 0.0, 0.0, 3.0)
		[
			SAssignNew(OutputDirEditor, SEditableTextBox)
			.IsEnabled(this, &SAdvancePakBuildPage::IsEditable)
		.OnTextCommitted(this, &SAdvancePakBuildPage::OnOutputDirCommitted)
		.HintText(OPTEXT("PleaseSelectOutputDirectory"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Right)
		.Padding(4.0, 0.0, 0.0, 0.0)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.IsEnabled(this, &SAdvancePakBuildPage::IsEditable)
		.Text(OPTEXT("Browse"))
		.ToolTipText(OPTEXT("BrowseForTheDirectory"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakBuildPage::DealwithOutputDirButtonClicked)
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SNew(STextBlock)
			.Text(OPTEXT("PleaseSelectConfigDirectory"))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)

		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.FillWidth(1.0)
		.MaxWidth(500.f)
		.Padding(4.0, 0.0, 0.0, 3.0)
		[
			SAssignNew(ConfigDirEditor, SEditableTextBox)
			.IsEnabled(this, &SAdvancePakBuildPage::IsEditable)
		.OnTextCommitted(this, &SAdvancePakBuildPage::OnConfigDirCommitted)
		.HintText(OPTEXT("PleaseSelectConfigDirectory"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Right)
		.Padding(4.0, 0.0, 0.0, 0.0)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.IsEnabled(this, &SAdvancePakBuildPage::IsEditable)
		.Text(OPTEXT("Browse"))
		.ToolTipText(OPTEXT("BrowseForTheDirectory"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakBuildPage::DealwithConfigDirButtonClicked)
		]
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SNew(STextBlock)
			.Text(OPTEXT("PleaseSelectBuildPlatform"))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(4.f, 4.f, 0.f, 0.f)
		[
			SAssignNew(BuildPlatformsView, SAdvancePakPlatformsForm, InModel)
			.IsSingleMode(true)
		.OnPlatformSelected(this, &SAdvancePakBuildPage::OnBuildConfigureChanged)
		.IsEnabled(this, &SAdvancePakBuildPage::IsEditable)
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SNew(STextBlock)
			.Text(OPTEXT("BuildOptionEditor"))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(4.f, 4.f, 0.f, 0.f)
		[
			SAssignNew(BuildOptionsEditor, SMultiLineEditableTextBox)
			.IsEnabled(this, &SAdvancePakBuildPage::IsEditable)
		.OnTextCommitted(this, &SAdvancePakBuildPage::OnBuildOptionCommitted)
		.AutoWrapText(true)
		]
		]
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0, 8.0, 8.0, 4.0)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			SNullWidget::NullWidget
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.IsEnabled(this, &SAdvancePakBuildPage::IsEditable)
		.Text(OPTEXT("ImportConfig"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakBuildPage::DealwithImportConfigClicked)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 4.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.IsEnabled(this, &SAdvancePakBuildPage::IsExportAble)
		.Text(OPTEXT("ExportConfig"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakBuildPage::DealwithExportConfigClicked)
		]


	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 4.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.IsEnabled(this, &SAdvancePakBuildPage::IsBuildAble)
		.Text(OPTEXT("StartBuild"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakBuildPage::DealwithBuildPublishClicked)
		]
		]

		]
		];

	BuildConfiguresView->SetObject(BuildConfiguresObject);
	BuildConfiguresView->SetEnabled(TAttribute<bool>(this, &SAdvancePakBuildPage::IsEditable));
}

void SAdvancePakBuildPage::CreateBuildConfiguresView()
{
	FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = true;
	DetailsViewArgs.bLockable = true;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::ComponentsAndActorsUseNameArea;
	DetailsViewArgs.bCustomNameAreaLocation = false;
	DetailsViewArgs.bCustomFilterAreaLocation = true;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;

	BuildConfiguresView = EditModule.CreateDetailView(DetailsViewArgs);

	BuildConfiguresObject = UAdvancePakBuildConfigures::Get();

	BuildConfiguresObject->OnBuildConfigurationChanged.BindRaw(this, &SAdvancePakBuildPage::OnBuildConfigureChanged);
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SAdvancePakBuildPage::IsEditable() const
{
	return !AdvancePakCreator->IsBuilding;
}

bool SAdvancePakBuildPage::IsBuildAble() const
{
	return !BuildConfiguresObject->Version.IsEmpty()
		&& BuildConfiguresObject->MapsToBuild.Num() > 0
		&& !BuildOptionsEditor->GetText().ToString().IsEmpty()
		&& !OutputDirEditor->GetText().ToString().IsEmpty()
		&& !ConfigDirEditor->GetText().ToString().IsEmpty()
		&& !AdvancePakCreator->IsBuilding;
}

bool SAdvancePakBuildPage::IsExportAble() const
{
	return !BuildConfiguresObject->Version.IsEmpty()
		&& BuildConfiguresObject->MapsToBuild.Num() > 0
		&& !BuildOptionsEditor->GetText().ToString().IsEmpty()
		&& !OutputDirEditor->GetText().ToString().IsEmpty()
		&& !ConfigDirEditor->GetText().ToString().IsEmpty()
		&& !AdvancePakCreator->IsBuilding;
}

void SAdvancePakBuildPage::OnOutputDirCommitted(const FText& InText, ETextCommit::Type CommitInfo)
{

}

FReply SAdvancePakBuildPage::DealwithOutputDirButtonClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
		void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

		const bool bFolderSelected = DesktopPlatform->OpenDirectoryDialog(
			ParentWindowHandle,
			FString("Choose a directory"),
			OutputDirEditor->GetText().ToString(),
			OutputPath
		);

		if (bFolderSelected)
		{
			OutputPath = FPaths::ConvertRelativePathToFull(OutputPath);
			OutputDirEditor->SetText(FText::FromString(OutputPath));

			OnBuildConfigureChanged();
		}
	}
	return FReply::Handled();
}

void SAdvancePakBuildPage::OnConfigDirCommitted(const FText& InText, ETextCommit::Type CommitInfo)
{

}

FReply SAdvancePakBuildPage::DealwithConfigDirButtonClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
		void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

		const bool bFolderSelected = DesktopPlatform->OpenDirectoryDialog(
			ParentWindowHandle,
			FString("Choose a directory"),
			ConfigDirEditor->GetText().ToString(),
			ConfigPath
		);

		if (bFolderSelected)
		{
			ConfigPath = FPaths::ConvertRelativePathToFull(ConfigPath);
			ConfigDirEditor->SetText(FText::FromString(ConfigPath));
		}
	}

	return FReply::Handled();
}

void SAdvancePakBuildPage::OnBuildOptionCommitted(const FText& InText, ETextCommit::Type CommitInfo)
{

}

void SAdvancePakBuildPage::OnBuildConfigureChanged()
{
	BuildOptionsEditor->SetText(FText::FromString(GetBuildPlatformOptions(BuildPlatformsView->GetSelectPlatform())));
}

FString SAdvancePakBuildPage::GetBuildPlatformOptions(const FString& PlatformName)
{
	FString OptionsString = FString();

	if (PlatformName.Equals("WindowsNoEditor"))
	{
		OptionsString = FString::Printf(TEXT("-ScriptsForProject=\"%s\" BuildCookRun -nocompile -nocompileeditor -installed -nop4 -project=\"%s\" -cook -stage -archive -package -clientconfig=%s -ue4exe=\"%s\" -archivedirectory=\"%s\" -compressed -SkipCookingEditorContent -pak -prereqs -targetplatform=Win64 -build -utf8output"),
			*FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()),
			*FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()),
			*AbsorbBuildConfigToString(BuildConfiguresObject->BuildConfig),
			*UAdvancePakEditorLibrary::UnrealCmdPath,
			*OutputDirEditor->GetText().ToString()
		);
	}
	else if (PlatformName.StartsWith("Android"))
	{
		FString FormatString;
		PlatformName.Split("_", NULL, &FormatString);
		if (!FormatString.IsEmpty())
		{
			OptionsString = FString::Printf(TEXT("-ScriptsForProject=\"%s\" BuildCookRun -nocompileeditor -installed -nop4 -project=\"%s\" -cook -stage -archive -package -clientconfig=%s -ue4exe=\"%s\" -archivedirectory=\"%s\" -compressed -SkipCookingEditorContent -pak -prereqs -targetplatform=Android -cookflavor=%s -build -utf8output -allmaps -nocompile"),
				*FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()), *FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()),
				*AbsorbBuildConfigToString(BuildConfiguresObject->BuildConfig),
				*UAdvancePakEditorLibrary::UnrealCmdPath,
				*OutputDirEditor->GetText().ToString(),
				*FormatString
			);
		}
	}
	else if (PlatformName.Equals("IOS"))
	{
		OptionsString = FString::Printf(TEXT("-ScriptsForProject=\"%s\" BuildCookRun -nocompileeditor -nop4 -project=\"%s\" -cook -stage -archive -archivedirectory=\"%s\" -package -clientconfig=%s -ue4exe=\"%s\" -compressed -SkipCookingEditorContent -pak -prereqs -targetplatform=IOS -build -utf8output"),
			*FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()),
			*FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()),
			*OutputDirEditor->GetText().ToString(),
			*AbsorbBuildConfigToString(BuildConfiguresObject->BuildConfig),
			*UAdvancePakEditorLibrary::UnrealCmdPath
		);
	}
	else if (PlatformName.Equals("MacNoEditor"))
	{
		OptionsString = FString::Printf(TEXT("-ScriptsForProject=\"%s\" BuildCookRun -nocompileeditor -nop4 -project=\"%s\" -cook -stage -archive -archivedirectory=\"%s\" -package -ue4exe=\"%s\" -pak -prereqs -nodebuginfo -targetplatform=Mac -build -target=%s -clientconfig=%s -utf8output"),
			*FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()),
			*FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()),
			*OutputDirEditor->GetText().ToString(),
			*UAdvancePakEditorLibrary::UnrealCmdPath,
			FApp::GetProjectName(),
			*AbsorbBuildConfigToString(BuildConfiguresObject->BuildConfig)
		);
	}

	return OptionsString;
}


FReply SAdvancePakBuildPage::DealwithImportConfigClicked()
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
			ConfigDirEditor->GetText().ToString(),
			TEXT(""),
			TEXT("AdvancePak json (*.json)|*.json"),
			EFileDialogFlags::None,
			SelectedFiles
		);

		if (bFileSelected && SelectedFiles.Num() > 0)
		{
			if (FPaths::FileExists(SelectedFiles[0]) && FPaths::GetCleanFilename(SelectedFiles[0]).Equals(UAdvancePakEditorLibrary::PublishConfigName))
			{
				FString JsonContext;
				FFileHelper::LoadFileToString(JsonContext, *SelectedFiles[0]);
				TSharedPtr<FAdvancePakPublishConfigure> BuildConfiguresPtr = UAdvancePakEditorLibrary::AbsorbJsonToPublishConfigure(JsonContext);
				OutputDirEditor->SetText(FText::FromString(BuildConfiguresPtr->OutputDir));
				ConfigDirEditor->SetText(FText::FromString(BuildConfiguresPtr->ConfigDir));
				BuildConfiguresObject->Version = BuildConfiguresPtr->Version;
				BuildPlatformsView->AddSelectPlatform(BuildConfiguresPtr->Platform);

				BuildConfiguresObject->MapsToBuild.Reset();
				for (auto MapPath : BuildConfiguresPtr->MapsToBuild)
				{
					FFilePath MapFilePath;
					MapFilePath.FilePath = MapPath;
					BuildConfiguresObject->MapsToBuild.Add(MapFilePath);
				}

				BuildConfiguresObject->AlwaysCookPaths.Reset();
				for (auto CookPath : BuildConfiguresPtr->AlwaysCookPaths)
				{
					FDirectoryPath CookDirectory;
					CookDirectory.Path = CookPath;
					BuildConfiguresObject->AlwaysCookPaths.Add(CookDirectory);
				}

				BuildConfiguresObject->NeverCookPaths.Reset();
				for (auto CookPath : BuildConfiguresPtr->NeverCookPaths)
				{
					FDirectoryPath CookDirectory;
					CookDirectory.Path = CookPath;
					BuildConfiguresObject->NeverCookPaths.Add(CookDirectory);
				}

				BuildConfiguresObject->AdditionalPaths.Reset();
				for (auto AdditionPath : BuildConfiguresPtr->AdditionalPaths)
				{
					FDirectoryPath AdditionDirectory;
					AdditionDirectory.Path = AdditionPath;
					BuildConfiguresObject->AdditionalPaths.Add(AdditionDirectory);
				}

				ImportVersion = BuildConfiguresPtr->Version;
				ImportConfigPath = BuildConfiguresPtr->ConfigDir;
				ImportPatchs = BuildConfiguresPtr->Patchs;
				ImportPackageDir = BuildConfiguresPtr->PackageDir;
			}
		}
	}

	return FReply::Handled();
}

FReply SAdvancePakBuildPage::DealwithExportConfigClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
		void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

		TArray<FString> SelectedFiles;

		const bool bFileSelected = DesktopPlatform->SaveFileDialog(
			ParentWindowHandle,
			FString("Save Config Json"),
			ConfigDirEditor->GetText().ToString(),
			*UAdvancePakEditorLibrary::PublishConfigName,
			TEXT("AdvancePak json (*.json)|*.json"),
			EFileDialogFlags::None,
			SelectedFiles
		);

		if (bFileSelected && SelectedFiles.Num() > 0)
		{
			FString JsonContext = UAdvancePakEditorLibrary::AbsorbPublishConfigureToJson(CreateBuildConfiguresPtr());
			FFileHelper::SaveStringToFile(JsonContext, *SelectedFiles[0]);
		}
	}

	return FReply::Handled();
}

FReply SAdvancePakBuildPage::DealwithBuildPublishClicked()
{
	UProjectPackagingSettings* PackagingConfigures = GetMutableDefault<UProjectPackagingSettings>();

	DefaultMapsToCook = PackagingConfigures->MapsToCook;
	DefaultDirectoriesToAlwaysCook = PackagingConfigures->DirectoriesToAlwaysCook;
	DefaultDirectoriesToNeverCook = PackagingConfigures->DirectoriesToNeverCook;
	DefaultDirectoriesToAlwaysStageAsUFS = PackagingConfigures->DirectoriesToAlwaysStageAsUFS;

	PackagingConfigures->MapsToCook.Reset();
	PackagingConfigures->DirectoriesToAlwaysCook.Reset();
	PackagingConfigures->DirectoriesToNeverCook.Reset();
	PackagingConfigures->DirectoriesToAlwaysStageAsUFS.Reset();

	for (auto MapPath : BuildConfiguresObject->MapsToBuild)
	{
		PackagingConfigures->MapsToCook.Add(MapPath);
	}
	for (auto CookPath : BuildConfiguresObject->AlwaysCookPaths)
	{
		PackagingConfigures->DirectoriesToAlwaysCook.Add(CookPath);
	}
	for (auto CookPath : BuildConfiguresObject->NeverCookPaths)
	{
		PackagingConfigures->DirectoriesToNeverCook.Add(CookPath);
	}
	for (auto AdditionPath : BuildConfiguresObject->AdditionalPaths)
	{
		PackagingConfigures->DirectoriesToAlwaysStageAsUFS.Add(AdditionPath);
	}

	PackagingConfigures->SaveConfig();
	PackagingConfigures->UpdateDefaultConfigFile();

	GConfig->SetString(TEXT("AdvancePak"), TEXT("Version"), *BuildConfiguresObject->Version, UAdvancePakLibrary::DefaultGameIniPath);
	GConfig->SetString(TEXT("AdvancePak"), TEXT("Platform"), *BuildPlatformsView->GetSelectPlatform(), UAdvancePakLibrary::DefaultGameIniPath);

	// Save Private Sig
	if (FPaths::FileExists(UAdvancePakLibrary::DefaultCryptoPath))
	{
		FString SigningPrivateExponent;
		GConfig->GetString(TEXT("/Script/CryptoKeys.CryptoKeysSettings"), TEXT("SigningPrivateExponent"), SigningPrivateExponent, UAdvancePakLibrary::DefaultCryptoPath);
		GConfig->SetString(TEXT("AdvancePak"), TEXT("PrivateExponent"), *SigningPrivateExponent, UAdvancePakLibrary::DefaultGameIniPath);
	}

	GConfig->Flush(false, UAdvancePakLibrary::DefaultGameIniPath);

	CreatePakCommand.Empty();

	AdvancePakCreator->IsBuilding = true;

	BuildTaskThread = MakeShareable(new FAdvancePakProcThread(
		TEXT("BuildTask"),
		UAdvancePakEditorLibrary::UnrealRunUAT,
		BuildOptionsEditor->GetText().ToString()
	));

	BuildTaskThread->ProcOutputDelegate.AddRaw(this, &SAdvancePakBuildPage::OnBuildTaskOutput);
	BuildTaskThread->ProcBeginDelegate.AddRaw(this, &SAdvancePakBuildPage::OnBuildTaskBegin);
	BuildTaskThread->ProcSucceedDelegate.AddRaw(this, &SAdvancePakBuildPage::OnBuildTaskSucceed);
	BuildTaskThread->ProcFailedDelegate.AddRaw(this, &SAdvancePakBuildPage::OnBuildTaskFailed);
	BuildTaskThread->Execute();

	return FReply::Handled();
}


void SAdvancePakBuildPage::OnBuildTaskOutput(FString OutputLog)
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

	if (OutputLog.Contains(TEXT("Output from")) && OutputLog.Contains(TEXT("-create")))
	{
		CreatePakCommand = OutputLog;
	}
}

void SAdvancePakBuildPage::OnBuildTaskBegin()
{
	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		UE_LOG(AdvancePakLog, Warning, TEXT("OnBuildTaskBegin"));

		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("Platform"), FText::FromString(BuildPlatformsView->GetSelectPlatform()));
		Arguments.Add(TEXT("TaskName"), OPTEXT(TEXT("BuildTask")));
		FNotificationInfo Info(FText::Format(LOCTEXT("UatTaskInProgressNotification", "{TaskName} for {Platform}..."), Arguments));

		Info.Image = FEditorStyle::GetBrush(TEXT("MainFrame.CookContent"));
		Info.bFireAndForget = false;
		Info.FadeOutDuration = 0.0f;
		Info.ExpireDuration = 0.0f;
		Info.Hyperlink = FSimpleDelegate::CreateRaw(this, &SAdvancePakBuildPage::DealwithUatLinkwork);
		Info.HyperlinkText = OPTEXT("ShowOutputLog");
		Info.ButtonDetails.Add(
			FNotificationButtonInfo(
				OPTEXT("Cancel"),
				OPTEXT("CancelTask"),
				FSimpleDelegate::CreateRaw(this, &SAdvancePakBuildPage::DealwithUatCancelButtonClicked),
				SNotificationItem::CS_Pending
			)
		);
		Info.ButtonDetails.Add(
			FNotificationButtonInfo(
				OPTEXT("Dismiss"),
				FText(),
				FSimpleDelegate::CreateRaw(this, &SAdvancePakBuildPage::DealwithDismissButtonClicked),
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

void SAdvancePakBuildPage::OnBuildTaskSucceed()
{
	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		UE_LOG(AdvancePakLog, Warning, TEXT("Buile Publish Succeed"));
		RestoreDefaultPackageConfigure();

		FString FormerPublishPath = OutputDirEditor->GetText().ToString() / BuildPlatformsView->GetSelectPlatform();
		FString FuturePublishPath = OutputDirEditor->GetText().ToString() / FString::Printf(TEXT("%s_%s"), *BuildPlatformsView->GetSelectPlatform(), *BuildConfiguresObject->Version);

		if (FPaths::DirectoryExists(FuturePublishPath))
		{
			IFileManager::Get().DeleteDirectory(*FuturePublishPath, false, true);
		}

		std::string oldName_Cstr(TCHAR_TO_UTF8(*FormerPublishPath));
		const char* oldstr = oldName_Cstr.c_str();

		std::string newName_Cstr(TCHAR_TO_UTF8(*FuturePublishPath));
		const char* newstr = newName_Cstr.c_str();

		rename(oldstr, newstr);

		CreatePakCommand.RemoveFromStart(TEXT("Output from: "));

		FString PakListPath;
		FParse::Value(*CreatePakCommand, TEXT("-create="), PakListPath);

		FString PakListContext;
		FFileHelper::LoadFileToString(PakListContext, *PakListPath);

		FString SaveConfigPath = ConfigDirEditor->GetText().ToString() / BuildConfiguresObject->Version;

		if (!FPaths::DirectoryExists(SaveConfigPath))
		{
			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
			PlatformFile.CreateDirectory(*SaveConfigPath);
		}

		FFileHelper::SaveStringToFile(PakListContext, *(SaveConfigPath / FString("PublishPakList.txt")));

		FString PrimaryCryptoPath;
		if (FParse::Value(*CreatePakCommand, TEXT("-cryptokeys="), PrimaryCryptoPath))
		{
			FString CryptoContext;
			FFileHelper::LoadFileToString(CryptoContext, *PrimaryCryptoPath);
			FString CryptoPath = SaveConfigPath / FString("Crypto.json");
			FFileHelper::SaveStringToFile(CryptoContext, *CryptoPath);
			CreatePakCommand = CreatePakCommand.Replace(*PrimaryCryptoPath, *CryptoPath);
		}

		TSharedPtr<FAdvancePakPublishConfigure> BuildConfiguresPtr = CreateBuildConfiguresPtr();

		BuildConfiguresPtr->PakCommand = CreatePakCommand;
		BuildConfiguresPtr->PakOptions = AbsorbCommamdToOptions(CreatePakCommand);

		TArray<FString> PakListArray;
		PakListContext.ParseIntoArray(PakListArray, TEXT("\n"));

		bool IsSetLineOptions = false;
		for (auto PakLine : PakListArray)
		{
			int i = 0;
			TSharedPtr<FAdvancePakPakLine> PakLinePtr = MakeShareable(new FAdvancePakPakLine());

			for (const TCHAR* PakLineEnd = *PakLine; *PakLineEnd != 0;)
			{
				FString Argument = FParse::Token(PakLineEnd, false);
				if (Argument.Len() > 0)
				{
					if (i == 0)
					{
						PakLinePtr->FilePath = Argument;
						PakLinePtr->Hash = LexToString(FMD5Hash::HashFile(*Argument));
					}
					else if (i == 1)
					{
						PakLinePtr->MountPath = Argument;
					}
					else
					{
						if (!IsSetLineOptions)
						{
							BuildConfiguresPtr->LineOptions += FString::Printf(TEXT("%s "), *Argument);
						}
					}
				}
				i++;
			}

			if (PakLinePtr->MountPath.EndsWith(UAdvancePakEditorLibrary::UassetSuffixName) || PakLinePtr->MountPath.EndsWith(UAdvancePakEditorLibrary::UexpSuffixName) || PakLinePtr->MountPath.EndsWith(UAdvancePakEditorLibrary::UbulkSuffixName) || PakLinePtr->MountPath.EndsWith(UAdvancePakEditorLibrary::UmapSuffixName) || PakLinePtr->MountPath.EndsWith(UAdvancePakEditorLibrary::UfontSuffixName) || PakLinePtr->MountPath.EndsWith(UAdvancePakEditorLibrary::UtxtSuffixName) || PakLinePtr->MountPath.EndsWith(UAdvancePakEditorLibrary::UtxtmapSuffixName))
			{
				FName PackageName = AbsorbMountPathToPackageName(PakLinePtr->MountPath);
				bool IsUnRegister = true;
				for (auto Regular : BuildConfiguresPtr->Regulars)
				{
					if (Regular.IsEqual(PackageName))
					{
						IsUnRegister = false;
						break;
					}
				}
				if (IsUnRegister)
				{
					BuildConfiguresPtr->Regulars.AddUnique(PackageName);
				}
			}
			else
			{
				TSharedPtr<FAdvancePakInformalInfo> Informal = MakeShareable(new FAdvancePakInformalInfo());
				Informal->InformalName = FName(*PakLinePtr->MountPath.Right(PakLinePtr->MountPath.Len() - 8));
				Informal->InformalPath = PakLinePtr->FilePath;
				BuildConfiguresPtr->Informals.Add(Informal);
			}

			IsSetLineOptions = true;
			BuildConfiguresPtr->PakFileList.Add(PakLinePtr);
		}


		BuildConfiguresPtr->LineOptions.TrimEnd();

		if (BuildConfiguresObject->IsDuplicateDatas && !ImportConfigPath.IsEmpty() && !ImportVersion.IsEmpty() && ImportPatchs.Num() > 0)
		{
			if (!ImportConfigPath.Equals(ConfigDirEditor->GetText().ToString()) || !ImportVersion.Equals(BuildConfiguresObject->Version))
			{
				for (auto Patch : ImportPatchs)
				{
					FString FormerPatchConfigPath = ImportConfigPath / ImportVersion / ImportVersion / FString::Printf(TEXT("%sConfig.json"), *Patch.ToString());
					if (IFileManager::Get().FileExists(*FormerPatchConfigPath))
					{
						FString FuturePatchConfigPath = SaveConfigPath / BuildConfiguresObject->Version / FString::Printf(TEXT("%sConfig.json"), *Patch.ToString());
						IFileManager::Get().Copy(*FuturePatchConfigPath, *FormerPatchConfigPath);
						BuildConfiguresPtr->Patchs.Add(Patch);
					}
				}
			}
			else
			{
				BuildConfiguresPtr->Patchs = ImportPatchs;
			}
		}
		if (BuildConfiguresObject->IsDuplicateDatas && !ImportPackageDir.IsEmpty())
		{
			BuildConfiguresPtr->PackageDir = ImportPackageDir;
		}

		FString JsonContext = UAdvancePakEditorLibrary::AbsorbPublishConfigureToJson(BuildConfiguresPtr);

		FFileHelper::SaveStringToFile(JsonContext, *(SaveConfigPath / UAdvancePakEditorLibrary::PublishConfigName));

		AdvancePakCreator->IsBuilding = false;

		TSharedPtr<SNotificationItem> NotificationItem = NotificationItemPtr.Pin();

		if (NotificationItem.IsValid())
		{
			NotificationItem->SetText(LOCTEXT("BuildSuccessedNotification", "Build Finished!"));
			NotificationItem->SetCompletionState(SNotificationItem::CS_Success);
			NotificationItem->ExpireAndFadeout();

			NotificationItemPtr.Reset();
		}

		BuildTaskThread.Reset();

		GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));

		UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("BuildPublishFloder"), FuturePublishPath);
		UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("ConfigPublishFloder"), SaveConfigPath);

		UE_LOG(AdvancePakLog, Warning, TEXT("Save Publish Succeed"));

	}, TStatId(), NULL, ENamedThreads::GameThread);
}

void SAdvancePakBuildPage::OnBuildTaskFailed()
{
	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		RestoreDefaultPackageConfigure();

		AdvancePakCreator->IsBuilding = false;

		TSharedPtr<SNotificationItem> NotificationItem = NotificationItemPtr.Pin();

		if (NotificationItem.IsValid())
		{
			NotificationItem->SetText(LOCTEXT("BuildFaildNotification", "Build Faild!"));
			NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
			NotificationItem->ExpireAndFadeout();

			NotificationItemPtr.Reset();
		}

		BuildTaskThread.Reset();

		GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));

		UE_LOG(AdvancePakLog, Error, TEXT("Build Publish Failed"));

	}, TStatId(), NULL, ENamedThreads::GameThread);
}

void SAdvancePakBuildPage::RestoreDefaultPackageConfigure()
{
	UProjectPackagingSettings* PackagingConfigures = GetMutableDefault<UProjectPackagingSettings>();

	PackagingConfigures->MapsToCook.Reset();
	PackagingConfigures->DirectoriesToAlwaysCook.Reset();
	PackagingConfigures->DirectoriesToNeverCook.Reset();
	PackagingConfigures->DirectoriesToAlwaysStageAsUFS.Reset();

	for (auto MapPath : DefaultMapsToCook)
	{
		PackagingConfigures->MapsToCook.Add(MapPath);
	}
	for (auto CookPath : DefaultDirectoriesToAlwaysCook)
	{
		PackagingConfigures->DirectoriesToAlwaysCook.Add(CookPath);
	}
	for (auto CookPath : DefaultDirectoriesToNeverCook)
	{
		PackagingConfigures->DirectoriesToNeverCook.Add(CookPath);
	}
	for (auto AdditionPath : DefaultDirectoriesToAlwaysStageAsUFS)
	{
		PackagingConfigures->DirectoriesToAlwaysStageAsUFS.Add(AdditionPath);
	}

	PackagingConfigures->SaveConfig();
	PackagingConfigures->UpdateDefaultConfigFile();
}

TSharedPtr<FAdvancePakPublishConfigure> SAdvancePakBuildPage::CreateBuildConfiguresPtr()
{
	TSharedPtr<FAdvancePakPublishConfigure> BuildConfiguresPtr = MakeShareable(new FAdvancePakPublishConfigure());
	BuildConfiguresPtr->OutputDir = OutputDirEditor->GetText().ToString();
	BuildConfiguresPtr->ConfigDir = ConfigDirEditor->GetText().ToString();
	BuildConfiguresPtr->Version = BuildConfiguresObject->Version;
	BuildConfiguresPtr->Platform = BuildPlatformsView->GetSelectPlatform();
	for (auto MapPath : BuildConfiguresObject->MapsToBuild)
	{
		BuildConfiguresPtr->MapsToBuild.Add(MapPath.FilePath);
	}
	for (auto CookPath : BuildConfiguresObject->AlwaysCookPaths)
	{
		BuildConfiguresPtr->AlwaysCookPaths.Add(CookPath.Path);
	}
	for (auto CookPath : BuildConfiguresObject->NeverCookPaths)
	{
		BuildConfiguresPtr->NeverCookPaths.Add(CookPath.Path);
	}
	for (auto AdditionPath : BuildConfiguresObject->AdditionalPaths)
	{
		BuildConfiguresPtr->AdditionalPaths.Add(AdditionPath.Path);
	}
	return BuildConfiguresPtr;
}

void SAdvancePakBuildPage::DealwithUatLinkwork()
{
	FGlobalTabmanager::Get()->InvokeTab(FName("OutputLog"));
}

void SAdvancePakBuildPage::DealwithUatCancelButtonClicked()
{
	if (BuildTaskThread.IsValid())
	{
		BuildTaskThread->Cancel();
	}
	AdvancePakCreator->IsBuilding = false;
}

void SAdvancePakBuildPage::DealwithDismissButtonClicked()
{
	TSharedPtr<SNotificationItem> NotificationItem = NotificationItemPtr.Pin();

	if (NotificationItem.IsValid())
	{
		NotificationItem->ExpireAndFadeout();
		NotificationItemPtr.Reset();
	}
}


FName SAdvancePakBuildPage::AbsorbMountPathToPackageName(FString MountPath)
{
	if (MountPath.StartsWith(UAdvancePakEditorLibrary::GameContentPath))
	{
		MountPath.RemoveFromStart(UAdvancePakEditorLibrary::GameContentPath);
		return FName(*(UAdvancePakEditorLibrary::GameStarPath / FPaths::GetBaseFilename(MountPath, false)));
	}
	else if (MountPath.StartsWith(UAdvancePakEditorLibrary::GamePluginsPath))
	{
		FString BaseMountPath = FPaths::GetBaseFilename(MountPath, false);
		FString ContentLeft = FString();
		FString ContentRight = FString();
		BaseMountPath.Split(UAdvancePakEditorLibrary::ContentSpacePath, &ContentLeft, &ContentRight);
		return FName(*((FString("/") + (FPaths::GetBaseFilename(ContentLeft) / ContentRight))));

	}
	else if (MountPath.StartsWith(UAdvancePakEditorLibrary::EngineContentPath))
	{
		MountPath.RemoveFromStart(UAdvancePakEditorLibrary::EngineContentPath);
		return FName(*(UAdvancePakEditorLibrary::EngineStartPath / FPaths::GetBaseFilename(MountPath, false)));
	}
	else if (MountPath.StartsWith(UAdvancePakEditorLibrary::EnginePluginsPath))
	{
		FString BaseMountPath = FPaths::GetBaseFilename(MountPath, false);
		FString ContentLeft = FString();
		FString ContentRight = FString();
		BaseMountPath.Split(UAdvancePakEditorLibrary::ContentSpacePath, &ContentLeft, &ContentRight);
		return FName(*((FString("/") + (FPaths::GetBaseFilename(ContentLeft) / ContentRight))));
	}
	else
	{
		UE_LOG(AdvancePakLog, Error, TEXT("Lack ResourceType [%s]"), *MountPath);
	}
	return NAME_None;
}

FString SAdvancePakBuildPage::AbsorbCommamdToOptions(const FString& PakCommamd)
{
	FString ResultCommamd;

	FString CryptokeysValue;
	if (FParse::Value(*PakCommamd, TEXT("-cryptokeys="), CryptokeysValue))
		ResultCommamd += FString::Printf(TEXT("-cryptokeys=\"%s\""), *CryptokeysValue);

	if (FParse::Param(*PakCommamd, TEXT("patchcompatibilitymode421")))
		ResultCommamd += TEXT(" -patchcompatibilitymode421");

	if (FParse::Param(*PakCommamd, TEXT("asynccompression")))
		ResultCommamd += TEXT(" -asynccompression");

	if (FParse::Param(*PakCommamd, TEXT("fallbackOrderForNonUassetFiles")))
		ResultCommamd += TEXT(" -fallbackOrderForNonUassetFiles");

	if (FParse::Param(*PakCommamd, TEXT("sign")))
		ResultCommamd += TEXT(" -sign");

	if (FParse::Param(*PakCommamd, TEXT("encryptindex")))
		ResultCommamd += TEXT(" -encryptindex");

	if (FParse::Param(*PakCommamd, TEXT("AlignFilesLargerThanBlock")))
		ResultCommamd += TEXT(" -AlignFilesLargerThanBlock");

	if (FParse::Param(*PakCommamd, TEXT("multiprocess")))
		ResultCommamd += TEXT(" -multiprocess");

	FString PickuppaddingalignValue;
	if (FParse::Value(*PakCommamd, TEXT("-patchpaddingalign="), PickuppaddingalignValue))
		ResultCommamd += FString::Printf(TEXT(" -patchpaddingalign=%s"), *PickuppaddingalignValue);

	FString AlignForMemoryMappingValue;
	if (FParse::Value(*PakCommamd, TEXT("-AlignForMemoryMapping="), AlignForMemoryMappingValue))
		ResultCommamd += FString::Printf(TEXT(" -AlignForMemoryMapping=%s"), *AlignForMemoryMappingValue);

	FString CompBlockSizeValue;
	if (FParse::Value(*PakCommamd, TEXT("-compressionblocksize="), CompBlockSizeValue))
		ResultCommamd += FString::Printf(TEXT(" -compressionblocksize=%s"), *CompBlockSizeValue);

	FString CompressionformatsValue;
	if (FParse::Value(*PakCommamd, TEXT("-compressionformats="), CompressionformatsValue))
		ResultCommamd += FString::Printf(TEXT(" -compressionformats=%s"), *CompressionformatsValue);

	FString CompressionformatValue;
	if (FParse::Value(*PakCommamd, TEXT("-compressionformat="), CompressionformatValue))
		ResultCommamd += FString::Printf(TEXT(" -compressionformat=%s"), *CompressionformatValue);

	return ResultCommamd;
}

FString SAdvancePakBuildPage::AbsorbBuildConfigToString(TEnumAsByte<EProjectPackagingBuildConfigurations> BuildConfig)
{
	switch (BuildConfig)
	{
	case EProjectPackagingBuildConfigurations::PPBC_Debug:
		return FString("Debug");
	case EProjectPackagingBuildConfigurations::PPBC_DebugGame:
		return FString("DebugGame");
	case EProjectPackagingBuildConfigurations::PPBC_Development:
		return FString("Development");
	case EProjectPackagingBuildConfigurations::PPBC_Test:
		return FString("Test");
	case EProjectPackagingBuildConfigurations::PPBC_Shipping:
		return FString("Shipping");
	}
	return FString("Development");
}	

#undef LOCTEXT_NAMESPACE


