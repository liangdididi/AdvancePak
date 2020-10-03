// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdvancePakPublishOperate.h"
#include "SlateOptMacros.h"
#include "Utility/AdvancePakCreator.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "EditorStyleSet.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SBox.h"
#include "Patcher/SAdvancePakPatchBar.h"
#include "Utility/AdvancePakProcThread.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Docking/TabManager.h"
#include "Misc/SecureHash.h"
#include "AdvancePakType.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Forms/SAdvancePakAccordsForm.h"
#include "Widgets/SBoxPanel.h"


class FAdvancePakCleanVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	TArray<FString> FilePaths;
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{
		if (!bIsDirectory)
		{
			FilePaths.Add(FString(FilenameOrDirectory));
		}
		return true;
	}
};

#define LOCTEXT_NAMESPACE "SAdvancePakPublishOperate"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakPublishOperate::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;
	AdvancePakCreator->OnActiveNameChanged.AddRaw(this, &SAdvancePakPublishOperate::OnConfiguresChanged);
	AdvancePakCreator->OnTotalConfiguresChanged.AddRaw(this, &SAdvancePakPublishOperate::OnConfiguresChanged);

	ChildSlot
		[
			SNew(SBorder)
			.Padding(4.f)
		[
			SNew(SVerticalBox)
			.IsEnabled_Lambda([this]() { return !AdvancePakCreator->IsPackaging && AdvancePakCreator->GetFuturePublish().IsValid(); })

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(PickupOptionsEditor, SMultiLineEditableTextBox)
			.OnTextCommitted(this, &SAdvancePakPublishOperate::OnPickupOptionsCommitted)
		.AutoWrapText(true)
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)

		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.FillWidth(1.0)
		.MaxWidth(400.f)
		[
			SAssignNew(PackageDirEditor, SEditableTextBox)
			.OnTextCommitted(this, &SAdvancePakPublishOperate::OnPackageDirCommitted)
		.HintText(OPTEXT("PleaseSelectPatchDirectory"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Right)
		.Padding(4.0, 0.0, 0.0, 0.0)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("OutputDir"))
		.ToolTipText(OPTEXT("BrowseForTheDirectory"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPublishOperate::DealwithPackageDirButtonClicked)
		]
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SAssignNew(RecursionCheckBox, SCheckBox)
			[
				SNew(STextBlock)
				.Text(OPTEXT("PickupAllAncientVersions"))
			]
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
		.OnClicked(this, &SAdvancePakPublishOperate::DealwithPakListButtonClicked)
		.IsEnabled_Lambda([this]() { return AdvancePakCreator->FormerPublish.IsValid(); })
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("Export"))
		.ToolTipText(OPTEXT("ExportAllPatchConfigure"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPublishOperate::DealwithExportButtonClicked)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("ReplicateConfig"))
		.ToolTipText(OPTEXT("ReplicateConfigureToPublish"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPublishOperate::DealwithReplicateButtonClicked)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("SaveConfig"))
		.ToolTipText(OPTEXT("SaveAllPatchsConfigure"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPublishOperate::DealwithSaveButtonClicked)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("PickupPatch"))
		.ToolTipText(OPTEXT("PickupPublishPatch"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPublishOperate::DealwithPickupPublishButtonClicked)
		.IsEnabled_Lambda([this]() { return AdvancePakCreator->FormerPublish.IsValid(); })
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text(OPTEXT("PickupAll"))
		.ToolTipText(OPTEXT("PickupAllPatch"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPublishOperate::DealwithPickupAllButtonClicked)
		]
		]
		]
		];

}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAdvancePakPublishOperate::SetPatchBox(SAdvancePakPatchBar* InPatchBox)
{
	PatchBox = InPatchBox;
}

void SAdvancePakPublishOperate::OnPackageDirCommitted(const FText& InText, ETextCommit::Type CommitInfo)
{
	AdvancePakCreator->GetFuturePublish()->PackageDir = InText.ToString();
}

FReply SAdvancePakPublishOperate::DealwithPackageDirButtonClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
		void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;
		FString PackagePath;
		const bool bFolderSelected = DesktopPlatform->OpenDirectoryDialog(
			ParentWindowHandle,
			FString("Choose a directory"),
			AdvancePakCreator->GetFuturePublish()->PackageDir,
			PackagePath
		);
		if (bFolderSelected)
		{
			PackagePath = FPaths::ConvertRelativePathToFull(PackagePath);
			PackageDirEditor->SetText(FText::FromString(PackagePath));
			AdvancePakCreator->GetFuturePublish()->PackageDir = PackagePath;
		}
	}
	return FReply::Handled();
}

FReply SAdvancePakPublishOperate::DealwithPakListButtonClicked()
{
	PromotePublishAccords();
	if (AdvancePakCreator->IsPublishAccordFaild)
	{
		UAdvancePakEditorLibrary::CreatePromptNotify(OPSTRING("PublishResourcesLacked"), false);
		return FReply::Handled();
	}
	if (PromotePublishPakList())
	{
		UAdvancePakEditorLibrary::CreatePromptNotify(OPSTRING("NoResourcesToPak"), false);
		return FReply::Handled();
	}
	UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("SavePublishPakListFile"), PakListPath);
	return FReply::Handled();
}

