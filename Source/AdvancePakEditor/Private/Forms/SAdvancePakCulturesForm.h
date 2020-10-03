// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SGridPanel;
class FAdvancePakCreator;
/**
 *
 */
class SAdvancePakCulturesForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakCulturesForm)
	{}
	SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

	const TArray<FString>& GetAllSelectCultures();

protected:

	ECheckBoxState DealwithCheckBoxIsChecked(int Index) const;

	void DealwithSelectLinkwork(bool IsAll);

	void DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState, int Index);

protected:

	TSharedPtr<SGridPanel> CultureGridPanel;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TArray<FString> CulturesList;

	TArray<FString> SelectCultures;

};
