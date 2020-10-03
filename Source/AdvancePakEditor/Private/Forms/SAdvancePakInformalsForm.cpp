// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakInformalsForm.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SHyperlink.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSeparator.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "Raws/SAdvancePakInformalsRaw.h"
#include "Widgets/Input/SComboBox.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakInformalsForm::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;
	InformalsList = InArgs._ListItemsSource;
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
			SAssignNew(InformalsListView, SListView<TSharedPtr<FAdvancePakInformalInfo>>)
			.HeaderRow(
				SNew(SHeaderRow)
				.Visibility(EVisibility::SelfHitTestInvisible)

				+ SHeaderRow::Column(AdvancePakCreator->ResourceNameName)
				.DefaultLabel(OPTEXT("ResourceName"))
				.FillWidth(8.0f)

				+ SHeaderRow::Column(AdvancePakCreator->BrowseName)
				.DefaultLabel(OPTEXT("BrowseName"))
				.FillWidth(0.4f)

			)
		.ItemHeight(16.f)
		.ListItemsSource(InformalsList)
		.OnGenerateRow(this, &SAdvancePakInformalsForm::DealwithInformalViewPromoteRow)
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
			.OnNavigate(this, &SAdvancePakInformalsForm::DealwithSelectLinkwork, true)
		.Text(OPTEXT("All"))
		.ToolTipText(OPTEXT("SelectAllResources"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakInformalsForm::DealwithSelectLinkwork, false)
		.Text(OPTEXT("None"))
		.ToolTipText(OPTEXT("SelectNoneResources"))
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SAssignNew(InformalEditor, SEditableTextBox)
			.HintText(OPTEXT("PleaseSelelctInformals"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.0, 0.0, 0.0, 0.0)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakInformalsForm::DealwithBrowseInformalsLinkwork)
		.Text(OPTEXT("Browse"))
		.ToolTipText(OPTEXT("BrowseForTheInformals"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.0, 0.0, 0.0, 0.0)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakInformalsForm::DealwithAddInformalsLinkwork)
		.Text(OPTEXT("Push"))
		.ToolTipText(OPTEXT("PushInformalToPatch"))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 8.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakInformalsForm::DealwithRemoveInformalsLinkwork)
		.Text(OPTEXT("Remove"))
		.ToolTipText(OPTEXT("RemoveInformalFromPatch"))
		]

		]

		]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

const TArray<FName>& SAdvancePakInformalsForm::GetSelectedInformals()
{
	return SelectedInformals;
}

void SAdvancePakInformalsForm::DemandViewRenovate()
{
	DemandSelectRenovate();
	InformalsListView->RequestListRefresh();
}

void SAdvancePakInformalsForm::DemandSelectRenovate()
{
	TArray<FName> AbandonInformals;

	for (auto InformalName : SelectedInformals)
	{
		bool IsAbandon = true;
		for (auto Informal : *InformalsList)
		{
			if (Informal->InformalName.IsEqual(InformalName))
			{
				IsAbandon = false;
				break;
			}
		}
		if (IsAbandon)
			AbandonInformals.Add(InformalName);
	}

	for (auto InformalName : AbandonInformals)
		SelectedInformals.Remove(InformalName);
}

void SAdvancePakInformalsForm::RemoveInformal(const FName& Informal)
{
	if (SelectedInformals.Contains(Informal))
		SelectedInformals.Remove(Informal);
}

void SAdvancePakInformalsForm::DealwithRemoveInformalsLinkwork()
{
	if (SelectedInformals.Num() == 0)
		return;

	if (IsInvolve)
	{
		for (auto InformalName : SelectedInformals)
		{
			int32 RemoveIndex = -1;
			for (int i = 0; i < AdvancePakCreator->InvolveInformals.Num(); ++i)
			{
				if (AdvancePakCreator->InvolveInformals[i]->InformalName.IsEqual(InformalName))
				{
					RemoveIndex = i;
					break;
				}
			}
			if (RemoveIndex >= 0)
			{
				AdvancePakCreator->GetFuturePatch()->InvolveInformals.RemoveAt(RemoveIndex);
				AdvancePakCreator->InvolveInformals.RemoveAt(RemoveIndex);
			}
		}
		AdvancePakCreator->InvolveInformalsForm->DemandViewRenovate();

		PromoteFour.ExecuteIfBound();
		PromoteEight.ExecuteIfBound();
		RenovateFive.ExecuteIfBound();
		RenovateOne.ExecuteIfBound();
		RenovateSix.ExecuteIfBound();
	}
	else
	{
		for (auto InformalName : SelectedInformals)
		{
			int32 RemoveIndex = -1;
			for (int i = 0; i < AdvancePakCreator->AbsolveInformals.Num(); ++i)
			{
				if (AdvancePakCreator->AbsolveInformals[i]->InformalName.IsEqual(InformalName))
				{
					RemoveIndex = i;
					break;
				}
			}
			if (RemoveIndex >= 0)
			{
				AdvancePakCreator->GetFuturePatch()->AbsolveInformals.RemoveAt(RemoveIndex);
				AdvancePakCreator->AbsolveInformals.RemoveAt(RemoveIndex);
			}
		}
		AdvancePakCreator->AbsolveInformalsForm->DemandViewRenovate();

		PromoteSix.ExecuteIfBound();
		PromoteEight.ExecuteIfBound();
		RenovateFive.ExecuteIfBound();
		RenovateOne.ExecuteIfBound();
		RenovateSix.ExecuteIfBound();
	}

	SelectedInformals.Reset();
}

