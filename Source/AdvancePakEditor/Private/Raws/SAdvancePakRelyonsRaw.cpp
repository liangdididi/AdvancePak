// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdvancePakRelyonsRaw.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Input/SHyperlink.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SComboBox.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "Widgets/SOverlay.h"
#include "Forms/SAdvancePakRelyonsForm.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakRelyonsRaw::Construct(const FArguments& InArgs, SAdvancePakRelyonsForm* InRelyonsForm)
{
	RelyonInfo = InArgs._RelyonInfo;
	AdvancePakCreator = InArgs._AdvancePakCreator;
	RelyonsForm = InRelyonsForm;
	SMultiColumnTableRow<TSharedPtr<FAdvancePakRelyonInfo>>::Construct(FSuperRowType::FArguments(), InArgs._OwnerTableView.ToSharedRef());
}

TSharedRef<SWidget> SAdvancePakRelyonsRaw::GenerateWidgetForColumn(const FName& ColumnName)
{
	if (ColumnName.IsEqual(AdvancePakCreator->PackageNameName))
	{
		return SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SExpanderArrow, SharedThis(this))
			]

		+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				CreatePackageNameWidget()
			];
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->PatchNameName))
	{
		if (!RelyonInfo->PatchName.IsNone())
		{
			return SNew(STextBlock)
				.Text_Lambda([this]() { return FText::FromName(RelyonInfo->PatchName); })
				.ColorAndOpacity(this, &SAdvancePakRelyonsRaw::DealwithRowColor);
		}
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->RelyonTypeName))
	{
		TSharedPtr<SComboBox<TSharedPtr<FString>>> RelyonTypeCombo;
		SAssignNew(RelyonTypeCombo, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&UAdvancePakEditorLibrary::RelyonTypeOptions)
			.OnGenerateWidget(this, &SAdvancePakRelyonsRaw::MakeComboItem)
			.OnSelectionChanged(this, &SAdvancePakRelyonsRaw::OnRelyonTypeChanged)
			[
				SNew(STextBlock)
				.Text(this, &SAdvancePakRelyonsRaw::GetRelyonTypeText)
			];
		RelyonTypeCombo->RefreshOptions();
		return RelyonTypeCombo.ToSharedRef();
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->UnViewName) && RelyonInfo->IsTop)
	{
		return SNew(SButton)
			.ContentPadding(FMargin(6.0, 2.0))
			.Text(OPTEXT("UnView"))
			.ToolTipText(OPTEXT("UnViewResources"))
			.OnClicked(this, &SAdvancePakRelyonsRaw::DealwithUnViewButtonClicked);
	}
	else if (ColumnName.IsEqual(AdvancePakCreator->BrowseName))
	{
		return SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.FillWidth(1.f)
			[
				PropertyCustomizationHelpers::MakeBrowseButton(
					FSimpleDelegate::CreateRaw(this, &SAdvancePakRelyonsRaw::DealwithResourceBrowse)
				)
			];
	}
	return SNullWidget::NullWidget;
}

