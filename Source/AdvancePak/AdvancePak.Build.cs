// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AdvancePak : ModuleRules
{
	public AdvancePak(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "Json",
                "JsonUtilities",
                "PakFile",
                "Http",
                "Sockets",
                "SSL",
                "AdvancePakSSL",
                "RSA",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        bool bWithCurl = false;

        if (Target.Platform == UnrealTargetPlatform.Win32 ||
            Target.Platform == UnrealTargetPlatform.Win64)
        {
            AddEngineThirdPartyPrivateStaticDependencies(Target, "WinHttp");
            AddEngineThirdPartyPrivateStaticDependencies(Target, "libcurl");
            AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenSSL");

            bWithCurl = true;
        }
        else if (Target.IsInPlatformGroup(UnrealPlatformGroup.Unix) ||
                Target.IsInPlatformGroup(UnrealPlatformGroup.Android))
        {
            AddEngineThirdPartyPrivateStaticDependencies(Target, "libcurl");
            AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenSSL");

            bWithCurl = true;
        }
        else if (Target.Platform == UnrealTargetPlatform.Switch)
        {
            AddEngineThirdPartyPrivateStaticDependencies(Target, "libcurl");

            bWithCurl = true;
        }
        else
        {
            PublicDefinitions.Add("WITH_LIBCURL=0");
        }

        if (bWithCurl)
        {
            PublicDefinitions.Add("CURL_ENABLE_DEBUG_CALLBACK=1");
            if (Target.Configuration != UnrealTargetConfiguration.Shipping)
            {
                PublicDefinitions.Add("CURL_ENABLE_NO_TIMEOUTS_OPTION=1");
            }
        }

        if (Target.Platform == UnrealTargetPlatform.IOS || Target.Platform == UnrealTargetPlatform.TVOS || Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicFrameworks.Add("Security");
        }

    }
}
