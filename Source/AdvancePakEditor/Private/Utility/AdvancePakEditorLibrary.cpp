// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "AdvancePakEditorLibrary.h"
#include "Json.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Misc/SecureHash.h"
#include "Interfaces/IPluginManager.h"




FText UAdvancePakEditorLibrary::LocText(const FString& KeyStr)
{
	if (LocTextMap.Contains(KeyStr))
	{
		return *LocTextMap.Find(KeyStr);
	}
	return FText();
}

EAdvancePakGatherMode UAdvancePakEditorLibrary::AbsorbStringToGatherMode(const FString& StrValue)
{
	return *GatherModeMap.Find(StrValue);
}

void UAdvancePakEditorLibrary::InitializeLibrary()
{
	AssetRegistryModule = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	PlatformFile = &FPlatformFileManager::Get().GetPlatformFile();

#if PLATFORM_WINDOWS
	UnrealPakPath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir()) / FString("Binaries") / FString("Win64") / FString("UnrealPak.exe");
	UnrealCmdPath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir()) / FString("Binaries") / FString("Win64") / FString("UE4Editor-Cmd.exe");
	UnrealRunUAT = FPaths::ConvertRelativePathToFull(FPaths::EngineDir()) / FString("Build") / FString("BatchFiles") / FString("RunUAT.bat");
#endif

#if PLATFORM_MAC
	UnrealPakPath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir()) / FString("Binaries") / FString("Mac") / FString("UnrealPak");
	UnrealCmdPath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir()) / FString("Binaries") / FString("Mac") / FString("UE4Editor.app") / FString("Contents") / FString("MacOS") / FString("UE4Editor");
	UnrealRunUAT = FPaths::ConvertRelativePathToFull(FPaths::EngineDir()) / FString("Build") / FString("BatchFiles") / FString("RunUAT.command");
#endif

	GameContentPath = FString::Printf(TEXT("../../../%s/Content/"), FApp::GetProjectName());
	GamePluginsPath = FString::Printf(TEXT("../../../%s/Plugins/"), FApp::GetProjectName());

	FullGameContentPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	FullGamePluginsPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir());
	FullEngineContentPath = FPaths::ConvertRelativePathToFull(FPaths::EngineContentDir());
	FullEnginePluginsPath = FPaths::ConvertRelativePathToFull(FPaths::EnginePluginsDir());

	FullEnginePath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir());
	FullProjectPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());

	CullEnginePath = FullEnginePath.Left(FullEnginePath.Len() - 8);
	CullProjectPath = FullProjectPath.Left(FullProjectPath.Len() - FString(FApp::GetProjectName()).Len() - 2);

	CookedPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir()) / FString("Cooked");

	ProjectStarPath = FString::Printf(TEXT("/%s"), FApp::GetProjectName());

	FString JsonContext;
	FFileHelper::LoadFileToString(JsonContext, *(IPluginManager::Get().FindPlugin("AdvancePak")->GetBaseDir() / FString("Resources") / FString("Translate.json")));
	TSharedPtr<FJsonValue> JsonValue;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonContext);
	if (FJsonSerializer::Deserialize(JsonReader, JsonValue))
	{
		for (auto LocTextValue : JsonValue->AsObject()->Values)
		{
			LocTextMap.Add(LocTextValue.Key, FText::FromString(LocTextValue.Value->AsString()));
		}
	}

	GatherTypeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("Regular")).ToString())));
	GatherTypeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("Informal")).ToString())));
	GatherTypeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("Both")).ToString())));

	RelyonTypeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("Nothing")).ToString())));
	RelyonTypeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("Soft")).ToString())));
	RelyonTypeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("Hard")).ToString())));
	RelyonTypeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("Searchable")).ToString())));
	RelyonTypeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("SoftManage")).ToString())));
	RelyonTypeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("HardManage")).ToString())));
	RelyonTypeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("Manage")).ToString())));
	RelyonTypeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("Packages")).ToString())));
	RelyonTypeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("All")).ToString())));

	GatherModeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("Local")).ToString())));
	GatherModeOptions.Add(MakeShareable(new FString((*LocTextMap.Find("Each")).ToString())));

	GatherTypeMap.Add((*LocTextMap.Find("Regular")).ToString(), EAdvancePakGatherType::Regular);
	GatherTypeMap.Add((*LocTextMap.Find("Informal")).ToString(), EAdvancePakGatherType::Informal);
	GatherTypeMap.Add((*LocTextMap.Find("Both")).ToString(), EAdvancePakGatherType::Both);

	RelyonTypeMap.Add((*LocTextMap.Find("Nothing")).ToString(), EAdvancePakRelyonType::Nothing);
	RelyonTypeMap.Add((*LocTextMap.Find("Soft")).ToString(), EAdvancePakRelyonType::Soft);
	RelyonTypeMap.Add((*LocTextMap.Find("Hard")).ToString(), EAdvancePakRelyonType::Hard);
	RelyonTypeMap.Add((*LocTextMap.Find("Searchable")).ToString(), EAdvancePakRelyonType::Searchable);
	RelyonTypeMap.Add((*LocTextMap.Find("SoftManage")).ToString(), EAdvancePakRelyonType::SoftManage);
	RelyonTypeMap.Add((*LocTextMap.Find("HardManage")).ToString(), EAdvancePakRelyonType::HardManage);
	RelyonTypeMap.Add((*LocTextMap.Find("Manage")).ToString(), EAdvancePakRelyonType::Manage);
	RelyonTypeMap.Add((*LocTextMap.Find("Packages")).ToString(), EAdvancePakRelyonType::Packages);
	RelyonTypeMap.Add((*LocTextMap.Find("All")).ToString(), EAdvancePakRelyonType::All);

	GatherModeMap.Add((*LocTextMap.Find("Local")).ToString(), EAdvancePakGatherMode::Local);
	GatherModeMap.Add((*LocTextMap.Find("Each")).ToString(), EAdvancePakGatherMode::Each);
}

