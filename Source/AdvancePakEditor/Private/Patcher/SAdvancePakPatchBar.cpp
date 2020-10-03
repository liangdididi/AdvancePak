// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakPatchBar.h"
#include "SlateOptMacros.h"
#include "Package/SAdvancePakPackageWork.h"
#include "SAdvancePakPatchConfigure.h"
#include "SAdvancePakPublishConfigure.h"
#include "SAdvancePakPatchLump.h"
#include "SAdvancePakPublishOperate.h"
#include "SAdvancePakPatchOperate.h"
#include "Utility/AdvancePakCreator.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Framework/Application/SlateApplication.h"
#include "Forms/SAdvancePakGathersForm.h"
#include "Forms/SAdvancePakInvolveRegularsForm.h"
#include "Forms/SAdvancePakAbsolveRegularsForm.h"
#include "Forms/SAdvancePakInformalsForm.h"
#include "Forms/SAdvancePakAccordsForm.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Forms/SAdvancePakPublishResourcesForm.h"
#include "Forms/SAdvancePakPatchResourcesForm.h"
#include "Utility/AdvancePakStateThread.h"


void SAdvancePakPatchItem::Construct(const FArguments& InArgs, SAdvancePakPatchBar* InPatchBar)
{
	PatchItem = InArgs._PatchItem;
	AdvancePakCreator = InArgs._AdvancePakCreator;
	STableRow<TSharedPtr<FAdvancePakPatchItem> >::ConstructInternal(STableRow::FArguments(), InArgs._OwnerTableView.ToSharedRef());
	PatchBar = InPatchBar;

	this->ChildSlot
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[
			SAssignNew(PatchLump, SAdvancePakPatchLump)
			.PatchItem(PatchItem)
		.AdvancePakCreator(AdvancePakCreator)
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Fill)
		[
			SNew(STextBlock)
			.Text(FText::FromString(PatchItem->Name.IsNone() ? FString("") : PatchItem->Name.ToString()))
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 13))
		.ColorAndOpacity(this, &SAdvancePakPatchItem::DealwithItemColor)
		]
		]
		];

	PatchLump->RenovateOne.BindLambda([this]() {
		PatchBar->RenovateOne.ExecuteIfBound();
	});
	PatchLump->RenovateTwo.BindLambda([this]() {
		PatchBar->RenovateTwo.ExecuteIfBound();
	});
	PatchLump->RenovateFour.BindLambda([this]() {
		PatchBar->RenovateFour.ExecuteIfBound();
	});
	PatchLump->RenovateFive.BindLambda([this]() {
		PatchBar->RenovateFive.ExecuteIfBound();
	});
	PatchLump->PromoteOne.BindLambda([this]() {
		PatchBar->PromoteOne.ExecuteIfBound();
	});
	PatchLump->PromoteTwo.BindLambda([this]() {
		PatchBar->PromoteTwo.ExecuteIfBound();
	});
	PatchLump->PromoteThree.BindLambda([this]() {
		PatchBar->PromoteThree.ExecuteIfBound();
	});
	PatchLump->PromoteFour.BindLambda([this]() {
		PatchBar->PromoteFour.ExecuteIfBound();
	});
	PatchLump->PromoteFive.BindLambda([this]() {
		PatchBar->PromoteFive.ExecuteIfBound();
	});
	PatchLump->PromoteSix.BindLambda([this]() {
		PatchBar->PromoteSix.ExecuteIfBound();
	});
	PatchLump->PromoteSeven.BindLambda([this]() {
		PatchBar->PromoteSeven.ExecuteIfBound();
	});
	PatchLump->PromoteEight.BindLambda([this]() {
		PatchBar->PromoteEight.ExecuteIfBound();
	});
	PatchLump->PromoteNine.BindLambda([this]() {
		PatchBar->PromoteNine.ExecuteIfBound();
	});
	PatchLump->PromoteTen.BindLambda([this]() {
		PatchBar->PromoteTen.ExecuteIfBound();
	});
}


FSlateColor SAdvancePakPatchItem::DealwithItemColor() const
{
	if (PatchItem->FutureType == EAdvancePakPatchType::Patch || PatchItem->FutureType == EAdvancePakPatchType::Publish)
	{
		if (PatchItem->FormerType == EAdvancePakPatchType::None)
		{
			return FLinearColor::Yellow;
		}
		else
		{
			return FLinearColor::Green;
		}
	}
	return FLinearColor::Red;
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakPatchBar::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;
	AdvancePakCreator->OnTotalConfiguresChanged.AddRaw(this, &SAdvancePakPatchBar::OnTotalConfiguresChanged);

	ChildSlot
		[
			SNew(SBorder)
			.Padding(4.f)
		[

			SNew(SHorizontalBox)
			.IsEnabled_Lambda([this]() { return !AdvancePakCreator->IsPackaging; })

		+ SHorizontalBox::Slot()
		.AutoWidth()
		[

			SNew(SBox)
			.WidthOverride(400.f)
		[

			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[

			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(4.f, 4.f, 4.f, 4.f)
		[

			SNew(SBorder)
			.Padding(4.f)
		[

			SNew(SVerticalBox)

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
			SAssignNew(FormerConfigDirEditor, SEditableTextBox)
			.HintText(OPTEXT("PleaseSelelctConfigFile"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Right)
		.Padding(4.0, 0.0, 0.0, 0.0)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text_Lambda([this]() { return AdvancePakCreator->FormerPublish.IsValid() ? OPTEXT("Clear") : OPTEXT("Browse"); })
		.ToolTipText(OPTEXT("BrowseOrClearConfig"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchBar::DealwithFormerButtonClicked)
		.IsEnabled_Raw(this, &SAdvancePakPatchBar::DealwithButtonEnabled)
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SAssignNew(FormerPlatformText, STextBlock)
			.Text(OPTEXT("PlatformNone"))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SAssignNew(FormerVersionText, STextBlock)
			.Text(OPTEXT("VersionNone"))
		]

		]

		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(0.f, 4.f, 4.f, 4.f)
		[

			SNew(SBorder)
			.Padding(4.f)
		[

			SNew(SVerticalBox)

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
			SAssignNew(FutureConfigDirEditor, SEditableTextBox)
			.HintText(OPTEXT("PleaseSelelctConfigFile"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Right)
		.Padding(4.0, 0.0, 0.0, 0.0)
		[
			SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
		.Text_Lambda([this]() { return AdvancePakCreator->GetFuturePublish().IsValid() ? OPTEXT("Clear") : OPTEXT("Browse"); })
		.ToolTipText(OPTEXT("BrowseOrClearConfig"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchBar::DealwithFutureButtonClicked)
		.IsEnabled_Raw(this, &SAdvancePakPatchBar::DealwithButtonEnabled)
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SAssignNew(FuturePlatformText, STextBlock)
			.Text(OPTEXT("PlatformNone"))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SAssignNew(FutureVersionText, STextBlock)
			.Text(OPTEXT("VersionNone"))
		]
		]

		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::FromString("(TvT)                           (TvT)                           (TvT)"))
		]
		]

		]
		]


	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.f, 0.f, 4.f, 0.f)
		[
			SNew(SSeparator)
			.Orientation(Orient_Vertical)
		]


	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			SAssignNew(PatchView, SListView<TSharedPtr<FAdvancePakPatchItem>>)
			.Orientation(Orient_Horizontal)
		.ListItemsSource(&PatchItems)
		.OnGenerateRow(this, &SAdvancePakPatchBar::DealwithPatchViewPromoteRow)
		.OnSelectionChanged(this, &SAdvancePakPatchBar::DealwithPatchViewSelectionChanged)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.f, 0.f, 4.f, 0.f)
		[
			SNew(SSeparator)
			.Orientation(Orient_Vertical)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(400.f)
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(PublishOperate, SAdvancePakPublishOperate, InModel)
			.Visibility_Lambda([this]()->EVisibility { return  AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName) ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed; })
		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(PatchOperate, SAdvancePakPatchOperate, InModel)
			.Visibility_Lambda([this]()->EVisibility { return  AdvancePakCreator->IsActivedPatch() ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed; })
		]

		]
		]
		]
		];

	PublishOperate->SetPatchBox(this);
}

