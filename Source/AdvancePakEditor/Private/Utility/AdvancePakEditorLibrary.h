// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/AssetRegistryInterface.h"
#include "Editor/UnrealEd/Classes/Settings/ProjectPackagingSettings.h"
#include "AssetRegistryModule.h"
#include "AdvancePakLibrary.h"
#include "Utility/AdvancePakEditorType.h"
#include "AdvancePakEditorLibrary.generated.h"

class FAssetRegistryModule;
struct FAssetData;

/**
 *
 */
UCLASS()
class UAdvancePakEditorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static FString NeverCookPathsName;
	static FString HashName;
	static FString InformalNameName;
	static FString InformalPathName;
	static TArray<TSharedPtr<FString>> GatherModeOptions;
	static FString UbulkSuffixName;
	static FString UmapSuffixName;
	static FString CookedProjectPath;
	static FString CookedEnginePath;
	static FString CookedGameContentPath;
	static FString CookedGamePluginsPath;
	static FString CookedEngineContentPath;
	static FString CookedEnginePluginsPath;
	static FString GameStarPath;
	static FString ProjectStarPath;
	static FString EngineStartPath;
	static TMap<FString, EAdvancePakGatherType> GatherTypeMap;
	static TMap<FString, EAdvancePakRelyonType> RelyonTypeMap;
	static FString VersionName;
	static FString PlatformName;
	static FString MapsToBuildName;
	static FString AlwaysCookPathsName;
	static TMap<FString, EAdvancePakGatherMode> GatherModeMap;
	static FString UnrealPakPath;
	static FString UnrealRunUAT;
	static FString UnrealCmdPath;
	static FString OutputDirName;
	static FString ConfigDirName;
	static FString ContentSpacePath;
	static FString NameName;
	static FString InvolveGathersName;
	static FString AbsolveGathersName;
	static FString UassetSuffixName;
	static FString UexpSuffixName;
	static FString ScriptStartPath;
	static FString ContentStartPath;
	static TArray<TSharedPtr<FString>> RelyonTypeOptions;
	static FString CookedPlatformPath;
	static FString GatherPathName;
	static FString FullPathName;
	static FString RelyonTypeName;
	static FString GatherTypeName;
	static FString GatherModeName;
	static FString AbsolveRegularsName;
	static FString AbsolveInformalsName;
	static FString UfontSuffixName;
	static FString UtxtSuffixName;
	static FString UtxtmapSuffixName;
	static FString GameContentPath;
	static FString GamePluginsPath;
	static FString EngineContentPath;
	static TMap<FString, FText> LocTextMap;
	static FString FullEnginePluginsPath;
	static FString FullProjectPath;
	static FString FullEnginePath;
	static FString CullProjectPath;
	static FString CullEnginePath;
	static FString CookedPath;
	static IPlatformFile* PlatformFile;
	static TArray<TSharedPtr<FString>> GatherTypeOptions;
	static FString EnginePluginsPath;
	static FString FullGameContentPath;
	static FString FullGamePluginsPath;
	static FString FullEngineContentPath;
	static FString CanGatherRegularName;
	static FString PublishConfigName;
	static FString AdditionalPathsName;
	static FAssetRegistryModule* AssetRegistryModule;
	static FString InvolveRegularsName;
	static FString InvolveInformalsName;
	static FString PakFileListName;
	static FString PakCommandName;
	static FString PakOptionsName;
	static FString PackageDirName;
	static FString LastConfigName;
	static FString LineOptionsName;
	static FString FilePathName;
	static FString MountPathName;
	static FString RegularsName;
	static FString InformalsName;
	static FString PatchsName;
	static FString ResourceTypeName;
	static FString PackageNameName;

public:

	static void InitializeLibrary();

	static FString AbsorbFullToMount(const FString& MountPath);

	static TSharedPtr<FAdvancePakPatchConfigure> AbsorbJsonToPatchConfigure(const FString& JsonContext);

	static void CreateSaveFileNotify(const FString& NotifyTitle, const FString& FloderPath);

	static void GetResourceDatasByPackageName(const FName& PackageName, TArray<FAssetData>& OutResourceDatas);

	static FString AbsorbPatchConfigureToJson(TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure);

	static FString AbsorbPublishConfigureToJson(TSharedPtr<FAdvancePakPublishConfigure> PublishConfigure);

	static TSharedPtr<FAdvancePakPublishConfigure> AbsorbJsonToPublishConfigure(const FString& JsonContext);

	static EAssetRegistryDependencyType::Type AbsorbRelyonType(const EAdvancePakRelyonType& RelyonType);

	static void GetSingleGatherRegulars(const TSharedPtr<FAdvancePakGatherOption>& GatherInfo, TArray<FName>& OutRegulars);

	static void CreatePromptNotify(const FString& NotifyTitle, const bool& IsSucceed = true);

	static void GetResourceRelyons(const FName& PackageName, const EAdvancePakRelyonType& RelyonType, TArray<FName>& OutPackageNames);

	static void GetMultiGatherRegulars(const TArray<TSharedPtr<FAdvancePakGatherOption>>& GatherInfos, TArray<FName>& OutRegulars);

	static void GetSingleGatherInformals(const TSharedPtr<FAdvancePakGatherOption>& GatherInfo, TMap<FName, FString>& OutInformals);

	static FName AbsorbFullToInformal(const FString& FullPath);

	static FAssetData GetResourceDataByPackageName(const FName& PackageName);

	static FText LocText(const FString& KeyStr);

	static void AbsorbRegularsToPakFileList(const TArray<FName>& Regulars, TArray<TSharedPtr<FAdvancePakPakLine>>& PakFileList, TArray<FName>& LackRegulars);

	static void AbsorbInformalsToPakFileList(const TArray<TSharedPtr<FAdvancePakInformalInfo>>& Informals, TArray<TSharedPtr<FAdvancePakPakLine>>& PakFileList, TMap<FName, FString>& LackInformals);

	static FString AbsorbPrimaryPathToCookedPath(const FString& PrimaryPath);

	static EAdvancePakRelyonType AbsorbStringToRelyonType(const FString& StrValue);

	static void GetMultiGatherInformals(const TArray<TSharedPtr<FAdvancePakGatherOption>>& GatherInfos, TMap<FName, FString>& OutInformals);

	static EAdvancePakGatherMode AbsorbStringToGatherMode(const FString& StrValue);

};


class FAdvancePakInformalsVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	TArray<FString> FilePaths;
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{
		FString FilePath = FString(FilenameOrDirectory);
		if (!bIsDirectory && !(FilePath.EndsWith(UAdvancePakEditorLibrary::UassetSuffixName) || FilePath.EndsWith(UAdvancePakEditorLibrary::UexpSuffixName) || FilePath.EndsWith(UAdvancePakEditorLibrary::UbulkSuffixName) || FilePath.EndsWith(UAdvancePakEditorLibrary::UmapSuffixName) || FilePath.EndsWith(UAdvancePakEditorLibrary::UfontSuffixName) || FilePath.EndsWith(UAdvancePakEditorLibrary::UtxtSuffixName) || FilePath.EndsWith(UAdvancePakEditorLibrary::UtxtmapSuffixName)))
		{
			FilePaths.Add(FilePath);
		}
		return true;
	}
};

class FAdvancePakRegularsVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	FString DotFilePath;
	TArray<FString> FilePaths;
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{
		FString FilePath = FString(FilenameOrDirectory);
		if (!bIsDirectory && FilePath.StartsWith(DotFilePath))
		{
			FilePaths.Add(FilePath);
		}
		return true;
	}
};