// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "SAdvancePakPackageWork.h"
#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "SAdvancePakBuildPage.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Patcher/SAdvancePakPatchBar.h"
#include "Patcher/SAdvancePakPatchConfigure.h"
#include "Patcher/SAdvancePakPublishConfigure.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Utility/AdvancePakCreator.h"
#include "Utility/AdvancePakStateThread.h"
#include "Forms/SAdvancePakAbsolveRegularsForm.h"
#include "Forms/SAdvancePakAccordsForm.h"
#include "Forms/SAdvancePakCulturesForm.h"
#include "Forms/SAdvancePakGathersForm.h"
#include "Forms/SAdvancePakInformalsForm.h"
#include "Forms/SAdvancePakInvolveRegularsForm.h"
#include "Forms/SAdvancePakMapsForm.h"
#include "Forms/SAdvancePakPatchResourcesForm.h"
#include "Forms/SAdvancePakPlatformsForm.h"
#include "Patcher/SAdvancePakPatchLump.h"
#include "Forms/SAdvancePakPublishResourcesForm.h"
#include "Forms/SAdvancePakRelyonsForm.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAdvancePakPackageWork::Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel)
{
	AdvancePakCreator = InModel;

	ChildSlot
		[

			SNew(SBorder)
			.Padding(4.f)
		[

			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(OPTEXT("PatchPage"))
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		.ColorAndOpacity(FLinearColor(1.f, 0.f, 1.f))
		]
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SNew(SExpandableArea)
			.AreaTitle(OPTEXT("PatchConfigures"))
		.AreaTitleFont(FCoreStyle::GetDefaultFontStyle("Bold", 10))
		.InitiallyCollapsed(false)
		.Padding(8.0f)
		.BodyContent()
		[
			SNew(SBox)
			.HeightOverride(120.f)
		[
			SAssignNew(PatchBox, SAdvancePakPatchBar, InModel)
		]
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 4.f, 0.f, 0.f)
		[
			SNew(SExpandableArea)
			.AreaTitle(OPTEXT("ResourceConfigures"))
		.AreaTitleFont(FCoreStyle::GetDefaultFontStyle("Bold", 10))
		.InitiallyCollapsed(false)
		.Padding(8.0f)
		.BodyContent()
		[
			SNew(SBox)
			.HeightOverride_Raw(this, &SAdvancePakPackageWork::DealwithPackageHeight)

		[

			SNew(SOverlay)

			+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SAssignNew(PublishConfigureView, SAdvancePakPublishConfigure, InModel)
			.Visibility_Lambda([this]()->EVisibility { return  AdvancePakCreator->ActiveName.IsEqual(AdvancePakCreator->PublishName) ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed; })
		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SAssignNew(PatchConfigureView, SAdvancePakPatchConfigure, InModel)
			.Visibility_Lambda([this]()->EVisibility { return  AdvancePakCreator->IsActivedPatch() ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed; })
		]

		]
		]
		]
		]
		];

	AdvancePakCreator->AbsolveGathersForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);
	AdvancePakCreator->AbsolveGathersForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);

	AdvancePakCreator->FormerRegularsForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);
	AdvancePakCreator->FormerInformalsForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);
	AdvancePakCreator->FutureRegularsForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);
	AdvancePakCreator->FutureInformalsForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);
	AdvancePakCreator->FormerPublishInformalsForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);
	AdvancePakCreator->FormerPublishRegularsForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);

	AdvancePakCreator->InvolveInformalsForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);
	AdvancePakCreator->AbsolveInformalsForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);

	AdvancePakCreator->FuturePatchRegularsForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);
	AdvancePakCreator->FuturePatchInformalsForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);
	AdvancePakCreator->FormerPatchRegularsForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);
	AdvancePakCreator->FormerPatchInformalsForm->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);

	PatchBox->RenovateOne.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState);

	AdvancePakCreator->AbsolveRegularsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);

	AdvancePakCreator->FuturePatchRegularsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);
	AdvancePakCreator->FuturePatchInformalsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);
	AdvancePakCreator->FormerPatchRegularsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);
	AdvancePakCreator->FormerPatchInformalsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);

	AdvancePakCreator->InvolveGathersForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);
	AdvancePakCreator->AbsolveGathersForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);

	AdvancePakCreator->FormerRegularsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);
	AdvancePakCreator->FormerInformalsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);
	AdvancePakCreator->FutureRegularsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);
	AdvancePakCreator->FutureInformalsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);
	AdvancePakCreator->FormerPublishInformalsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);
	AdvancePakCreator->FormerPublishRegularsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);

	AdvancePakCreator->InvolveRegularsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);

	AdvancePakCreator->RelyonsForm->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);

	PatchBox->RenovateTwo.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState);

	AdvancePakCreator->AbsolveRegularsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);

	AdvancePakCreator->InvolveGathersForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);
	AdvancePakCreator->AbsolveGathersForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);

	AdvancePakCreator->FormerRegularsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);
	AdvancePakCreator->FormerInformalsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);
	AdvancePakCreator->FutureRegularsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);
	AdvancePakCreator->FutureInformalsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);
	AdvancePakCreator->FormerPublishInformalsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);
	AdvancePakCreator->FormerPublishRegularsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);

	AdvancePakCreator->FuturePatchRegularsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);
	AdvancePakCreator->FuturePatchInformalsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);
	AdvancePakCreator->FormerPatchRegularsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);
	AdvancePakCreator->FormerPatchInformalsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);

	AdvancePakCreator->InvolveRegularsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);

	AdvancePakCreator->RelyonsForm->RenovateThree.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState);

	AdvancePakCreator->InvolveGathersForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);
	AdvancePakCreator->AbsolveGathersForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);

	AdvancePakCreator->AbsolveRegularsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);

	AdvancePakCreator->FuturePatchRegularsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);
	AdvancePakCreator->FuturePatchInformalsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);
	AdvancePakCreator->FormerPatchRegularsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);
	AdvancePakCreator->FormerPatchInformalsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);

	AdvancePakCreator->InvolveRegularsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);

	AdvancePakCreator->FormerRegularsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);
	AdvancePakCreator->FormerInformalsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);
	AdvancePakCreator->FutureRegularsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);
	AdvancePakCreator->FutureInformalsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);
	AdvancePakCreator->FormerPublishInformalsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);
	AdvancePakCreator->FormerPublishRegularsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);

	AdvancePakCreator->RelyonsForm->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);

	PatchBox->RenovateFour.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm);

	AdvancePakCreator->InvolveGathersForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);
	AdvancePakCreator->AbsolveGathersForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);

	PatchBox->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);

	AdvancePakCreator->InvolveInformalsForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);
	AdvancePakCreator->AbsolveInformalsForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);

	AdvancePakCreator->FuturePatchRegularsForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);
	AdvancePakCreator->FuturePatchInformalsForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);
	AdvancePakCreator->FormerPatchRegularsForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);
	AdvancePakCreator->FormerPatchInformalsForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);

	AdvancePakCreator->FormerPublishInformalsForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);
	AdvancePakCreator->FormerPublishRegularsForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);

	AdvancePakCreator->FormerRegularsForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);
	AdvancePakCreator->FormerInformalsForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);
	AdvancePakCreator->FutureRegularsForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);
	AdvancePakCreator->FutureInformalsForm->RenovateFive.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm);

	AdvancePakCreator->InvolveGathersForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);
	AdvancePakCreator->AbsolveGathersForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);

	AdvancePakCreator->FormerRegularsForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);
	AdvancePakCreator->FormerInformalsForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);
	AdvancePakCreator->FutureRegularsForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);
	AdvancePakCreator->FutureInformalsForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);
	AdvancePakCreator->FormerPublishInformalsForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);
	AdvancePakCreator->FormerPublishRegularsForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);

	AdvancePakCreator->InvolveInformalsForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);
	AdvancePakCreator->AbsolveInformalsForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);

	AdvancePakCreator->FuturePatchRegularsForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);
	AdvancePakCreator->FuturePatchInformalsForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);
	AdvancePakCreator->FormerPatchRegularsForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);
	AdvancePakCreator->FormerPatchInformalsForm->RenovateSix.BindRaw(this, &SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState);

	PatchBox->PromoteOne.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveGathersRegulars);

	AdvancePakCreator->InvolveGathersForm->PromoteOne.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveGathersRegulars);
	AdvancePakCreator->AbsolveGathersForm->PromoteOne.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveGathersRegulars);

	PatchBox->PromoteTwo.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveGathersInformals);

	AdvancePakCreator->InvolveGathersForm->PromoteTwo.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveGathersInformals);
	AdvancePakCreator->AbsolveGathersForm->PromoteTwo.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveGathersInformals);

	PatchBox->PromoteThree.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveGathersInformals);

	AdvancePakCreator->InvolveGathersForm->PromoteThree.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveGathersInformals);
	AdvancePakCreator->AbsolveGathersForm->PromoteThree.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveGathersInformals);

	PatchBox->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);

	AdvancePakCreator->InvolveInformalsForm->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);
	AdvancePakCreator->AbsolveInformalsForm->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);

	AdvancePakCreator->FormerRegularsForm->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);
	AdvancePakCreator->FormerInformalsForm->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);
	AdvancePakCreator->FutureRegularsForm->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);
	AdvancePakCreator->FutureInformalsForm->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);
	AdvancePakCreator->FormerPublishInformalsForm->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);
	AdvancePakCreator->FormerPublishRegularsForm->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);

	AdvancePakCreator->FuturePatchRegularsForm->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);
	AdvancePakCreator->FuturePatchInformalsForm->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);
	AdvancePakCreator->FormerPatchRegularsForm->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);
	AdvancePakCreator->FormerPatchInformalsForm->PromoteFour.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveInformalsInformals);

	PatchBox->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);

	AdvancePakCreator->FuturePatchRegularsForm->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);
	AdvancePakCreator->FuturePatchInformalsForm->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);
	AdvancePakCreator->FormerPatchRegularsForm->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);
	AdvancePakCreator->FormerPatchInformalsForm->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);

	AdvancePakCreator->InvolveRegularsForm->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);

	AdvancePakCreator->FormerRegularsForm->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);
	AdvancePakCreator->FormerInformalsForm->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);
	AdvancePakCreator->FutureRegularsForm->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);
	AdvancePakCreator->FutureInformalsForm->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);
	AdvancePakCreator->FormerPublishInformalsForm->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);
	AdvancePakCreator->FormerPublishRegularsForm->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);

	AdvancePakCreator->RelyonsForm->PromoteFive.BindRaw(this, &SAdvancePakPackageWork::PromoteInvolveRegularsRegulars);

	AdvancePakCreator->InvolveInformalsForm->PromoteSix.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveInformalsInformals);
	AdvancePakCreator->AbsolveInformalsForm->PromoteSix.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveInformalsInformals);

	PatchBox->PromoteSix.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveInformalsInformals);

	AdvancePakCreator->FuturePatchRegularsForm->PromoteSix.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveInformalsInformals);
	AdvancePakCreator->FuturePatchInformalsForm->PromoteSix.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveInformalsInformals);
	AdvancePakCreator->FormerPatchRegularsForm->PromoteSix.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveInformalsInformals);
	AdvancePakCreator->FormerPatchInformalsForm->PromoteSix.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveInformalsInformals);

	PatchBox->PromoteSeven.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveRegularsRegulars);

	AdvancePakCreator->FuturePatchRegularsForm->PromoteSeven.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveRegularsRegulars);
	AdvancePakCreator->FuturePatchInformalsForm->PromoteSeven.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveRegularsRegulars);
	AdvancePakCreator->FormerPatchRegularsForm->PromoteSeven.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveRegularsRegulars);
	AdvancePakCreator->FormerPatchInformalsForm->PromoteSeven.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveRegularsRegulars);

	AdvancePakCreator->AbsolveRegularsForm->PromoteSeven.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveRegularsRegulars);

	AdvancePakCreator->RelyonsForm->PromoteSeven.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveRegularsRegulars);

	PatchBox->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);

	AdvancePakCreator->InvolveGathersForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);
	AdvancePakCreator->AbsolveGathersForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);

	AdvancePakCreator->InvolveInformalsForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);
	AdvancePakCreator->AbsolveInformalsForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);

	AdvancePakCreator->FuturePatchRegularsForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);
	AdvancePakCreator->FuturePatchInformalsForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);
	AdvancePakCreator->FormerPatchRegularsForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);
	AdvancePakCreator->FormerPatchInformalsForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);

	AdvancePakCreator->FormerRegularsForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);
	AdvancePakCreator->FormerInformalsForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);
	AdvancePakCreator->FutureRegularsForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);
	AdvancePakCreator->FutureInformalsForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);
	AdvancePakCreator->FormerPublishInformalsForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);
	AdvancePakCreator->FormerPublishRegularsForm->PromoteEight.BindRaw(this, &SAdvancePakPackageWork::PromotePatchInformals);

	AdvancePakCreator->InvolveGathersForm->PromoteNine.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveGathersRegulars);
	AdvancePakCreator->AbsolveGathersForm->PromoteNine.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveGathersRegulars);

	PatchBox->PromoteNine.BindRaw(this, &SAdvancePakPackageWork::PromoteAbsolveGathersRegulars);

	AdvancePakCreator->AbsolveRegularsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);

	AdvancePakCreator->InvolveGathersForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);
	AdvancePakCreator->AbsolveGathersForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);

	AdvancePakCreator->InvolveRegularsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);

	AdvancePakCreator->FuturePatchRegularsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);
	AdvancePakCreator->FuturePatchInformalsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);
	AdvancePakCreator->FormerPatchRegularsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);
	AdvancePakCreator->FormerPatchInformalsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);

	AdvancePakCreator->FormerRegularsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);
	AdvancePakCreator->FormerInformalsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);
	AdvancePakCreator->FutureRegularsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);
	AdvancePakCreator->FutureInformalsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);
	AdvancePakCreator->FormerPublishInformalsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);
	AdvancePakCreator->FormerPublishRegularsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);

	AdvancePakCreator->RelyonsForm->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);

	PatchBox->PromoteTen.BindRaw(this, &SAdvancePakPackageWork::PromotePatchRegulars);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