bool SAdvancePakPatchBar::DealwithButtonEnabled() const
{
	return !AdvancePakCreator->IsPackaging
		&& !AdvancePakCreator->IsPublishFormerRegularsStateRunning
		&& !AdvancePakCreator->IsPublishFormerInformalsStateRunning
		&& !AdvancePakCreator->IsPatchFormerPublishRegularsStateRunning
		&& !AdvancePakCreator->IsPatchFormerPublishInformalsStateRunning
		&& !AdvancePakCreator->IsPatchFormerPatchRegularsStateRunning
		&& !AdvancePakCreator->IsPatchFormerPatchInformalsStateRunning;
}

FReply SAdvancePakPatchBar::DealwithFormerButtonClicked()
{
	if (AdvancePakCreator->FormerPublish.IsValid())
	{
		if (AdvancePakCreator->FormerPublish.IsValid())
			AdvancePakCreator->FormerPublish.Reset();

		AdvancePakCreator->FormerPatchs.Reset();

		AdvancePakCreator->IsFormerPublishDirty = true;

		AdvancePakCreator->IsFormerPatchDirty = true;

		AdvancePakCreator->OnTotalConfiguresChanged.Broadcast();

		if (AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName))
		{
			RenovatePublishFormerViews();
		}
		else if (AdvancePakCreator->IsActivedPatch())
		{
			RenovatePatchFormerViews();
		}

		FormerConfigDirEditor->SetText(FText());
		FormerVersionText->SetText(OPTEXT("VersionNone"));
		FormerPlatformText->SetText(OPTEXT("PlatformNone"));
	}
	else
	{
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

		if (DesktopPlatform)
		{
			TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
			void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

			TArray<FString> SelectedFiles;

			const bool bFileSelected = DesktopPlatform->OpenFileDialog(
				ParentWindowHandle,
				FString("Choose Publish Config Json"),
				TEXT(""),
				TEXT(""),
				TEXT("HotCreator json (*.json)|*.json"),
				EFileDialogFlags::None,
				SelectedFiles
			);

			if (bFileSelected && SelectedFiles.Num() > 0)
			{
				if (FPaths::GetCleanFilename(SelectedFiles[0]).Equals(UAdvancePakEditorLibrary::PublishConfigName))
				{
					LoadConfigConfigures(FPaths::ConvertRelativePathToFull(SelectedFiles[0]), true);
				}
			}
		}
	}
	return FReply::Handled();
}

FReply SAdvancePakPatchBar::DealwithFutureButtonClicked()
{
	if (AdvancePakCreator->GetFuturePublish().IsValid())
	{
		if (AdvancePakCreator->FuturePublish.IsValid())
			AdvancePakCreator->FuturePublish.Reset();

		AdvancePakCreator->FuturePatchs.Reset();

		AdvancePakCreator->IsFuturePublishDirty = true;

		AdvancePakCreator->OnTotalConfiguresChanged.Broadcast();

		if (AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName))
		{
			RenovatePublishFormerViews();
			RenovatePublishFutureViews();
		}
		else if (AdvancePakCreator->IsActivedPatch())
		{
			RenovatePatchFutureViews();
			RenovatePatchFormerViews();
		}

		FutureConfigDirEditor->SetText(FText());
		FutureVersionText->SetText(OPTEXT("VersionNone"));
		FuturePlatformText->SetText(OPTEXT("PlatformNone"));
	}
	else
	{
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

		if (DesktopPlatform)
		{
			TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
			void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

			TArray<FString> SelectedFiles;

			const bool bFileSelected = DesktopPlatform->OpenFileDialog(
				ParentWindowHandle,
				FString("Choose Publish Config Json"),
				TEXT(""),
				TEXT(""),
				TEXT("HotCreator json (*.json)|*.json"),
				EFileDialogFlags::None,
				SelectedFiles
			);

			if (bFileSelected && SelectedFiles.Num() > 0)
			{
				if (FPaths::GetCleanFilename(SelectedFiles[0]).Equals(UAdvancePakEditorLibrary::PublishConfigName))
				{
					LoadConfigConfigures(FPaths::ConvertRelativePathToFull(SelectedFiles[0]), false);
				}
			}
		}
	}
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


