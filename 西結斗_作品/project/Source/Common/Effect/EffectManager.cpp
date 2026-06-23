#include "EffectManager.h"
#include "../../Screen.h"
#include "../ResourceLoader.h"
#include "EffectBase.h"
#include "../../Component/Transform/Transform.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/Object/Object3D.h"

namespace {
	const float EFFECT_EDGE = -400.0f;
	const float EFFECT_EDGE_PLUS = 400.0f;
}


EffectManager::EffectManager()
{
	//------------------------------------ エフェクトのロードをしている -----------------------------------------------

	ResourceLoad::LoadEffect("enemyChrage3",				".efkefc",Effect_ID::ENEMY_FLASH,				50.0f,true);
	ResourceLoad::LoadEffect("hitEffect",					".efkefc",Effect_ID::HIT_EFFECT,				50.0f,true);
	ResourceLoad::LoadEffect("specialHit",					".efkefc",Effect_ID::SPECIAL_HIT_EFFECT,		50.0f,true);
	ResourceLoad::LoadEffect("justAvoidEffect",				".efkefc",Effect_ID::JUST_AVOID_EFFECT,			50.0f,true);
	ResourceLoad::LoadEffect("playerFlash",					".efkefc",Effect_ID::PLAYER_FLASH,				50.0f,true);
	ResourceLoad::LoadEffect("aura",						".efkefc",Effect_ID::PLAYER_AURA,				50.0f,true);
	ResourceLoad::LoadEffect("playerHit",					".efkefc",Effect_ID::PLAYER_HIT,				50.0f,true);
	ResourceLoad::LoadEffect("playerAttackEffect",			".efkefc",Effect_ID::PLAYER_ATTACK,				50.0f,true);
	ResourceLoad::LoadEffect("hitSlash",					".efkefc",Effect_ID::PLAYER_SLASH_ATTACK,		30.0f,true);
	ResourceLoad::LoadEffect("enemyAttackCicle",			".efkefc",Effect_ID::ENEMY_ATTACK_CICLE,		50.0f,true);
	ResourceLoad::LoadEffect("AlphaCutoff",					".efkefc",Effect_ID::EFFECT_WALL,				50.0f,true);
	ResourceLoad::LoadEffect("bossWave1",					".efkefc",Effect_ID::BOSS_WAVE,					50.0f,true);
	ResourceLoad::LoadEffect("bossGround",					".efkefc",Effect_ID::BOSS_GROUND,				30.0f,true);
	ResourceLoad::LoadEffect("bossRoor",					".efkefc",Effect_ID::BOSS_ROAR,					10.0f,true);
	ResourceLoad::LoadEffect("bossRoor",					".efkefc",Effect_ID::BOSS_ROAR,					10.0f,true);
	ResourceLoad::LoadEffect("PlayerSpecialSlash",			".efkefc",Effect_ID::PLAYER_SPECIAL_SLASH,		50.0f,true);
	ResourceLoad::LoadEffect("PlayerSpecialSlash",			".efkefc",Effect_ID::PLAYER_SPECIAL_SLASH,		50.0f,true);
	ResourceLoad::LoadEffect("PlayerSpecialPlace",			".efkefc",Effect_ID::PLAYER_SPECIAL_PLACE,		50.0f,true);
	ResourceLoad::LoadEffect("SpecialFinalAttack",			".efkefc",Effect_ID::PLAYER_SPECIAL_FINAL,		50.0f,true);
	ResourceLoad::LoadEffect("PlayerSpecialAttackFlash",	".efkefc",Effect_ID::PLAYER_SPECIAL_FLASH,		50.0f,true);
	ResourceLoad::LoadEffect("PlayerSpecialAttackCharge",	".efkefc",Effect_ID::PLAYER_SPECIAL_CHARGE,		50.0f,true);
	ResourceLoad::LoadEffect("PlayerChargeFirstEffect",		".efkefc",Effect_ID::PLAYER_CHARGE_FIRST,		20.0f,true);
	ResourceLoad::LoadEffect("PlayerChargeSecondEffect",	".efkefc",Effect_ID::PLAYER_CHARGE_SECOND,		40.0f,true);
	ResourceLoad::LoadEffect("PlayerChargeFinalEffect",		".efkefc",Effect_ID::PLAYER_CHARGE_FINAL,		60.0f,true);
	ResourceLoad::LoadEffect("specialAttackCan",			".efkefc",Effect_ID::PLAYER_SPECIAL_ATTACK_CAN,	30.0f,true);
	ResourceLoad::LoadEffect("PlayerChargeEnd",				".efkefc",Effect_ID::PLAYER_CHARGE_END,			35.0f,true);
	ResourceLoad::LoadEffect("RockBlast",					".efkefc",Effect_ID::ROCK_BLAST,				100.0f,true);
	ResourceLoad::LoadEffect("RockBreak",					".efkefc",Effect_ID::ROCK_BREAK,				100.0f,true);
	ResourceLoad::LoadEffect("RockFall",					".efkefc",Effect_ID::ROCK_FALL,					100.0f,true);
	ResourceLoad::LoadEffect("BossRockHitRush",				".efkefc",Effect_ID::BOSS_ROCK_HIT_RUSH,		50.0f,true);
	ResourceLoad::LoadEffect("playerAttackRock",			".efkefc",Effect_ID::PLAYER_ATTACK_ROCK,		50.0f,true);

	ResourceLoad::LoadEffect("enemyCooperateAttack",		".efk",	  Effect_ID::COOPERATEATTACK,			50.0f, true);
	
	//-----------------------------------------------------------------------------------------------------------------

	//Effekseer_SetGraphicsDeviceDXLib();
	//SetDrawOrder(-50000);
}

