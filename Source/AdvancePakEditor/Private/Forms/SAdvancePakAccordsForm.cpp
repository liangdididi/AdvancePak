// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakAccordsForm.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SHyperlink.h"
#include "Utility/AdvancePakCreator.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Images/SImage.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor.h"
#include "Raws/SAdvancePakAccordsRaw.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SBorder.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakAccordsForm::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;
	AccordsList = InArgs._ListItemsSource;
	IsPublish = InArgs._IsPublish;

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
			SAssignNew(AccordsListView, SListView<TSharedPtr<FAdvancePakAccordInfo>>)
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
		.ListItemsSource(AccordsList)
		.OnGenerateRow(this, &SAdvancePakAccordsForm::DealwithAccordViewPromoteRow)
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
			.IsEnabled_Lambda([this]() { return AdvancePakCreator->GetFuturePublish().IsValid(); })

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SImage)
			.Image(FEditorStyle::GetBrush(TEXT("Icons.Error")))
		.Visibility(this, &SAdvancePakAccordsForm::DealwithPromptVisibility)
		]

	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.HAlign(HAlign_Left)
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SAssignNew(AccordPromptText, STextBlock)
			.ColorAndOpacity(FLinearColor(1.f, 0.f, 0.f))
		.Text(OPTEXT("ThoseResourceDidnotCookedOrExsit"))
		.Visibility(this, &SAdvancePakAccordsForm::DealwithPromptVisibility)
		]


	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 0.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakAccordsForm::DealwithAccordPublishLinkwork)
		.Text(OPTEXT("AccordPublish"))
		.ToolTipText(OPTEXT("AccordPublishResources"))
		.Visibility_Lambda([this]()->EVisibility { return IsPublish ? EVisibility::Visible : EVisibility::Collapsed; })
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 8.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakAccordsForm::DealwithAccordAllLinkwork)
		.Text(OPTEXT("AccordAll"))
		.ToolTipText(OPTEXT("AccordAllResources"))
		.Visibility_Lambda([this]()->EVisibility { return IsPublish ? EVisibility::Visible : EVisibility::Collapsed; })
		]


	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.f, 8.f, 0.f)
		[
			SNew(SHyperlink)
			.OnNavigate(this, &SAdvancePakAccordsForm::DealwithAccordPatchLinkwork)
		.Text(OPTEXT("Accord"))
		.ToolTipText(OPTEXT("AccordPatchResources"))
		.IsEnabled_Lambda([this]() { return (IsPublish && AdvancePakCreator->GetFuturePublish().IsValid()) || (!IsPublish && AdvancePakCreator->GetFuturePatch().IsValid()); })
		.Visibility_Lambda([this]()->EVisibility { return IsPublish ? EVisibility::Collapsed : EVisibility::Visible; })
		]

		]
		]
		];
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION


TSharedRef<ITableRow> SAdvancePakAccordsForm::DealwithAccordViewPromoteRow(TSharedPtr<FAdvancePakAccordInfo> AccordInfo, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SAdvancePakAccordsRaw, this)
		.OwnerTableView(OwnerTable)
		.AdvancePakCreator(AdvancePakCreator)
		.AccordInfo(AccordInfo)
		;
}

void SAdvancePakAccordsForm::DealwithAccordPatchLinkwork()
{
	PromotePatchAccords(AdvancePakCreator->ActiveName);
	RenovatePatchAccordsForm();
}