bool SAdvancePakPatchBar::LoadConfigConfigures(FString ConfigPath, bool IsFormerConfig)
{
	FString PublishContext;
	FFileHelper::LoadFileToString(PublishContext, *ConfigPath);
	TSharedPtr<FAdvancePakPublishConfigure> PublishConfigure = UAdvancePakEditorLibrary::AbsorbJsonToPublishConfigure(PublishContext);
	TArray<FName> AbandonPatchs;
	TArray<TSharedPtr<FAdvancePakPatchConfigure>> PatchConfigures;
	for (auto PatchName : PublishConfigure->Patchs)
	{
		FString PatchConfigPath = FPaths::GetPath(ConfigPath) / PublishConfigure->Version / (PatchName.ToString() + TEXT("Config.json"));
		if (!FPaths::FileExists(PatchConfigPath))
		{
			AbandonPatchs.Add(PatchName);
			continue;
		}
		FString PatchContent;
		FFileHelper::LoadFileToString(PatchContent, *PatchConfigPath);
		TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = UAdvancePakEditorLibrary::AbsorbJsonToPatchConfigure(PatchContent);
		PatchConfigures.Add(PatchConfigure);
	}
	for (auto PatchName : AbandonPatchs)
	{
		PublishConfigure->Patchs.Remove(PatchName);
	}
	if (IsFormerConfig)
	{
		if (AdvancePakCreator->GetFuturePublish().IsValid() && (!AdvancePakCreator->GetFuturePublish()->Platform.Equals(PublishConfigure->Platform) || AdvancePakCreator->GetFuturePublish()->Version.Equals(PublishConfigure->Version)))
		{
			return false;
		}
		if (AdvancePakCreator->FormerPublish.IsValid())
		{
			AdvancePakCreator->FormerPublish.Reset();
		}
		AdvancePakCreator->FormerPublish = PublishConfigure;
		AdvancePakCreator->FormerPatchs.Reset();
		for (auto PatchConfigure : PatchConfigures)
		{
			AdvancePakCreator->FormerPatchs.Add(PatchConfigure->Name, PatchConfigure);
		}
		AdvancePakCreator->IsFormerPublishDirty = true;
		AdvancePakCreator->IsFormerPatchDirty = true;
		ResetCookedPath();
		AdvancePakCreator->OnTotalConfiguresChanged.Broadcast();
		if (AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName))
		{
			RenovatePublishFormerViews();
		}
		else if (AdvancePakCreator->IsActivedPatch())
		{
			RenovatePatchFormerViews();
		}
		FormerConfigDirEditor->SetText(FText::FromString(ConfigPath));
		FormerVersionText->SetText(FText::FromString(FString::Printf(TEXT("%s : %s"), *OPSTRING("Version"), *PublishConfigure->Version)));
		FormerPlatformText->SetText(FText::FromString(FString::Printf(TEXT("%s : %s"), *OPSTRING("Platform"), *PublishConfigure->Platform)));
	}
	else
	{
		if (AdvancePakCreator->FormerPublish.IsValid() && (!AdvancePakCreator->FormerPublish->Platform.Equals(PublishConfigure->Platform) || AdvancePakCreator->FormerPublish->Version.Equals(PublishConfigure->Version)))
		{
			return false;
		}
		if (AdvancePakCreator->FuturePublish.IsValid())
		{
			AdvancePakCreator->FuturePublish.Reset();
		}
		AdvancePakCreator->FuturePublish = PublishConfigure;
		AdvancePakCreator->FuturePatchs.Reset();
		for (auto PatchConfigure : PatchConfigures)
		{
			AdvancePakCreator->FuturePatchs.Add(PatchConfigure->Name, PatchConfigure);
		}
		AdvancePakCreator->IsFuturePublishDirty = true;
		ResetCookedPath();
		AdvancePakCreator->OnTotalConfiguresChanged.Broadcast();
		if (AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName))
		{
			RenovatePublishFormerViews();
			RenovatePublishFutureViews();
		}
		else if (AdvancePakCreator->IsActivedPatch())
		{
			RenovatePatchFutureViews();
			RenovatePatchFormerViews();
		}
		FutureConfigDirEditor->SetText(FText::FromString(ConfigPath));
		FutureVersionText->SetText(FText::FromString(FString::Printf(TEXT("%s : %s"), *OPSTRING("Version"), *PublishConfigure->Version)));
		FuturePlatformText->SetText(FText::FromString(FString::Printf(TEXT("%s : %s"), *OPSTRING("Platform"), *PublishConfigure->Platform)));
	}
	return true;
}

void SAdvancePakPatchBar::OnTotalConfiguresChanged()
{
	TSharedPtr<FAdvancePakPublishConfigure> FormerPublish = AdvancePakCreator->FormerPublish;
	TSharedPtr<FAdvancePakPublishConfigure> FuturePublish = AdvancePakCreator->GetFuturePublish();
	PatchItems.Reset();
	if (FormerPublish.IsValid())
	{
		if (FuturePublish.IsValid())
		{
			bool IsSelected = false;
			TSharedPtr<FAdvancePakPatchItem> PublishItem = MakeShareable(new FAdvancePakPatchItem());
			PublishItem->Name = AdvancePakCreator->PublishName;
			PublishItem->FormerType = EAdvancePakPatchType::Publish;
			PublishItem->FutureType = EAdvancePakPatchType::Publish;
			if (AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName))
			{
				PublishItem->IsSelected = true;
				IsSelected = true;
			}
			PatchItems.Add(PublishItem);
			for (auto Patch : FormerPublish->Patchs)
			{
				TSharedPtr<FAdvancePakPatchItem> PatchItem = MakeShareable(new FAdvancePakPatchItem());
				PatchItem->Name = Patch;
				PatchItem->FormerType = EAdvancePakPatchType::Patch;
				if (FuturePublish->Patchs.Contains(Patch))
				{
					PatchItem->FutureType = EAdvancePakPatchType::Patch;
				}
				else
				{
					PatchItem->FutureType = EAdvancePakPatchType::Copy;
				}
				if (!IsSelected && AdvancePakCreator->ActiveName.IsEqual(Patch))
				{
					PatchItem->IsSelected = true;
					IsSelected = true;
				}
				PatchItems.Add(PatchItem);
			}

			for (auto Patch : FuturePublish->Patchs)
			{
				if (FormerPublish->Patchs.Contains(Patch))
				{
					continue;
				}
				TSharedPtr<FAdvancePakPatchItem> PatchItem = MakeShareable(new FAdvancePakPatchItem());
				PatchItem->Name = Patch;
				PatchItem->FormerType = EAdvancePakPatchType::None;
				PatchItem->FutureType = EAdvancePakPatchType::Patch;
				if (!IsSelected && AdvancePakCreator->ActiveName.IsEqual(Patch))
				{
					PatchItem->IsSelected = true;
					IsSelected = true;
				}
				PatchItems.Add(PatchItem);
			}
			TSharedPtr<FAdvancePakPatchItem> AddItem = MakeShareable(new FAdvancePakPatchItem());
			AddItem->FormerType = EAdvancePakPatchType::None;
			AddItem->FutureType = EAdvancePakPatchType::Add;
			PatchItems.Add(AddItem);
			if (!IsSelected)
			{
				PatchItems[0]->IsSelected = true;
				ResetActiveName(AdvancePakCreator->PublishName);
			}
		}
		else
		{
			bool IsSelected = false;
			TSharedPtr<FAdvancePakPatchItem> PublishItem = MakeShareable(new FAdvancePakPatchItem());
			PublishItem->Name = AdvancePakCreator->PublishName;
			PublishItem->FormerType = EAdvancePakPatchType::Publish;
			PublishItem->FutureType = EAdvancePakPatchType::None;
			if (AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName))
			{
				PublishItem->IsSelected = true;
				IsSelected = true;
			}
			PatchItems.Add(PublishItem);
			for (auto Patch : FormerPublish->Patchs)
			{
				TSharedPtr<FAdvancePakPatchItem> PatchItem = MakeShareable(new FAdvancePakPatchItem());
				PatchItem->Name = Patch;
				PatchItem->FormerType = EAdvancePakPatchType::Patch;
				PatchItem->FutureType = EAdvancePakPatchType::None;
				if (!IsSelected && AdvancePakCreator->ActiveName.IsEqual(Patch))
				{
					PatchItem->IsSelected = true;
					IsSelected = true;
				}
				PatchItems.Add(PatchItem);
			}
			if (!IsSelected)
			{
				PatchItems[0]->IsSelected = true;
				ResetActiveName(AdvancePakCreator->PublishName);
			}
		}
	}
	else
	{
		if (FuturePublish.IsValid())
		{
			bool IsSelected = false;
			TSharedPtr<FAdvancePakPatchItem> PublishItem = MakeShareable(new FAdvancePakPatchItem());
			PublishItem->Name = AdvancePakCreator->PublishName;
			PublishItem->FormerType = EAdvancePakPatchType::None;
			PublishItem->FutureType = EAdvancePakPatchType::Publish;
			if (AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName))
			{
				PublishItem->IsSelected = true;
				IsSelected = true;
			}
			PatchItems.Add(PublishItem);
			for (auto Patch : FuturePublish->Patchs)
			{
				TSharedPtr<FAdvancePakPatchItem> PatchItem = MakeShareable(new FAdvancePakPatchItem());
				PatchItem->Name = Patch;
				PatchItem->FormerType = EAdvancePakPatchType::None;
				PatchItem->FutureType = EAdvancePakPatchType::Patch;
				if (!IsSelected && AdvancePakCreator->ActiveName.IsEqual(Patch))
				{
					PatchItem->IsSelected = true;
					IsSelected = true;
				}
				PatchItems.Add(PatchItem);
			}
			TSharedPtr<FAdvancePakPatchItem> AddItem = MakeShareable(new FAdvancePakPatchItem());
			AddItem->FormerType = EAdvancePakPatchType::None;
			AddItem->FutureType = EAdvancePakPatchType::Add;
			PatchItems.Add(AddItem);
			if (!IsSelected)
			{
				PatchItems[0]->IsSelected = true;
				ResetActiveName(AdvancePakCreator->PublishName);
			}
		}
		else
		{
			ResetActiveName();
		}
	}
	PatchView->RequestListRefresh();
}

