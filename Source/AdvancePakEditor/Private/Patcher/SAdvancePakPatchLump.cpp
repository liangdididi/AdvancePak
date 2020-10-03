// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakPatchLump.h"
#include "SlateOptMacros.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "AdvancePakEditorStyle.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SCheckBox.h"
#include "Utility/AdvancePakCreator.h"
#include "Forms/SAdvancePakGathersForm.h"
#include "Forms/SAdvancePakInvolveRegularsForm.h"
#include "Forms/SAdvancePakAbsolveRegularsForm.h"
#include "Forms/SAdvancePakInformalsForm.h"
#include "Forms/SAdvancePakAccordsForm.h"
#include "Forms/SAdvancePakPublishResourcesForm.h"
#include "Forms/SAdvancePakPatchResourcesForm.h"
#include "Utility/AdvancePakStateThread.h"

FName SAdvancePakPatchLump::PublishName(TEXT("Publish"));

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakPatchLump::Construct(const FArguments& InArgs)
{
	PatchItem = InArgs._PatchItem;
	AdvancePakCreator = InArgs._AdvancePakCreator;
	PatchEditType = EPatchEditType::None;

	ChildSlot
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[

			SNew(SBox)
			.Padding(4.f)
		.WidthOverride(200.f)
		[

			SNew(SBorder)
			[

				SNew(SOverlay)

				+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.ColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, 0.5f)))
		.Image(FAdvancePakEditorStyle::Get().GetBrush(TEXT("AdvancePakEditor.WhiteBoard")))
		.Visibility_Lambda([this]()->EVisibility {return PatchItem->IsSelected ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed; })
		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(4.f)
		[
			SAssignNew(NameEditor, SEditableTextBox)
			.HintText(OPTEXT("InputPatchName"))
		.Visibility_Lambda([this]()->EVisibility { return PatchEditType == EPatchEditType::Copy || PatchEditType == EPatchEditType::Delete || PatchEditType == EPatchEditType::None ? EVisibility::Collapsed : EVisibility::Visible; })
		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(CopyCheckBox, SCheckBox)
			.IsChecked(ECheckBoxState::Checked)
		.Visibility_Lambda([this]()->EVisibility { return PatchEditType == EPatchEditType::Copy ? EVisibility::Visible : EVisibility::Collapsed; })
		[
			SNew(STextBlock)
			.Text(OPTEXT(TEXT("CopyConfigure")))
		.ToolTipText(OPTEXT(TEXT("CopyConfigureFromFormerPatch")))
		]
		]

			]
		]

		]

	+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[


			SNew(SBox)
			.Padding(FMargin(4.f, 0.f, 4.f, 4.f))
		.WidthOverride(200.f)
		[

			SNew(SBorder)
			[

				SNew(SOverlay)

				+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.Image(FAdvancePakEditorStyle::Get().GetBrush(TEXT("AdvancePakEditor.WhiteBoard")))
		.ColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, 0.5f)))
		.Visibility_Lambda([this]()->EVisibility {return PatchItem->IsSelected ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed; })
		]

	+ SOverlay::Slot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			.IsEnabled(this, &SAdvancePakPatchLump::IsEditable)
		.Visibility_Lambda([this]()->EVisibility { return PatchEditType == EPatchEditType::None ? EVisibility::Collapsed : EVisibility::SelfHitTestInvisible; })

		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(4.f)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Text(OPTEXT("Sure"))
		.ToolTipText(OPTEXT("SureRenamePatch"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchLump::DealwithSureClicked)
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(4.f)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Text(OPTEXT("Cancel"))
		.ToolTipText(OPTEXT("CancelRenamePatch"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchLump::DealwithCancelClicked)
		]

		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			.Visibility_Lambda([this]()->EVisibility { return PatchEditType == EPatchEditType::None && PatchItem->FutureType == EAdvancePakPatchType::Patch ? EVisibility::Visible : EVisibility::Collapsed; })

		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(4.f)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Text(OPTEXT("Rename"))
		.ToolTipText(OPTEXT("RenamePatch"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchLump::DealwithRenameClicked)
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(4.f)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Text(OPTEXT("Delete"))
		.ToolTipText(OPTEXT("DeletePatch"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchLump::DealwithDeleteClicked)
		]

		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[

			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.FillWidth(0.5f)
		[
			SNullWidget::NullWidget
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(4.f)
		[

			SNew(SButton)
			.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Text(OPTEXT("Copy"))
		.ToolTipText(OPTEXT("CopyPatch"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchLump::DealwithCopyClicked)
		.Visibility_Lambda([this]()->EVisibility { return PatchEditType == EPatchEditType::None && PatchItem->FutureType == EAdvancePakPatchType::Copy ? EVisibility::Visible : EVisibility::Collapsed; })
		]

	+ SHorizontalBox::Slot()
		.FillWidth(0.5f)
		[
			SNullWidget::NullWidget
		]
		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[

			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.FillWidth(0.5f)
		[
			SNullWidget::NullWidget
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.Padding(4.f)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Text(OPTEXT("Add"))
		.ToolTipText(OPTEXT("AddPatch"))
		.ButtonStyle(FAdvancePakEditorStyle::Get(), TEXT("AdvancePakEditor.Button"))
		.OnClicked(this, &SAdvancePakPatchLump::DealwithAddClicked)
		.Visibility_Lambda([this]()->EVisibility { return PatchEditType == EPatchEditType::None && PatchItem->FutureType == EAdvancePakPatchType::Add ? EVisibility::Visible : EVisibility::Collapsed; })
		]

	+ SHorizontalBox::Slot()
		.FillWidth(0.5f)
		[
			SNullWidget::NullWidget
		]

		]

			]
		]


		]
		];


}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SAdvancePakPatchLump::IsEditable() const
{
	return !AdvancePakCreator->IsPackaging;
}

