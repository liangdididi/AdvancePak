// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakGathersForm.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SComboBox.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "Raws/SAdvancePakGathersRaw.h"
#include "Widgets/Input/SHyperlink.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"



BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakGathersForm::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;
	GathersList = InArgs._ListItemsSource;
	IsInvolve = InArgs._IsInvolve;

	ChildSlot
		[

			SNew(SBorder)
			.Padding(4.f)
		[

			SNew(SVerticalBox)
			.IsEnabled_Lambda([this]() { return !AdvancePakCreator->IsPackaging; })

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(InArgs._Title)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		.ColorAndOpacity(FLinearColor(0.f, 1.f, 0.f))
		]
		]

	+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(GathersListView, SListView<TSharedPtr<FAdvancePakGatherOption>>)
			.HeaderRow(
				SNew(SHeaderRow)
				.Visibility(EVisibility::SelfHitTestInvisible)

				+ SHeaderRow::Column(AdvancePakCreator->GatherPathName)
				.DefaultLabel(OPTEXT("GatherPath"))
				.FillWidth(5.0f)

				+ SHeaderRow::Column(AdvancePakCreator->GatherTypeName)
				.DefaultLabel(OPTEXT("GatherType"))
				.FillWidth(1.0f)

				+ SHeaderRow::Column(AdvancePakCreator->RelyonTypeName)
				.DefaultLabel(OPTEXT("RelyonType"))
				.FillWidth(1.0f)

				+ SHeaderRow::Column(AdvancePakCreator->GatherModeName)
				.DefaultLabel(OPTEXT("GatherMode"))
				.FillWidth(1.0f)

				+ SHeaderRow::Column(AdvancePakCreator->BrowseName)
				.DefaultLabel(OPTEXT("BrowseName"))
				.FillWidth(0.4f)

			)
		.ItemHeight(16.f)
		.ListItemsSource(GathersList)
		.OnGenerateRow(this, &SAdvancePakGathersForm::DealwithGatherViewPromoteRow)
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
			.IsEnabled_Lambda([this]() { return AdvancePakCreator->GetFuturePatch().IsValid(); })

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
			.OnNavigate(this, &SAdvancePakGathersForm::DealwithSelectLinkwork, true)
		.Text(OPTEXT("All"))
		.ToolTipText(OPTEXT("SelectAllGathers"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakGathersForm::DealwithSelectLinkwork, false)
		.Text(OPTEXT("None"))
		.ToolTipText(OPTEXT("SelectNoneGathers"))
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SAssignNew(GatherPathEditor, SEditableTextBox)
			.HintText(OPTEXT("PleaseSelelctGatherDirectory"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.0, 0.0, 0.0, 0.0)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakGathersForm::DealwithBrowseGathersLinkwork)
		.Text(OPTEXT("Browse"))
		.ToolTipText(OPTEXT("BrowseForTheDirectory"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.0, 0.0, 0.0, 0.0)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakGathersForm::DealwithAddGathersLinkwork)
		.Text(OPTEXT("Push"))
		.ToolTipText(OPTEXT("PushGatherToPatch"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 8.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakGathersForm::DealwithRemoveGathersLinkwork)
		.Text(OPTEXT("Remove"))
		.ToolTipText(OPTEXT("RemoveGatherFromPatch"))
		]

		]

		]
		];
}



END_SLATE_FUNCTION_BUILD_OPTIMIZATION

const TArray<FString>& SAdvancePakGathersForm::GetSelectedGathers()
{
	return SelectedGathers;
}

void SAdvancePakGathersForm::AddGather(const FString& GatherPath)
{
	if (!SelectedGathers.Contains(GatherPath))
		SelectedGathers.Add(GatherPath);
}

void SAdvancePakGathersForm::DealwithSelectLinkwork(bool IsAll)
{
	if (IsAll)
	{
		for (auto GatherInfo : *GathersList)
		{
			SelectedGathers.AddUnique(GatherInfo->GatherPath);
		}
	}
	else
	{
		SelectedGathers.Reset();
	}
}

