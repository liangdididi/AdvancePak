// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AdvancePakEditorStyle.h"
#include "AdvancePakEditor.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr< FSlateStyleSet > FAdvancePakEditorStyle::StyleInstance = NULL;

void FAdvancePakEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FAdvancePakEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FAdvancePakEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("AdvancePakEditorStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

const FVector2D Icon2x2(2.0f, 2.0f);
const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon64x64(64.0f, 64.0f);

const FLinearColor SelectionColor_Normal(0.3f, 0.3f, 0.5f);
const FLinearColor SelectionColor_Hovered(0.728f, 0.364f, 0.003f);
const FLinearColor SelectionColor_Pressed(0.701f, 0.525f, 0.003f);

TSharedRef< FSlateStyleSet > FAdvancePakEditorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("AdvancePakEditorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("AdvancePak")->GetBaseDir() / TEXT("Resources"));

	Style->Set("AdvancePakEditor.PluginAction", new IMAGE_BRUSH(TEXT("ButtonIcon_40x"), Icon40x40));
	Style->Set("AdvancePakEditor.SmallAction", new IMAGE_BRUSH(TEXT("ButtonIcon_16x"), Icon16x16));
	Style->Set("AdvancePakEditor.WhiteBoard", new IMAGE_BRUSH(TEXT("White_2x"), Icon2x2));

	const FButtonStyle TotalButtonStyle = FButtonStyle()
		.SetNormal(IMAGE_BRUSH("White_2x", Icon64x64, SelectionColor_Normal))
		.SetHovered(IMAGE_BRUSH("White_2x", Icon64x64, SelectionColor_Hovered))
		.SetPressed(IMAGE_BRUSH("White_2x", Icon64x64, SelectionColor_Pressed));
	Style->Set("AdvancePakEditor.Button", TotalButtonStyle);

	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FAdvancePakEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FAdvancePakEditorStyle::Get()
{
	return *StyleInstance;
}
