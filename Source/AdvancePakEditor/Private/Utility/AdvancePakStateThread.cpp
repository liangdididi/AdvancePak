#include "AdvancePakStateThread.h"

void FAdvancePakStateThread::Execute()
{
	WorkThread.Reset(FRunnableThread::Create(this, *ThreadName, 0));
}

void FAdvancePakStateThread::Join()
{
	StateJoinDelagete.Broadcast();
	WorkThread->WaitForCompletion();
}

void FAdvancePakStateThread::Stop()
{
	StateStopDelagete.Broadcast();
}

void FAdvancePakStateThread::Exit()
{
	StateExitDelagete.Broadcast();
}

uint32 FAdvancePakStateThread::Run()
{
	StateRunDelagete.Broadcast();
	CallBack();
	StateCompleteDelagete.Broadcast();
	return 0;
}
