#include "AdvancePakProcThread.h"

void FAdvancePakProcThread::Execute()
{
	WorkThread.Reset(FRunnableThread::Create(this, *ThreadName, 0));
}

void FAdvancePakProcThread::Cancel()
{
	if (ProcHandle.IsValid() && FPlatformProcess::IsApplicationRunning(ProcId))
	{
		IsCancel = true;
		FPlatformProcess::TerminateProc(ProcHandle, true);
		ProcHandle.Reset();
		ProcId = 0;
	}
}

uint32 FAdvancePakProcThread::Run()
{
	if (FPaths::FileExists(ProcURL) && FPlatformProcess::CreatePipe(ReadPipe, WritePipe))
	{

		ProcHandle = FPlatformProcess::CreateProc(*ProcURL, *ProcParams, false, true, true, &ProcId, 0, NULL, WritePipe);
		if (ProcHandle.IsValid() && FPlatformProcess::IsApplicationRunning(ProcId))
			ProcBeginDelegate.Broadcast();

		while (ProcHandle.IsValid() && FPlatformProcess::IsApplicationRunning(ProcId))
		{
			FPlatformProcess::Sleep(0.5f);

			OutputBuffer += FPlatformProcess::ReadPipe(ReadPipe);

			int32 LineStartIdx = 0;
			for (int32 Idx = 0; Idx < OutputBuffer.Len(); Idx++)
			{
				if (OutputBuffer[Idx] == '\n')
				{
					ProcOutputDelegate.Broadcast(OutputBuffer.Mid(LineStartIdx, Idx - LineStartIdx));

					if (Idx + 1 < OutputBuffer.Len() && OutputBuffer[Idx + 1] == '\n')
					{
						Idx++;
					}

					LineStartIdx = Idx + 1;
				}
			}

			OutputBuffer = OutputBuffer.Mid(LineStartIdx, MAX_int32);
		}

		int32 ReturnCode;
		FPlatformProcess::GetProcReturnCode(ProcHandle, &ReturnCode);
		if (ReturnCode != 0 || IsCancel)
			ProcFailedDelegate.Broadcast();
		else
			ProcSucceedDelegate.Broadcast();

	}
	return 0;
}

void FAdvancePakProcThread::Stop()
{
	if (ProcHandle.IsValid() && FPlatformProcess::IsApplicationRunning(ProcId))
	{
		FPlatformProcess::TerminateProc(ProcHandle, true);
		ProcHandle.Reset();
		ProcId = 0;
	}
}

void FAdvancePakProcThread::Exit()
{
	if (ProcHandle.IsValid())
	{
		FPlatformProcess::CloseProc(ProcHandle);
		FPlatformProcess::ClosePipe(ReadPipe, WritePipe);
	}
}