void SAdvancePakPublishOperate::RecursionPickupVersion()
{
	if (RecursionCheckBox->GetCheckedState() != ECheckBoxState::Checked)
	{
		return;
	}
	if (AdvancePakCreator->FormerPublish->LastConfig.IsEmpty())
	{
		return;
	}
	if (!FPaths::FileExists(AdvancePakCreator->FormerPublish->LastConfig))
	{
		UAdvancePakEditorLibrary::CreatePromptNotify(OPSTRING("LastConfigNotExsit"), false);
		return;
	}
	if (!PatchBox->LoadConfigConfigures(AdvancePakCreator->FormerPublish->LastConfig, true))
	{
		UAdvancePakEditorLibrary::CreatePromptNotify(OPSTRING("LoadConfigNotMatch"), false);
		return;
	}
	DealwithPickupAllButtonClicked();
}

void SAdvancePakPublishOperate::PromotePublishAccords()
{
	AdvancePakCreator->PublishAccords.Reset();
	AdvancePakCreator->IsPublishAccordFaild = false;
	AdvancePakCreator->IsPublishReduced = false;
	for (auto PakLine : AdvancePakCreator->FuturePublish->PakFileList)
	{
		if (IFileManager::Get().FileExists(*PakLine->FilePath))
		{
			continue;
		}
		TSharedPtr<FAdvancePakAccordInfo> AccordInfo = MakeShareable(new FAdvancePakAccordInfo());
		AccordInfo->Name = PakLine->MountPath;
		AccordInfo->Path = PakLine->FilePath;
		AccordInfo->AccordType = EAdvancePakAccordType::Delete;
		AccordInfo->IsRegular = false;
		AdvancePakCreator->PublishAccords.Add(AccordInfo);
	}
	if (AdvancePakCreator->PublishAccords.Num() > 0)
	{
		AdvancePakCreator->IsPublishAccordFaild = true;
		return;
	}
	if (AdvancePakCreator->FormerPublish.IsValid())
	{
		TArray<TSharedPtr<FAdvancePakAccordInfo>> AddAccords;
		TArray<TSharedPtr<FAdvancePakAccordInfo>> UpdateAccords;
		TArray<TSharedPtr<FAdvancePakAccordInfo>> KeepAccords;
		TArray<TSharedPtr<FAdvancePakAccordInfo>> DeleteAccords;
		for (auto FuturePakLine : AdvancePakCreator->FuturePublish->PakFileList)
		{
			EAdvancePakAccordType AccordType = EAdvancePakAccordType::Add;
			for (auto FormerPakLine : AdvancePakCreator->FormerPublish->PakFileList)
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
		for (auto FormerPakLine : AdvancePakCreator->FormerPublish->PakFileList)
		{
			bool IsDeleted = true;
			for (auto FuturePakLine : AdvancePakCreator->FuturePublish->PakFileList)
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
		AdvancePakCreator->IsPublishReduced = DeleteAccords.Num() > 0;
		AdvancePakCreator->PublishAccords.Append(AddAccords);
		AdvancePakCreator->PublishAccords.Append(UpdateAccords);
		AdvancePakCreator->PublishAccords.Append(KeepAccords);
		AdvancePakCreator->PublishAccords.Append(DeleteAccords);
	}
	else
	{
		for (auto PakLine : AdvancePakCreator->FuturePublish->PakFileList)
		{
			TSharedPtr<FAdvancePakAccordInfo> AccordInfo = MakeShareable(new FAdvancePakAccordInfo());
			AccordInfo->Name = PakLine->MountPath;
			AccordInfo->Path = PakLine->FilePath;
			AccordInfo->AccordType = EAdvancePakAccordType::Add;
			AdvancePakCreator->PublishAccords.Add(AccordInfo);
		}
	}
}

void SAdvancePakPublishOperate::RenovatePublishAccordsForm()
{
	if (!AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName))
	{
		return;
	}
	if (!AdvancePakCreator->FuturePublish.IsValid())
	{
		AdvancePakCreator->PublishAccords.Reset();
		AdvancePakCreator->PublishAccordsForm->DemandViewRenovate();
		return;
	}
	AdvancePakCreator->PublishAccordsForm->DemandViewRenovate(AdvancePakCreator->IsPublishAccordFaild);
}

void SAdvancePakPublishOperate::PromotePatchAccords(const FName& PatchName /*= NAME_None*/)
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

bool SAdvancePakPublishOperate::PromotePatchPakList(const FName& PatchName /*= NAME_None*/)
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

bool SAdvancePakPublishOperate::PromotePublishPakList()
{
	if (!AdvancePakCreator->FormerPublish.IsValid())
	{
		return false;
	}
	PakListPath = AdvancePakCreator->FuturePublish->ConfigDir / AdvancePakCreator->FuturePublish->Version / AdvancePakCreator->FormerPublish->Version / FString("PublishPakList.txt");
	PakFilePath = (AdvancePakCreator->FuturePublish->PackageDir.IsEmpty() ? AdvancePakCreator->FuturePublish->ConfigDir : AdvancePakCreator->FuturePublish->PackageDir) / AdvancePakCreator->FuturePublish->Version / AdvancePakCreator->FormerPublish->Version / FString("Publish.pak");
	FString PakListContext;
	for (auto AccordInfo : AdvancePakCreator->PublishAccords)
	{
		if (AccordInfo->AccordType == EAdvancePakAccordType::Add || AccordInfo->AccordType == EAdvancePakAccordType::Update)
		{
			PakListContext += FString::Printf(TEXT("\"%s\" \"%s\" %s\n"), *AccordInfo->Path, *AccordInfo->Name, *AdvancePakCreator->FuturePublish->LineOptions);
		}
	}
	if (PakListContext.IsEmpty())
	{
		return false;
	}
	return FFileHelper::SaveStringToFile(PakListContext, *PakListPath);
}

FString SAdvancePakPublishOperate::AbsorbOptionsToCmdLine(const FString& PakCommamd)
{
	FString ResultCommamd;
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

FReply SAdvancePakPublishOperate::DealwithExportButtonClicked()
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
			TSharedPtr<FAdvancePakPublishConfigure> PublishConfigure = AdvancePakCreator->GetFuturePublish();
			FString CacheConfigDir = PublishConfigure->ConfigDir;
			PublishConfigure->ConfigDir = ExportPath;
			FString PublishContext = UAdvancePakEditorLibrary::AbsorbPublishConfigureToJson(PublishConfigure);
			PublishConfigure->ConfigDir = CacheConfigDir;
			FFileHelper::SaveStringToFile(PublishContext, *(ExportPath / PublishConfigure->Version / UAdvancePakEditorLibrary::PublishConfigName));
			for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(AdvancePakCreator->FuturePatchs); It; ++It)
			{
				FString PatchContext = UAdvancePakEditorLibrary::AbsorbPatchConfigureToJson(It.Value());
				FFileHelper::SaveStringToFile(PatchContext, *(ExportPath / PublishConfigure->Version / PublishConfigure->Version / FString::Printf(TEXT("%sConfig.json"), *It.Key().ToString())));
			}
			UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("ReplicatePublishConfig"), *ExportPath);
			if (PublishConfigure->Patchs.Num() > 0)
			{
				UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("ReplicatePatchsConfig"), *(ExportPath / PublishConfigure->Version / PublishConfigure->Version));
			}
		}
	}
	return FReply::Handled();
}