TSharedRef<ITableRow> SAdvancePakPatchBar::DealwithPatchViewPromoteRow(TSharedPtr<FAdvancePakPatchItem> PatchItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SAdvancePakPatchItem, this)
		.OwnerTableView(OwnerTable)
		.PatchItem(PatchItem)
		.AdvancePakCreator(AdvancePakCreator)
		;
}

void SAdvancePakPatchBar::DealwithPatchViewSelectionChanged(TSharedPtr<FAdvancePakPatchItem> PatchItem, ESelectInfo::Type SelectInfo)
{
	if (!PatchItem.IsValid())
	{
		return;
	}
	if (PatchItem->FutureType == EAdvancePakPatchType::Add)
	{
		return;
	}
	for (auto Patch : PatchItems)
	{
		if (Patch->IsSelected && Patch != PatchItem)
		{
			Patch->IsSelected = false;
			break;
		}
	}
	PatchItem->IsSelected = true;
	ResetActiveName(PatchItem->Name);
}

void SAdvancePakPatchBar::ResetActiveName(const FName& NewName)
{
	if (AdvancePakCreator->ActiveName.IsEqual(NewName))
	{
		return;
	}
	AdvancePakCreator->ActiveName = NewName;
	if (AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName))
	{
		RenovatePublishFormerViews();
		RenovatePublishFutureViews();
	}
	else if (AdvancePakCreator->IsActivedPatch())
	{
		RenovatePatchFutureViews();
		RenovatePatchFormerViews();
	}
	AdvancePakCreator->OnActiveNameChanged.Broadcast();
}

void SAdvancePakPatchBar::ResetCookedPath()
{
	if (AdvancePakCreator->FormerPublish.IsValid())
	{
		UAdvancePakEditorLibrary::CookedPlatformPath = UAdvancePakEditorLibrary::CookedPath / AdvancePakCreator->FormerPublish->Platform;
		UAdvancePakEditorLibrary::CookedEnginePath = UAdvancePakEditorLibrary::CookedPlatformPath / FString("Engine");
		UAdvancePakEditorLibrary::CookedEngineContentPath = UAdvancePakEditorLibrary::CookedEnginePath / FString("Content");
		UAdvancePakEditorLibrary::CookedEnginePluginsPath = UAdvancePakEditorLibrary::CookedEnginePath / FString("Plugins");
		UAdvancePakEditorLibrary::CookedProjectPath = UAdvancePakEditorLibrary::CookedPlatformPath / FApp::GetProjectName();
		UAdvancePakEditorLibrary::CookedGameContentPath = UAdvancePakEditorLibrary::CookedProjectPath / FString("Content");
		UAdvancePakEditorLibrary::CookedGamePluginsPath = UAdvancePakEditorLibrary::CookedProjectPath / FString("Plugins");
		return;
	}
	if (AdvancePakCreator->FuturePublish.IsValid())
	{
		UAdvancePakEditorLibrary::CookedPlatformPath = UAdvancePakEditorLibrary::CookedPath / AdvancePakCreator->FuturePublish->Platform;
		UAdvancePakEditorLibrary::CookedEnginePath = UAdvancePakEditorLibrary::CookedPlatformPath / FString("Engine");
		UAdvancePakEditorLibrary::CookedEngineContentPath = UAdvancePakEditorLibrary::CookedEnginePath / FString("Content");
		UAdvancePakEditorLibrary::CookedEnginePluginsPath = UAdvancePakEditorLibrary::CookedEnginePath / FString("Plugins");
		UAdvancePakEditorLibrary::CookedProjectPath = UAdvancePakEditorLibrary::CookedPlatformPath / FApp::GetProjectName();
		UAdvancePakEditorLibrary::CookedGameContentPath = UAdvancePakEditorLibrary::CookedProjectPath / FString("Content");
		UAdvancePakEditorLibrary::CookedGamePluginsPath = UAdvancePakEditorLibrary::CookedProjectPath / FString("Plugins");
		return;
	}
}

