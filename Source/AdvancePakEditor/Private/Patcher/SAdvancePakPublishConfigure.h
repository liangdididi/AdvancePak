// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class FAdvancePakCreator;
class SGridPanel;

/**
 *
 */
class SAdvancePakPublishConfigure : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdvancePakPublishConfigure)
	{}
	SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

protected:

	FOptionalSize DealwithScrollWidth() const;

	FReply DealwithInterfaceChanged(int32 FaceIndex);

protected:

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TSharedPtr<SGridPanel> ConfigureGrid;

	float ScrollWidth;

};