EffectManager::~EffectManager()
{
	AllStopEffekseer();
	effect.clear();
}

void EffectManager::DeleteAllEffect()
{
	for (auto e = effect.begin(); e != effect.end(); e++) {
		delete* e;
	}
	effect.clear();
}

void EffectManager::Update()
{
	/*for (auto e = effect.begin(); e != effect.end();) {

		EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();

		if (!base->IsActive()) {
			delete* e;
			(*e) = nullptr;
			e = effect.erase(e);
		}
		else {
			e++;
		}
	}*/

	UpdateEffekseer2D();
	UpdateEffekseer3D();
}

void EffectManager::Draw()
{
	Effekseer_Sync3DSetting();
	DrawEffekseer3D();
	DrawEffekseer2D();
}

BaseObject* EffectManager::CreateEffekseer(Transform _transform, BaseObject* _parent, Effect_ID::EFFECT_ID _id, float _time, bool effect3D)
{
	//エフェクトの生成
	Object3D* obj = new Object3D();
	obj->Init(_transform.position, _transform.rotation, _transform.scale, std::to_string(_id));
	EffectBase* base =  obj->Component()->AddComponent<EffectBase>();
	base->EffectInit(_transform,_parent,_id,_time,effect3D);
	effect.emplace_back(obj);
	obj->SetDrawOrder(10);
	
	return obj;
}

void EffectManager::StopEffekseer(Effect_ID::EFFECT_ID _id)
{
	for (auto e = effect.begin(); e != effect.end();) {

		EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();
		if (base->GetEffectID() == _id) {
			base->EffectStop();
			delete* e;
			(*e) = nullptr;
			e = effect.erase(e);
			return;
		}
		else {
			e++;
		}
	}
}

void EffectManager::SetSpeedEffekseer(Effect_ID::EFFECT_ID _id,float _speed)
{
	for (auto e = effect.begin(); e != effect.end();) {

		EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();
		if (base->GetEffectID() == _id) {
			base->EffectSpeedSet(_speed);
			return;
		}
		else {
			e++;
		}
	}
}

void EffectManager::AllStopEffekseer()
{
	for (auto e = effect.begin(); e != effect.end();) {

		EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();	
		base->EffectStop(); 
		delete* e;
		(*e) = nullptr;
		e = effect.erase(e);
	}
}

bool EffectManager::IsPlayIng(Effect_ID::EFFECT_ID _id)
{
	for (auto e = effect.begin(); e != effect.end();) {

		EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();
		if (base->GetEffectID() == _id) {
			return true;
		}
		else {
			e++;
		}
	}
	return false;
}

void EffectManager::SetColor(Effect_ID::EFFECT_ID _id, Color::Rgb _rgb)
{
	for (auto e = effect.begin(); e != effect.end();) {

		EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();
		if (base->GetEffectID() == _id) {
			base->SetColor(_rgb);
			e++;
		}
		else {
			e++;
		}
	}
}

void EffectManager::ParentTransformRemove(BaseObject* _obj)
{
	for (auto e = effect.begin(); e != effect.end();) {
		if (_obj == *e) { //オブジェクトが一致したら削除して関数終了(一個しか削除しない)
			EffectBase* base = (*e)->Component()->GetComponent<EffectBase>();
			base->ParentTransformRemove();
			return;
		}
		else {
			e++;
		}
	}
}

void EffectManager::RemoveEffekseer(BaseObject* _obj)
{
	effect.remove(_obj);
}