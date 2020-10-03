// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AdvancePakEditor.h"
#include "AdvancePakEditorStyle.h"
#include "AdvancePakEditorCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ContentBrowserModule.h"
#include "ContentBrowserDelegates.h"
#include "LevelEditor.h"
#include "AdvancePakLibrary.h"
#include "AssetRegistryModule.h"
#include "ILauncherServicesModule.h"
#include "ITargetDeviceServicesModule.h"
#include "Utility/AdvancePakCreator.h"
#include "SAdvancePakPage.h"

static const FName AdvancePakEditorTabName("AdvancePakEditor");

#define LOCTEXT_NAMESPACE "FAdvancePakEditorModule"

void FAdvancePakEditorModule::StartupModule()
{
	FAdvancePakEditorStyle::Initialize();
	FAdvancePakEditorStyle::ReloadTextures();

	FAdvancePakEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAdvancePakEditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FAdvancePakEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FAdvancePakEditorModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FAdvancePakEditorModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AdvancePakEditorTabName, FOnSpawnTab::CreateRaw(this, &FAdvancePakEditorModule::OnSpawnAdvancePakEditor))
		.SetDisplayName(LOCTEXT("FAdvancePakEditorTitle", "AdvancePakEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FAdvancePakEditorModule::ShutdownModule()
{

	FAdvancePakEditorStyle::Shutdown();

	FAdvancePakEditorCommands::Unregister();
}

void FAdvancePakEditorModule::PluginButtonClicked()
{
	TSharedRef<SDockTab> DockTab = FGlobalTabmanager::Get()->InvokeTab(AdvancePakEditorTabName);
}

void FAdvancePakEditorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FAdvancePakEditorCommands::Get().PluginAction);
}

void FAdvancePakEditorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FAdvancePakEditorCommands::Get().PluginAction);
}

TSharedRef<class SDockTab> FAdvancePakEditorModule::OnSpawnAdvancePakEditor(const class FSpawnTabArgs& SpawnTabArgs)
{
	const TSharedRef<SDockTab> DockTab = SNew(SDockTab)
		.Icon(FAdvancePakEditorStyle::Get().GetBrush("AdvancePakEditor.SmallAction"))
		.TabRole(ETabRole::NomadTab);

	ILauncherServicesModule& ProjectLauncherServicesModule = FModuleManager::LoadModuleChecked<ILauncherServicesModule>("LauncherServices");

	TSharedRef<FAdvancePakCreator> AdvancePakCreator = MakeShareable(new FAdvancePakCreator(
		ProjectLauncherServicesModule.GetProfileManager()
	));

	DockTab->SetContent(SNew(SAdvancePakPage, AdvancePakCreator));

	return DockTab;
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAdvancePakEditorModule, AdvancePakEditor)