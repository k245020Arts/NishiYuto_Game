#include "UIManager.h"
#include "../ButtonUI.h"
#include "../../Object/BaseObject.h"
#include "../../Transform/Transform.h"
#include "../../ComponentManager.h"
#include "../../../Common/ResourceLoader.h"
#include "../../../Common/Debug/Debug.h"
#include "../../../Common/Debug/DebugLogText.h"
#include "../../Color/Color.h"
#include "../../../Enemy/EnemyManager.h"
#include "../../../Camera/Camera.h"

//#define PATTERN1

UIManager::UIManager()
{
	SetDrawOrder(-200000);
#ifdef PATTERN1
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Mark_Avoid", ID::A_BUTTON);
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Mark_Normal", ID::B_BUTTON);
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Mark_Special", ID::X_BUTTON);
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Mark_Heavy", ID::Y_BUTTON);
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Number_01", ID::DAMAGE_UI_BUTTON);
#else
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Mark_Avoid2", ID::A_BUTTON);
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Mark_Normal2", ID::B_BUTTON);
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Mark_Special2", ID::X_BUTTON);
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Mark_Heavy2", ID::Y_BUTTON);
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Number_01", ID::DAMAGE_UI_BUTTON);

#endif // PATTERN1

	
	draw = true;
}

UIManager::~UIManager()
{
	uiObjects.clear();
}

void UIManager::Update()
{
	/*if (!FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>()->IsCutScene()) {
		assert(false);
	}*/
}

void UIManager::Draw()
{
	if (!draw || !Debug::UIDraw()) {
		return;
	}

	for (auto itr = uiObjects.begin(); itr != uiObjects.end(); itr++) {
		
		(*itr)->SetDraw(true);
		
		(*itr)->Draw();
		
		(*itr)->SetDraw(false);
	}
	/*if (FindGameObject<EnemyManager>()->GetEnemySize() >= 1) {
		assert(false);
	}*/
	
	

}

void UIManager::ButtonUISet()
{
#ifdef PATTERN1
	Object2D* abutton = new Object2D();
	abutton->Init(Transform(VECTOR3(1650, 1000, 0), VZero, VOne), "AButton");
	ButtonUI* a = abutton->Component()->AddComponent<ButtonUI>();
	a->Start(ButtonUI::A_BUTTON, ResourceLoad::GetHandle(ID::A_BUTTON), LIGHT_GREEN);
	abutton->SetDrawOrder(-1000);

	Object2D* bbutton = new Object2D();
	bbutton->Init(Transform(VECTOR3(1750, 900, 0), VZero, VOne), "BButton");
	ButtonUI* b = bbutton->Component()->AddComponent<ButtonUI>();
	b->Start(ButtonUI::B_BUTTON, ResourceLoad::GetHandle(ID::B_BUTTON), MAGENTA);
	bbutton->SetDrawOrder(-1000);

	Object2D* xbutton = new Object2D();
	xbutton->Init(Transform(VECTOR3(1550, 900, 0), VZero, VOne), "XButton");
	ButtonUI* c = xbutton->Component()->AddComponent<ButtonUI>();
	c->Start(ButtonUI::X_BUTTON, ResourceLoad::GetHandle(ID::X_BUTTON), LIGHT_BLUE);
	xbutton->SetDrawOrder(-1000);

	Object2D* ybutton = new Object2D();
	ybutton->Init(Transform(VECTOR3(1650, 800, 0), VZero, VOne), "YButton");
	ButtonUI* y = ybutton->Component()->AddComponent<ButtonUI>();
	y->Start(ButtonUI::Y_BUTTON, ResourceLoad::GetHandle(ID::Y_BUTTON), GOLD);
	ybutton->SetDrawOrder(-1000);
	ybutton->SetDrawOrder(-1000);
#else
	Object2D* abutton = new Object2D();
	abutton->Init(Transform(VECTOR3(1650, 1000, 0), VZero, VOne), "AButton");
	ButtonUI* a = abutton->Component()->AddComponent<ButtonUI>();
	a->Start(ButtonUI::A_BUTTON, ResourceLoad::GetHandle(ID::A_BUTTON), DARK_BLUE);
	abutton->SetDrawOrder(-1000);

	Object2D* bbutton = new Object2D();
	bbutton->Init(Transform(VECTOR3(1750, 900, 0), VZero, VOne), "BButton");
	ButtonUI* b = bbutton->Component()->AddComponent<ButtonUI>();
	b->Start(ButtonUI::B_BUTTON, ResourceLoad::GetHandle(ID::B_BUTTON), DARK_RED);
	bbutton->SetDrawOrder(-1000);

	Object2D* xbutton = new Object2D();
	xbutton->Init(Transform(VECTOR3(1550, 900, 0), VZero, VOne), "XButton");
	ButtonUI* c = xbutton->Component()->AddComponent<ButtonUI>();
	c->Start(ButtonUI::X_BUTTON, ResourceLoad::GetHandle(ID::X_BUTTON), DARK_GREEN);
	xbutton->SetDrawOrder(-1000);

	Object2D* ybutton = new Object2D();
	ybutton->Init(Transform(VECTOR3(1650, 800, 0), VZero, VOne), "YButton");
	ButtonUI* y = ybutton->Component()->AddComponent<ButtonUI>();
	y->Start(ButtonUI::Y_BUTTON, ResourceLoad::GetHandle(ID::Y_BUTTON), GOLD);
	ybutton->SetDrawOrder(-1000);
#endif // PATTERN1

	
}

void UIManager::UIPush(BaseObject* _base)
{
	uiObjects.emplace_back(_base);
}

void UIManager::UIDelete(BaseObject* _base)
{
	for (auto itr = uiObjects.begin(); itr != uiObjects.end(); itr++) {
		if (*itr == _base) {
			uiObjects.erase(itr);
			return;
		}
	}
}