FString UAdvancePakEditorLibrary::AbsorbPatchConfigureToJson(TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(NameName, PatchConfigure->Name.ToString());
	JsonObject->SetStringField(PakOptionsName, PatchConfigure->PakOptions);
	TArray<TSharedPtr<FJsonValue>> InvolveGathersArray;
	for (auto InvolveGather : PatchConfigure->InvolveGathers)
	{
		TSharedPtr<FJsonObject> InvolveGatherObject = MakeShareable(new FJsonObject);
		InvolveGatherObject->SetStringField(GatherPathName, InvolveGather->GatherPath);
		InvolveGatherObject->SetStringField(FullPathName, InvolveGather->FullPath);
		InvolveGatherObject->SetStringField(RelyonTypeName, UAdvancePakEditorType::GetEnumNameByValue("EAdvancePakRelyonType", InvolveGather->RelyonType));
		InvolveGatherObject->SetStringField(GatherTypeName, UAdvancePakEditorType::GetEnumNameByValue("EAdvancePakGatherType", InvolveGather->GatherType));
		InvolveGatherObject->SetStringField(GatherModeName, UAdvancePakEditorType::GetEnumNameByValue("EAdvancePakGatherMode", InvolveGather->GatherMode));
		InvolveGatherObject->SetBoolField(CanGatherRegularName, InvolveGather->CanGatherRegular);
		TSharedPtr<FJsonValueObject> InvolveGatherValue = MakeShareable(new FJsonValueObject(InvolveGatherObject));
		InvolveGathersArray.Add(InvolveGatherValue);
	}
	JsonObject->SetArrayField(InvolveGathersName, InvolveGathersArray);
	TArray<TSharedPtr<FJsonValue>> AbsolveGathersArray;
	for (auto AbsolveGather : PatchConfigure->AbsolveGathers)
	{
		TSharedPtr<FJsonObject> AbsolveGatherObject = MakeShareable(new FJsonObject);
		AbsolveGatherObject->SetStringField(GatherPathName, AbsolveGather->GatherPath);
		AbsolveGatherObject->SetStringField(FullPathName, AbsolveGather->FullPath);
		AbsolveGatherObject->SetStringField(RelyonTypeName, UAdvancePakEditorType::GetEnumNameByValue("EAdvancePakRelyonType", AbsolveGather->RelyonType));
		AbsolveGatherObject->SetStringField(GatherTypeName, UAdvancePakEditorType::GetEnumNameByValue("EAdvancePakGatherType", AbsolveGather->GatherType));
		AbsolveGatherObject->SetStringField(GatherModeName, UAdvancePakEditorType::GetEnumNameByValue("EAdvancePakGatherMode", AbsolveGather->GatherMode));
		AbsolveGatherObject->SetBoolField(CanGatherRegularName, AbsolveGather->CanGatherRegular);
		TSharedPtr<FJsonValueObject> AbsolveGatherValue = MakeShareable(new FJsonValueObject(AbsolveGatherObject));
		AbsolveGathersArray.Add(AbsolveGatherValue);
	}
	JsonObject->SetArrayField(AbsolveGathersName, AbsolveGathersArray);
	TArray<TSharedPtr<FJsonValue>> InvolveRegularsArray;
	for (auto InvolveRegular : PatchConfigure->InvolveRegulars)
	{
		TSharedPtr<FJsonObject> InvolveRegularObject = MakeShareable(new FJsonObject);
		InvolveRegularObject->SetStringField(PackageNameName, InvolveRegular->PackageName.ToString());
		InvolveRegularObject->SetStringField(RelyonTypeName, UAdvancePakEditorType::GetEnumNameByValue("EAdvancePakRelyonType", InvolveRegular->RelyonType));
		InvolveRegularObject->SetStringField(GatherModeName, UAdvancePakEditorType::GetEnumNameByValue("EAdvancePakGatherMode", InvolveRegular->GatherMode));
		TSharedPtr<FJsonValueObject> InvolveResourceValue = MakeShareable(new FJsonValueObject(InvolveRegularObject));
		InvolveRegularsArray.Add(InvolveResourceValue);
	}
	JsonObject->SetArrayField(InvolveRegularsName, InvolveRegularsArray);
	TArray<TSharedPtr<FJsonValue>> AbsolveRegularsArray;
	for (auto AbsolveRegular : PatchConfigure->AbsolveRegulars)
	{
		AbsolveRegularsArray.Add(MakeShared<FJsonValueString>(AbsolveRegular.ToString()));
	}
	JsonObject->SetArrayField(AbsolveRegularsName, AbsolveRegularsArray);
	TArray<TSharedPtr<FJsonValue>> InvolveInformalsArray;
	for (auto InvolveInformal : PatchConfigure->InvolveInformals)
	{
		TSharedPtr<FJsonObject> InvolveInformalObject = MakeShareable(new FJsonObject);
		InvolveInformalObject->SetStringField(InformalNameName, InvolveInformal->InformalName.ToString());
		InvolveInformalObject->SetStringField(InformalPathName, InvolveInformal->InformalPath);
		TSharedPtr<FJsonValueObject> InvolveInformalValue = MakeShareable(new FJsonValueObject(InvolveInformalObject));
		InvolveInformalsArray.Add(InvolveInformalValue);
	}
	JsonObject->SetArrayField(InvolveInformalsName, InvolveInformalsArray);
	TArray<TSharedPtr<FJsonValue>> AbsolveInformalsArray;
	for (auto AbsolveInformal : PatchConfigure->AbsolveInformals)
	{
		TSharedPtr<FJsonObject> AbsolveInformalObject = MakeShareable(new FJsonObject);
		AbsolveInformalObject->SetStringField(InformalNameName, AbsolveInformal->InformalName.ToString());
		AbsolveInformalObject->SetStringField(InformalPathName, AbsolveInformal->InformalPath);
		TSharedPtr<FJsonValueObject> AbsolveInformalValue = MakeShareable(new FJsonValueObject(AbsolveInformalObject));
		AbsolveInformalsArray.Add(AbsolveInformalValue);
	}
	JsonObject->SetArrayField(AbsolveInformalsName, AbsolveInformalsArray);
	TArray<TSharedPtr<FJsonValue>> PakFileListArray;
	for (auto PakLine : PatchConfigure->PakFileList)
	{
		TSharedPtr<FJsonObject> PakLineObject = MakeShareable(new FJsonObject);
		PakLineObject->SetStringField(FilePathName, PakLine->FilePath);
		PakLineObject->SetStringField(MountPathName, PakLine->MountPath);
		PakLineObject->SetStringField(HashName, PakLine->Hash);
		TSharedPtr<FJsonValueObject> PakLineValue = MakeShareable(new FJsonValueObject(PakLineObject));
		PakFileListArray.Add(PakLineValue);
	}
	JsonObject->SetArrayField(PakFileListName, PakFileListArray);
	TArray<TSharedPtr<FJsonValue>> RegularsArray;
	for (auto Regular : PatchConfigure->Regulars)
	{
		RegularsArray.Add(MakeShared<FJsonValueString>(Regular.ToString()));
	}
	JsonObject->SetArrayField(RegularsName, RegularsArray);
	TArray<TSharedPtr<FJsonValue>> InformalsArray;
	for (auto Informal : PatchConfigure->Informals)
	{
		TSharedPtr<FJsonObject> InformalObject = MakeShareable(new FJsonObject);
		InformalObject->SetStringField(InformalNameName, Informal->InformalName.ToString());
		InformalObject->SetStringField(InformalPathName, Informal->InformalPath);
		TSharedPtr<FJsonValueObject> InformalValue = MakeShareable(new FJsonValueObject(InformalObject));
		InformalsArray.Add(InformalValue);
	}
	JsonObject->SetArrayField(InformalsName, InformalsArray);
	FString JsonContext;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonContext);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	return JsonContext;
}