void SAdvancePakPatchBar::RenovatePatchFutureViews()
{
	if (!AdvancePakCreator->IsActivedPatch())
	{
		return;
	}
	if (AdvancePakCreator->FuturePatchs.Contains(AdvancePakCreator->ActiveName))
	{
		PromoteOne.ExecuteIfBound();
		PromoteNine.ExecuteIfBound();
		PromoteFive.ExecuteIfBound();
		PromoteSeven.ExecuteIfBound();
		PromoteTwo.ExecuteIfBound();
		PromoteThree.ExecuteIfBound();
		PromoteFour.ExecuteIfBound();
		PromoteSix.ExecuteIfBound();
		PromoteTen.ExecuteIfBound();
		PromoteEight.ExecuteIfBound();
	}
	else
	{
		AdvancePakCreator->InvolveGathersRegulars.Reset();
		AdvancePakCreator->AbsolveGathersRegulars.Reset();
		AdvancePakCreator->InvolveGathersInformals.Reset();
		AdvancePakCreator->AbsolveGathersInformals.Reset();
		AdvancePakCreator->InvolveRegularsRegulars.Reset();
		AdvancePakCreator->AbsolveRegularsRegulars.Reset();
		AdvancePakCreator->InvolveInformalsInformals.Reset();
		AdvancePakCreator->AbsolveInformalsInformals.Reset();
		AdvancePakCreator->PatchFutureRegulars.Reset();
		AdvancePakCreator->PatchFutureInformals.Reset();
	}

	if (!AdvancePakCreator->FuturePublish.IsValid() || !AdvancePakCreator->FuturePatchs.Contains(AdvancePakCreator->ActiveName))
	{
		AdvancePakCreator->InvolveGathers.Reset();
		AdvancePakCreator->InvolveGathersForm->DemandViewRenovate();

		AdvancePakCreator->AbsolveGathers.Reset();
		AdvancePakCreator->AbsolveGathersForm->DemandViewRenovate();

		AdvancePakCreator->InvolveRegulars.Reset();
		AdvancePakCreator->InvolveRegularsForm->DemandViewRenovate();

		AdvancePakCreator->AbsolveRegulars.Reset();
		AdvancePakCreator->AbsolveRegularsForm->DemandViewRenovate();

		AdvancePakCreator->InvolveInformals.Reset();
		AdvancePakCreator->InvolveInformalsForm->DemandViewRenovate();

		AdvancePakCreator->AbsolveInformals.Reset();
		AdvancePakCreator->AbsolveInformalsForm->DemandViewRenovate();
	}
	else
	{
		AdvancePakCreator->InvolveGathers.Reset();
		AdvancePakCreator->InvolveGathers.Append(AdvancePakCreator->GetFuturePatch()->InvolveGathers);
		AdvancePakCreator->InvolveGathersForm->DemandViewRenovate();

		AdvancePakCreator->AbsolveGathers.Reset();
		AdvancePakCreator->AbsolveGathers.Append(AdvancePakCreator->GetFuturePatch()->AbsolveGathers);
		AdvancePakCreator->AbsolveGathersForm->DemandViewRenovate();

		AdvancePakCreator->InvolveRegulars.Reset();
		AdvancePakCreator->InvolveRegulars.Append(AdvancePakCreator->GetFuturePatch()->InvolveRegulars);
		AdvancePakCreator->InvolveRegularsForm->DemandViewRenovate();

		AdvancePakCreator->AbsolveRegulars.Reset();
		for (auto PackageName : AdvancePakCreator->GetFuturePatch()->AbsolveRegulars)
		{
			AdvancePakCreator->AbsolveRegulars.Add(MakeShareable(new FName(PackageName)));
		}
		AdvancePakCreator->AbsolveRegularsForm->DemandViewRenovate();

		AdvancePakCreator->InvolveInformals.Reset();
		AdvancePakCreator->InvolveInformals.Append(AdvancePakCreator->GetFuturePatch()->InvolveInformals);
		AdvancePakCreator->InvolveInformalsForm->DemandViewRenovate();

		AdvancePakCreator->AbsolveInformals.Reset();
		AdvancePakCreator->AbsolveInformals.Append(AdvancePakCreator->GetFuturePatch()->AbsolveInformals);
		AdvancePakCreator->AbsolveInformalsForm->DemandViewRenovate();
	}

	RenovateFour.ExecuteIfBound();
	RenovateFive.ExecuteIfBound();

	AdvancePakCreator->PatchAccords.Reset();
	AdvancePakCreator->PatchAccordsForm->DemandViewRenovate();
}

void SAdvancePakPatchBar::RenovatePublishFutureViews()
{
	if (!AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName))
		return;

	if (AdvancePakCreator->IsFuturePublishDirty)
	{
		if (AdvancePakCreator->FuturePublish.IsValid())
		{
			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> AddResources;
			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> KeepResources;

			for (auto Regular : AdvancePakCreator->FuturePublish->Regulars)
			{
				TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPublishResourceInfo());
				ResourceInfo->ResourceName = Regular;
				ResourceInfo->PatchName = AdvancePakCreator->PublishName;

				if (AdvancePakCreator->CacheFormerRegulars.Contains(Regular))
				{
					ResourceInfo->ResourceState = EAdvancePakResourceState::Allot;
					KeepResources.Add(ResourceInfo);
				}
				else
				{
					ResourceInfo->ResourceState = EAdvancePakResourceState::Free;
					AddResources.Add(ResourceInfo);
				}
			}

			for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(AdvancePakCreator->FuturePatchs); It; ++It)
			{
				for (auto Regular : It.Value()->Regulars)
				{
					TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPublishResourceInfo());
					ResourceInfo->ResourceName = Regular;
					ResourceInfo->PatchName = It.Key();
					if (AdvancePakCreator->CacheFormerRegulars.Contains(Regular))
					{
						ResourceInfo->ResourceState = EAdvancePakResourceState::Allot;
						KeepResources.Add(ResourceInfo);
					}
					else
					{
						ResourceInfo->ResourceState = EAdvancePakResourceState::Free;
						AddResources.Add(ResourceInfo);
					}
				}
			}

			AdvancePakCreator->PublishFutureRegulars.Reset();
			AdvancePakCreator->PublishFutureRegulars.Append(AddResources);
			AdvancePakCreator->PublishFutureRegulars.Append(KeepResources);
			AdvancePakCreator->FutureRegularsForm->DemandViewRenovate();

			AddResources.Reset();
			KeepResources.Reset();
			for (auto Informal : AdvancePakCreator->FuturePublish->Informals)
			{
				TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPublishResourceInfo());
				ResourceInfo->ResourceName = Informal->InformalName;
				ResourceInfo->ResourcePath = Informal->InformalPath;
				ResourceInfo->PatchName = AdvancePakCreator->PublishName;
				if (AdvancePakCreator->CacheFormerInformals.Contains(Informal->InformalName))
				{
					ResourceInfo->ResourceState = EAdvancePakResourceState::Allot;
					KeepResources.Add(ResourceInfo);
				}
				else
				{
					ResourceInfo->ResourceState = EAdvancePakResourceState::Free;
					AddResources.Add(ResourceInfo);
				}
			}

			for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(AdvancePakCreator->FuturePatchs); It; ++It)
			{
				for (auto Informal : It.Value()->Informals)
				{
					TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPublishResourceInfo());
					ResourceInfo->ResourceName = Informal->InformalName;
					ResourceInfo->ResourcePath = Informal->InformalPath;
					ResourceInfo->PatchName = It.Key();
					if (AdvancePakCreator->CacheFormerInformals.Contains(Informal->InformalName))
					{
						ResourceInfo->ResourceState = EAdvancePakResourceState::Allot;
						KeepResources.Add(ResourceInfo);
					}
					else
					{
						ResourceInfo->ResourceState = EAdvancePakResourceState::Free;
						AddResources.Add(ResourceInfo);
					}
				}
			}

			AdvancePakCreator->PublishFutureInformals.Reset();
			AdvancePakCreator->PublishFutureInformals.Append(AddResources);
			AdvancePakCreator->PublishFutureInformals.Append(KeepResources);
			AdvancePakCreator->FutureInformalsForm->DemandViewRenovate();
		}
		else
		{
			AdvancePakCreator->PublishFutureRegulars.Reset();
			AdvancePakCreator->FutureRegularsForm->DemandViewRenovate();

			AdvancePakCreator->PublishFutureInformals.Reset();
			AdvancePakCreator->FutureInformalsForm->DemandViewRenovate();
		}

		AdvancePakCreator->IsFuturePublishDirty = false;
	}
	AdvancePakCreator->PublishAccords.Reset();
	AdvancePakCreator->PublishAccordsForm->DemandViewRenovate();
}

