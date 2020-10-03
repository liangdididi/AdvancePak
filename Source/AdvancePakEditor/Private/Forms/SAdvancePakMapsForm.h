// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SGridPanel;
class FAdvancePakCreator;

namespace EShowMapsChoices
{
	enum Type
	{
		/** Show all available maps. */
		ShowAllMaps,

		/** Only show maps that are to be cooked. */
		ShowCookedMaps
	};
}

/**
 *
 */
class SAdvancePakMapsForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakMapsForm)
	{}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

	const TArray<FString>& GetAllSelectMaps();

protected:

	void DealwithCheckBoxCheckStateChanged(ECheckBoxState NewState, int Index);

	ECheckBoxState DealwithCheckBoxIsChecked(int Index) const;

	void DealwithSelectLinkwork(bool IsAll);

	void DealwithShowCheckBoxCheckStateChanged(ECheckBoxState NewState, EShowMapsChoices::Type Choice);

	ECheckBoxState DealwithShowCheckBoxIsChecked(EShowMapsChoices::Type Choice) const;

	void RenovateMapList();

protected:

	TArray<FString> MapList;

	TArray<FString> SelectMaps;

	TSharedPtr<SGridPanel> MapGridPanel;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	/** Holds the current 'Show maps' check box choice. */
	EShowMapsChoices::Type ShowMapsChoice;
};