void UAdvancePakEditorLibrary::GetSingleGatherRegulars(const TSharedPtr<FAdvancePakGatherOption>& GatherInfo, TArray<FName>& OutRegulars)
{
	if (!GatherInfo->CanGatherRegular)
	{
		return;
	}
	TArray<FAssetData> PrimaryResourceDatas;
	FARFilter ARFilter;
	ARFilter.bIncludeOnlyOnDiskAssets = true;
	ARFilter.bRecursivePaths = GatherInfo->GatherMode == EAdvancePakGatherMode::Each;
	ARFilter.PackagePaths.Add(FName(*GatherInfo->GatherPath));
	AssetRegistryModule->Get().GetAssets(ARFilter, PrimaryResourceDatas);
	for (const FAssetData& ResourceData : PrimaryResourceDatas)
	{
		if (ResourceData.IsRedirector())
		{
			continue;
		}
		OutRegulars.Add(ResourceData.PackageName);
	}
	for (const FAssetData& ResourceData : PrimaryResourceDatas)
	{
		if (ResourceData.IsRedirector())
		{
			continue;
		}
		TArray<FName> RelyonNames;
		GetResourceRelyons(ResourceData.PackageName, GatherInfo->RelyonType, RelyonNames);
		for (const FName& PackageName : RelyonNames)
		{
			if (OutRegulars.Contains(PackageName))
			{
				continue;
			}
			FAssetData RelyonData = GetResourceDataByPackageName(PackageName);
			if (RelyonData.IsRedirector())
			{
				continue;
			}
			OutRegulars.Add(PackageName);
		}
	}
}

void UAdvancePakEditorLibrary::AbsorbRegularsToPakFileList(const TArray<FName>& Regulars, TArray<TSharedPtr<FAdvancePakPakLine>>& PakFileList, TArray<FName>& LackRegulars)
{
	for (auto PackageName : Regulars)
	{
		FString PrimaryPath = FPaths::ConvertRelativePathToFull(FPackageName::LongPackageNameToFilename(PackageName.ToString()));
		FString MountPath = AbsorbFullToMount(PrimaryPath);
		FString CookedResourcePath = AbsorbPrimaryPathToCookedPath(PrimaryPath);
		FString FloderPath = FPaths::GetPath(CookedResourcePath);
		if (!FPaths::DirectoryExists(FloderPath))
		{
			LackRegulars.Add(PackageName);
			continue;
		}
		FAdvancePakRegularsVisitor RegularsVisitor;
		RegularsVisitor.DotFilePath = CookedResourcePath + TEXT(".");
		PlatformFile->IterateDirectory(*FloderPath, RegularsVisitor);
		if (RegularsVisitor.FilePaths.Num() == 0)
		{
			LackRegulars.Add(PackageName);
			continue;
		}
		if (LackRegulars.Num() == 0)
		{
			for (auto FilePath : RegularsVisitor.FilePaths)
			{
				TSharedPtr<FAdvancePakPakLine> PakLine = MakeShareable(new FAdvancePakPakLine());
				PakLine->FilePath = FilePath;
				PakLine->MountPath = MountPath + FPaths::GetExtension(FilePath, true);
				PakLine->Hash = LexToString(FMD5Hash::HashFile(*FilePath));
				PakFileList.Add(PakLine);
			}
		}
	}
	if (LackRegulars.Num() > 0)
	{
		PakFileList.Reset();
	}
}

