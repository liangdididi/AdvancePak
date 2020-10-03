#include "AdvancePakUpdateSucceed.h"
#include "AdvancePakWork.h"
#include "AdvancePakLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Online/HTTP/Public/HttpModule.h"
#include "AdvancePakType.h"
#include "AdvancePakAgency.h" 
#include "Misc/Paths.h"
#include "PakFile/Public/IPlatformFilePak.h"

void FAdvancePakUpdateSucceed::EnterProcess()
{
	Work->OnUpdateSucceedNotify.Broadcast();
}
