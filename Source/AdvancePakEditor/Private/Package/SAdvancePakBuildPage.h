// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Utility/AdvancePakEditorType.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/SListView.h"

class SEditableTextBox;
class SMultiLineEditableTextBox;
class IDetailsView;
class SAdvancePakPlatformsForm;
class FAdvancePakProcThread;
class SNotificationItem;

/**
 *
 */
class SAdvancePakBuildPage : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SAdvancePakBuildPage)
	{}
	SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, const TSharedRef<FAdvancePakCreator>& InModel);

protected:

	void CreateBuildConfiguresView();

	bool IsEditable() const;

	bool IsBuildAble() const;

	bool IsExportAble() const;

	FReply DealwithImportConfigClicked();

	FReply DealwithExportConfigClicked();

	FReply DealwithBuildPublishClicked();

	void OnOutputDirCommitted(const FText& InText, ETextCommit::Type CommitInfo);

	FReply DealwithOutputDirButtonClicked();

	void OnConfigDirCommitted(const FText& InText, ETextCommit::Type CommitInfo);

	FReply DealwithConfigDirButtonClicked();

	void OnBuildOptionCommitted(const FText& InText, ETextCommit::Type CommitInfo);

	void OnBuildConfigureChanged();

	FString GetBuildPlatformOptions(const FString& PlatformName);

	void OnBuildTaskOutput(FString OutputLog);

	void OnBuildTaskBegin();

	void OnBuildTaskSucceed();

	void OnBuildTaskFailed();

	void RestoreDefaultPackageConfigure();

	TSharedPtr<FAdvancePakPublishConfigure> CreateBuildConfiguresPtr();

	void DealwithUatLinkwork();

	void DealwithUatCancelButtonClicked();

	void DealwithDismissButtonClicked();

	FName AbsorbMountPathToPackageName(FString MountPath);

	FString AbsorbCommamdToOptions(const FString& PakCommamd);

	FString AbsorbBuildConfigToString(TEnumAsByte<EProjectPackagingBuildConfigurations> BuildConfig);

protected:

	TSharedPtr<SEditableTextBox> OutputDirEditor;

	TSharedPtr<SEditableTextBox> ConfigDirEditor;

	TSharedPtr<SMultiLineEditableTextBox> BuildOptionsEditor;

	FString OutputPath;

	FString ConfigPath;

	TSharedPtr<IDetailsView> BuildConfiguresView;

	UAdvancePakBuildConfigures* BuildConfiguresObject;

	TSharedPtr<SAdvancePakPlatformsForm> BuildPlatformsView;

	mutable TSharedPtr<FAdvancePakProcThread> BuildTaskThread;

	TArray<FFilePath> DefaultMapsToCook;

	TArray<FDirectoryPath> DefaultDirectoriesToAlwaysCook;

	TArray<FDirectoryPath> DefaultDirectoriesToNeverCook;

	TArray<FDirectoryPath> DefaultDirectoriesToAlwaysStageAsUFS;

	FString CreatePakCommand;

	TSharedPtr<FAdvancePakCreator> AdvancePakCreator;

	TWeakPtr<SNotificationItem> NotificationItemPtr;

	FString ImportVersion;

	FString ImportConfigPath;

	TArray<FName> ImportPatchs;

	FString ImportPackageDir;
};
