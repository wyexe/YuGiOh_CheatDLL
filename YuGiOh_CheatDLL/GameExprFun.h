#ifndef __YUGIOH_CHEATDLL_GAME_GAMEEXPRFUN_H__
#define __YUGIOH_CHEATDLL_GAME_GAMEEXPRFUN_H__

#include <MyTools/Log.h>
#include <MyTools/CLExpression.h>
#include <MyTools/ClassInstance.h>
#include <MyTools/CLHook.h>

class CGameExprFun : public CExprFunBase, virtual public CClassInstance<CGameExprFun>
{
public:
	CGameExprFun();
	virtual ~CGameExprFun();

	virtual VOID Release();

	virtual std::vector<ExpressionFunPtr>& GetVec();

	struct CutCardContent
	{
		HANDLE hEvent;
		DWORD  dwCardId;
	};
	static CutCardContent& GetCutCardEvent();
private:
	virtual VOID Help(CONST std::vector<std::wstring>&);

	VOID GetCard(CONST std::vector<std::wstring>&);

	VOID HookCutCard(CONST std::vector<std::wstring>&);

	VOID SetCutCardId(CONST std::vector<std::wstring>&);

	VOID HookALLCard(CONST std::vector<std::wstring>&);

	VOID ReleaseHook(_In_ _Out_opt_ MYHOOK_CONTENT& HookContent);
private:
	MYHOOK_CONTENT HookCutCardContent;
	MYHOOK_CONTENT ALLCardHookContent;
};




#endif // !__YUGIOH_CHEATDLL_GAME_GAMEEXPRFUN_H__