FOptionalSize SAdvancePakPackageWork::DealwithPackageHeight() const
{
	if (AdvancePakCreator->ActiveName.IsNone())
	{
		return 0.f;
	}
	return 800.f;
}

void SAdvancePakPackageWork::RenovatePatchFormerPublishInformalsState()
{
	if (AdvancePakCreator->PatchFormerPublishInformalsStateThread.IsValid())
	{
		return;
	}

	AdvancePakCreator->PatchFormerPublishInformalsStateThread = MakeShareable(new FAdvancePakStateThread(TEXT("PatchFormerPublishInformalsStateTask"), [this]() {

		AdvancePakCreator->IsPatchFormerPublishInformalsStateRunning = true;

		TMap<FName, TPair<FName, FString>> FreeResources;
		TMap<FName, TPair<FName, FString>> AllotResources;
		TMap<FName, TPair<FName, FString>> InValidResources;
		for (auto Informal : AdvancePakCreator->FormerPublishInformals)
		{
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
			AdvancePakCreator->FormerPublishInformals[i]->ResourceName = It.Key();
			AdvancePakCreator->FormerPublishInformals[i]->PatchName = It.Value().Key;
			AdvancePakCreator->FormerPublishInformals[i]->ResourcePath = It.Value().Value;
			AdvancePakCreator->FormerPublishInformals[i]->ResourceState = EAdvancePakResourceState::Free;
			++i;
		}
		for (TMap<FName, TPair<FName, FString>>::TIterator It(AllotResources); It; ++It)
		{
			AdvancePakCreator->FormerPublishInformals[i]->ResourceName = It.Key();
			AdvancePakCreator->FormerPublishInformals[i]->PatchName = It.Value().Key;
			AdvancePakCreator->FormerPublishInformals[i]->ResourcePath = It.Value().Value;
			AdvancePakCreator->FormerPublishInformals[i]->ResourceState = EAdvancePakResourceState::Allot;
			++i;
		}
		for (TMap<FName, TPair<FName, FString>>::TIterator It(InValidResources); It; ++It)
		{
			AdvancePakCreator->FormerPublishInformals[i]->ResourceName = It.Key();
			AdvancePakCreator->FormerPublishInformals[i]->PatchName = It.Value().Key;
			AdvancePakCreator->FormerPublishInformals[i]->ResourcePath = It.Value().Value;
			AdvancePakCreator->FormerPublishInformals[i]->ResourceState = EAdvancePakResourceState::InValid;
			++i;
		}

		AdvancePakCreator->FormerPublishInformalsForm->DemandSelectRenovate();

	}));

	AdvancePakCreator->PatchFormerPublishInformalsStateThread->StateCompleteDelagete.AddLambda([this]() {
		FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			AdvancePakCreator->IsPatchFormerPublishInformalsStateRunning = false;
			AdvancePakCreator->PatchFormerPublishInformalsStateThread.Reset();
		}, TStatId(), NULL, ENamedThreads::GameThread);
	});
	AdvancePakCreator->PatchFormerPublishInformalsStateThread->Execute();
}

