#include "stdafx.h"
#include "GameExprFun.h"
#include "ObjectTraverse.h"
#include <MyTools/Character.h>
#define _SELF L"GameExprFun.cpp"

CGameExprFun::CGameExprFun()
{
}

CGameExprFun::~CGameExprFun()
{
	Release();
}

VOID CGameExprFun::Release()
{
	ReleaseHook(HookCutCardContent);
	ReleaseHook(ALLCardHookContent);
}

std::vector<ExpressionFunPtr>& CGameExprFun::GetVec()
{
	static std::vector<ExpressionFunPtr> Vec = 
	{
		{ std::bind(&CGameExprFun::Help, this, std::placeholders::_1), L"Help" },
		{ std::bind(&CGameExprFun::GetCard, this, std::placeholders::_1), L"GetCard" },
		{ std::bind(&CGameExprFun::HookCutCard, this, std::placeholders::_1), L"HookCutCard" },
		{ std::bind(&CGameExprFun::SetCutCardId, this, std::placeholders::_1), L"SetCutCardId" },
		{ std::bind(&CGameExprFun::HookALLCard, this, std::placeholders::_1), L"HookALLCard" },
	};

	return Vec;
}

VOID CGameExprFun::Help(CONST std::vector<std::wstring>&)
{
	auto& Vec = GetVec();
	for (CONST auto& itm : Vec)
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"FunctionName=%s", itm.wsFunName.c_str());
}

VOID CGameExprFun::GetCard(CONST std::vector<std::wstring>&)
{
	std::vector<Card> VecCard;
	CObjectTraverse::GetInstance().GetVecCard(VecCard);
	LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"Card.size()=%d", VecCard.size());
	for (auto& itm : VecCard)
	{
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"--------Card:%s-------------------", itm.wsCardName.c_str());
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"CardId=%X,Race=%s,CardType=%s,Power=%d,Defence=%d", \
			itm.dwCardId, itm.wsRace.c_str(), itm.GetCardTypeText(), itm.dwPower, itm.dwDefence);
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"Remark:%s", itm.wsRemark.c_str());
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"----------------------------------", itm.wsCardName.c_str());
	}
}

VOID CGameExprFun::ReleaseHook(_In_ _Out_opt_ MYHOOK_CONTENT& HookContent)
{
	if (HookContent.dwHookAddr != NULL)
	{
		CLHook::UnHook_Fun_Jmp_MyAddr(&HookContent);
		HookContent.Release();
	}
	LOG_C(CLog::em_Log_Type_Custome, L"解除Hook完毕!");
}


__declspec(naked) VOID WINAPI fnHookCutCard()
{
	static DWORD dwESP = 0;
	__asm MOV dwESP, ESP
	__asm CMP DX, 0x1B;
	__asm JNZ lbEnd;

	__asm PUSHAD;
	static DWORD dwCardId = NULL;
	static Card* pCard = nullptr;
	dwCardId = CGameExprFun::GetCutCardEvent().dwCardId;
	if (dwCardId != NULL)
	{
		__asm MOV EAX, dwESP;
		__asm MOV EBX, dwCardId;
		__asm MOV DWORD PTR DS : [EAX], EBX;
	}
	else
	{
		__asm MOV EAX, dwESP;
		__asm MOV EAX, DWORD PTR DS : [EAX];
		__asm MOV dwCardId, EAX;
	}
	LOG_C(CLog::em_Log_Type::em_Log_Type_Warning, L"Next CardId:=%X", dwCardId);
	pCard = CObjectTraverse::GetInstance().FindCardById(dwCardId);
	if (pCard != nullptr)
	{
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"--------Card:%s-------------------", pCard->wsCardName.c_str());
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"CardId=%X,Race=%s,CardType=%s,Power=%d,Defence=%d", \
			pCard->dwCardId, pCard->wsRace.c_str(), pCard->GetCardTypeText(), pCard->dwPower, pCard->dwDefence);
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"Remark:%s", pCard->wsRemark.c_str());
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"----------------------------------", pCard->wsCardName.c_str());
	}
	CCharacter::WriteDWORD(0xA5506C, dwCardId);
	CGameExprFun::GetCutCardEvent().dwCardId = NULL;
	__asm POPAD;
lbEnd:;
	__asm MOV WORD PTR DS : [ESP + 0x14],DX
	__asm PUSH 0x40D32E + 0x5;
	__asm RETN
}

VOID CGameExprFun::HookCutCard(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 1)
	{
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"HookCutCard(1=开启 0=关闭)");
		return;
	}

	if (VecParm.at(0) == L"1")
	{
		if (HookCutCardContent.dwHookAddr == NULL)
		{
			HookCutCardContent.dwFunAddr = reinterpret_cast<DWORD>(fnHookCutCard);
			HookCutCardContent.dwHookAddr = 0x40D32E;
			HookCutCardContent.uNopCount = 0x0;
			CLHook::Hook_Fun_Jmp_MyAddr(&HookCutCardContent);
		}
		LOG_C(CLog::em_Log_Type_Custome, L"Hook完毕!");
	}
	else
	{
		ReleaseHook(HookCutCardContent);
	}
}

VOID CGameExprFun::SetCutCardId(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 1)
	{
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"SetCutCardId(CardId)");
		return;
	}
	GetCutCardEvent().dwCardId = wcstol(VecParm.at(0).c_str(), nullptr, 16);
}

CGameExprFun::CutCardContent& CGameExprFun::GetCutCardEvent()
{
	static CutCardContent CutCardContent_ = { ::CreateEventW(NULL, FALSE, FALSE, NULL), NULL };
	return CutCardContent_;
}

__declspec(naked) VOID WINAPI fnHookAllCard()
{
	static DWORD dwESI = 0;
	__asm MOV dwESI, ESI;
	__asm PUSHAD;

	dwESI += 0xC;
	for (int i = 0; i < 0x45A; ++i)
	{
		CCharacter::WriteBYTE(dwESI + i * 2, 0x1);
	}

	__asm POPAD;
	__asm MOV ECX, 0x462;
	__asm PUSH 0x4833C3 + 0x5;
	__asm RETN;
}

VOID CGameExprFun::HookALLCard(CONST std::vector<std::wstring>& VecParm)
{
	// 004833C3    B9 62040000     mov     ecx, 462     ; 自己的卡片数量
	// 004833C8    BF C03CA500     mov     edi, 00A53CC0; EDI = 鉴赏卡片的地址
	if (VecParm.size() == 0)
	{
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"HookALLCard(1=开启 0=关闭)");
		return;
	}

	if (VecParm.at(0) == L"1")
	{
		if (ALLCardHookContent.dwHookAddr == NULL)
		{
			ALLCardHookContent.dwFunAddr = reinterpret_cast<DWORD>(fnHookAllCard);
			ALLCardHookContent.dwHookAddr = 0x4833C3;
			ALLCardHookContent.uNopCount = 0x0;
			CLHook::Hook_Fun_Jmp_MyAddr(&ALLCardHookContent);
		}
		LOG_C(CLog::em_Log_Type_Custome, L"Hook完毕!");
	}
	else
	{
		ReleaseHook(ALLCardHookContent);
	}
}