void SAdvancePakPublishOperate::OnConfiguresChanged()
{
	if (AdvancePakCreator->GetFuturePublish().IsValid())
	{
		PickupOptionsEditor->SetText(FText::FromString(AdvancePakCreator->GetFuturePublish()->PakOptions));
		PackageDirEditor->SetText(FText::FromString(AdvancePakCreator->GetFuturePublish()->PackageDir));
	}
	else
	{
		PickupOptionsEditor->SetText(FText());
		PackageDirEditor->SetText(FText());
	}
}

void SAdvancePakPublishOperate::OnPickupOptionsCommitted(const FText& InText, ETextCommit::Type CommitInfo)
{
	AdvancePakCreator->GetFuturePublish()->PakOptions = InText.ToString();
}

FReply SAdvancePakPublishOperate::DealwithReplicateButtonClicked()
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
				PublishConfigure->Patchs.Reset();
				PublishConfigure->Patchs.Append(AdvancePakCreator->GetFuturePublish()->Patchs);
				FString PublishContext = UAdvancePakEditorLibrary::AbsorbPublishConfigureToJson(PublishConfigure);
				FFileHelper::SaveStringToFile(PublishContext, *ConfigPath);
				for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(AdvancePakCreator->FuturePatchs); It; ++It)
				{
					FString PatchContext = UAdvancePakEditorLibrary::AbsorbPatchConfigureToJson(It.Value());
					FFileHelper::SaveStringToFile(PatchContext, *(PublishConfigure->ConfigDir / PublishConfigure->Version / PublishConfigure->Version / FString::Printf(TEXT("%sConfig.json"), *It.Key().ToString())));
				}
				UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("ReplicatePublishConfig"), *ConfigPath);
				if (PublishConfigure->Patchs.Num() > 0)
				{
					UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("ReplicatePatchsConfig"), *(PublishConfigure->ConfigDir / PublishConfigure->Version / PublishConfigure->Version));
				}
			}
		}
	}
	return FReply::Handled();
}

FReply SAdvancePakPublishOperate::DealwithSaveButtonClicked()
{
	FString PublishConfigPath = AdvancePakCreator->GetFuturePublish()->ConfigDir / AdvancePakCreator->GetFuturePublish()->Version / UAdvancePakEditorLibrary::PublishConfigName;
	FString PublishContext = UAdvancePakEditorLibrary::AbsorbPublishConfigureToJson(AdvancePakCreator->GetFuturePublish());
	FFileHelper::SaveStringToFile(PublishContext, *PublishConfigPath);
	UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("SavePublishConfig"), PublishConfigPath);
	for (auto PatchName : AdvancePakCreator->GetFuturePublish()->Patchs)
	{
		PromotePatchAccords(PatchName);
		FString PatchConfigPath = AdvancePakCreator->GetFuturePublish()->ConfigDir / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->GetFuturePublish()->Version / (PatchName.ToString() + TEXT("Config.json"));
		TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = *AdvancePakCreator->FuturePatchs.Find(PatchName);
		FString PatchContext = UAdvancePakEditorLibrary::AbsorbPatchConfigureToJson(PatchConfigure);
		FFileHelper::SaveStringToFile(PatchContext, *PatchConfigPath);
		UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("SavePatchConfig") + PatchName.ToString(), PatchConfigPath);
	}
	return FReply::Handled();
}