void SAdvancePakPackageWork::RenovatePatchFormerPublishRegularsState()
{
	if (AdvancePakCreator->PatchFormerPublishRegularsStateThread.IsValid())
	{
		return;
	}

	AdvancePakCreator->PatchFormerPublishRegularsStateThread = MakeShareable(new FAdvancePakStateThread(TEXT("PatchFormerPublishRegularsStateTask"), [this]() {

		AdvancePakCreator->IsPatchFormerPublishRegularsStateRunning = true;

		TMap<FName, FName> FreeResources;
		TMap<FName, FName> AllotResources;
		TMap<FName, FName> InValidResources;
		for (auto Regular : AdvancePakCreator->FormerPublishRegulars)
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
			AdvancePakCreator->FormerPublishRegulars[i]->ResourceName = It.Key();
			AdvancePakCreator->FormerPublishRegulars[i]->PatchName = It.Value();
			AdvancePakCreator->FormerPublishRegulars[i]->ResourceState = EAdvancePakResourceState::Free;
			++i;
		}
		for (TMap<FName, FName>::TIterator It(AllotResources); It; ++It)
		{
			AdvancePakCreator->FormerPublishRegulars[i]->ResourceName = It.Key();
			AdvancePakCreator->FormerPublishRegulars[i]->PatchName = It.Value();
			AdvancePakCreator->FormerPublishRegulars[i]->ResourceState = EAdvancePakResourceState::Allot;
			++i;
		}
		for (TMap<FName, FName>::TIterator It(InValidResources); It; ++It)
		{
			AdvancePakCreator->FormerPublishRegulars[i]->ResourceName = It.Key();
			AdvancePakCreator->FormerPublishRegulars[i]->PatchName = It.Value();
			AdvancePakCreator->FormerPublishRegulars[i]->ResourceState = EAdvancePakResourceState::InValid;
			++i;
		}

		AdvancePakCreator->FormerPublishRegularsForm->DemandSelectRenovate();

	}));

	AdvancePakCreator->PatchFormerPublishRegularsStateThread->StateCompleteDelagete.AddLambda([this]() {
		FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			AdvancePakCreator->IsPatchFormerPublishRegularsStateRunning = false;
			AdvancePakCreator->PatchFormerPublishRegularsStateThread.Reset();
		}, TStatId(), NULL, ENamedThreads::GameThread);
	});
	AdvancePakCreator->PatchFormerPublishRegularsStateThread->Execute();
}