void SAdvancePakGathersForm::DealwithAddGathersLinkwork()
{
	if (!GatherOption.IsValid())
		return;

	GatherPathEditor->SetText(FText());

	if (IsInvolve)
	{
		for (auto InvolveGather : AdvancePakCreator->InvolveGathers)
		{
			if (InvolveGather->GatherPath.Equals(GatherOption->GatherPath))
				return;
		}

		AdvancePakCreator->GetFuturePatch()->InvolveGathers.Add(GatherOption);
		AdvancePakCreator->InvolveGathers.Add(GatherOption);
		AdvancePakCreator->InvolveGathersForm->DemandViewRenovate();

		AdvancePakCreator->IsFuturePublishDirty = true;

		switch (GatherOption->GatherType)
		{
		case EAdvancePakGatherType::Regular:
			PromoteOne.ExecuteIfBound();
			PromoteTen.ExecuteIfBound();
			RenovateFour.ExecuteIfBound();
			RenovateTwo.ExecuteIfBound();
			RenovateThree.ExecuteIfBound();
			break;
		case EAdvancePakGatherType::Informal:
			PromoteTwo.ExecuteIfBound();
			PromoteEight.ExecuteIfBound();
			RenovateFive.ExecuteIfBound();
			RenovateOne.ExecuteIfBound();
			RenovateSix.ExecuteIfBound();
			break;
		case EAdvancePakGatherType::Both:
			PromoteOne.ExecuteIfBound();
			PromoteTwo.ExecuteIfBound();
			PromoteTen.ExecuteIfBound();
			PromoteEight.ExecuteIfBound();
			RenovateFour.ExecuteIfBound();
			RenovateFive.ExecuteIfBound();
			RenovateTwo.ExecuteIfBound();
			RenovateOne.ExecuteIfBound();
			RenovateThree.ExecuteIfBound();
			RenovateSix.ExecuteIfBound();
			break;
		}
	}
	else
	{
		for (auto AbsolveGather : AdvancePakCreator->AbsolveGathers)
		{
			if (AbsolveGather->GatherPath.Equals(GatherOption->GatherPath))
				return;
		}

		AdvancePakCreator->GetFuturePatch()->AbsolveGathers.Add(GatherOption);
		AdvancePakCreator->AbsolveGathers.Add(GatherOption);
		AdvancePakCreator->AbsolveGathersForm->DemandViewRenovate();

		AdvancePakCreator->IsFuturePublishDirty = true;

		switch (GatherOption->GatherType)
		{
		case EAdvancePakGatherType::Regular:
			PromoteNine.ExecuteIfBound();
			PromoteTen.ExecuteIfBound();
			RenovateFour.ExecuteIfBound();
			RenovateTwo.ExecuteIfBound();
			RenovateThree.ExecuteIfBound();
			break;
		case EAdvancePakGatherType::Informal:
			PromoteThree.ExecuteIfBound();
			PromoteEight.ExecuteIfBound();
			RenovateFive.ExecuteIfBound();
			RenovateOne.ExecuteIfBound();
			RenovateSix.ExecuteIfBound();
			break;
		case EAdvancePakGatherType::Both:
			PromoteNine.ExecuteIfBound();
			PromoteThree.ExecuteIfBound();
			PromoteTen.ExecuteIfBound();
			PromoteEight.ExecuteIfBound();
			RenovateFour.ExecuteIfBound();
			RenovateFive.ExecuteIfBound();
			RenovateTwo.ExecuteIfBound();
			RenovateOne.ExecuteIfBound();
			RenovateThree.ExecuteIfBound();
			RenovateSix.ExecuteIfBound();
			break;
		}
	}

	GatherOption.Reset();
}

TSharedRef<ITableRow> SAdvancePakGathersForm::DealwithGatherViewPromoteRow(TSharedPtr<FAdvancePakGatherOption> GatherInfo, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SAdvancePakGathersRaw, this)
		.OwnerTableView(OwnerTable)
		.AdvancePakCreator(AdvancePakCreator)
		.GatherInfo(GatherInfo)
		;
}