FReply SAdvancePakPublishOperate::DealwithPickupPublishButtonClicked()
{
	PromotePublishAccords();
	if (AdvancePakCreator->IsPublishAccordFaild)
	{
		UAdvancePakEditorLibrary::CreatePromptNotify(OPSTRING("PublishResourcesLacked"), false);
		return FReply::Handled();
	}
	if (!PromotePublishPakList())
	{
		UAdvancePakEditorLibrary::CreatePromptNotify(OPSTRING("NoResourcesToPak"), false);
		return FReply::Handled();
	}
	AdvancePakCreator->IsPackaging = true;
	PickupTaskThread = MakeShareable(new FAdvancePakProcThread(
		TEXT("PickupTask"),
		UAdvancePakEditorLibrary::UnrealPakPath,
		PromotePickupOptions()
	));
	PickupTaskThread->ProcOutputDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskOutput);
	PickupTaskThread->ProcBeginDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskBegin);
	PickupTaskThread->ProcSucceedDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupPublishTaskSucceed);
	PickupTaskThread->ProcFailedDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskFailed);
	PickupTaskThread->Execute();
	return FReply::Handled();
}

FReply SAdvancePakPublishOperate::DealwithPickupAllButtonClicked()
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	VersionConfig = MakeShareable(new FAdvancePakVersionConfig());
	VersionConfig->CmdLine = AbsorbOptionsToCmdLine(PickupOptionsEditor->GetText().ToString());
	if (AdvancePakCreator->FormerPublish.IsValid())
	{
		for (auto PatchName : AdvancePakCreator->FormerPublish->Patchs)
		{
			if (!AdvancePakCreator->GetFuturePublish()->Patchs.Contains(PatchName))
			{
				VersionConfig->DeletePatchs.Add(PatchName.ToString());
			}
		}
		FString CleanConfigPath = AdvancePakCreator->GetFuturePublish()->ConfigDir / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->FormerPublish->Version;
		FAdvancePakCleanVisitor CleanConfigVisitor;
		PlatformFile.IterateDirectory(*CleanConfigPath, CleanConfigVisitor);
		for (auto CleanFile : CleanConfigVisitor.FilePaths)
		{
			PlatformFile.DeleteFile(*CleanFile);
		}
		if (!AdvancePakCreator->GetFuturePublish()->PackageDir.IsEmpty())
		{
			FString CleanPackagePath = AdvancePakCreator->GetFuturePublish()->PackageDir / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->FormerPublish->Version;

			FAdvancePakCleanVisitor CleanPackageVisitor;
			PlatformFile.IterateDirectory(*CleanPackagePath, CleanPackageVisitor);
			for (auto CleanFile : CleanPackageVisitor.FilePaths)
				PlatformFile.DeleteFile(*CleanFile);
		}
		AdvancePakCreator->GetFuturePublish()->LastConfig = AdvancePakCreator->FormerPublish->ConfigDir / AdvancePakCreator->FormerPublish->Version / UAdvancePakEditorLibrary::PublishConfigName;
		FString PublishContext = UAdvancePakEditorLibrary::AbsorbPublishConfigureToJson(AdvancePakCreator->GetFuturePublish());
		FFileHelper::SaveStringToFile(PublishContext, *(AdvancePakCreator->GetFuturePublish()->ConfigDir / AdvancePakCreator->GetFuturePublish()->Version / UAdvancePakEditorLibrary::PublishConfigName));
		PickupIndex = -1;
		PromotePublishAccords();
		if (AdvancePakCreator->IsPublishAccordFaild)
		{
			RenovatePublishAccordsForm();
			UAdvancePakEditorLibrary::CreatePromptNotify(OPSTRING("PublishResourcesLacked"), false);
			return FReply::Handled();
		}
		if (!PromotePublishPakList())
		{
			if (AdvancePakCreator->IsPublishReduced)
			{
				TSharedPtr<FAdvancePakPatchInfo> PatchInfo = MakeShareable(new FAdvancePakPatchInfo());
				for (auto AccordInfo : AdvancePakCreator->PublishAccords)
				{
					if (AccordInfo->AccordType == EAdvancePakAccordType::Delete)
					{
						PatchInfo->DeleteResources.Add(AccordInfo->Name);
					}
				}
				VersionConfig->UpdatePatchs.Add(AdvancePakCreator->PublishName.ToString(), PatchInfo);
			}
			else
			{
				VersionConfig->KeepPatchs.Add(AdvancePakCreator->PublishName.ToString());
			}
			while (AdvancePakCreator->GetFuturePublish()->Patchs.Num() > ++PickupIndex)
			{
				FName PickupName = AdvancePakCreator->GetFuturePublish()->Patchs[PickupIndex];
				PromotePatchAccords(PickupName);
				if (AdvancePakCreator->IsPatchAccordFaild)
				{
					AdvancePakCreator->PublishAccords.Reset();
					AdvancePakCreator->PublishAccords.Append(AdvancePakCreator->PatchAccords);
					AdvancePakCreator->IsPublishAccordFaild = true;
					RenovatePublishAccordsForm();
					UAdvancePakEditorLibrary::CreatePromptNotify(PickupName.ToString() + OPSTRING("ResourcesLacked"), false);
					return FReply::Handled();
				}
				if (!PromotePatchPakList(PickupName))
				{
					if (AdvancePakCreator->IsPatchReduced)
					{
						TSharedPtr<FAdvancePakPatchInfo> PatchInfo = MakeShareable(new FAdvancePakPatchInfo());
						for (auto AccordInfo : AdvancePakCreator->PatchAccords)
						{
							if (AccordInfo->AccordType == EAdvancePakAccordType::Delete)
							{
								PatchInfo->DeleteResources.Add(AccordInfo->Name);
							}
						}
						VersionConfig->UpdatePatchs.Add(PickupName.ToString(), PatchInfo);
					}
					else
					{
						VersionConfig->KeepPatchs.Add(PickupName.ToString());
					}
				}
				else
				{
					if (!AdvancePakCreator->FormerPublish->Patchs.Contains(PickupName))
					{
						VersionConfig->AddPatchs.Add(PickupName.ToString());
					}
					else
					{
						TSharedPtr<FAdvancePakPatchInfo> PatchInfo = MakeShareable(new FAdvancePakPatchInfo());
						for (auto AccordInfo : AdvancePakCreator->PatchAccords)
						{
							switch (AccordInfo->AccordType)
							{
							case EAdvancePakAccordType::Add:
								PatchInfo->AddResources.Add(AccordInfo->Name);
								break;
							case EAdvancePakAccordType::Update:
								PatchInfo->UpdateResources.Add(AccordInfo->Name);
								break;
							case EAdvancePakAccordType::Delete:
								PatchInfo->DeleteResources.Add(AccordInfo->Name);
								break;
							}
						}
						VersionConfig->UpdatePatchs.Add(PickupName.ToString(), PatchInfo);
					}
					AdvancePakCreator->IsPackaging = true;
					PickupTaskThread = MakeShareable(new FAdvancePakProcThread(
						TEXT("PickupTask"),
						UAdvancePakEditorLibrary::UnrealPakPath,
						PromotePickupOptions()
					));
					PickupTaskThread->ProcOutputDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskOutput);
					PickupTaskThread->ProcBeginDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskBegin);
					PickupTaskThread->ProcSucceedDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupAllTaskSucceed);
					PickupTaskThread->ProcFailedDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskFailed);
					PickupTaskThread->Execute();
					return FReply::Handled();
				}
			}
			SaveVersionConfig();
			UAdvancePakEditorLibrary::CreatePromptNotify(AdvancePakCreator->FormerPublish->Version + OPSTRING("VersionPickupCompleted"));
			RecursionPickupVersion();
		}
		else
		{
			TSharedPtr<FAdvancePakPatchInfo> PatchInfo = MakeShareable(new FAdvancePakPatchInfo());
			for (auto AccordInfo : AdvancePakCreator->PublishAccords)
			{
				switch (AccordInfo->AccordType)
				{
				case EAdvancePakAccordType::Add:
					PatchInfo->AddResources.Add(AccordInfo->Name);
					break;
				case EAdvancePakAccordType::Update:
					PatchInfo->UpdateResources.Add(AccordInfo->Name);
					break;
				case EAdvancePakAccordType::Delete:
					PatchInfo->DeleteResources.Add(AccordInfo->Name);
					break;
				}
			}
			VersionConfig->UpdatePatchs.Add(AdvancePakCreator->PublishName.ToString(), PatchInfo);
			AdvancePakCreator->IsPackaging = true;
			PickupTaskThread = MakeShareable(new FAdvancePakProcThread(
				TEXT("PickupTask"),
				UAdvancePakEditorLibrary::UnrealPakPath,
				PromotePickupOptions()
			));
			PickupTaskThread->ProcOutputDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskOutput);
			PickupTaskThread->ProcBeginDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskBegin);
			PickupTaskThread->ProcSucceedDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupAllTaskSucceed);
			PickupTaskThread->ProcFailedDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskFailed);
			PickupTaskThread->Execute();
		}
	}
	else
	{
		if (AdvancePakCreator->FuturePatchs.Num() == 0)
		{
			return FReply::Handled();
		}
		for (auto PatchName : AdvancePakCreator->GetFuturePublish()->Patchs)
		{
			VersionConfig->AddPatchs.Add(PatchName.ToString());
		}
		FString CleanConfigPath = AdvancePakCreator->GetFuturePublish()->ConfigDir / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->GetFuturePublish()->Version;
		FAdvancePakCleanVisitor CleanConfigVisitor;
		PlatformFile.IterateDirectory(*CleanConfigPath, CleanConfigVisitor);
		for (auto CleanFile : CleanConfigVisitor.FilePaths)
		{
			PlatformFile.DeleteFile(*CleanFile);
		}
		if (!AdvancePakCreator->GetFuturePublish()->PackageDir.IsEmpty())
		{
			FString CleanPackagePath = AdvancePakCreator->GetFuturePublish()->PackageDir / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->GetFuturePublish()->Version;
			FAdvancePakCleanVisitor CleanPackageVisitor;
			PlatformFile.IterateDirectory(*CleanPackagePath, CleanPackageVisitor);
			for (auto CleanFile : CleanPackageVisitor.FilePaths)
			{
				PlatformFile.DeleteFile(*CleanFile);
			}
		}
		FString PublishContext = UAdvancePakEditorLibrary::AbsorbPublishConfigureToJson(AdvancePakCreator->GetFuturePublish());
		FFileHelper::SaveStringToFile(PublishContext, *(AdvancePakCreator->GetFuturePublish()->ConfigDir / AdvancePakCreator->GetFuturePublish()->Version / UAdvancePakEditorLibrary::PublishConfigName));
		PickupIndex = 0;
		FName PickupName = AdvancePakCreator->GetFuturePublish()->Patchs[PickupIndex];
		PromotePatchAccords(PickupName);
		if (AdvancePakCreator->IsPatchAccordFaild)
		{
			AdvancePakCreator->PublishAccords.Reset();
			AdvancePakCreator->PublishAccords.Append(AdvancePakCreator->PatchAccords);
			AdvancePakCreator->IsPublishAccordFaild = true;
			RenovatePublishAccordsForm();
			UAdvancePakEditorLibrary::CreatePromptNotify(PickupName.ToString() + OPSTRING("ResourcesLacked"), false);
		}
		else
		{
			if (PromotePatchPakList(PickupName))
			{
				AdvancePakCreator->IsPackaging = true;
				FString PatchContext = UAdvancePakEditorLibrary::AbsorbPatchConfigureToJson(AdvancePakCreator->GetFuturePatch(PickupName));
				FFileHelper::SaveStringToFile(PatchContext, *(AdvancePakCreator->GetFuturePublish()->ConfigDir / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->GetFuturePublish()->Version / (PickupName.ToString() + TEXT("Config.json"))));
				PickupTaskThread = MakeShareable(new FAdvancePakProcThread(
					TEXT("PickupTask"),
					UAdvancePakEditorLibrary::UnrealPakPath,
					PromotePickupOptions()
				));
				PickupTaskThread->ProcOutputDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskOutput);
				PickupTaskThread->ProcBeginDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskBegin);
				PickupTaskThread->ProcSucceedDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupAllTaskSucceed);
				PickupTaskThread->ProcFailedDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskFailed);
				PickupTaskThread->Execute();
			}
			else
			{
				UAdvancePakEditorLibrary::CreatePromptNotify(PickupName.ToString() + OPSTRING("NoResourcesToPak"), false);
			}
		}
	}
	return FReply::Handled();
}