void SAdvancePakPackageWork::RenovatePatchFormerPatchRegularsState()
{
	if (AdvancePakCreator->PatchFormerPatchRegularsStateThread.IsValid())
	{
		return;
	}

	AdvancePakCreator->PatchFormerPatchRegularsStateThread = MakeShareable(new FAdvancePakStateThread(TEXT("PatchFormerPatchRegularsStateTask"), [this]() {

		AdvancePakCreator->IsPatchFormerPatchRegularsStateRunning = true;

		TArray<FName> FreeResources;
		TArray<FName> AllotResources;
		TArray<FName> InValidResources;
		for (auto Regular : AdvancePakCreator->FormerPatchRegulars)
		{
			switch (AdvancePakCreator->GetRegularResourceState(Regular->ResourceName))
			{
			case EAdvancePakResourceState::Free:
				FreeResources.Add(Regular->ResourceName);
				break;
			case EAdvancePakResourceState::Allot:
				AllotResources.Add(Regular->ResourceName);
				break;
			case EAdvancePakResourceState::InValid:
				InValidResources.Add(Regular->ResourceName);
				break;
			}
		}
		int i = 0;
		for (int j = 0; j < FreeResources.Num(); ++i, ++j)
		{
			AdvancePakCreator->FormerPatchRegulars[i]->ResourceName = FreeResources[j];
			AdvancePakCreator->FormerPatchRegulars[i]->ResourceState = EAdvancePakResourceState::Free;
		}
		for (int j = 0; j < AllotResources.Num(); ++i, ++j)
		{
			AdvancePakCreator->FormerPatchRegulars[i]->ResourceName = AllotResources[j];
			AdvancePakCreator->FormerPatchRegulars[i]->ResourceState = EAdvancePakResourceState::Allot;
		}
		for (int j = 0; j < InValidResources.Num(); ++i, ++j)
		{
			AdvancePakCreator->FormerPatchRegulars[i]->ResourceName = InValidResources[j];
			AdvancePakCreator->FormerPatchRegulars[i]->ResourceState = EAdvancePakResourceState::InValid;
		}

		AdvancePakCreator->FormerPatchRegularsForm->DemandSelectRenovate();
	}));

	AdvancePakCreator->PatchFormerPatchRegularsStateThread->StateCompleteDelagete.AddLambda([this]() {
		FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			AdvancePakCreator->IsPatchFormerPatchRegularsStateRunning = false;
			AdvancePakCreator->PatchFormerPatchRegularsStateThread.Reset();
		}, TStatId(), NULL, ENamedThreads::GameThread);
	});
	AdvancePakCreator->PatchFormerPatchRegularsStateThread->Execute();
}

