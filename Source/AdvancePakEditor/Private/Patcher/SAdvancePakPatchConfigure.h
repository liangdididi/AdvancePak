// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class FAdvancePakCreator;

/**
 *
 */
class SAdvancePakPatchConfigure : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPatchConfigure)
	{}
	SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

protected:

	FOptionalSize DealwithScrollWidth() const;

	FReply DealwithInterfaceChanged(int32 FaceIndex);

protected:

	TSharedPtr<SGridPanel> ConfigureGrid;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	float ScrollWidth;
};