void SAdvancePakPatchBar::RenovatePatchFormerViews()
{
	if (!AdvancePakCreator->IsActivedPatch())
		return;

	if (AdvancePakCreator->IsFormerPatchDirty)
	{
		if (AdvancePakCreator->FormerPublish.IsValid())
		{
			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> FreeRegularInfos;
			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> AllotRegularInfos;
			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> InValidRegularInfos;
			for (auto Regular : AdvancePakCreator->FormerPublish->Regulars)
			{
				TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPublishResourceInfo());
				ResourceInfo->ResourceName = Regular;
				ResourceInfo->PatchName = AdvancePakCreator->PublishName;
				ResourceInfo->ResourceState = AdvancePakCreator->GetRegularResourceState(Regular);

				switch (ResourceInfo->ResourceState)
				{
				case EAdvancePakResourceState::Free:
					FreeRegularInfos.Add(ResourceInfo);
					break;
				case EAdvancePakResourceState::Allot:
					AllotRegularInfos.Add(ResourceInfo);
					break;
				case EAdvancePakResourceState::InValid:
					InValidRegularInfos.Add(ResourceInfo);
					break;
				}
			}

			for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(AdvancePakCreator->FormerPatchs); It; ++It)
			{
				for (auto Regular : It.Value()->Regulars)
				{
					TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPublishResourceInfo());
					ResourceInfo->ResourceName = Regular;
					ResourceInfo->PatchName = It.Key();
					ResourceInfo->ResourceState = AdvancePakCreator->GetRegularResourceState(Regular);

					switch (ResourceInfo->ResourceState)
					{
					case EAdvancePakResourceState::Free:
						FreeRegularInfos.Add(ResourceInfo);
						break;
					case EAdvancePakResourceState::Allot:
						AllotRegularInfos.Add(ResourceInfo);
						break;
					case EAdvancePakResourceState::InValid:
						InValidRegularInfos.Add(ResourceInfo);
						break;
					}
				}
			}
			AdvancePakCreator->FormerPublishRegulars.Reset();
			AdvancePakCreator->FormerPublishRegulars.Append(FreeRegularInfos);
			AdvancePakCreator->FormerPublishRegulars.Append(AllotRegularInfos);
			AdvancePakCreator->FormerPublishRegulars.Append(InValidRegularInfos);
			AdvancePakCreator->FormerPublishRegularsForm->DemandViewRenovate();


			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> FreeInformalInfos;
			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> AllotInformalInfos;
			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> InValidInformalInfos;
			for (auto Informal : AdvancePakCreator->FormerPublish->Informals)
			{
				TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPublishResourceInfo());
				ResourceInfo->ResourceName = Informal->InformalName;
				ResourceInfo->ResourcePath = Informal->InformalPath;
				ResourceInfo->PatchName = AdvancePakCreator->PublishName;
				ResourceInfo->ResourceState = AdvancePakCreator->GetInformalResourceState(Informal->InformalName, Informal->InformalPath);

				switch (ResourceInfo->ResourceState)
				{
				case EAdvancePakResourceState::Free:
					FreeInformalInfos.Add(ResourceInfo);
					break;
				case EAdvancePakResourceState::Allot:
					AllotInformalInfos.Add(ResourceInfo);
					break;
				case EAdvancePakResourceState::InValid:
					InValidInformalInfos.Add(ResourceInfo);
					break;
				}
			}

			for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(AdvancePakCreator->FormerPatchs); It; ++It)
			{
				for (auto Informal : It.Value()->Informals)
				{
					TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPublishResourceInfo());
					ResourceInfo->ResourceName = Informal->InformalName;
					ResourceInfo->PatchName = It.Key();
					ResourceInfo->ResourceState = AdvancePakCreator->GetInformalResourceState(Informal->InformalName, Informal->InformalPath);

					switch (ResourceInfo->ResourceState)
					{
					case EAdvancePakResourceState::Free:
						FreeInformalInfos.Add(ResourceInfo);
						break;
					case EAdvancePakResourceState::Allot:
						AllotInformalInfos.Add(ResourceInfo);
						break;
					case EAdvancePakResourceState::InValid:
						InValidInformalInfos.Add(ResourceInfo);
						break;
					}
				}
			}

			AdvancePakCreator->FormerPublishInformals.Reset();
			AdvancePakCreator->FormerPublishInformals.Append(FreeInformalInfos);
			AdvancePakCreator->FormerPublishInformals.Append(AllotInformalInfos);
			AdvancePakCreator->FormerPublishInformals.Append(InValidInformalInfos);
			AdvancePakCreator->FormerPublishInformalsForm->DemandViewRenovate();
		}
		else
		{
			AdvancePakCreator->FormerPublishRegulars.Reset();
			AdvancePakCreator->FormerPublishRegularsForm->DemandViewRenovate();

			AdvancePakCreator->FormerPublishInformals.Reset();
			AdvancePakCreator->FormerPublishInformalsForm->DemandViewRenovate();
		}
		AdvancePakCreator->IsFormerPatchDirty = false;
	}
	else
	{
		RenovateTwo.ExecuteIfBound();
		RenovateOne.ExecuteIfBound();
	}

	if (!AdvancePakCreator->FormerPublish.IsValid() || !AdvancePakCreator->FormerPatchs.Contains(AdvancePakCreator->ActiveName))
	{
		AdvancePakCreator->FormerPatchRegulars.Reset();
		AdvancePakCreator->FormerPatchRegularsForm->DemandViewRenovate();

		AdvancePakCreator->FormerPatchInformals.Reset();
		AdvancePakCreator->FormerPatchInformalsForm->DemandViewRenovate();
	}
	else
	{
		TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = AdvancePakCreator->GetFormerPatch(AdvancePakCreator->ActiveName);

		TArray<TSharedPtr<FAdvancePakPatchResourceInfo>> FreeRegularInfos;
		TArray<TSharedPtr<FAdvancePakPatchResourceInfo>> AllotRegularInfos;
		TArray<TSharedPtr<FAdvancePakPatchResourceInfo>> InValidRegularInfos;
		for (auto Regular : PatchConfigure->Regulars)
		{
			TSharedPtr<FAdvancePakPatchResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPatchResourceInfo());
			ResourceInfo->ResourceName = Regular;
			ResourceInfo->ResourceState = AdvancePakCreator->GetRegularResourceState(Regular);

			switch (ResourceInfo->ResourceState)
			{
			case EAdvancePakResourceState::Free:
				FreeRegularInfos.Add(ResourceInfo);
				break;
			case EAdvancePakResourceState::Allot:
				AllotRegularInfos.Add(ResourceInfo);
				break;
			case EAdvancePakResourceState::InValid:
				InValidRegularInfos.Add(ResourceInfo);
				break;
			}
		}
		AdvancePakCreator->FormerPatchRegulars.Reset();
		AdvancePakCreator->FormerPatchRegulars.Append(FreeRegularInfos);
		AdvancePakCreator->FormerPatchRegulars.Append(AllotRegularInfos);
		AdvancePakCreator->FormerPatchRegulars.Append(InValidRegularInfos);
		AdvancePakCreator->FormerPatchRegularsForm->DemandViewRenovate();


		TArray<TSharedPtr<FAdvancePakPatchResourceInfo>> FreeInformalInfos;
		TArray<TSharedPtr<FAdvancePakPatchResourceInfo>> AllotInformalInfos;
		TArray<TSharedPtr<FAdvancePakPatchResourceInfo>> InValidInformalInfos;
		for (auto Informal : PatchConfigure->Informals)
		{
			TSharedPtr<FAdvancePakPatchResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPatchResourceInfo());
			ResourceInfo->ResourceName = Informal->InformalName;
			ResourceInfo->ResourcePath = Informal->InformalPath;
			ResourceInfo->ResourceState = AdvancePakCreator->GetInformalResourceState(Informal->InformalName, Informal->InformalPath);

			switch (ResourceInfo->ResourceState)
			{
			case EAdvancePakResourceState::Free:
				FreeInformalInfos.Add(ResourceInfo);
				break;
			case EAdvancePakResourceState::Allot:
				AllotInformalInfos.Add(ResourceInfo);
				break;
			case EAdvancePakResourceState::InValid:
				InValidInformalInfos.Add(ResourceInfo);
				break;
			}
		}

		AdvancePakCreator->FormerPatchInformals.Reset();
		AdvancePakCreator->FormerPatchInformals.Append(FreeInformalInfos);
		AdvancePakCreator->FormerPatchInformals.Append(AllotInformalInfos);
		AdvancePakCreator->FormerPatchInformals.Append(InValidInformalInfos);
		AdvancePakCreator->FormerPatchInformalsForm->DemandViewRenovate();
	}
}

