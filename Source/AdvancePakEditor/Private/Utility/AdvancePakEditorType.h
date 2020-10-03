// Fill out your copyright notice in the Futurecription page of Project Configures.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "AdvancePakType.h"
#include "Editor/UnrealEd/Classes/Settings/ProjectPackagingSettings.h"
#include "AdvancePakEditorType.generated.h"

class FAdvancePakInformalInfo;

DECLARE_LOG_CATEGORY_EXTERN(AdvancePakLog, Log, All);

#define OPTEXT(KeyStr) UAdvancePakEditorLibrary::LocText(KeyStr)
#define OPSTRING(KeyStr) UAdvancePakEditorLibrary::LocText(KeyStr).ToString()

/**
 *
 */


UENUM()
enum class EAdvancePakGatherMode : uint8
{
	Local = 0,
	Each = 1
};

UENUM()
enum class EAdvancePakResourceState : uint8
{
	Free = 0,
	Allot,
	InValid
};

UENUM()
enum class EAdvancePakRelyonType : uint8
{
	Nothing = 0,
	Soft,
	Hard,
	Searchable,
	SoftManage,
	HardManage,
	Manage,
	Packages,
	All
};

UENUM()
enum class EAdvancePakGatherType : uint8
{
	Regular = 0,
	Informal,
	Both
};

UENUM()
enum class EAdvancePakPatchType : uint8
{
	None = 0,
	Publish,
	Patch,
	Copy,
	Add,
};

class FAdvancePakPatchItem
{
public:
	bool IsSelected;
	FName Name;
	EAdvancePakPatchType FormerType;
	EAdvancePakPatchType FutureType;
	FAdvancePakPatchItem() : IsSelected(false) {}
};

class FAdvancePakPublishResourceInfo
{
public:
	FName ResourceName;
	FString ResourcePath;
	FName PatchName;
	EAdvancePakResourceState ResourceState;
};

class FAdvancePakPatchResourceInfo
{
public:
	FName ResourceName;
	FString ResourcePath;
	EAdvancePakResourceState ResourceState;
};

class FAdvancePakRelyonInfo
{
public:
	FName PackageName;
	FName PatchName;
	EAdvancePakRelyonType RelyonType;
	bool IsTop;
	TMap<FName, TSharedPtr<FAdvancePakRelyonInfo>> Relyons;
};

UENUM()
enum class EAdvancePakAccordType : uint8
{
	Add = 0,
	Update,
	Keep,
	Delete
};

class FAdvancePakAccordInfo
{
public:
	EAdvancePakAccordType AccordType;
	bool IsRegular;
	FString Name;
	FString Path;
};

class FAdvancePakPakLine
{
public:
	FString FilePath;
	FString MountPath;
	FString Hash;
};

class FAdvancePakPublishConfigure
{
public:
	TArray<FString> NeverCookPaths;
	TArray<FName> Regulars;
	TArray<TSharedPtr<FAdvancePakInformalInfo>> Informals;
	TArray<FName> Patchs;
	TArray<FString> AdditionalPaths;
	FString PackageDir;
	FString LastConfig;
	FString ConfigDir;
	FString Version;
	FString Platform;
	TArray<FString> MapsToBuild;
	TArray<FString> AlwaysCookPaths;
	FString PakOptions;
	FString LineOptions;
	TArray<TSharedPtr<FAdvancePakPakLine>> PakFileList;
	FString PakCommand;
	FString OutputDir;
};

class FAdvancePakGatherOption
{
public:
	FString GatherPath;
	EAdvancePakGatherType GatherType;
	EAdvancePakGatherMode GatherMode;
	bool CanGatherRegular;
	FString FullPath;
	EAdvancePakRelyonType RelyonType;
};

class FAdvancePakRegularOption
{
public:
	FName PackageName;
	EAdvancePakRelyonType RelyonType;
	EAdvancePakGatherMode GatherMode;
};

class FAdvancePakInformalInfo
{
public:
	FName InformalName;
	FString InformalPath;
};

