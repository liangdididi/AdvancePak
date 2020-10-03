// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AdvancePakEditorCommands.h"

#define LOCTEXT_NAMESPACE "FAdvancePakEditorModule"

void FAdvancePakEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "AdvancePak", "Execute AdvancePakEditor action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