TSharedPtr<FAdvancePakPatchConfigure> UAdvancePakEditorLibrary::AbsorbJsonToPatchConfigure(const FString& JsonContext)
{
	TSharedPtr<FAdvancePakPatchConfigure> PatchConfigure;
	TSharedPtr<FJsonValue> JsonValue;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonContext);
	if (FJsonSerializer::Deserialize(JsonReader, JsonValue))
	{
		PatchConfigure = MakeShareable(new FAdvancePakPatchConfigure());
		PatchConfigure->Name = FName(*JsonValue->AsObject()->GetStringField(NameName));
		PatchConfigure->PakOptions = JsonValue->AsObject()->GetStringField(PakOptionsName);
		for (auto InvolveGatherObject : JsonValue->AsObject()->GetArrayField(InvolveGathersName))
		{
			TSharedPtr<FAdvancePakGatherOption> InvolveGather = MakeShareable(new FAdvancePakGatherOption());
			InvolveGather->GatherPath = InvolveGatherObject->AsObject()->GetStringField(GatherPathName);
			InvolveGather->FullPath = InvolveGatherObject->AsObject()->GetStringField(FullPathName);
			InvolveGather->RelyonType = UAdvancePakEditorType::GetEnumValueByName<EAdvancePakRelyonType>("EAdvancePakRelyonType", InvolveGatherObject->AsObject()->GetStringField(RelyonTypeName));
			InvolveGather->GatherType = UAdvancePakEditorType::GetEnumValueByName<EAdvancePakGatherType>("EAdvancePakGatherType", InvolveGatherObject->AsObject()->GetStringField(GatherTypeName));
			InvolveGather->GatherMode = UAdvancePakEditorType::GetEnumValueByName<EAdvancePakGatherMode>("EAdvancePakGatherMode", InvolveGatherObject->AsObject()->GetStringField(GatherModeName));
			InvolveGather->CanGatherRegular = InvolveGatherObject->AsObject()->GetBoolField(CanGatherRegularName);
			PatchConfigure->InvolveGathers.Add(InvolveGather);
		}
		for (auto AbsolveGatherObject : JsonValue->AsObject()->GetArrayField(AbsolveGathersName))
		{
			TSharedPtr<FAdvancePakGatherOption> AbsolveGather = MakeShareable(new FAdvancePakGatherOption());
			AbsolveGather->GatherPath = AbsolveGatherObject->AsObject()->GetStringField(GatherPathName);
			AbsolveGather->FullPath = AbsolveGatherObject->AsObject()->GetStringField(FullPathName);
			AbsolveGather->RelyonType = UAdvancePakEditorType::GetEnumValueByName<EAdvancePakRelyonType>("EAdvancePakRelyonType", AbsolveGatherObject->AsObject()->GetStringField(RelyonTypeName));
			AbsolveGather->GatherType = UAdvancePakEditorType::GetEnumValueByName<EAdvancePakGatherType>("EAdvancePakGatherType", AbsolveGatherObject->AsObject()->GetStringField(GatherTypeName));
			AbsolveGather->GatherMode = UAdvancePakEditorType::GetEnumValueByName<EAdvancePakGatherMode>("EAdvancePakGatherMode", AbsolveGatherObject->AsObject()->GetStringField(GatherModeName));
			AbsolveGather->CanGatherRegular = AbsolveGatherObject->AsObject()->GetBoolField(CanGatherRegularName);
			PatchConfigure->AbsolveGathers.Add(AbsolveGather);
		}
		for (auto InvolveRegularObject : JsonValue->AsObject()->GetArrayField(InvolveRegularsName))
		{
			TSharedPtr<FAdvancePakRegularOption> InvolveRegular = MakeShareable(new FAdvancePakRegularOption());
			InvolveRegular->PackageName = FName(*InvolveRegularObject->AsObject()->GetStringField(PackageNameName));
			InvolveRegular->RelyonType = UAdvancePakEditorType::GetEnumValueByName<EAdvancePakRelyonType>("EAdvancePakRelyonType", InvolveRegularObject->AsObject()->GetStringField(RelyonTypeName));
			InvolveRegular->GatherMode = UAdvancePakEditorType::GetEnumValueByName<EAdvancePakGatherMode>("EAdvancePakGatherMode", InvolveRegularObject->AsObject()->GetStringField(GatherModeName));
			PatchConfigure->InvolveRegulars.Add(InvolveRegular);
		}
		for (auto AbsolveRegularObject : JsonValue->AsObject()->GetArrayField(AbsolveRegularsName))
		{
			PatchConfigure->AbsolveRegulars.Add(FName(*AbsolveRegularObject->AsString()));
		}
		for (auto InvolveInformalObject : JsonValue->AsObject()->GetArrayField(InvolveInformalsName))
		{
			TSharedPtr<FAdvancePakInformalInfo> InvolveInformal = MakeShareable(new FAdvancePakInformalInfo());
			InvolveInformal->InformalName = FName(*InvolveInformalObject->AsObject()->GetStringField(InformalNameName));
			InvolveInformal->InformalPath = InvolveInformalObject->AsObject()->GetStringField(InformalPathName);
			PatchConfigure->InvolveInformals.Add(InvolveInformal);
		}
		for (auto AbsolveInformalObject : JsonValue->AsObject()->GetArrayField(AbsolveInformalsName))
		{
			TSharedPtr<FAdvancePakInformalInfo> AbsolveInformal = MakeShareable(new FAdvancePakInformalInfo());
			AbsolveInformal->InformalName = FName(*AbsolveInformalObject->AsObject()->GetStringField(InformalNameName));
			AbsolveInformal->InformalPath = AbsolveInformalObject->AsObject()->GetStringField(InformalPathName);
			PatchConfigure->AbsolveInformals.Add(AbsolveInformal);
		}
		for (auto PakLineObject : JsonValue->AsObject()->GetArrayField(PakFileListName))
		{
			TSharedPtr<FAdvancePakPakLine> PakLine = MakeShareable(new FAdvancePakPakLine());
			PakLine->FilePath = PakLineObject->AsObject()->GetStringField(FilePathName);
			PakLine->MountPath = PakLineObject->AsObject()->GetStringField(MountPathName);
			PakLine->Hash = PakLineObject->AsObject()->GetStringField(HashName);
			PatchConfigure->PakFileList.Add(PakLine);
		}
		for (auto RegularObject : JsonValue->AsObject()->GetArrayField(RegularsName))
		{
			PatchConfigure->Regulars.Add(FName(*RegularObject->AsString()));
		}
		for (auto InformalObject : JsonValue->AsObject()->GetArrayField(InformalsName))
		{
			TSharedPtr<FAdvancePakInformalInfo> Informal = MakeShareable(new FAdvancePakInformalInfo());
			Informal->InformalName = FName(*InformalObject->AsObject()->GetStringField(InformalNameName));
			Informal->InformalPath = InformalObject->AsObject()->GetStringField(InformalPathName);
			PatchConfigure->Informals.Add(Informal);
		}
	}
	return PatchConfigure;
}

void UAdvancePakEditorLibrary::CreateSaveFileNotify(const FString& NotifyTitle, const FString& FloderPath)
{
	FNotificationInfo Info(FText::FromString(NotifyTitle));
	Info.bFireAndForget = true;
	Info.ExpireDuration = 5.0f;
	Info.bUseSuccessFailIcons = false;
	Info.bUseLargeFont = false;
	Info.Hyperlink = FSimpleDelegate::CreateStatic(
		[](FString SourceFilePath)
	{
		FPlatformProcess::ExploreFolder(*SourceFilePath);
	},
		FloderPath
		);
	Info.HyperlinkText = FText::FromString(FloderPath);
	FSlateNotificationManager::Get().AddNotification(Info)->SetCompletionState(SNotificationItem::CS_Success);
	GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));
}

void UAdvancePakEditorLibrary::CreatePromptNotify(const FString& NotifyTitle, const bool& IsSucceed)
{
	FNotificationInfo Info(FText::FromString(NotifyTitle));
	Info.bFireAndForget = true;
	Info.ExpireDuration = 5.0f;
	Info.bUseSuccessFailIcons = true;
	Info.bUseLargeFont = true;
	FSlateNotificationManager::Get().AddNotification(Info)->SetCompletionState(SNotificationItem::CS_Success);
	if (IsSucceed)
	{
		GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));
	}
	else
	{
		GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));
	}
}

FString UAdvancePakEditorLibrary::AbsorbFullToMount(const FString& FullPath)
{
	if (FullPath.StartsWith(FullGameContentPath))
	{
		return GameContentPath / FullPath.Right(FullPath.Len() - FullGameContentPath.Len());
	}
	else if (FullPath.StartsWith(FullGamePluginsPath))
	{
		return GamePluginsPath / FullPath.Right(FullPath.Len() - FullGamePluginsPath.Len());
	}
	else if (FullPath.StartsWith(FullEngineContentPath))
	{
		return EngineContentPath / FullPath.Right(FullPath.Len() - FullEngineContentPath.Len());
	}
	else if (FullPath.StartsWith(FullEnginePluginsPath))
	{
		return EnginePluginsPath / FullPath.Right(FullPath.Len() - FullEnginePluginsPath.Len());
	}
	return FString();
}