void SAdvancePakPublishOperate::OnPickupAllTaskSucceed()
{
	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		PickupTaskThread.Reset();
		TSharedPtr<SNotificationItem> NotificationItem = NotificationItemPtr.Pin();
		if (NotificationItem.IsValid())
		{
			NotificationItem->SetText(OPTEXT("PickupSucceed"));
			NotificationItem->SetCompletionState(SNotificationItem::CS_Success);
			NotificationItem->ExpireAndFadeout();
			NotificationItemPtr.Reset();
		}
		UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("ExportPakFile"), PakFilePath);
		UAdvancePakEditorLibrary::CreateSaveFileNotify(OPSTRING("ExportListFile"), PakListPath);
		GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));
		if (AdvancePakCreator->FormerPublish.IsValid())
		{
			while (AdvancePakCreator->GetFuturePublish()->Patchs.Num() > ++PickupIndex)
			{
				FName PickupName = AdvancePakCreator->GetFuturePublish()->Patchs[PickupIndex];
				PromotePatchAccords(PickupName);
				if (AdvancePakCreator->IsPatchAccordFaild)
				{
					AdvancePakCreator->PublishAccords.Reset();
					AdvancePakCreator->PublishAccords.Append(AdvancePakCreator->PatchAccords);
					AdvancePakCreator->IsPublishAccordFaild = true;
					RenovatePublishAccordsForm();
					AdvancePakCreator->IsPackaging = false;
					UAdvancePakEditorLibrary::CreatePromptNotify(PickupName.ToString() + OPSTRING("ResourcesLacked"), false);
					return;
				}
				if (!PromotePatchPakList(PickupName))
				{
					if (AdvancePakCreator->IsPatchReduced)
					{
						TSharedPtr<FAdvancePakPatchInfo> PatchInfo = MakeShareable(new FAdvancePakPatchInfo());
						for (auto AccordInfo : AdvancePakCreator->PatchAccords)
						{
							if (AccordInfo->AccordType == EAdvancePakAccordType::Delete)
							{
								PatchInfo->DeleteResources.Add(AccordInfo->Name);
							}
						}
						VersionConfig->UpdatePatchs.Add(PickupName.ToString(), PatchInfo);
					}
					else
					{
						VersionConfig->KeepPatchs.Add(PickupName.ToString());
					}
				}
				else
				{
					if (!AdvancePakCreator->FormerPublish->Patchs.Contains(PickupName))
					{
						VersionConfig->AddPatchs.Add(PickupName.ToString());
					}
					else
					{
						TSharedPtr<FAdvancePakPatchInfo> PatchInfo = MakeShareable(new FAdvancePakPatchInfo());
						for (auto AccordInfo : AdvancePakCreator->PatchAccords)
						{
							switch (AccordInfo->AccordType)
							{
							case EAdvancePakAccordType::Add:
								PatchInfo->AddResources.Add(AccordInfo->Name);
								break;
							case EAdvancePakAccordType::Update:
								PatchInfo->UpdateResources.Add(AccordInfo->Name);
								break;
							case EAdvancePakAccordType::Delete:
								PatchInfo->DeleteResources.Add(AccordInfo->Name);
								break;
							}
						}
						VersionConfig->UpdatePatchs.Add(PickupName.ToString(), PatchInfo);
					}
					PickupTaskThread = MakeShareable(new FAdvancePakProcThread(
						TEXT("PickupTask"),
						UAdvancePakEditorLibrary::UnrealPakPath,
						PromotePickupOptions()
					));
					PickupTaskThread->ProcOutputDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskOutput);
					PickupTaskThread->ProcBeginDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskBegin);
					PickupTaskThread->ProcSucceedDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupAllTaskSucceed);
					PickupTaskThread->ProcFailedDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskFailed);
					PickupTaskThread->Execute();
					return;
				}
			}
			AdvancePakCreator->IsPackaging = false;
			SaveVersionConfig();
			UAdvancePakEditorLibrary::CreatePromptNotify(AdvancePakCreator->FormerPublish->Version + OPSTRING("VersionPickupCompleted"));
			RecursionPickupVersion();
		}
		else
		{
			PickupIndex++;
			if (AdvancePakCreator->GetFuturePublish()->Patchs.Num() > PickupIndex)
			{
				FName PickupName = AdvancePakCreator->GetFuturePublish()->Patchs[PickupIndex];
				PromotePatchAccords(PickupName);
				if (AdvancePakCreator->IsPatchAccordFaild)
				{
					AdvancePakCreator->PublishAccords.Reset();
					AdvancePakCreator->PublishAccords.Append(AdvancePakCreator->PatchAccords);
					AdvancePakCreator->IsPublishAccordFaild = true;
					RenovatePublishAccordsForm();
					AdvancePakCreator->IsPackaging = false;
					UAdvancePakEditorLibrary::CreatePromptNotify(PickupName.ToString() + OPSTRING("ResourcesLacked"), false);
				}
				else
				{
					if (PromotePatchPakList(PickupName))
					{
						FString PatchContext = UAdvancePakEditorLibrary::AbsorbPatchConfigureToJson(AdvancePakCreator->GetFuturePatch(PickupName));
						FFileHelper::SaveStringToFile(PatchContext, *(AdvancePakCreator->GetFuturePublish()->ConfigDir / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->GetFuturePublish()->Version / (PickupName.ToString() + TEXT("Config.json"))));
						PickupTaskThread = MakeShareable(new FAdvancePakProcThread(
							TEXT("PickupTask"),
							UAdvancePakEditorLibrary::UnrealPakPath,
							PromotePickupOptions()
						));
						PickupTaskThread->ProcOutputDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskOutput);
						PickupTaskThread->ProcBeginDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskBegin);
						PickupTaskThread->ProcSucceedDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupAllTaskSucceed);
						PickupTaskThread->ProcFailedDelegate.AddRaw(this, &SAdvancePakPublishOperate::OnPickupTaskFailed);
						PickupTaskThread->Execute();
					}
					else
					{
						AdvancePakCreator->IsPackaging = false;
						UAdvancePakEditorLibrary::CreatePromptNotify(PickupName.ToString() + OPSTRING("NoResourcesToPak"), false);
					}
				}
			}
			else
			{
				AdvancePakCreator->IsPackaging = false;
				SaveVersionConfig();
			}
		}

	}, TStatId(), NULL, ENamedThreads::GameThread);
}