void SAdvancePakGathersForm::DealwithRemoveGathersLinkwork()
{
	if (SelectedGathers.Num() == 0)
		return;

	if (IsInvolve)
	{
		for (auto GatherPath : SelectedGathers)
		{
			int32 RemoveIndex = -1;
			for (int i = 0; i < AdvancePakCreator->InvolveGathers.Num(); ++i)
			{
				if (AdvancePakCreator->InvolveGathers[i]->GatherPath.Equals(GatherPath))
				{
					RemoveIndex = i;
					break;
				}
			}
			if (RemoveIndex >= 0)
			{
				AdvancePakCreator->GetFuturePatch()->InvolveGathers.RemoveAt(RemoveIndex);
				AdvancePakCreator->InvolveGathers.RemoveAt(RemoveIndex);
			}
		}

		AdvancePakCreator->InvolveGathersForm->DemandViewRenovate();

		AdvancePakCreator->IsFuturePublishDirty = true;
		PromoteOne.ExecuteIfBound();
		PromoteTwo.ExecuteIfBound();
		PromoteTen.ExecuteIfBound();
		PromoteEight.ExecuteIfBound();
		RenovateFour.ExecuteIfBound();
		RenovateFive.ExecuteIfBound();
		RenovateTwo.ExecuteIfBound();
		RenovateOne.ExecuteIfBound();
		RenovateThree.ExecuteIfBound();
		RenovateSix.ExecuteIfBound();
	}
	else
	{
		for (auto GatherPath : SelectedGathers)
		{
			int32 RemoveIndex = -1;
			for (int i = 0; i < AdvancePakCreator->AbsolveGathers.Num(); ++i)
			{
				if (AdvancePakCreator->AbsolveGathers[i]->GatherPath.Equals(GatherPath))
				{
					RemoveIndex = i;
					break;
				}
			}
			if (RemoveIndex >= 0)
			{
				AdvancePakCreator->GetFuturePatch()->AbsolveGathers.RemoveAt(RemoveIndex);
				AdvancePakCreator->AbsolveGathers.RemoveAt(RemoveIndex);
			}
		}

		AdvancePakCreator->AbsolveGathersForm->DemandViewRenovate();

		AdvancePakCreator->IsFuturePublishDirty = true;
		PromoteNine.ExecuteIfBound();
		PromoteThree.ExecuteIfBound();
		PromoteTen.ExecuteIfBound();
		PromoteEight.ExecuteIfBound();
		RenovateFour.ExecuteIfBound();
		RenovateFive.ExecuteIfBound();
		RenovateTwo.ExecuteIfBound();
		RenovateOne.ExecuteIfBound();
		RenovateThree.ExecuteIfBound();
		RenovateSix.ExecuteIfBound();
	}
	SelectedGathers.Reset();
}

bool SAdvancePakGathersForm::AbsorbFullToGather(FString FullPath, FString& GatherPath, bool& CanGatherRegular)
{
	if (!FullPath.StartsWith(UAdvancePakEditorLibrary::FullProjectPath) && !FullPath.StartsWith(UAdvancePakEditorLibrary::FullEnginePath))
	{
		return false;
	}

	if (FullPath.EndsWith(UAdvancePakEditorLibrary::ContentStartPath))
		FullPath += TEXT("/");


	if (FullPath.StartsWith(UAdvancePakEditorLibrary::CookedGameContentPath))
	{
		GatherPath = UAdvancePakEditorLibrary::GameStarPath / FullPath.Right(FullPath.Len() - UAdvancePakEditorLibrary::CookedGameContentPath.Len());
		CanGatherRegular = true;
	}
	else if (FullPath.StartsWith(UAdvancePakEditorLibrary::CookedGamePluginsPath) && FullPath.Contains(UAdvancePakEditorLibrary::ContentSpacePath))
	{
		FString ContentLeft = FString();
		FString ContentRight = FString();
		FullPath.Split(UAdvancePakEditorLibrary::ContentSpacePath, &ContentLeft, &ContentRight);
		GatherPath = FString("/") + (FPaths::GetBaseFilename(ContentLeft) / ContentRight);
		CanGatherRegular = true;
	}
	else if (FullPath.StartsWith(UAdvancePakEditorLibrary::CookedEngineContentPath))
	{
		GatherPath = UAdvancePakEditorLibrary::EngineStartPath / FullPath.Right(FullPath.Len() - UAdvancePakEditorLibrary::CookedEngineContentPath.Len());
		CanGatherRegular = true;
	}
	else if (FullPath.StartsWith(UAdvancePakEditorLibrary::CookedEnginePluginsPath) && FullPath.Contains(UAdvancePakEditorLibrary::ContentSpacePath))
	{
		FString ContentLeft = FString();
		FString ContentRight = FString();
		FullPath.Split(UAdvancePakEditorLibrary::ContentSpacePath, &ContentLeft, &ContentRight);
		GatherPath = FString("/") + (FPaths::GetBaseFilename(ContentLeft) / ContentRight);
		CanGatherRegular = true;
	}
	else if (FullPath.StartsWith(UAdvancePakEditorLibrary::CookedProjectPath))
	{
		GatherPath = FullPath.Right(FullPath.Len() - UAdvancePakEditorLibrary::CookedPlatformPath.Len());
		CanGatherRegular = false;
	}
	else if (FullPath.StartsWith(UAdvancePakEditorLibrary::CookedEnginePath))
	{
		GatherPath = FullPath.Right(FullPath.Len() - UAdvancePakEditorLibrary::CookedPlatformPath.Len());
		CanGatherRegular = false;
	}
	else if (FullPath.StartsWith(UAdvancePakEditorLibrary::FullGameContentPath))
	{
		GatherPath = UAdvancePakEditorLibrary::GameStarPath / FullPath.Right(FullPath.Len() - UAdvancePakEditorLibrary::FullGameContentPath.Len());
		CanGatherRegular = true;
	}
	else if (FullPath.StartsWith(UAdvancePakEditorLibrary::FullGamePluginsPath) && FullPath.Contains(UAdvancePakEditorLibrary::ContentSpacePath))
	{
		FString ContentLeft = FString();
		FString ContentRight = FString();
		FullPath.Split(UAdvancePakEditorLibrary::ContentSpacePath, &ContentLeft, &ContentRight);
		GatherPath = FString("/") + (FPaths::GetBaseFilename(ContentLeft) / ContentRight);
		CanGatherRegular = true;
	}
	else if (FullPath.StartsWith(UAdvancePakEditorLibrary::FullEngineContentPath))
	{
		GatherPath = UAdvancePakEditorLibrary::EngineStartPath / FullPath.Right(FullPath.Len() - UAdvancePakEditorLibrary::FullEngineContentPath.Len());
		CanGatherRegular = true;
	}
	else if (FullPath.StartsWith(UAdvancePakEditorLibrary::FullEnginePluginsPath) && FullPath.Contains(UAdvancePakEditorLibrary::ContentSpacePath))
	{
		FString ContentLeft = FString();
		FString ContentRight = FString();
		FullPath.Split(UAdvancePakEditorLibrary::ContentSpacePath, &ContentLeft, &ContentRight);
		GatherPath = FString("/") + (FPaths::GetBaseFilename(ContentLeft) / ContentRight);
		CanGatherRegular = true;
	}
	else if (FullPath.StartsWith(UAdvancePakEditorLibrary::FullProjectPath))
	{
		GatherPath = FullPath.Right(FullPath.Len() - UAdvancePakEditorLibrary::CullProjectPath.Len());
		CanGatherRegular = false;
	}
	else if (FullPath.StartsWith(UAdvancePakEditorLibrary::FullEnginePath))
	{
		GatherPath = FullPath.Right(FullPath.Len() - UAdvancePakEditorLibrary::CullEnginePath.Len());
		CanGatherRegular = false;
	}

	GatherPath.RemoveFromEnd(TEXT("/"));
	return true;
}

