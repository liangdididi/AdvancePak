// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SGridPanel;
class FAdvancePakCreator;

DECLARE_DELEGATE(FAdvancePakPlatfromSelected)

/**
 *
 */
class SAdvancePakPlatformsForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPlatformsForm) : _IsSingleMode(false)
	{}
	SLATE_ATTRIBUTE(bool, IsSingleMode)
		SLATE_EVENT(FAdvancePakPlatfromSelected, OnPlatformSelected)
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

	void AddSelectPlatform(const FString& InPlatform);

	const TArray<FString>& GetAllSelectPlatforms();

	void RemoveSelectPlatform(const FString& InPlatform);

	FString GetSelectPlatform();

protected:

	void DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState, int Index);

	ECheckBoxState DealwithCheckBoxIsChecked(int Index) const;

	void DealwithSelectLinkwork(bool IsAll);

protected:

	TArray<FString> PlatformList;

	TSharedPtr<SGridPanel> PlatformGridPanel;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TArray<FString> SelectPlatforms;

	bool IsSingleMode;

	FAdvancePakPlatfromSelected OnPlatformSelected;
};