void SAdvancePakPublishOperate::OnPickupPublishTaskSucceed()
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

void SAdvancePakPublishOperate::OnPickupTaskFailed()
{
	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		AdvancePakCreator->IsPackaging = false;
		PickupTaskThread.Reset();
		TSharedPtr<SNotificationItem> NotificationItem = NotificationItemPtr.Pin();
		if (NotificationItem.IsValid())
		{
			NotificationItem->SetText(OPTEXT("PickupFailed"));
			NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
			NotificationItem->ExpireAndFadeout();
			NotificationItemPtr.Reset();
		}
		GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));
	}, TStatId(), NULL, ENamedThreads::GameThread);
}

void SAdvancePakPublishOperate::DealwithPickupLinkwork()
{
	FGlobalTabmanager::Get()->InvokeTab(FName("OutputLog"));
}

void SAdvancePakPublishOperate::DealwithPickupCancelButtonClicked()
{
	if (PickupTaskThread.IsValid())
	{
		PickupTaskThread->Cancel();
	}
	AdvancePakCreator->IsPackaging = false;
}

void SAdvancePakPublishOperate::DealwithDismissButtonClicked()
{
	TSharedPtr<SNotificationItem> NotificationItem = NotificationItemPtr.Pin();
	if (NotificationItem.IsValid())
	{
		NotificationItem->ExpireAndFadeout();
		NotificationItemPtr.Reset();
	}
}

