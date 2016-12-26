#ifndef __YUGIOH_CHEATDLL_GAME_OBJECTTRAVERSE_H__
#define __YUGIOH_CHEATDLL_GAME_OBJECTTRAVERSE_H__

#include <MyTools/ClassInstance.h>
#include <vector>
#include <MyTools/CLPublic.h>

enum em_Card_Type
{
	em_Card_Type_Magic,
	em_Card_Type_Trap,
	em_Card_Type_Monter
};

struct Card
{
	std::wstring wsCardName;
	std::wstring wsRace;
	std::wstring wsRemark;
	em_Card_Type emCardType;
	DWORD		 dwPower;
	DWORD		 dwDefence;
	DWORD		 dwCardId;

	Card()
	{
		dwPower = dwDefence = dwCardId = NULL;
	}
	LPCWSTR GetCardTypeText() CONST
	{
		struct CardTypeText
		{
			em_Card_Type emType;
			std::wstring wsText;
		};
		static CONST std::vector<CardTypeText> Vec = 
		{
			{ em_Card_Type_Magic, L"Ä§·¨¿¨" },
			{ em_Card_Type_Trap, L"ÏÝÚå¿¨" },
			{ em_Card_Type_Monter, L"¹ÖÊÞ¿¨" },
		};
		CONST static std::wstring wsUnKnowCaardText = L"Î´Öª¿¨";

		auto p = CLPublic::Vec_find_if_Const(Vec, [this](CONST CardTypeText& CardTypeText_){ return CardTypeText_.emType == emCardType; });
		return p == nullptr ? wsUnKnowCaardText.c_str() : p->wsText.c_str();
	}
};

class CObjectTraverse : public CClassInstance<CObjectTraverse>
{
public:
	CObjectTraverse() = default;
	~CObjectTraverse() = default;

	CObjectTraverse(CONST CObjectTraverse&) = delete;
	VOID operator=(CONST CObjectTraverse&) = delete;

	UINT GetVecCard(_Out_ std::vector<Card>& VecCard) CONST;

	Card* FindCardById(_In_ DWORD dwCardId) CONST;
private:
	VOID SetCardName(_In_ _Out_ Card& Card_) CONST;

	VOID SetCardRace(_In_ _Out_ Card& Card_) CONST;

	VOID SetCardRemark(_In_ _Out_ Card& Card_) CONST;

	VOID SetCardType(_In_ _Out_ Card& Card_) CONST;

	VOID SetCardPower(_In_ _Out_ Card& Card_) CONST;

	VOID SetCardDefence(_In_ _Out_ Card& Card_) CONST;

};


#endif // !__YUGIOH_CHEATDLL_GAME_OBJECTTRAVERSE_H__
