// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Utility/AdvancePakCreator.h"

class SEditableTextBox;
class IDetailsView;
class FAdvancePakProcThread;
class UCookAdvancedConfigures;
class SNotificationItem;
class SEditableTextBox;
class SAdvancePakPlatformsForm;
class SAdvancePakCulturesForm;
class SAdvancePakMapsForm;

/**
 * Implements the profile page for the session launcher wizard.
 */
class SAdvancePakCookPage
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SAdvancePakCookPage) { }
	SLATE_END_ARGS()

public:

	/**
	 * Destructor.
	 */
	~SAdvancePakCookPage();

public:

	/**
	 * Constructs the widget.
	 *
	 * @param InArgs The Slate argument list.
	 * @param InModel The data model.
	 */
	void Construct(	const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel );

private:

	void CreateAdvancedConfigureDetailView();

	FReply StartCookResources();

	bool StartCookResourcesEnabled();

	void DealwithCookProcResult(FString OutputLog);

	void DealwithCookProcBegin();

	void DealwithCookProcSucceed();

	void DealwithCookProcFailed();

	void DealwithUatLinkwork();

	void DealwithUatCancelButtonClicked();

	void DealwithDismissButtonClicked();

private:

	TSharedPtr<IDetailsView> AdvancedConfiguresView;

	TWeakPtr<SNotificationItem> NotificationItemPtr;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	UCookAdvancedConfigures* CookAdvancedConfigures;

	TSharedPtr<FAdvancePakProcThread> CookProcTaskThread;

	TSharedPtr<SAdvancePakPlatformsForm> PlatformsView;

	TSharedPtr<SAdvancePakCulturesForm> CulturesView;

	TSharedPtr<SAdvancePakMapsForm> MapsView;
};