void SAdvancePakPublishOperate::OnPickupTaskOutput(FString OutputLog)
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

void SAdvancePakPublishOperate::OnPickupTaskBegin()
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
		Info.Hyperlink = FSimpleDelegate::CreateRaw(this, &SAdvancePakPublishOperate::DealwithPickupLinkwork);
		Info.HyperlinkText = OPTEXT("ShowOutputLog");
		Info.ButtonDetails.Add(
			FNotificationButtonInfo(
				OPTEXT("Cancel"),
				OPTEXT("CancelTask"),
				FSimpleDelegate::CreateRaw(this, &SAdvancePakPublishOperate::DealwithPickupCancelButtonClicked),
				SNotificationItem::CS_Pending
			)
		);
		Info.ButtonDetails.Add(
			FNotificationButtonInfo(
				OPTEXT("Dismiss"),
				FText(),
				FSimpleDelegate::CreateRaw(this, &SAdvancePakPublishOperate::DealwithDismissButtonClicked),
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

FString SAdvancePakPublishOperate::PromotePickupOptions()
{
	return FString::Printf(TEXT("%s -create=\"%s\" %s"), *PakFilePath, *PakListPath, *PickupOptionsEditor->GetText().ToString());
}

void SAdvancePakPublishOperate::SaveVersionConfig()
{
	if (!VersionConfig.IsValid())
	{
		return;
	}
	FString PackagePath = (AdvancePakCreator->GetFuturePublish()->PackageDir.IsEmpty() ? AdvancePakCreator->GetFuturePublish()->ConfigDir : AdvancePakCreator->GetFuturePublish()->PackageDir);
	if (AdvancePakCreator->FormerPublish.IsValid())
	{
		TArray<FString> TargetPatchs;
		VersionConfig->UpdatePatchs.GenerateKeyArray(TargetPatchs);
		TargetPatchs.Append(VersionConfig->AddPatchs);
		for (auto PatchName : TargetPatchs)
		{
			FString PatchPakName = PatchName + UAdvancePakLibrary::PakSuffixName;
			FString PatchPakPath = PackagePath / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->FormerPublish->Version / PatchPakName;
			TSharedPtr<FAdvancePakPatchProp> PatchProp = MakeShareable(new FAdvancePakPatchProp());
			PatchProp->Size = (uint64)UAdvancePakEditorLibrary::PlatformFile->FileSize(*PatchPakPath);
			PatchProp->Hash = LexToString(FMD5Hash::HashFile(*PatchPakPath));
			VersionConfig->PatchProps.Add(PatchPakName, PatchProp);
		}
		FString VersionContext = UAdvancePakLibrary::AbsorbVersionConfigToJson(VersionConfig);
		FFileHelper::SaveStringToFile(VersionContext, *(PackagePath / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->FormerPublish->Version / UAdvancePakLibrary::VersionConfigName));
	}
	else
	{
		for (auto PatchName : VersionConfig->AddPatchs)
		{
			if (AdvancePakCreator->GetFuturePublish()->PakOptions.Contains(TEXT("-sign")))
			{
				FString PatchSigName = PatchName + UAdvancePakLibrary::SigSuffixName;
				FString PatchSigPath = PackagePath / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->GetFuturePublish()->Version / PatchSigName;
				if (UAdvancePakEditorLibrary::PlatformFile->FileExists(*PatchSigPath))
				{
					TSharedPtr<FAdvancePakPatchProp> SigProp = MakeShareable(new FAdvancePakPatchProp());
					SigProp->Size = (uint64)UAdvancePakEditorLibrary::PlatformFile->FileSize(*PatchSigPath);
					SigProp->Hash = LexToString(FMD5Hash::HashFile(*PatchSigPath));
					VersionConfig->PatchProps.Add(PatchSigName, SigProp);
				}
			}
			FString PatchPakName = PatchName + UAdvancePakLibrary::PakSuffixName;
			FString PatchPakPath = PackagePath / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->GetFuturePublish()->Version / PatchPakName;
			TSharedPtr<FAdvancePakPatchProp> PakProp = MakeShareable(new FAdvancePakPatchProp());
			PakProp->Size = (uint64)UAdvancePakEditorLibrary::PlatformFile->FileSize(*PatchPakPath);
			PakProp->Hash = LexToString(FMD5Hash::HashFile(*PatchPakPath));
			VersionConfig->PatchProps.Add(PatchPakName, PakProp);
		}
		FString VersionContext = UAdvancePakLibrary::AbsorbVersionConfigToJson(VersionConfig);
		FFileHelper::SaveStringToFile(VersionContext, *(PackagePath / AdvancePakCreator->GetFuturePublish()->Version / AdvancePakCreator->GetFuturePublish()->Version / UAdvancePakLibrary::VersionConfigName));
	}
	VersionConfig.Reset();
}

#undef LOCTEXT_NAMESPACE