FReply SAdvancePakPatchLump::DealwithRenameClicked()
{
	PatchEditType = EPatchEditType::Rename;
	return FReply::Handled();
}

FReply SAdvancePakPatchLump::DealwithDeleteClicked()
{
	PatchEditType = EPatchEditType::Delete;
	return FReply::Handled();
}

FReply SAdvancePakPatchLump::DealwithCopyClicked()
{
	PatchEditType = EPatchEditType::Copy;
	return FReply::Handled();
}

FReply SAdvancePakPatchLump::DealwithAddClicked()
{
	PatchEditType = EPatchEditType::Add;
	return FReply::Handled();
}

FReply SAdvancePakPatchLump::DealwithCancelClicked()
{
	PatchEditType = EPatchEditType::None;
	return FReply::Handled();
}

void SAdvancePakPatchLump::RenovatePatchFutureViews()
{
	if (!AdvancePakCreator->IsActivedPatch())
		return;

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

void SAdvancePakPatchLump::RenovatePublishFutureViews()
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

void SAdvancePakPatchLump::RenovatePatchFormerViews()
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

void SAdvancePakPatchLump::RenovatePublishFormerViews()
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
						FPlatformProcess::Sleep(0.001f);

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

TSharedPtr<FAdvancePakPatchConfigure> SAdvancePakPatchLump::CreatePatchConfigureCopy(const TSharedPtr<FAdvancePakPatchConfigure>& PatchConfigure)
{
	TSharedPtr<FAdvancePakPatchConfigure> CopyConfigure = MakeShareable(new FAdvancePakPatchConfigure());
	CopyConfigure->Name = PatchConfigure->Name;

	for (auto InvolveGather : PatchConfigure->InvolveGathers)
	{
		if (!FPaths::DirectoryExists(InvolveGather->FullPath))
			continue;

		TSharedPtr<FAdvancePakGatherOption> CopyInvolveGather = MakeShareable(new FAdvancePakGatherOption());
		CopyInvolveGather->GatherPath = InvolveGather->GatherPath;
		CopyInvolveGather->FullPath = InvolveGather->FullPath;
		CopyInvolveGather->GatherType = InvolveGather->GatherType;
		CopyInvolveGather->GatherMode = InvolveGather->GatherMode;
		CopyInvolveGather->RelyonType = InvolveGather->RelyonType;
		CopyInvolveGather->CanGatherRegular = InvolveGather->CanGatherRegular;

		CopyConfigure->InvolveGathers.Add(CopyInvolveGather);
	}

	for (auto AbsolveGather : PatchConfigure->AbsolveGathers)
	{
		if (!FPaths::DirectoryExists(AbsolveGather->FullPath))
			continue;

		TSharedPtr<FAdvancePakGatherOption> CopyAbsolveGather = MakeShareable(new FAdvancePakGatherOption());
		CopyAbsolveGather->GatherPath = AbsolveGather->GatherPath;
		CopyAbsolveGather->FullPath = AbsolveGather->FullPath;
		CopyAbsolveGather->GatherType = AbsolveGather->GatherType;
		CopyAbsolveGather->GatherMode = AbsolveGather->GatherMode;
		CopyAbsolveGather->RelyonType = AbsolveGather->RelyonType;
		CopyAbsolveGather->CanGatherRegular = AbsolveGather->CanGatherRegular;

		CopyConfigure->AbsolveGathers.Add(CopyAbsolveGather);
	}

	for (auto InvolveRegular : PatchConfigure->InvolveRegulars)
	{
		if (!AdvancePakCreator->CheckRegularExist(InvolveRegular->PackageName))
			continue;

		TSharedPtr<FAdvancePakRegularOption> CopyInvolveRegular = MakeShareable(new FAdvancePakRegularOption());
		CopyInvolveRegular->PackageName = InvolveRegular->PackageName;
		CopyInvolveRegular->RelyonType = InvolveRegular->RelyonType;
		CopyInvolveRegular->GatherMode = InvolveRegular->GatherMode;

		CopyConfigure->InvolveRegulars.Add(CopyInvolveRegular);
	}

	for (auto AbsolveRegular : PatchConfigure->AbsolveRegulars)
	{
		if (!AdvancePakCreator->CheckRegularExist(AbsolveRegular))
			continue;

		CopyConfigure->AbsolveRegulars.Add(AbsolveRegular);
	}

	for (auto InvolveInformal : PatchConfigure->InvolveInformals)
	{
		if (!FPaths::FileExists(InvolveInformal->InformalPath))
			continue;

		CopyConfigure->InvolveInformals.Add(InvolveInformal);
	}

	for (auto AbsolveInformal : PatchConfigure->AbsolveInformals)
	{
		if (!FPaths::FileExists(AbsolveInformal->InformalPath))
			continue;

		CopyConfigure->InvolveInformals.Add(AbsolveInformal);
	}

	for (auto Regular : PatchConfigure->Regulars)
	{
		if (!AdvancePakCreator->CheckRegularExist(Regular))
			continue;

		CopyConfigure->Regulars.Add(Regular);
	}

	for (auto Informal : PatchConfigure->Informals)
	{
		if (!FPaths::FileExists(Informal->InformalPath))
			continue;

		CopyConfigure->Informals.Add(Informal);
	}

	return CopyConfigure;
}

FReply SAdvancePakPatchLump::DealwithSureClicked()
{
	if (PatchEditType == EPatchEditType::Delete)
	{
		if (AdvancePakCreator->FuturePatchs.Contains(PatchItem->Name))
			AdvancePakCreator->FuturePatchs.Remove(PatchItem->Name);

		if (AdvancePakCreator->FuturePublish->Patchs.Contains(PatchItem->Name))
			AdvancePakCreator->FuturePublish->Patchs.Remove(PatchItem->Name);

		AdvancePakCreator->IsFuturePublishDirty = true;

		FName OldName = AdvancePakCreator->ActiveName;

		AdvancePakCreator->OnTotalConfiguresChanged.Broadcast();

		if (OldName.IsEqual(AdvancePakCreator->ActiveName))
		{
			RenovatePublishFormerViews();
			RenovatePublishFutureViews();
			RenovatePatchFutureViews();
			RenovatePatchFormerViews();
		}
	}
	else if (PatchEditType == EPatchEditType::Copy)
	{
		TSharedPtr<FAdvancePakPatchConfigure> NewPatchConfigure;
		if (CopyCheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			TSharedPtr<FAdvancePakPatchConfigure> FormerPatchConfigure = AdvancePakCreator->GetFormerPatch(PatchItem->Name);
			NewPatchConfigure = CreatePatchConfigureCopy(FormerPatchConfigure);
			NewPatchConfigure->PakOptions = AdvancePakCreator->GetFuturePublish()->PakOptions;
		}
		else
		{
			NewPatchConfigure = MakeShareable(new FAdvancePakPatchConfigure());
			NewPatchConfigure->Name = PatchItem->Name;
			NewPatchConfigure->PakOptions = AdvancePakCreator->GetFuturePublish()->PakOptions;
		}

		AdvancePakCreator->FuturePublish->Patchs.Add(NewPatchConfigure->Name);

		AdvancePakCreator->FuturePatchs.Add(NewPatchConfigure->Name, NewPatchConfigure);

		AdvancePakCreator->IsFuturePublishDirty = true;

		FName OldName = AdvancePakCreator->ActiveName;

		AdvancePakCreator->OnTotalConfiguresChanged.Broadcast();

		if (OldName.IsEqual(AdvancePakCreator->ActiveName))
		{
			RenovatePublishFormerViews();
			RenovatePublishFutureViews();
			RenovatePatchFutureViews();
			RenovatePatchFormerViews();
		}
	}
	else if (PatchEditType == EPatchEditType::Rename || PatchEditType == EPatchEditType::Add)
	{
		FName PatchName = FName(*NameEditor->GetText().ToString());

		if (PatchName.IsNone())
		{
			NameEditor->SetText(FText());
			NameEditor->SetHintText(OPTEXT("Input Patch Name"));
			return FReply::Handled();
		}

		if (PatchName.IsEqual(PublishName) || AdvancePakCreator->GetFuturePublish()->Patchs.Contains(PatchName))
		{
			NameEditor->SetText(FText());
			NameEditor->SetHintText(OPTEXT("RepeatedPatchName"));
			return FReply::Handled();
		}

		if (PatchName.ToString().Contains(FString("_")))
		{
			NameEditor->SetText(FText());
			NameEditor->SetHintText(OPTEXT("DoNotContain"));
			return FReply::Handled();
		}

		if (PatchEditType == EPatchEditType::Rename)
		{
			TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = *AdvancePakCreator->FuturePatchs.Find(PatchItem->Name);

			AdvancePakCreator->FuturePublish->Patchs.Remove(PatchItem->Name);
			AdvancePakCreator->FuturePatchs.Remove(PatchItem->Name);

			PatchConfigure->Name = PatchName;

			AdvancePakCreator->FuturePublish->Patchs.Add(PatchName);
			AdvancePakCreator->FuturePatchs.Add(PatchName, PatchConfigure);

			AdvancePakCreator->IsFuturePublishDirty = true;

			FName OldName = AdvancePakCreator->ActiveName;

			AdvancePakCreator->OnTotalConfiguresChanged.Broadcast();

			if (OldName.IsEqual(AdvancePakCreator->ActiveName))
			{
				RenovatePublishFormerViews();
				RenovatePublishFutureViews();
				RenovatePatchFutureViews();
				RenovatePatchFormerViews();
			}
		}
		else
		{
			TSharedPtr<FAdvancePakPatchConfigure> NewPatchConfigure = MakeShareable(new FAdvancePakPatchConfigure());
			NewPatchConfigure->Name = PatchName;
			NewPatchConfigure->PakOptions = AdvancePakCreator->GetFuturePublish()->PakOptions;

			AdvancePakCreator->FuturePublish->Patchs.Add(NewPatchConfigure->Name);

			AdvancePakCreator->FuturePatchs.Add(NewPatchConfigure->Name, NewPatchConfigure);

			AdvancePakCreator->IsFuturePublishDirty = true;

			FName OldName = AdvancePakCreator->ActiveName;

			AdvancePakCreator->OnTotalConfiguresChanged.Broadcast();

			if (OldName.IsEqual(AdvancePakCreator->ActiveName))
			{
				RenovatePublishFormerViews();
				RenovatePublishFutureViews();
				RenovatePatchFutureViews();
				RenovatePatchFormerViews();
			}
		}
	}

	return FReply::Handled();
}