void SAdvancePakPackageWork::RenovatePatchFuturePatchRegularsForm()
{
	if (!AdvancePakCreator->IsActivedPatch())
	{
		return;
	}

	if (!AdvancePakCreator->FuturePublish.IsValid() || !AdvancePakCreator->FuturePatchs.Contains(AdvancePakCreator->ActiveName))
	{
		AdvancePakCreator->FuturePatchRegulars.Reset();
		AdvancePakCreator->FuturePatchRegularsForm->DemandViewRenovate();
		return;
	}

	AdvancePakCreator->FuturePatchRegularsForm->DemandViewRenovate();
}

void SAdvancePakPackageWork::RenovatePatchFuturePatchInformalsForm()
{
	if (!AdvancePakCreator->IsActivedPatch())
		return;

	if (!AdvancePakCreator->FuturePublish.IsValid() || !AdvancePakCreator->FuturePatchs.Contains(AdvancePakCreator->ActiveName))
	{
		AdvancePakCreator->FuturePatchInformals.Reset();
		AdvancePakCreator->FuturePatchInformalsForm->DemandViewRenovate();
		return;
	}

	AdvancePakCreator->FuturePatchInformalsForm->DemandViewRenovate();
}

void SAdvancePakPackageWork::RenovatePatchFormerPatchInformalsState()
{
	if (AdvancePakCreator->PatchFormerPatchInformalsStateThread.IsValid())
	{
		return;
	}

	AdvancePakCreator->PatchFormerPatchInformalsStateThread = MakeShareable(new FAdvancePakStateThread(TEXT("PatchFormerPatchInformalsStateTask"), [this]() {

		AdvancePakCreator->IsPatchFormerPatchInformalsStateRunning = true;

		TMap<FName, FString> FreeResources;
		TMap<FName, FString> AllotResources;
		TMap<FName, FString> InValidResources;
		for (auto Informal : AdvancePakCreator->FormerPatchInformals)
		{
			switch (AdvancePakCreator->GetInformalResourceState(Informal->ResourceName, Informal->ResourcePath))
			{
			case EAdvancePakResourceState::Free:
				FreeResources.Add(Informal->ResourceName, Informal->ResourcePath);
				break;
			case EAdvancePakResourceState::Allot:
				AllotResources.Add(Informal->ResourceName, Informal->ResourcePath);
				break;
			case EAdvancePakResourceState::InValid:
				InValidResources.Add(Informal->ResourceName, Informal->ResourcePath);
				break;
			}
		}
		int i = 0;
		for (TMap<FName, FString>::TIterator It(FreeResources); It; ++It)
		{
			AdvancePakCreator->FormerPatchInformals[i]->ResourceName = It.Key();
			AdvancePakCreator->FormerPatchInformals[i]->ResourcePath = It.Value();
			AdvancePakCreator->FormerPatchInformals[i]->ResourceState = EAdvancePakResourceState::Free;
			++i;
		}
		for (TMap<FName, FString>::TIterator It(AllotResources); It; ++It)
		{
			AdvancePakCreator->FormerPatchInformals[i]->ResourceName = It.Key();
			AdvancePakCreator->FormerPatchInformals[i]->ResourcePath = It.Value();
			AdvancePakCreator->FormerPatchInformals[i]->ResourceState = EAdvancePakResourceState::Allot;
			++i;
		}
		for (TMap<FName, FString>::TIterator It(InValidResources); It; ++It)
		{
			AdvancePakCreator->FormerPatchInformals[i]->ResourceName = It.Key();
			AdvancePakCreator->FormerPatchInformals[i]->ResourcePath = It.Value();
			AdvancePakCreator->FormerPatchInformals[i]->ResourceState = EAdvancePakResourceState::InValid;
			++i;
		}

		AdvancePakCreator->FormerPatchInformalsForm->DemandSelectRenovate();
	}));

	AdvancePakCreator->PatchFormerPatchInformalsStateThread->StateCompleteDelagete.AddLambda([this]() {
		FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			AdvancePakCreator->IsPatchFormerPatchInformalsStateRunning = false;
			AdvancePakCreator->PatchFormerPatchInformalsStateThread.Reset();
		}, TStatId(), NULL, ENamedThreads::GameThread);
	});
	AdvancePakCreator->PatchFormerPatchInformalsStateThread->Execute();
}