class FAdvancePakPatchConfigure
{
public:
	TArray<TSharedPtr<FAdvancePakInformalInfo>> AbsolveInformals;
	TArray<TSharedPtr<FAdvancePakPakLine>> PakFileList;
	TArray<FName> Regulars;
	FName Name;
	TArray<TSharedPtr<FAdvancePakGatherOption>> InvolveGathers;
	TArray<TSharedPtr<FAdvancePakGatherOption>> AbsolveGathers;
	TArray<TSharedPtr<FAdvancePakInformalInfo>> Informals;
	FString PakOptions;
	TArray<TSharedPtr<FAdvancePakRegularOption>> InvolveRegulars;
	TArray<FName> AbsolveRegulars;
	TArray<TSharedPtr<FAdvancePakInformalInfo>> InvolveInformals;
};

UCLASS(config = Game)
class UCookAdvancedConfigures : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(config, EditAnywhere, Category = AdvancePakCookConfigures, meta = (DisplayName = "迭代烘焙:仅烘焙上次烘焙后修改的内容"))
		bool bCookIterate;

	UPROPERTY(config, EditAnywhere, Category = AdvancePakCookConfigures, meta = (DisplayName = "烘焙打包配置下的所有资源"))
		bool bCookAll;

	UPROPERTY(config, EditAnywhere, Category = AdvancePakCookConfigures, meta = (DisplayName = "压缩内容"))
		bool bCompressed;

	UPROPERTY(config, EditAnywhere, Category = AdvancePakCookConfigures, meta = (DisplayName = "保存无版本的包"))
		bool bUnVersioned;

	UPROPERTY(config, EditAnywhere, Category = AdvancePakCookConfigures, meta = (DisplayName = "烘焙输出目录"))
		FDirectoryPath CookOutputDirectory;

	UPROPERTY(config, EditAnywhere, Category = AdvancePakCookConfigures, meta = (DisplayName = "额外的烘焙选项"))
		FString AdvancedOptions;

};


UCLASS()
class UAdvancePakBuildConfigures : public UObject
{
	GENERATED_BODY()

public:

	UAdvancePakBuildConfigures();

	FORCEINLINE static UAdvancePakBuildConfigures* Get()
	{
		static bool bInitialized = false;
		UAdvancePakBuildConfigures* DefaultConfigures = GetMutableDefault<UAdvancePakBuildConfigures>();
		DefaultConfigures->AddToRoot();
		if (!bInitialized)
		{
			bInitialized = true;
		}

		return DefaultConfigures;
	}

public:

	UPROPERTY(EditAnywhere, meta = (DisplayName = "版本号"))
		FString Version;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "编译配置"))
		TEnumAsByte<EProjectPackagingBuildConfigurations> BuildConfig;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "打包版本中要包括的地图列表", RelativeToGameContentDir, LongPackageName))
		TArray<FFilePath> MapsToBuild;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "要烘焙的额外资源目录", LongPackageName))
		TArray<FDirectoryPath> AlwaysCookPaths;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "永不烘焙的目录", LongPackageName))
		TArray<FDirectoryPath> NeverCookPaths;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "要打包的额外非资源目录", RelativeToGameContentDir))
		TArray<FDirectoryPath> AdditionalPaths;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "是否复制增益配置"))
		bool IsDuplicateDatas;

	DECLARE_DELEGATE(FOnBuildConfiguresChanged)

	FOnBuildConfiguresChanged OnBuildConfigurationChanged;

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif
};


UCLASS()
class UAdvancePakEditorType : public UObject
{
	GENERATED_BODY()

public:

	template<typename TEnum>
	static FString GetEnumNameByValue(const FString& Name, const TEnum& Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!EnumPtr)
			return FString("InValid");
		return EnumPtr->GetNameStringByIndex((int32)Value);
	}

	template<typename TEnum>
	static TEnum GetEnumValueByName(const FString& Name, const FString& Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!EnumPtr) {
			return TEnum(0);
		}
		return (TEnum)EnumPtr->GetIndexByNameString(Value);
	}
};