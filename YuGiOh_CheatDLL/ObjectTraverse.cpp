#include "stdafx.h"
#include "ObjectTraverse.h"
#include <MyTools/Character.h>

UINT CObjectTraverse::GetVecCard(_Out_ std::vector<Card>& VecCard) CONST
{
	for (UINT i = 1; i <= 0x45A; ++i)
	{
		Card Card_;
		Card_.dwCardId = i;
		SetCardName(Card_);
		if (Card_.wsCardName == L"")
			continue;

		SetCardRace(Card_);
		SetCardRemark(Card_);
		SetCardType(Card_);
		if (Card_.emCardType == em_Card_Type::em_Card_Type_Monter)
		{
			SetCardPower(Card_);
			SetCardDefence(Card_);
		}
		VecCard.push_back(std::move(Card_));
	}

	return VecCard.size();
}

VOID CObjectTraverse::SetCardName(_In_ _Out_ Card& Card_) CONST
{
	// [5F25A8+8] + ID<<6
	CHAR* pszNamePtr = reinterpret_cast<CHAR*>(CCharacter::ReadDWORD(0x5F25A8 + 8) + (Card_.dwCardId << 0x6));
	Card_.wsCardName = CCharacter::ASCIIToUnicode(pszNamePtr);
}

VOID CObjectTraverse::SetCardRace(_In_ _Out_ Card& Card_) CONST
{
	/*
	ID += 1F4
	Offset = [5F24E8+8]+ID*4
	Offset+[5F24B8+8]=种族
	*/
	DWORD dwOffset = CCharacter::ReadDWORD(CCharacter::ReadDWORD(0x5F24E8 + 0x8) + (Card_.dwCardId + 0x1F4) * 4);
	CHAR* pszRacePtr = reinterpret_cast<CHAR*>(CCharacter::ReadDWORD(0x5F24B8 + 0x8) + dwOffset);
	Card_.wsRace = CCharacter::ASCIIToUnicode(pszRacePtr);
}

VOID CObjectTraverse::SetCardRemark(_In_ _Out_ Card& Card_) CONST
{
	/*
	Offset = [5F2578+8]+ID*4
	EAX=[5F2548+8]+Offset=介绍
	*/
	DWORD dwOffset = CCharacter::ReadDWORD(CCharacter::ReadDWORD(0x5F2578 + 0x8) + Card_.dwCardId * 4);
	CHAR* pszRemarkPtr = reinterpret_cast<CHAR*>(CCharacter::ReadDWORD(0x5F2548 + 0x8) + dwOffset);
	Card_.wsRemark = CCharacter::ASCIIToUnicode(pszRemarkPtr);
}

VOID CObjectTraverse::SetCardType(_In_ _Out_ Card& Card_) CONST
{
	/*
	([5F2480+8]+ID*4) >> 0x14 & 0x1F=
	1=龙族
	3=恶魔族
	4=炎族
	6=岩石族
	7=机械族
	B=兽族
	C=兽战士族
	E=水族
	F=战士族
	12=魔法使用族
	15=陷阱卡
	16=魔法卡
	*/
	DWORD dwType = ((CCharacter::ReadDWORD(0x5F2480 + 0x8) + Card_.dwCardId * 4) << 0x14) & 0x1F;
	switch (dwType)
	{
	case 0x15:
		Card_.emCardType = em_Card_Type::em_Card_Type_Trap;
		break;
	case 0x16:
		Card_.emCardType = em_Card_Type::em_Card_Type_Magic;
		break;
	default:
		Card_.emCardType = em_Card_Type::em_Card_Type_Monter;
		break;
	}
}

VOID CObjectTraverse::SetCardPower(_In_ _Out_ Card& Card_) CONST
{
	// (([5F2480+8]+ID*4) >> 9)&0x1FF * 5 << 1 = 攻击力
	DWORD dwAddr = CCharacter::ReadDWORD(CCharacter::ReadDWORD(0x5F2480 + 0x8) + Card_.dwCardId * 0x4);
	dwAddr = (dwAddr >> 0x9) & 0x1FF;
	dwAddr *= 5;
	dwAddr <<= 0x1;
	Card_.dwPower = dwAddr;
}

VOID CObjectTraverse::SetCardDefence(_In_ _Out_ Card& Card_) CONST
{
	// ([5F2480+8]+ID*4)&0x1FF * 5 << 1 = 防御力
	DWORD dwAddr = CCharacter::ReadDWORD(CCharacter::ReadDWORD(0x5F2480 + 0x8) + Card_.dwCardId * 0x4);
	dwAddr &= 0x1FF;
	dwAddr *=  5;
	dwAddr <<= 0x1;
	Card_.dwDefence = dwAddr;
}

Card* CObjectTraverse::FindCardById(_In_ DWORD dwCardId) CONST
{
	static std::vector<Card> VecCard;
	if (VecCard.size() == 0)
		GetVecCard(VecCard);

	return CLPublic::Vec_find_if(VecCard, [dwCardId](Card& Card_){ return Card_.dwCardId == dwCardId; });
}