void SAdvancePakAccordsForm::PromotePublishAccords()
{
	AdvancePakCreator->PublishAccords.Reset();
	AdvancePakCreator->IsPublishAccordFaild = false;
	AdvancePakCreator->IsPublishReduced = false;

	for (auto PakLine : AdvancePakCreator->FuturePublish->PakFileList)
	{
		if (IFileManager::Get().FileExists(*PakLine->FilePath))
			continue;

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

void SAdvancePakAccordsForm::RenovatePublishAccordsForm()
{
	if (!AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName))
		return;

	if (!AdvancePakCreator->FuturePublish.IsValid())
	{
		AdvancePakCreator->PublishAccords.Reset();
		AdvancePakCreator->PublishAccordsForm->DemandViewRenovate();
		return;
	}

	AdvancePakCreator->PublishAccordsForm->DemandViewRenovate(AdvancePakCreator->IsPublishAccordFaild);
}

void SAdvancePakAccordsForm::RenovatePatchAccordsForm()
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

void SAdvancePakAccordsForm::PromotePatchAccords(const FName& PatchName /*= NAME_None*/)
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

void SAdvancePakAccordsForm::DemandViewRenovate(bool IsFailed)
{
	IsAccordFailed = IsFailed;
	AccordsListView->RequestListRefresh();
}

void SAdvancePakAccordsForm::DealwithAccordPublishLinkwork()
{
	PromotePublishAccords();
	RenovatePublishAccordsForm();
}

EVisibility SAdvancePakAccordsForm::DealwithPromptVisibility() const
{
	return IsAccordFailed ? EVisibility::SelfHitTestInvisible : EVisibility::Hidden;
}

void SAdvancePakAccordsForm::DealwithAccordAllLinkwork()
{
	PromotePublishAccords();

	if (AdvancePakCreator->IsPublishAccordFaild)
	{
		UAdvancePakEditorLibrary::CreatePromptNotify(OPSTRING("PublishResourcesLacked"), false);
	}
	else
	{
		TArray<TSharedPtr<FAdvancePakPakLine>> FutureAllPakList;
		FutureAllPakList.Append(AdvancePakCreator->FuturePublish->PakFileList);

		bool IsPatchAccordFaild = false;
		for (auto PatchName : AdvancePakCreator->FuturePublish->Patchs)
		{
			PromotePatchAccords(PatchName);
			if (AdvancePakCreator->IsPatchAccordFaild)
			{
				AdvancePakCreator->PublishAccords.Reset();
				AdvancePakCreator->PublishAccords.Append(AdvancePakCreator->PatchAccords);
				AdvancePakCreator->IsPublishAccordFaild = true;
				UAdvancePakEditorLibrary::CreatePromptNotify(PatchName.ToString() + OPSTRING("ResourcesLacked"), false);
				IsPatchAccordFaild = true;
			}
			FutureAllPakList.Append(AdvancePakCreator->GetFuturePatch(PatchName)->PakFileList);
		}

		if (!IsPatchAccordFaild)
		{
			if (AdvancePakCreator->FormerPublish.IsValid())
			{
				TArray<TSharedPtr<FAdvancePakPakLine>> FormerAllPakList;
				FormerAllPakList.Append(AdvancePakCreator->FormerPublish->PakFileList);

				for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(AdvancePakCreator->FormerPatchs); It; ++It)
				{
					FormerAllPakList.Append(It.Value()->PakFileList);
				}

				TArray<TSharedPtr<FAdvancePakAccordInfo>> AddAccords;
				TArray<TSharedPtr<FAdvancePakAccordInfo>> UpdateAccords;
				TArray<TSharedPtr<FAdvancePakAccordInfo>> KeepAccords;
				TArray<TSharedPtr<FAdvancePakAccordInfo>> DeleteAccords;
				for (auto FuturePakLine : FutureAllPakList)
				{
					EAdvancePakAccordType AccordType = EAdvancePakAccordType::Add;
					for (auto FormerPakLine : FormerAllPakList)
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
				for (auto FormerPakLine : FormerAllPakList)
				{
					bool IsDeleted = true;
					for (auto FuturePakLine : FutureAllPakList)
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

				AdvancePakCreator->PublishAccords.Reset();

				AdvancePakCreator->PublishAccords.Append(AddAccords);
				AdvancePakCreator->PublishAccords.Append(UpdateAccords);
				AdvancePakCreator->PublishAccords.Append(KeepAccords);
				AdvancePakCreator->PublishAccords.Append(DeleteAccords);
			}
			else
			{
				AdvancePakCreator->PublishAccords.Reset();
				for (auto PakLine : FutureAllPakList)
				{
					TSharedPtr<FAdvancePakAccordInfo> AccordInfo = MakeShareable(new FAdvancePakAccordInfo());
					AccordInfo->Name = PakLine->MountPath;
					AccordInfo->Path = PakLine->FilePath;
					AccordInfo->AccordType = EAdvancePakAccordType::Add;
					AdvancePakCreator->PublishAccords.Add(AccordInfo);
				}
			}
		}
	}

	RenovatePublishAccordsForm();
}