EAssetRegistryDependencyType::Type UAdvancePakEditorLibrary::AbsorbRelyonType(const EAdvancePakRelyonType& RelyonType)
{
	switch (RelyonType)
	{
	case EAdvancePakRelyonType::Nothing:
		return EAssetRegistryDependencyType::None;
	case EAdvancePakRelyonType::Soft:
		return EAssetRegistryDependencyType::Soft;
	case EAdvancePakRelyonType::Hard:
		return EAssetRegistryDependencyType::Hard;
	case EAdvancePakRelyonType::Searchable:
		return EAssetRegistryDependencyType::SearchableName;
	case EAdvancePakRelyonType::SoftManage:
		return EAssetRegistryDependencyType::SoftManage;
	case EAdvancePakRelyonType::HardManage:
		return EAssetRegistryDependencyType::HardManage;
	case EAdvancePakRelyonType::Manage:
		return EAssetRegistryDependencyType::Manage;
	case EAdvancePakRelyonType::Packages:
		return EAssetRegistryDependencyType::Packages;
	case EAdvancePakRelyonType::All:
		return EAssetRegistryDependencyType::All;
	}
	return EAssetRegistryDependencyType::None;
}

FName UAdvancePakEditorLibrary::AbsorbFullToInformal(const FString& FullPath)
{
	if (!FullPath.StartsWith(FullProjectPath) && !FullPath.StartsWith(FullEnginePath))
	{
		return NAME_None;
	}
	if (FullPath.StartsWith(CookedGameContentPath))
	{
		return FName(*(GameStarPath / FullPath.Right(FullPath.Len() - CookedGameContentPath.Len())));
	}
	else if (FullPath.StartsWith(CookedGamePluginsPath) && FullPath.Contains(ContentSpacePath))
	{
		FString ContentLeft = FString();
		FString ContentRight = FString();
		FullPath.Split(ContentSpacePath, &ContentLeft, &ContentRight);
		return FName(*((FString("/") + (FPaths::GetBaseFilename(ContentLeft) / ContentRight))));
	}
	else if (FullPath.StartsWith(CookedEngineContentPath))
	{
		return FName(*(EngineStartPath / FullPath.Right(FullPath.Len() - CookedEngineContentPath.Len())));
	}
	else if (FullPath.StartsWith(CookedEnginePluginsPath) && FullPath.Contains(ContentSpacePath))
	{
		FString ContentLeft = FString();
		FString ContentRight = FString();
		FullPath.Split(ContentSpacePath, &ContentLeft, &ContentRight);
		return FName(*((FString("/") + (FPaths::GetBaseFilename(ContentLeft) / ContentRight))));
	}
	else if (FullPath.StartsWith(CookedProjectPath))
	{
		return FName(*FullPath.Right(FullPath.Len() - CookedPlatformPath.Len()));
	}
	else if (FullPath.StartsWith(CookedEnginePath))
	{
		return FName(*FullPath.Right(FullPath.Len() - CookedPlatformPath.Len()));
	}
	else if (FullPath.StartsWith(FullGameContentPath))
	{
		return FName(*(GameStarPath / FullPath.Right(FullPath.Len() - FullGameContentPath.Len())));
	}
	else if (FullPath.StartsWith(FullGamePluginsPath) && FullPath.Contains(ContentSpacePath))
	{
		FString ContentLeft = FString();
		FString ContentRight = FString();
		FullPath.Split(ContentSpacePath, &ContentLeft, &ContentRight);
		return FName(*((FString("/") + (FPaths::GetBaseFilename(ContentLeft) / ContentRight))));
	}
	else if (FullPath.StartsWith(FullEngineContentPath))
	{
		return FName(*(EngineStartPath / FullPath.Right(FullPath.Len() - FullEngineContentPath.Len())));
	}
	else if (FullPath.StartsWith(FullEnginePluginsPath) && FullPath.Contains(ContentSpacePath))
	{
		FString ContentLeft = FString();
		FString ContentRight = FString();
		FullPath.Split(ContentSpacePath, &ContentLeft, &ContentRight);
		return FName(*((FString("/") + (FPaths::GetBaseFilename(ContentLeft) / ContentRight))));
	}
	else if (FullPath.StartsWith(FullProjectPath))
	{
		return FName(*FullPath.Right(FullPath.Len() - CullProjectPath.Len()));
	}
	else if (FullPath.StartsWith(FullEnginePath))
	{
		return FName(*FullPath.Right(FullPath.Len() - CullEnginePath.Len()));
	}
	return NAME_None;
}

FAssetData UAdvancePakEditorLibrary::GetResourceDataByPackageName(const FName& PackageName)
{
	TArray<FAssetData> ResourceDatas;
	if (AssetRegistryModule->Get().GetAssetsByPackageName(PackageName, ResourceDatas) && ResourceDatas.Num() > 0 && !ResourceDatas[0].IsRedirector())
	{
		return ResourceDatas[0];
	}
	return FAssetData();
}

void UAdvancePakEditorLibrary::GetResourceDatasByPackageName(const FName& PackageName, TArray<FAssetData>& OutResourceDatas)
{
	AssetRegistryModule->Get().GetAssetsByPackageName(PackageName, OutResourceDatas);
}

void UAdvancePakEditorLibrary::GetResourceRelyons(const FName& PackageName, const EAdvancePakRelyonType& RelyonType, TArray<FName>& OutPackageNames)
{
	TArray<FName> TotalRelyons;
	AssetRegistryModule->Get().GetDependencies(PackageName, TotalRelyons, AbsorbRelyonType(RelyonType));
	for (auto Relyon : TotalRelyons)
	{
		if (Relyon.ToString().StartsWith(ScriptStartPath) || GetResourceDataByPackageName(Relyon).IsRedirector())
		{
			continue;
		}
		OutPackageNames.Add(Relyon);
	}
}

