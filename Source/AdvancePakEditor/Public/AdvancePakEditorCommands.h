// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AdvancePakEditorStyle.h"

class FAdvancePakEditorCommands : public TCommands<FAdvancePakEditorCommands>
{
public:

	FAdvancePakEditorCommands()
		: TCommands<FAdvancePakEditorCommands>(TEXT("AdvancePakEditor"), NSLOCTEXT("Contexts", "AdvancePakEditor", "AdvancePakEditor Plugin"), NAME_None, FAdvancePakEditorStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
