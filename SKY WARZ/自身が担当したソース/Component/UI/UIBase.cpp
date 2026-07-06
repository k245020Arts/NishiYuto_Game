#include "UIBase.h"
#include "UIManager/UIManager.h"

UIBase::UIBase()
{
	uiManager = FindGameObject<UIManager>();
}

UIBase::~UIBase()
{
	//リストから削除
	if (uiManager != nullptr) {
		uiManager->UIDelete(obj);
	}
	
}

void UIBase::Awake()
{
	if (uiManager == nullptr) {
		return; //UiManagerがnullptrならリターン
	}
	uiManager->UIPush(obj);//リストに登録
	obj->SetDraw(false);
}