void SAdvancePakInformalsForm::DealwithAddInformalsLinkwork()
{
	if (InformalInfos.Num() == 0)
		return;

	InformalEditor->SetText(FText());

	if (IsInvolve)
	{
		TSharedPtr<FAdvancePakPatchConfigure> FuturePatch = AdvancePakCreator->GetFuturePatch();

		bool IsChanged = false;
		for (auto InformalInfo : InformalInfos)
		{
			bool IsExisted = false;
			for (auto Informal : AdvancePakCreator->InvolveInformals)
			{
				if (Informal->InformalName.IsEqual(InformalInfo->InformalName))
				{
					IsExisted = true;
					break;
				}
			}

			if (!IsExisted)
			{
				IsChanged = true;
				FuturePatch->InvolveInformals.Add(InformalInfo);
				AdvancePakCreator->InvolveInformals.Add(InformalInfo);
			}
		}

		if (IsChanged)
			AdvancePakCreator->InvolveInformalsForm->DemandViewRenovate();

		PromoteFour.ExecuteIfBound();
		PromoteEight.ExecuteIfBound();
		RenovateFive.ExecuteIfBound();
		RenovateOne.ExecuteIfBound();
		RenovateSix.ExecuteIfBound();
	}
	else
	{
		TSharedPtr<FAdvancePakPatchConfigure> FuturePatch = AdvancePakCreator->GetFuturePatch();

		bool IsChanged = false;
		for (auto InformalInfo : InformalInfos)
		{
			bool IsExisted = false;
			for (auto Informal : AdvancePakCreator->AbsolveInformals)
			{
				if (Informal->InformalName.IsEqual(InformalInfo->InformalName))
				{
					IsExisted = true;
					break;
				}
			}

			if (!IsExisted)
			{
				IsChanged = true;
				FuturePatch->AbsolveInformals.Add(InformalInfo);
				AdvancePakCreator->AbsolveInformals.Add(InformalInfo);
			}
		}

		if (IsChanged)
			AdvancePakCreator->AbsolveInformalsForm->DemandViewRenovate();

		PromoteSix.ExecuteIfBound();
		PromoteEight.ExecuteIfBound();
		RenovateFive.ExecuteIfBound();
		RenovateOne.ExecuteIfBound();
		RenovateSix.ExecuteIfBound();
	}

	InformalInfos.Reset();
}

void SAdvancePakInformalsForm::DealwithSelectLinkwork(bool IsAll)
{
	if (IsAll)
	{
		for (auto Informal : *InformalsList)
		{
			SelectedInformals.AddUnique(Informal->InformalName);
		}
	}
	else
	{
		SelectedInformals.Reset();
	}
}

void SAdvancePakInformalsForm::DealwithBrowseInformalsLinkwork()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
		void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

		TArray<FString> SelectedFiles;

		const bool bFileSelected = DesktopPlatform->OpenFileDialog(
			ParentWindowHandle,
			FString("Choose Informal Files"),
			TEXT(""),
			TEXT(""),
			TEXT("AdvancePak All (*.*)|*.*"),
			EFileDialogFlags::Multiple,
			SelectedFiles
		);

		if (bFileSelected && SelectedFiles.Num() > 0)
		{
			InformalInfos.Reset();
			InformalEditor->SetText(FText());

			for (auto SelectedFile : SelectedFiles)
			{
				FString FilePath = FPaths::ConvertRelativePathToFull(SelectedFile);

				if (!(FilePath.EndsWith(UAdvancePakEditorLibrary::UassetSuffixName) || FilePath.EndsWith(UAdvancePakEditorLibrary::UexpSuffixName) || FilePath.EndsWith(UAdvancePakEditorLibrary::UbulkSuffixName) || FilePath.EndsWith(UAdvancePakEditorLibrary::UmapSuffixName) || FilePath.EndsWith(UAdvancePakEditorLibrary::UfontSuffixName) || FilePath.EndsWith(UAdvancePakEditorLibrary::UtxtSuffixName) || FilePath.EndsWith(UAdvancePakEditorLibrary::UtxtmapSuffixName)))
				{
					FName InformalName = UAdvancePakEditorLibrary::AbsorbFullToInformal(FilePath);
					if (InformalName.IsNone())
						continue;

					if (InformalEditor->GetText().ToString().IsEmpty())
						InformalEditor->SetText(FText::FromName(InformalName));
					else
						InformalEditor->SetText(FText::FromString(FString::Printf(TEXT("%s;%s"), *InformalEditor->GetText().ToString(), *InformalName.ToString())));

					TSharedPtr<FAdvancePakInformalInfo> InformalInfo = MakeShareable(new FAdvancePakInformalInfo());
					InformalInfo->InformalName = InformalName;
					InformalInfo->InformalPath = FilePath;

					InformalInfos.Add(InformalInfo);
				}
			}
		}
	}
}

TSharedRef<ITableRow> SAdvancePakInformalsForm::DealwithInformalViewPromoteRow(TSharedPtr<FAdvancePakInformalInfo> Informal, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SAdvancePakInformalsRaw, this)
		.OwnerTableView(OwnerTable)
		.AdvancePakCreator(AdvancePakCreator)
		.Informal(Informal)
		;
}

void SAdvancePakInformalsForm::AddInformal(const FName& Informal)
{
	if (!SelectedInformals.Contains(Informal))
		SelectedInformals.Add(Informal);
}