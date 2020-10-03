// Fill out your copyright notice in the Futurecription page of Project Configures.


#include "AdvancePakEditorType.h"

DEFINE_LOG_CATEGORY(AdvancePakLog);

#if WITH_EDITOR

void UAdvancePakBuildConfigures::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAdvancePakBuildConfigures, BuildConfig))
	{
		OnBuildConfigurationChanged.ExecuteIfBound();
	}
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAdvancePakBuildConfigures, Version))
	{
		if (Version.Contains(FString("_")))
		{
			Version = Version.Replace(TEXT("_"), TEXT(""));
		}
	}
}

#endif

UAdvancePakBuildConfigures::UAdvancePakBuildConfigures()
{
	BuildConfig = EProjectPackagingBuildConfigurations::PPBC_Development;
	IsDuplicateDatas = true;
}