void SAdvancePakPackageWork::RenovateRelyonsPatchRecursion(const TSharedPtr<FAdvancePakRelyonInfo>& RelyonInfo)
{
	RelyonInfo->PatchName = AdvancePakCreator->GetFutureRegularPatchName(RelyonInfo->PackageName);
	for (TMap<FName, TSharedPtr<FAdvancePakRelyonInfo>>::TIterator It(RelyonInfo->Relyons); It; ++It)
	{
		RenovateRelyonsPatchRecursion(It.Value());
	}
}

void SAdvancePakPackageWork::PromoteInvolveGathersRegulars()
{
	TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = AdvancePakCreator->GetFuturePatch();
	AdvancePakCreator->InvolveGathersRegulars.Reset();
	UAdvancePakEditorLibrary::GetMultiGatherRegulars(PatchConfigure->InvolveGathers, AdvancePakCreator->InvolveGathersRegulars);
}

void SAdvancePakPackageWork::PromoteInvolveGathersInformals()
{
	TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = AdvancePakCreator->GetFuturePatch();
	AdvancePakCreator->InvolveGathersInformals.Reset();
	UAdvancePakEditorLibrary::GetMultiGatherInformals(PatchConfigure->InvolveGathers, AdvancePakCreator->InvolveGathersInformals);
}