TSharedRef<SWidget> SAdvancePakRelyonsRaw::CreatePackageNameWidget()
{
	return SNew(SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		[
			SNew(SCheckBox)
			.IsChecked(this, &SAdvancePakRelyonsRaw::DealwithCheckBoxIsChecked)
		.OnCheckStateChanged(this, &SAdvancePakRelyonsRaw::DealwithCheckBoxCheckStateChanged)
		.Visibility_Lambda([this]()->EVisibility { return RelyonInfo->PatchName.IsNone() || RelyonInfo->PatchName.IsEqual(AdvancePakCreator->ActiveName) ? EVisibility::Visible : EVisibility::Collapsed;  })
		[
			SNew(STextBlock)
			.Text(FText::FromName(RelyonInfo->PackageName))
		.ToolTipText(FText::FromName(RelyonInfo->PackageName))
		.ColorAndOpacity(this, &SAdvancePakRelyonsRaw::DealwithRowColor)
		]
		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(18.f, 0.f, 0.f, 0.f)
		[
			SNew(STextBlock)
			.Text(FText::FromName(RelyonInfo->PackageName))
		.ToolTipText(FText::FromName(RelyonInfo->PackageName))
		.ColorAndOpacity(FLinearColor(1.f, 0.f, 0.f))
		.Visibility_Lambda([this]()->EVisibility { return !(RelyonInfo->PatchName.IsNone() || RelyonInfo->PatchName.IsEqual(AdvancePakCreator->ActiveName)) ? EVisibility::Visible : EVisibility::Collapsed;  })
		];
}

void SAdvancePakRelyonsRaw::DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState)
{
	if (RelyonInfo->PatchName.IsNone())
	{
		if (NewState == ECheckBoxState::Checked)
		{
			RelyonsForm->AddInvolve(RelyonInfo->PackageName);
		}
		else
		{
			RelyonsForm->RemoveInvolve(RelyonInfo->PackageName);
		}
	}
	else
	{
		if (NewState == ECheckBoxState::Checked)
		{
			RelyonsForm->AddAbsolve(RelyonInfo->PackageName);
		}
		else
		{
			RelyonsForm->RemoveAbsolve(RelyonInfo->PackageName);
		}
	}
}

ECheckBoxState SAdvancePakRelyonsRaw::DealwithCheckBoxIsChecked() const
{
	if (RelyonInfo->PatchName.IsNone())
	{
		if (RelyonsForm->GetSelectedInvolves().Contains(RelyonInfo->PackageName))
			return ECheckBoxState::Checked;
	}
	else
	{
		if (RelyonsForm->GetSelectedAbsolves().Contains(RelyonInfo->PackageName))
			return ECheckBoxState::Checked;
	}
	return ECheckBoxState::Unchecked;
}

TSharedRef<SWidget> SAdvancePakRelyonsRaw::MakeComboItem(TSharedPtr<FString> ComboItem)
{
	check(ComboItem.IsValid());
	return	SNew(STextBlock)
		.Text(FText::FromString(*ComboItem));
}

void SAdvancePakRelyonsRaw::OnRelyonTypeChanged(TSharedPtr<FString> ComboItem, ESelectInfo::Type SelectInfo)
{
	check(ComboItem.IsValid());
	EAdvancePakRelyonType NewRelyonType = UAdvancePakEditorLibrary::AbsorbStringToRelyonType(*ComboItem);

	if (RelyonInfo->RelyonType != NewRelyonType)
	{
		RelyonInfo->RelyonType = NewRelyonType;

		if (RelyonInfo->RelyonType == EAdvancePakRelyonType::Nothing)
		{
			if (RelyonInfo->Relyons.Num() != 0)
			{
				RelyonInfo->Relyons.Reset();
				RelyonsForm->DemandViewRenovate();
			}
		}
		else
		{
			TArray<FName> Relyons;
			UAdvancePakEditorLibrary::GetResourceRelyons(RelyonInfo->PackageName, RelyonInfo->RelyonType, Relyons);

			bool IsRelyonsChanged = false;

			TMap<FName, TSharedPtr<FAdvancePakRelyonInfo>> AbandonRelyons;

			for (TMap<FName, TSharedPtr<FAdvancePakRelyonInfo>>::TIterator It(RelyonInfo->Relyons); It; ++It)
			{
				if (!Relyons.Contains(It.Key()))
					AbandonRelyons.Add(It.Key(), It.Value());
			}

			if (AbandonRelyons.Num() > 0)
			{
				IsRelyonsChanged = true;
				for (TMap<FName, TSharedPtr<FAdvancePakRelyonInfo>>::TIterator It(AbandonRelyons); It; ++It)
				{
					if (It.Key().IsNone())
						RelyonsForm->RemoveInvolve(It.Key());
					else if (It.Key().IsEqual(AdvancePakCreator->ActiveName))
						RelyonsForm->RemoveAbsolve(It.Key());
					RelyonInfo->Relyons.Remove(It.Key());
				}
			}

			for (auto PackageName : Relyons)
			{
				if (RelyonInfo->Relyons.Contains(PackageName) || RelyonInfo->PackageName.IsEqual(PackageName))
					continue;
				TSharedPtr<FAdvancePakRelyonInfo> NewRelyonInfo = MakeShareable(new FAdvancePakRelyonInfo());
				NewRelyonInfo->PackageName = PackageName;
				NewRelyonInfo->PatchName = AdvancePakCreator->GetFutureRegularPatchName(PackageName);
				NewRelyonInfo->RelyonType = EAdvancePakRelyonType::Packages;
				RelyonInfo->Relyons.Add(PackageName, NewRelyonInfo);

				IsRelyonsChanged = true;
			}

			if (IsRelyonsChanged)
				RelyonsForm->DemandViewRenovate();
		}
	}
}

FText SAdvancePakRelyonsRaw::GetRelyonTypeText() const
{
	return FText::FromString(*UAdvancePakEditorLibrary::RelyonTypeOptions[(int32)RelyonInfo->RelyonType]);
}

void SAdvancePakRelyonsRaw::DealwithResourceBrowse()
{
	TArray<FAssetData> ResourceDatas;
	UAdvancePakEditorLibrary::GetResourceDatasByPackageName(RelyonInfo->PackageName, ResourceDatas);
	if (ResourceDatas.Num() > 0)
		GEditor->SyncBrowserToObjects(ResourceDatas);
}

FSlateColor SAdvancePakRelyonsRaw::DealwithRowColor() const
{
	if (RelyonInfo->PatchName.IsNone())
		return FLinearColor::Green;
	if (RelyonInfo->PatchName.IsEqual(AdvancePakCreator->ActiveName))
		return FLinearColor::Yellow;
	return FLinearColor::Red;
}

FReply SAdvancePakRelyonsRaw::DealwithUnViewButtonClicked()
{
	bool IsChanged = false;
	int32 RemoveIndex = -1;
	for (int i = 0; i < AdvancePakCreator->Relyons.Num(); ++i)
	{
		if (AdvancePakCreator->Relyons[i]->PackageName.IsEqual(RelyonInfo->PackageName))
		{
			RemoveIndex = i;
			break;
		}
	}
	if (RemoveIndex >= 0)
	{
		AdvancePakCreator->Relyons.RemoveAt(RemoveIndex);
		IsChanged = true;
	}
	if (IsChanged)
	{
		AdvancePakCreator->RelyonsForm->DemandViewRenovate();
	}

	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