FString UAdvancePakEditorLibrary::AbsorbPublishConfigureToJson(TSharedPtr<FAdvancePakPublishConfigure> PublishConfigure)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(OutputDirName, PublishConfigure->OutputDir);
	JsonObject->SetStringField(ConfigDirName, PublishConfigure->ConfigDir);
	JsonObject->SetStringField(VersionName, PublishConfigure->Version);
	JsonObject->SetStringField(PlatformName, PublishConfigure->Platform);
	JsonObject->SetStringField(PakCommandName, PublishConfigure->PakCommand);
	JsonObject->SetStringField(LineOptionsName, PublishConfigure->LineOptions);
	JsonObject->SetStringField(PakOptionsName, PublishConfigure->PakOptions);
	JsonObject->SetStringField(PackageDirName, PublishConfigure->PackageDir);
	JsonObject->SetStringField(LastConfigName, PublishConfigure->LastConfig);
	TArray<TSharedPtr<FJsonValue>> MapsToBuildArray;
	for (auto MapPath : PublishConfigure->MapsToBuild)
	{
		MapsToBuildArray.Add(MakeShared<FJsonValueString>(MapPath));
	}
	JsonObject->SetArrayField(MapsToBuildName, MapsToBuildArray);
	TArray<TSharedPtr<FJsonValue>> AlwaysCookPathsArray;
	for (auto CookPath : PublishConfigure->AlwaysCookPaths)
	{
		AlwaysCookPathsArray.Add(MakeShared<FJsonValueString>(CookPath));
	}
	JsonObject->SetArrayField(AlwaysCookPathsName, AlwaysCookPathsArray);
	TArray<TSharedPtr<FJsonValue>> NeverCookPathsArray;
	for (auto CookPath : PublishConfigure->NeverCookPaths)
	{
		NeverCookPathsArray.Add(MakeShared<FJsonValueString>(CookPath));
	}
	JsonObject->SetArrayField(NeverCookPathsName, NeverCookPathsArray);
	TArray<TSharedPtr<FJsonValue>> AdditionalsArray;
	for (auto Addition : PublishConfigure->AdditionalPaths)
	{
		AdditionalsArray.Add(MakeShared<FJsonValueString>(Addition));
	}
	JsonObject->SetArrayField(AdditionalPathsName, AdditionalsArray);
	TArray<TSharedPtr<FJsonValue>> PakFileListArray;
	for (auto PakLine : PublishConfigure->PakFileList)
	{
		TSharedPtr<FJsonObject> PakLineObject = MakeShareable(new FJsonObject);
		PakLineObject->SetStringField(FilePathName, PakLine->FilePath);
		PakLineObject->SetStringField(MountPathName, PakLine->MountPath);
		PakLineObject->SetStringField(HashName, PakLine->Hash);
		TSharedPtr<FJsonValueObject> PakLineValue = MakeShareable(new FJsonValueObject(PakLineObject));
		PakFileListArray.Add(PakLineValue);
	}
	JsonObject->SetArrayField(PakFileListName, PakFileListArray);
	TArray<TSharedPtr<FJsonValue>> RegularsArray;
	for (auto Regular : PublishConfigure->Regulars)
	{
		RegularsArray.Add(MakeShared<FJsonValueString>(Regular.ToString()));
	}
	JsonObject->SetArrayField(RegularsName, RegularsArray);
	TArray<TSharedPtr<FJsonValue>> InformalsArray;
	for (auto Informal : PublishConfigure->Informals)
	{
		TSharedPtr<FJsonObject> InformalObject = MakeShareable(new FJsonObject);
		InformalObject->SetStringField(InformalNameName, Informal->InformalName.ToString());
		InformalObject->SetStringField(InformalPathName, Informal->InformalPath);
		TSharedPtr<FJsonValueObject> InformalValue = MakeShareable(new FJsonValueObject(InformalObject));
		InformalsArray.Add(InformalValue);
	}
	JsonObject->SetArrayField(InformalsName, InformalsArray);
	TArray<TSharedPtr<FJsonValue>> PatchNamesArray;
	for (auto Patch : PublishConfigure->Patchs)
	{
		PatchNamesArray.Add(MakeShared<FJsonValueString>(Patch.ToString()));
	}
	JsonObject->SetArrayField(PatchsName, PatchNamesArray);
	FString JsonContext;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonContext);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	return JsonContext;
}

void UAdvancePakEditorLibrary::GetMultiGatherRegulars(const TArray<TSharedPtr<FAdvancePakGatherOption>>& GatherInfos, TArray<FName>& OutRegulars)
{
	for (auto GatherInfo : GatherInfos)
	{
		TArray<FName> SingleRegulars;
		GetSingleGatherRegulars(GatherInfo, SingleRegulars);
		for (const FName& PackageName : SingleRegulars)
		{
			OutRegulars.AddUnique(PackageName);
		}
	}
}

void UAdvancePakEditorLibrary::GetSingleGatherInformals(const TSharedPtr<FAdvancePakGatherOption>& GatherInfo, TMap<FName, FString>& OutInformals)
{
	if (GatherInfo->GatherType == EAdvancePakGatherType::Regular)
	{
		return;
	}
	FAdvancePakInformalsVisitor InformalsVisitor;
	if (GatherInfo->GatherMode == EAdvancePakGatherMode::Local)
	{
		PlatformFile->IterateDirectory(*GatherInfo->FullPath, InformalsVisitor);
	}
	else
	{
		PlatformFile->IterateDirectoryRecursively(*GatherInfo->FullPath, InformalsVisitor);
	}

	for (auto FilePath : InformalsVisitor.FilePaths)
	{
		FName InformalName = AbsorbFullToInformal(FilePath);
		if (OutInformals.Contains(InformalName) || InformalName.IsNone())
		{
			continue;
		}
		OutInformals.Add(InformalName, FilePath);
	}
}

FString UAdvancePakEditorLibrary::AbsorbPrimaryPathToCookedPath(const FString& PrimaryPath)
{
	if (PrimaryPath.StartsWith(FullGameContentPath))
	{
		return CookedGameContentPath / PrimaryPath.Right(PrimaryPath.Len() - FullGameContentPath.Len());
	}
	else if (PrimaryPath.StartsWith(FullGamePluginsPath))
	{
		return CookedGamePluginsPath / PrimaryPath.Right(PrimaryPath.Len() - FullGamePluginsPath.Len());
	}
	else if (PrimaryPath.StartsWith(FullEngineContentPath))
	{
		return CookedEngineContentPath / PrimaryPath.Right(PrimaryPath.Len() - FullEngineContentPath.Len());
	}
	else if (PrimaryPath.StartsWith(FullEnginePluginsPath))
	{
		return CookedEnginePluginsPath / PrimaryPath.Right(PrimaryPath.Len() - FullEnginePluginsPath.Len());
	}
	return FString();
}

FAssetRegistryModule* UAdvancePakEditorLibrary::AssetRegistryModule = NULL;

IPlatformFile* UAdvancePakEditorLibrary::PlatformFile = NULL;

TMap<FString, EAdvancePakGatherType> UAdvancePakEditorLibrary::GatherTypeMap = TMap<FString, EAdvancePakGatherType>();

TMap<FString, EAdvancePakRelyonType> UAdvancePakEditorLibrary::RelyonTypeMap = TMap<FString, EAdvancePakRelyonType>();