void SAdvancePakPackageWork::PromoteAbsolveGathersInformals()
{
	TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = AdvancePakCreator->GetFuturePatch();
	AdvancePakCreator->AbsolveGathersInformals.Reset();
	UAdvancePakEditorLibrary::GetMultiGatherInformals(PatchConfigure->AbsolveGathers, AdvancePakCreator->AbsolveGathersInformals);
}

void SAdvancePakPackageWork::PromoteInvolveInformalsInformals()
{
	TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = AdvancePakCreator->GetFuturePatch();
	AdvancePakCreator->InvolveInformalsInformals.Reset();
	for (auto InvolveInformal : PatchConfigure->InvolveInformals)
	{
		AdvancePakCreator->InvolveInformalsInformals.Add(InvolveInformal->InformalName, InvolveInformal->InformalPath);
	}
}

void SAdvancePakPackageWork::PromoteInvolveRegularsRegularsRecursion(const FName& ResourceName, const EAdvancePakRelyonType& RelyonType, TArray<FName>& RecursionRegulars)
{
	RecursionRegulars.AddUnique(ResourceName);

	TArray<FName> RelyonRegulars;
	UAdvancePakEditorLibrary::GetResourceRelyons(ResourceName, RelyonType, RelyonRegulars);
	for (auto PackageName : RelyonRegulars)
	{
		if (!RecursionRegulars.Contains(PackageName))
		{
			PromoteInvolveRegularsRegularsRecursion(PackageName, RelyonType, RecursionRegulars);
		}
	}
}

void SAdvancePakPackageWork::PromoteInvolveRegularsRegulars()
{
	TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = AdvancePakCreator->GetFuturePatch();
	AdvancePakCreator->InvolveRegularsRegulars.Reset();
	TArray<FName> RecursionRegulars;
	for (auto InvolveRegular : PatchConfigure->InvolveRegulars)
	{
		if (InvolveRegular->GatherMode == EAdvancePakGatherMode::Local)
		{
			AdvancePakCreator->InvolveRegularsRegulars.AddUnique(InvolveRegular->PackageName);
			TArray<FName> CacheRegulars;
			UAdvancePakEditorLibrary::GetResourceRelyons(InvolveRegular->PackageName, InvolveRegular->RelyonType, CacheRegulars);
			for (auto PackageName : CacheRegulars)
			{
				AdvancePakCreator->InvolveRegularsRegulars.AddUnique(PackageName);
			}
		}
		else
		{
			PromoteInvolveRegularsRegularsRecursion(InvolveRegular->PackageName, InvolveRegular->RelyonType, RecursionRegulars);
		}
	}
	for (auto PackageName : RecursionRegulars)
	{
		AdvancePakCreator->InvolveRegularsRegulars.AddUnique(PackageName);
	}
}

void SAdvancePakPackageWork::PromoteAbsolveInformalsInformals()
{
	TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = AdvancePakCreator->GetFuturePatch();
	AdvancePakCreator->AbsolveInformalsInformals.Reset();
	for (auto AbsolveInformal : PatchConfigure->AbsolveInformals)
	{
		AdvancePakCreator->AbsolveInformalsInformals.Add(AbsolveInformal->InformalName, AbsolveInformal->InformalPath);
	}
}

void SAdvancePakPackageWork::PromoteAbsolveRegularsRegulars()
{
	TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = AdvancePakCreator->GetFuturePatch();
	AdvancePakCreator->AbsolveRegularsRegulars.Reset();
	AdvancePakCreator->AbsolveRegularsRegulars.Append(PatchConfigure->AbsolveRegulars);
}

