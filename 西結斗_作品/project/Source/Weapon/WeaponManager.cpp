#include "weaponManager.h"
#include "../Component/Object/Object3D.h"
#include "../Component/Transform/transform.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "../Common/ResourceLoader.h"
#include "CharaWeapon.h"
#include "../Component/ComponentManager.h"

WeaponManager::WeaponManager()
{
	eObj = nullptr;
	pObj = nullptr;
	eWeapon = nullptr;
	pWeapon = nullptr;
}

WeaponManager::~WeaponManager()
{
}

void WeaponManager::Update()
{
}

void WeaponManager::Draw()
{
}

void WeaponManager::CreatePlayerWeapon()
{
	pObj =  new Object3D();
	pObj->Init((VECTOR3)MV1GetFramePosition(ResourceLoad::GetHandle(ID::P_MODEL), 73), VZero, VECTOR3(1, 1, 1), "PLAYER_WEAPON");

	Object3D* player = FindGameObjectWithTag<Object3D>("PLAYER");
	
	pWeapon = pObj->Component()->AddComponent<CharaWeapon>();
	
	//CharaWeapon* weaponL = pW->Component()->AddComponent<CharaWeapon>();
	pWeapon->ObjectPointer(player,73,ID::P_MODEL, ResourceLoad::LoadModel("Sword6", ID::IDType::P_WEAPON));
	
	pWeapon->SetImage(ResourceLoad::GetHandle(ID::SWORD_EFFECT));
	//pWeapon->CreateSwordEffect();
	//weaponL->ObjectPointer(player,9,ID::P_MODEL, Load::LoadModel(Load::MODEL_PATH + "Sword", ID::IDType::P_WEAPON));
}

void WeaponManager::CreateEnemyWeapon()
{
	eObj = new Object3D();
	eObj->Init((VECTOR3)MV1GetFramePosition(ResourceLoad::GetHandle(ID::E_MODEL), 28), VECTOR3(0.0f,0.0f,90.0f * DegToRad), VECTOR3(1, 1, 1), "ENEMY_WEAPON");

	Object3D* enemy = FindGameObjectWithTag<Object3D>("ENEMY");

	eWeapon = eObj->Component()->AddComponent<CharaWeapon>();

	eWeapon->ObjectPointer(enemy, 10, ID::E_MODEL, -1);
	eWeapon->SetImage(ResourceLoad::GetHandle(ID::SWORD_EFFECT));
	//wea->CreateSwordEffect();
}

void WeaponManager::CreateTrailPlayer(VECTOR3 _nPos, VECTOR3 _fPos, float _r, float _g, float _b, float _a, int index, float _time)
{
	pWeapon->CreateSwordEffect(_nPos,_fPos,_r,_g,_b,_a,index,_time);
}

void WeaponManager::CreateTrailEnemy(VECTOR3 _nPos, VECTOR3 _fPos, float _r, float _g, float _b, float _a, int index, float _time)
{
	eWeapon->CreateSwordEffect(_nPos, _fPos, _r, _g, _b, _a, index, _time);
}