void SAdvancePakPatchBar::RenovatePublishFormerViews()
{
	if (!AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName))
		return;

	if (AdvancePakCreator->IsFormerPublishDirty)
	{
		if (AdvancePakCreator->FormerPublish.IsValid())
		{
			AdvancePakCreator->CacheFormerRegulars.Reset();

			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> FreeRegularInfos;
			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> AllotRegularInfos;
			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> InValidRegularInfos;

			AdvancePakCreator->CacheFormerRegulars.Append(AdvancePakCreator->FormerPublish->Regulars);
			for (auto Regular : AdvancePakCreator->FormerPublish->Regulars)
			{
				TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPublishResourceInfo());
				ResourceInfo->ResourceName = Regular;
				ResourceInfo->PatchName = AdvancePakCreator->PublishName;
				ResourceInfo->ResourceState = AdvancePakCreator->GetRegularResourceState(Regular);

				switch (ResourceInfo->ResourceState)
				{
				case EAdvancePakResourceState::Free:
					FreeRegularInfos.Add(ResourceInfo);
					break;
				case EAdvancePakResourceState::Allot:
					AllotRegularInfos.Add(ResourceInfo);
					break;
				case EAdvancePakResourceState::InValid:
					InValidRegularInfos.Add(ResourceInfo);
					break;
				}
			}

			for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(AdvancePakCreator->FormerPatchs); It; ++It)
			{
				AdvancePakCreator->CacheFormerRegulars.Append(It.Value()->Regulars);
				for (auto Regular : It.Value()->Regulars)
				{
					TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPublishResourceInfo());
					ResourceInfo->ResourceName = Regular;
					ResourceInfo->PatchName = It.Key();
					ResourceInfo->ResourceState = AdvancePakCreator->GetRegularResourceState(Regular);

					switch (ResourceInfo->ResourceState)
					{
					case EAdvancePakResourceState::Free:
						FreeRegularInfos.Add(ResourceInfo);
						break;
					case EAdvancePakResourceState::Allot:
						AllotRegularInfos.Add(ResourceInfo);
						break;
					case EAdvancePakResourceState::InValid:
						InValidRegularInfos.Add(ResourceInfo);
						break;
					}
				}
			}

			AdvancePakCreator->FormerRegulars.Reset();
			AdvancePakCreator->FormerRegulars.Append(FreeRegularInfos);
			AdvancePakCreator->FormerRegulars.Append(AllotRegularInfos);
			AdvancePakCreator->FormerRegulars.Append(InValidRegularInfos);
			AdvancePakCreator->FormerRegularsForm->DemandViewRenovate();

			AdvancePakCreator->CacheFormerInformals.Reset();

			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> FreeInformalInfos;
			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> AllotInformalInfos;
			TArray<TSharedPtr<FAdvancePakPublishResourceInfo>> InValidInformalInfos;

			for (auto Informal : AdvancePakCreator->FormerPublish->Informals)
			{
				AdvancePakCreator->CacheFormerInformals.Add(Informal->InformalName);

				TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPublishResourceInfo());
				ResourceInfo->ResourceName = Informal->InformalName;
				ResourceInfo->ResourcePath = Informal->InformalPath;
				ResourceInfo->PatchName = AdvancePakCreator->PublishName;
				ResourceInfo->ResourceState = AdvancePakCreator->GetInformalResourceState(Informal->InformalName, Informal->InformalPath);

				switch (ResourceInfo->ResourceState)
				{
				case EAdvancePakResourceState::Free:
					FreeInformalInfos.Add(ResourceInfo);
					break;
				case EAdvancePakResourceState::Allot:
					AllotInformalInfos.Add(ResourceInfo);
					break;
				case EAdvancePakResourceState::InValid:
					InValidInformalInfos.Add(ResourceInfo);
					break;
				}
			}

			for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(AdvancePakCreator->FormerPatchs); It; ++It)
			{
				for (auto Informal : It.Value()->Informals)
				{
					AdvancePakCreator->CacheFormerInformals.Add(Informal->InformalName);

					TSharedPtr<FAdvancePakPublishResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPublishResourceInfo());
					ResourceInfo->ResourceName = Informal->InformalName;
					ResourceInfo->ResourcePath = Informal->InformalPath;
					ResourceInfo->PatchName = It.Key();
					ResourceInfo->ResourceState = AdvancePakCreator->GetInformalResourceState(Informal->InformalName, Informal->InformalPath);

					switch (ResourceInfo->ResourceState)
					{
					case EAdvancePakResourceState::Free:
						FreeInformalInfos.Add(ResourceInfo);
						break;
					case EAdvancePakResourceState::Allot:
						AllotInformalInfos.Add(ResourceInfo);
						break;
					case EAdvancePakResourceState::InValid:
						InValidInformalInfos.Add(ResourceInfo);
						break;
					}
				}
			}

			AdvancePakCreator->FormerInformals.Reset();
			AdvancePakCreator->FormerInformals.Append(FreeInformalInfos);
			AdvancePakCreator->FormerInformals.Append(AllotInformalInfos);
			AdvancePakCreator->FormerInformals.Append(InValidInformalInfos);
			AdvancePakCreator->FormerInformalsForm->DemandViewRenovate();
		}
		else
		{
			AdvancePakCreator->CacheFormerRegulars.Reset();
			AdvancePakCreator->FormerRegulars.Reset();
			AdvancePakCreator->FormerRegularsForm->DemandViewRenovate();

			AdvancePakCreator->CacheFormerInformals.Reset();
			AdvancePakCreator->FormerInformals.Reset();
			AdvancePakCreator->FormerInformalsForm->DemandViewRenovate();
		}

		AdvancePakCreator->IsFormerPublishDirty = false;
	}
	else
	{
		if (AdvancePakCreator->IsFuturePublishDirty)
		{
			if (!AdvancePakCreator->PublishFormerRegularsStateThread.IsValid())
			{
				AdvancePakCreator->PublishFormerRegularsStateThread = MakeShareable(new FAdvancePakStateThread(TEXT("PublishFormerRegularsStateTask"), [this]() {

					AdvancePakCreator->IsPublishFormerRegularsStateRunning = true;

					TMap<FName, FName> FreeResources;
					TMap<FName, FName> AllotResources;
					TMap<FName, FName> InValidResources;
					for (auto Regular : AdvancePakCreator->FormerRegulars)
					{
						switch (AdvancePakCreator->GetRegularResourceState(Regular->ResourceName))
						{
						case EAdvancePakResourceState::Free:
							FreeResources.Add(Regular->ResourceName, Regular->PatchName);
							break;
						case EAdvancePakResourceState::Allot:
							AllotResources.Add(Regular->ResourceName, Regular->PatchName);
							break;
						case EAdvancePakResourceState::InValid:
							InValidResources.Add(Regular->ResourceName, Regular->PatchName);
							break;
						}
					}
					int i = 0;
					for (TMap<FName, FName>::TIterator It(FreeResources); It; ++It)
					{
						AdvancePakCreator->FormerRegulars[i]->ResourceName = It.Key();
						AdvancePakCreator->FormerRegulars[i]->PatchName = It.Value();
						AdvancePakCreator->FormerRegulars[i]->ResourceState = EAdvancePakResourceState::Free;
						++i;
					}
					for (TMap<FName, FName>::TIterator It(AllotResources); It; ++It)
					{
						AdvancePakCreator->FormerRegulars[i]->ResourceName = It.Key();
						AdvancePakCreator->FormerRegulars[i]->PatchName = It.Value();
						AdvancePakCreator->FormerRegulars[i]->ResourceState = EAdvancePakResourceState::Allot;
						++i;
					}
					for (TMap<FName, FName>::TIterator It(InValidResources); It; ++It)
					{
						AdvancePakCreator->FormerRegulars[i]->ResourceName = It.Key();
						AdvancePakCreator->FormerRegulars[i]->PatchName = It.Value();
						AdvancePakCreator->FormerRegulars[i]->ResourceState = EAdvancePakResourceState::InValid;
						++i;
					}

					AdvancePakCreator->FormerRegularsForm->DemandSelectRenovate();

				}));

				AdvancePakCreator->PublishFormerRegularsStateThread->StateCompleteDelagete.AddLambda([this]() {
					FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
					{
						AdvancePakCreator->IsPublishFormerRegularsStateRunning = false;
						AdvancePakCreator->PublishFormerRegularsStateThread.Reset();
					}, TStatId(), NULL, ENamedThreads::GameThread);
				});
				AdvancePakCreator->PublishFormerRegularsStateThread->Execute();
			}

			if (!AdvancePakCreator->PublishFormerInformalsStateThread.IsValid())
			{

				AdvancePakCreator->PublishFormerInformalsStateThread = MakeShareable(new FAdvancePakStateThread(TEXT("PublishFormerInformalsStateTask"), [this]() {

					AdvancePakCreator->IsPublishFormerInformalsStateRunning = true;

					TMap<FName, TPair<FName, FString>> FreeResources;
					TMap<FName, TPair<FName, FString>> AllotResources;
					TMap<FName, TPair<FName, FString>> InValidResources;

					for (auto Informal : AdvancePakCreator->FormerInformals)
					{
						FPlatformProcess::Sleep(0.01f);

						switch (AdvancePakCreator->GetInformalResourceState(Informal->ResourceName, Informal->ResourcePath))
						{
						case EAdvancePakResourceState::Free:
							FreeResources.Add(Informal->ResourceName, TPair<FName, FString>(Informal->PatchName, Informal->ResourcePath));
							break;
						case EAdvancePakResourceState::Allot:
							AllotResources.Add(Informal->ResourceName, TPair<FName, FString>(Informal->PatchName, Informal->ResourcePath));
							break;
						case EAdvancePakResourceState::InValid:
							InValidResources.Add(Informal->ResourceName, TPair<FName, FString>(Informal->PatchName, Informal->ResourcePath));
							break;
						}
					}

					int i = 0;
					for (TMap<FName, TPair<FName, FString>>::TIterator It(FreeResources); It; ++It)
					{
						AdvancePakCreator->FormerInformals[i]->ResourceName = It.Key();
						AdvancePakCreator->FormerInformals[i]->PatchName = It.Value().Key;
						AdvancePakCreator->FormerInformals[i]->ResourcePath = It.Value().Value;
						AdvancePakCreator->FormerInformals[i]->ResourceState = EAdvancePakResourceState::Free;
						++i;
					}
					for (TMap<FName, TPair<FName, FString>>::TIterator It(AllotResources); It; ++It)
					{
						AdvancePakCreator->FormerInformals[i]->ResourceName = It.Key();
						AdvancePakCreator->FormerInformals[i]->PatchName = It.Value().Key;
						AdvancePakCreator->FormerInformals[i]->ResourcePath = It.Value().Value;
						AdvancePakCreator->FormerInformals[i]->ResourceState = EAdvancePakResourceState::Allot;
						++i;
					}
					for (TMap<FName, TPair<FName, FString>>::TIterator It(InValidResources); It; ++It)
					{
						AdvancePakCreator->FormerInformals[i]->ResourceName = It.Key();
						AdvancePakCreator->FormerInformals[i]->PatchName = It.Value().Key;
						AdvancePakCreator->FormerInformals[i]->ResourcePath = It.Value().Value;
						AdvancePakCreator->FormerInformals[i]->ResourceState = EAdvancePakResourceState::InValid;
						++i;
					}

					AdvancePakCreator->FormerInformalsForm->DemandSelectRenovate();

				}));

				AdvancePakCreator->PublishFormerInformalsStateThread->StateCompleteDelagete.AddLambda([this]() {
					FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
					{
						AdvancePakCreator->IsPublishFormerInformalsStateRunning = false;
						AdvancePakCreator->PublishFormerInformalsStateThread.Reset();
					}, TStatId(), NULL, ENamedThreads::GameThread);
				});
				AdvancePakCreator->PublishFormerInformalsStateThread->Execute();
			}
		}
	}
}




