FString UAdvancePakEditorLibrary::GatherTypeName(TEXT("GatherType"));

FString UAdvancePakEditorLibrary::GatherModeName(TEXT("GatherMode"));

FString UAdvancePakEditorLibrary::CanGatherRegularName(TEXT("CanGatherRegular"));

FString UAdvancePakEditorLibrary::PublishConfigName(TEXT("PublishConfig.json"));

FString UAdvancePakEditorLibrary::OutputDirName(TEXT("OutputDir"));

FString UAdvancePakEditorLibrary::ConfigDirName(TEXT("ConfigDir"));

FString UAdvancePakEditorLibrary::VersionName(TEXT("Version"));

FString UAdvancePakEditorLibrary::PlatformName(TEXT("Platform"));

TMap<FString, EAdvancePakGatherMode> UAdvancePakEditorLibrary::GatherModeMap = TMap<FString, EAdvancePakGatherMode>();

FString UAdvancePakEditorLibrary::FullPathName(TEXT("FullPath"));

FString UAdvancePakEditorLibrary::RelyonTypeName(TEXT("Relyon"));

FString UAdvancePakEditorLibrary::PatchsName(TEXT("Patchs"));

FString UAdvancePakEditorLibrary::ResourceTypeName(TEXT("ResourceType"));

void UAdvancePakEditorLibrary::GetMultiGatherInformals(const TArray<TSharedPtr<FAdvancePakGatherOption>>& GatherInfos, TMap<FName, FString>& OutInformals)
{
	for (auto GatherInfo : GatherInfos)
	{
		TMap<FName, FString> SingleInformals;
		GetSingleGatherInformals(GatherInfo, SingleInformals);
		for (TMap<FName, FString>::TIterator It(SingleInformals); It; ++It)
		{
			if (OutInformals.Contains(It.Key()))
			{
				continue;
			}
			OutInformals.Add(It.Key(), It.Value());
		}
	}
}

TSharedPtr<FAdvancePakPublishConfigure> UAdvancePakEditorLibrary::AbsorbJsonToPublishConfigure(const FString& JsonContext)
{
	TSharedPtr<FAdvancePakPublishConfigure> PublishConfigure;
	TSharedPtr<FJsonValue> JsonValue;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonContext);
	if (FJsonSerializer::Deserialize(JsonReader, JsonValue))
	{
		PublishConfigure = MakeShareable(new FAdvancePakPublishConfigure());
		PublishConfigure->OutputDir = JsonValue->AsObject()->GetStringField(OutputDirName);
		PublishConfigure->ConfigDir = JsonValue->AsObject()->GetStringField(ConfigDirName);
		PublishConfigure->Version = JsonValue->AsObject()->GetStringField(VersionName);
		PublishConfigure->Platform = JsonValue->AsObject()->GetStringField(PlatformName);
		PublishConfigure->PakCommand = JsonValue->AsObject()->GetStringField(PakCommandName);
		PublishConfigure->LineOptions = JsonValue->AsObject()->GetStringField(LineOptionsName);
		PublishConfigure->PakOptions = JsonValue->AsObject()->GetStringField(PakOptionsName);
		PublishConfigure->PackageDir = JsonValue->AsObject()->GetStringField(PackageDirName);
		PublishConfigure->LastConfig = JsonValue->AsObject()->GetStringField(LastConfigName);
		for (auto MapObject : JsonValue->AsObject()->GetArrayField(MapsToBuildName))
		{
			PublishConfigure->MapsToBuild.Add(MapObject->AsString());
		}
		for (auto CookObject : JsonValue->AsObject()->GetArrayField(AlwaysCookPathsName))
		{
			PublishConfigure->AlwaysCookPaths.Add(CookObject->AsString());
		}
		for (auto CookObject : JsonValue->AsObject()->GetArrayField(NeverCookPathsName))
		{
			PublishConfigure->NeverCookPaths.Add(CookObject->AsString());
		}
		for (auto AdditionObject : JsonValue->AsObject()->GetArrayField(AdditionalPathsName))
		{
			PublishConfigure->AdditionalPaths.Add(AdditionObject->AsString());
		}
		for (auto PakLineObject : JsonValue->AsObject()->GetArrayField(PakFileListName))
		{
			TSharedPtr<FAdvancePakPakLine> PakLine = MakeShareable(new FAdvancePakPakLine());
			PakLine->FilePath = PakLineObject->AsObject()->GetStringField(FilePathName);
			PakLine->MountPath = PakLineObject->AsObject()->GetStringField(MountPathName);
			PakLine->Hash = PakLineObject->AsObject()->GetStringField(HashName);
			PublishConfigure->PakFileList.Add(PakLine);
		}
		for (auto RegularObject : JsonValue->AsObject()->GetArrayField(RegularsName))
		{
			PublishConfigure->Regulars.Add(FName(*RegularObject->AsString()));
		}
		for (auto InformalObject : JsonValue->AsObject()->GetArrayField(InformalsName))
		{
			TSharedPtr<FAdvancePakInformalInfo> Informal = MakeShareable(new FAdvancePakInformalInfo());
			Informal->InformalName = FName(*InformalObject->AsObject()->GetStringField(InformalNameName));
			Informal->InformalPath = InformalObject->AsObject()->GetStringField(InformalPathName);
			PublishConfigure->Informals.Add(Informal);
		}
		for (auto PatchObject : JsonValue->AsObject()->GetArrayField(PatchsName))
		{
			PublishConfigure->Patchs.Add(FName(*PatchObject->AsString()));
		}
	}
	return PublishConfigure;
}


void UAdvancePakEditorLibrary::AbsorbInformalsToPakFileList(const TArray<TSharedPtr<FAdvancePakInformalInfo>>& Informals, TArray<TSharedPtr<FAdvancePakPakLine>>& PakFileList, TMap<FName, FString>& LackInformals)
{
	for (auto Informal : Informals)
	{
		if (!FPaths::FileExists(Informal->InformalPath))
		{
			LackInformals.Add(Informal->InformalName, Informal->InformalPath);
			continue;
		}
		if (LackInformals.Num() == 0)
		{
			TSharedPtr<FAdvancePakPakLine> PakLine = MakeShareable(new FAdvancePakPakLine());
			PakLine->FilePath = Informal->InformalPath;
			PakLine->MountPath = FString("../../..") + Informal->InformalName.ToString();
			PakLine->Hash = LexToString(FMD5Hash::HashFile(*Informal->InformalPath));
			PakFileList.Add(PakLine);
		}
	}
	if (LackInformals.Num() > 0)
	{
		PakFileList.Reset();
	}
}