void SAdvancePakPackageWork::PromotePatchInformals()
{
	AdvancePakCreator->PatchFutureInformals.Reset();
	AdvancePakCreator->PatchFutureInformals.Append(AdvancePakCreator->InvolveGathersInformals);
	for (TMap<FName, FString>::TIterator It(AdvancePakCreator->AbsolveGathersInformals); It; ++It)
	{
		AdvancePakCreator->PatchFutureInformals.Remove(It.Key());
	}
	for (TMap<FName, FString>::TIterator It(AdvancePakCreator->InvolveInformalsInformals); It; ++It)
	{
		if (AdvancePakCreator->PatchFutureInformals.Contains(It.Key()))
		{
			continue;
		}
		AdvancePakCreator->PatchFutureInformals.Add(It.Key(), It.Value());
	}
	for (TMap<FName, FString>::TIterator It(AdvancePakCreator->AbsolveInformalsInformals); It; ++It)
	{
		AdvancePakCreator->PatchFutureInformals.Remove(It.Key());
	}

	for (auto Informal : AdvancePakCreator->FuturePublish->Informals)
	{
		AdvancePakCreator->PatchFutureInformals.Remove(Informal->InformalName);
	}

	for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(AdvancePakCreator->FuturePatchs); It; ++It)
	{
		if (It.Key().IsEqual(AdvancePakCreator->ActiveName))
		{
			continue;
		}
		for (auto Informal : It.Value()->Informals)
		{
			AdvancePakCreator->PatchFutureInformals.Remove(Informal->InformalName);
		}
	}

	TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = AdvancePakCreator->GetFuturePatch();

	AdvancePakCreator->FuturePatchInformals.Reset();
	PatchConfigure->Informals.Reset();
	for (TMap<FName, FString>::TIterator It(AdvancePakCreator->PatchFutureInformals); It; ++It)
	{
		TSharedPtr<FAdvancePakPatchResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPatchResourceInfo());
		ResourceInfo->ResourceName = It.Key();
		ResourceInfo->ResourcePath = It.Value();
		ResourceInfo->ResourceState = EAdvancePakResourceState::Free;
		AdvancePakCreator->FuturePatchInformals.Add(ResourceInfo);

		TSharedPtr<FAdvancePakInformalInfo> InformalInfo = MakeShareable(new FAdvancePakInformalInfo());
		InformalInfo->InformalName = It.Key();
		InformalInfo->InformalPath = It.Value();
		PatchConfigure->Informals.Add(InformalInfo);
	}
}

void SAdvancePakPackageWork::PromoteAbsolveGathersRegulars()
{
	TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = AdvancePakCreator->GetFuturePatch();
	AdvancePakCreator->AbsolveGathersRegulars.Reset();
	UAdvancePakEditorLibrary::GetMultiGatherRegulars(PatchConfigure->AbsolveGathers, AdvancePakCreator->AbsolveGathersRegulars);
}

void SAdvancePakPackageWork::PromotePatchRegulars()
{
	AdvancePakCreator->PatchFutureRegulars.Reset();
	AdvancePakCreator->PatchFutureRegulars.Append(AdvancePakCreator->InvolveGathersRegulars);
	for (auto PackageName : AdvancePakCreator->AbsolveGathersRegulars)
	{
		AdvancePakCreator->PatchFutureRegulars.Remove(PackageName);
	}
	for (auto PackageName : AdvancePakCreator->InvolveRegularsRegulars)
	{
		AdvancePakCreator->PatchFutureRegulars.AddUnique(PackageName);
	}
	for (auto PackageName : AdvancePakCreator->AbsolveRegularsRegulars)
	{
		AdvancePakCreator->PatchFutureRegulars.Remove(PackageName);
	}
	for (auto PackageName : AdvancePakCreator->FuturePublish->Regulars)
	{
		AdvancePakCreator->PatchFutureRegulars.Remove(PackageName);
	}

	for (TMap<FName, TSharedPtr<FAdvancePakPatchConfigure>>::TIterator It(AdvancePakCreator->FuturePatchs); It; ++It)
	{
		if (It.Key().IsEqual(AdvancePakCreator->ActiveName))
		{
			continue;
		}
		for (auto PackageName : It.Value()->Regulars)
		{
			AdvancePakCreator->PatchFutureRegulars.Remove(PackageName);
		}
	}

	TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure = AdvancePakCreator->GetFuturePatch();

	AdvancePakCreator->FuturePatchRegulars.Reset();
	PatchConfigure->Regulars.Reset();
	for (auto PackageName : AdvancePakCreator->PatchFutureRegulars)
	{
		TSharedPtr<FAdvancePakPatchResourceInfo> ResourceInfo = MakeShareable(new FAdvancePakPatchResourceInfo());
		ResourceInfo->ResourceName = PackageName;
		ResourceInfo->ResourceState = EAdvancePakResourceState::Free;
		AdvancePakCreator->FuturePatchRegulars.Add(ResourceInfo);

		PatchConfigure->Regulars.Add(PackageName);
	}

	for (auto Relyon : AdvancePakCreator->Relyons)
	{
		RenovateRelyonsPatchRecursion(Relyon);
	}
}
