// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Internationalization/Text.h"
#include "Framework/SlateDelegates.h"
#include "Layout/Visibility.h"
#include "SlateFwd.h"
#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class FAdvancePakCreator;
class SWidget;


/**
 * Implements the launcher settings widget.
 */
class SAdvancePakPage
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SAdvancePakPage) { }
	SLATE_EVENT(FOnClicked, OnCloseClicked)
	SLATE_END_ARGS()

public:

	/** Default constructor. */
	SAdvancePakPage();

	/** Futuretructor. */
	~SAdvancePakPage();

public:

	/**
	 * Constructs the widget.
	 *
	 * @param InArgs The Slate argument list.
	 * @param InModel The data model.
	 */
	void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

	void EnterEditMode();


private:

	/** Holds a pointer to the data model. */
	TSharedPtr<FAdvancePakCreator> Model;

	/** Holds a delegate to be invoked when this panel is closed. */
	FOnClicked OnCloseClicked;

	/** Holds a delegate to be invoked when a rerun of the profile is requested. */
	FOnClicked OnRerunClicked;

	/** Hold a pointer to the launch profile name edit box. */
	TSharedPtr<SInlineEditableTextBlock> NameEditBox;
};