FString UAdvancePakEditorLibrary::PackageNameName(TEXT("PackageName"));

FString UAdvancePakEditorLibrary::HashName(TEXT("Hash"));

FString UAdvancePakEditorLibrary::InformalNameName(TEXT("InformalName"));

FString UAdvancePakEditorLibrary::InformalPathName(TEXT("InformalPath"));

TMap<FString, FText> UAdvancePakEditorLibrary::LocTextMap = TMap<FString, FText>();

TArray<TSharedPtr<FString>> UAdvancePakEditorLibrary::GatherTypeOptions = TArray<TSharedPtr<FString>>();

TArray<TSharedPtr<FString>> UAdvancePakEditorLibrary::RelyonTypeOptions = TArray<TSharedPtr<FString>>();

FString UAdvancePakEditorLibrary::UmapSuffixName(TEXT(".umap"));

FString UAdvancePakEditorLibrary::PakCommandName(TEXT("PakCommand"));

FString UAdvancePakEditorLibrary::PakOptionsName(TEXT("PakOptions"));

FString UAdvancePakEditorLibrary::PackageDirName(TEXT("PackageDir"));

FString UAdvancePakEditorLibrary::LastConfigName(TEXT("LastConfig"));

FString UAdvancePakEditorLibrary::InvolveRegularsName(TEXT("InvolveRegulars"));

EAdvancePakRelyonType UAdvancePakEditorLibrary::AbsorbStringToRelyonType(const FString& StrValue)
{
	return *RelyonTypeMap.Find(StrValue);
}

FString UAdvancePakEditorLibrary::MapsToBuildName(TEXT("MapsToBuild"));

FString UAdvancePakEditorLibrary::AlwaysCookPathsName(TEXT("AlwaysCookPaths"));

TArray<TSharedPtr<FString>> UAdvancePakEditorLibrary::GatherModeOptions = TArray<TSharedPtr<FString>>();

FString UAdvancePakEditorLibrary::FullGamePluginsPath(TEXT(""));

FString UAdvancePakEditorLibrary::FullEngineContentPath(TEXT(""));

FString UAdvancePakEditorLibrary::FullEnginePluginsPath(TEXT(""));

FString UAdvancePakEditorLibrary::FullProjectPath(TEXT(""));

FString UAdvancePakEditorLibrary::FullEnginePath(TEXT(""));

FString UAdvancePakEditorLibrary::CullProjectPath(TEXT(""));

FString UAdvancePakEditorLibrary::CullEnginePath(TEXT(""));

FString UAdvancePakEditorLibrary::UassetSuffixName(TEXT(".uasset"));

FString UAdvancePakEditorLibrary::UexpSuffixName(TEXT(".uexp"));

FString UAdvancePakEditorLibrary::UbulkSuffixName(TEXT(".ubulk"));

FString UAdvancePakEditorLibrary::LineOptionsName(TEXT("LineOptions"));

FString UAdvancePakEditorLibrary::FilePathName(TEXT("FilePath"));

FString UAdvancePakEditorLibrary::MountPathName(TEXT("MountPath"));

FString UAdvancePakEditorLibrary::UfontSuffixName(TEXT(".ufont"));

FString UAdvancePakEditorLibrary::UtxtSuffixName(TEXT(".utxt"));

FString UAdvancePakEditorLibrary::UtxtmapSuffixName(TEXT(".utxtmap"));

FString UAdvancePakEditorLibrary::GameContentPath(TEXT(""));

FString UAdvancePakEditorLibrary::GamePluginsPath(TEXT(""));

FString UAdvancePakEditorLibrary::EngineContentPath(TEXT("../../../Engine/Content/"));

FString UAdvancePakEditorLibrary::EnginePluginsPath(TEXT("../../../Engine/Plugins/"));

FString UAdvancePakEditorLibrary::FullGameContentPath(TEXT(""));

FString UAdvancePakEditorLibrary::CookedPath(TEXT(""));

FString UAdvancePakEditorLibrary::CookedPlatformPath(TEXT(""));

FString UAdvancePakEditorLibrary::CookedProjectPath(TEXT(""));

FString UAdvancePakEditorLibrary::CookedEnginePath(TEXT(""));

FString UAdvancePakEditorLibrary::CookedGameContentPath(TEXT(""));

FString UAdvancePakEditorLibrary::CookedGamePluginsPath(TEXT(""));

FString UAdvancePakEditorLibrary::UnrealPakPath(TEXT(""));

FString UAdvancePakEditorLibrary::UnrealRunUAT(TEXT(""));

FString UAdvancePakEditorLibrary::UnrealCmdPath(TEXT(""));

FString UAdvancePakEditorLibrary::CookedEngineContentPath(TEXT(""));

FString UAdvancePakEditorLibrary::CookedEnginePluginsPath(TEXT(""));

FString UAdvancePakEditorLibrary::GameStarPath(TEXT("/Game"));

FString UAdvancePakEditorLibrary::ProjectStarPath(TEXT(""));

FString UAdvancePakEditorLibrary::EngineStartPath(TEXT("/Engine"));

FString UAdvancePakEditorLibrary::ScriptStartPath(TEXT("/Script"));

FString UAdvancePakEditorLibrary::ContentStartPath(TEXT("/Content"));

FString UAdvancePakEditorLibrary::ContentSpacePath(TEXT("/Content/"));

FString UAdvancePakEditorLibrary::NameName(TEXT("Name"));

FString UAdvancePakEditorLibrary::InvolveGathersName(TEXT("InvolveGathers"));

FString UAdvancePakEditorLibrary::AbsolveGathersName(TEXT("AbsolveGathers"));

FString UAdvancePakEditorLibrary::NeverCookPathsName(TEXT("NeverCookPaths"));

FString UAdvancePakEditorLibrary::AdditionalPathsName(TEXT("AdditionalPaths"));

FString UAdvancePakEditorLibrary::PakFileListName(TEXT("PakFileList"));

FString UAdvancePakEditorLibrary::RegularsName(TEXT("Regulars"));

FString UAdvancePakEditorLibrary::InformalsName(TEXT("Informals"));

FString UAdvancePakEditorLibrary::InvolveInformalsName(TEXT("InvolveInformals"));

FString UAdvancePakEditorLibrary::AbsolveRegularsName(TEXT("AbsolveRegulars"));

FString UAdvancePakEditorLibrary::AbsolveInformalsName(TEXT("AbsolveInformals"));

FString UAdvancePakEditorLibrary::GatherPathName(TEXT("GatherPath"));
