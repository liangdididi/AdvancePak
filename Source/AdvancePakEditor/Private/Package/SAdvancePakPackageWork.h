// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class FAdvancePakCreator;
class SAdvancePakPatchBar;
class SAdvancePakPatchConfigure;
class SAdvancePakPublishConfigure;
/**
 * 
 */
class ADVANCEPAKEDITOR_API SAdvancePakPackageWork : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPackageWork)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);


protected:

	FOptionalSize DealwithPackageHeight() const;

	void RenovatePatchFormerPublishInformalsState();

	void RenovatePatchFormerPublishRegularsState();

	void RenovatePatchFormerPatchRegularsState();

	void RenovatePatchFuturePatchRegularsForm();

	void RenovatePatchFuturePatchInformalsForm();

	void RenovatePatchFormerPatchInformalsState();

	void RenovateRelyonsPatchRecursion(const TSharedPtr<FAdvancePakRelyonInfo>& RelyonInfo);

	void PromoteInvolveGathersRegulars();

	void PromoteInvolveGathersInformals();

	void PromoteAbsolveGathersInformals();

	void PromoteInvolveInformalsInformals();

	void PromoteInvolveRegularsRegularsRecursion(const FName& ResourceName, const EAdvancePakRelyonType& RelyonType, TArray<FName>& RecursionRegulars);

	void PromoteInvolveRegularsRegulars();

	void PromoteAbsolveInformalsInformals();

	void PromoteAbsolveRegularsRegulars();

	void PromotePatchInformals();

	void PromoteAbsolveGathersRegulars();

	void PromotePatchRegulars();

protected:

	TSharedPtr<SAdvancePakPatchBar> PatchBox;

	TSharedPtr<SAdvancePakPatchConfigure> PatchConfigureView;

	TSharedPtr<SAdvancePakPublishConfigure> PublishConfigureView;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

};