void SAdvancePakGathersForm::RemoveGather(const FString& GatherPath)
{
	if (SelectedGathers.Contains(GatherPath))
		SelectedGathers.Remove(GatherPath);
}

void SAdvancePakGathersForm::DemandSelectRenovate()
{
	TArray<FString> AbandonGathers;

	for (auto GatherPath : SelectedGathers)
	{
		bool IsAbandon = true;
		for (auto GatherInfo : *GathersList)
		{
			if (GatherInfo->GatherPath.Equals(GatherPath))
			{
				IsAbandon = false;
				break;
			}
		}
		if (IsAbandon)
			AbandonGathers.Add(GatherPath);
	}

	for (auto GatherPath : AbandonGathers)
		SelectedGathers.Remove(GatherPath);
}

void SAdvancePakGathersForm::DemandViewRenovate()
{
	DemandSelectRenovate();
	GathersListView->RequestListRefresh();
}

void SAdvancePakGathersForm::DealwithBrowseGathersLinkwork()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
		void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

		FString FullPath;

		const bool bFolderSelected = DesktopPlatform->OpenDirectoryDialog(
			ParentWindowHandle,
			FString("Choose a directory"),
			GatherPathEditor->GetText().ToString(),
			FullPath
		);

		if (bFolderSelected)
		{
			FullPath = FPaths::ConvertRelativePathToFull(FullPath);

			if (GatherOption.IsValid())
			{
				GatherOption.Reset();
			}

			GatherOption = MakeShareable(new FAdvancePakGatherOption());

			if (AbsorbFullToGather(FullPath, GatherOption->GatherPath, GatherOption->CanGatherRegular))
			{
				GatherPathEditor->SetText(FText::FromString(GatherOption->GatherPath));

				GatherOption->FullPath = FullPath;
				if (GatherOption->CanGatherRegular)
				{
					GatherOption->GatherType = EAdvancePakGatherType::Regular;
				}
				else
				{
					GatherOption->GatherType = EAdvancePakGatherType::Informal;
				}
			}
			else
			{
				GatherPathEditor->SetText(FText());
				GatherPathEditor->SetHintText(OPTEXT("PleaseSelectProjectOrEngineDirectory"));
			}
		}
	}
}
