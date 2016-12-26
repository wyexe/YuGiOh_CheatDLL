// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "GameExprFun.h"
DWORD WINAPI _WorkThread(LPVOID lpParm)
{
	CLog::GetInstance().SetClientName(L"YuGiOh", L"C:\\");
	CLog::GetInstance().GetLogExpr().SetVecExprFunPtr(CGameExprFun::GetInstance().GetVec());
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	static HANDLE hThread = NULL;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (hThread == nullptr)
			hThread = cbBEGINTHREADEX(NULL, NULL, _WorkThread, NULL, NULL, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

