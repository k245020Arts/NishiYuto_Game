#include "EnemyBase.h"
#include "../Component/Animator/Animator.h"
#include "../Component/Color/color.h"
#include "../State/StateManager.h"
#include "../Weapon/WeaponManager.h"
#include "../Component/Physics/Physics.h"
#include "../Player/PlayerState/AttackState/PlayerAttackStateBase.h"
#include "../Player/PlayerState/AttackState/PlayerAttack3.h"
#include "../Player/PlayerState/AttackState/PlayerAttack2.h"
#include "../Common/Memory/MemoryCount.h"
#include "EnemyManager.h"
#include "../Common/ResourceLoader.h"
#include "../Component/UI/EnemyDamageUI.h"
#include "../Component/ComponentManager.h"
#include "../Camera/Camera.h"

namespace {
	std::shared_ptr<PlayerAttackStateBase> pAttack;
}

EnemyBase::EnemyBase()
{
	loopNum = -1;
	hit = false;
	hitCounter = 0.0f;
	pState = nullptr;
	alpha = 255;
	FindGameObject<EnemyManager>()->AddList(this, obj);
	upPos = VECTOR3(0, 600, 0);
	cursolImage = -1;
	damageFlash = 0.0f;
	specialAttackHit = false;
	justAvoidCollTime = 0.0f;

}

EnemyBase::~EnemyBase()
{
	FindGameObject<EnemyManager>()->RemoveList(this, obj);
}

void EnemyBase::Update()
{
	if (!enemyBaseComponent.anim->AnimEventCan()) {
		if (attackColl.instance != nullptr) {
			DeleteColliderComponent(&attackColl);
			attackColl.instance = nullptr;
		}
	}
	justAvoidCollTime -= Time::DeltaTimeRate();
	float time = enemyBaseComponent.anim->EventStartTime(enemyBaseComponent.anim->GetCurrentID());
	if (time + 6.0f >= enemyBaseComponent.anim->GetCurrentFrame()) {
		if (justAvoidColl.instance != nullptr) {
			DeleteColliderComponent(&justAvoidColl);
			justAvoidColl.instance = nullptr;
		}
		justAvoidCollTime = 0.0f;
	}
	//敵のダメージフラッシュが終わったら元に戻す
	if (damageFlash > 0.0f) {
		damageFlash -= Time::DeltaTimeRate();
		if (damageFlash < 0.0f) {
			enemyBaseComponent.color->setRGB(Color::Rgb(255.0f, 255.0f, 255.0f, 255.0f));
		}
	}
	if (CheckHitKey(KEY_INPUT_G)) {
		obj->GetTransform()->position = EnemyInformation::BASE_POS;
	}
	//多段ヒットの処理
	if (loopNum >= 0) {
		hitCounter -= Time::DeltaTimeRate();
		if (hitCounter <= 0.0f) {
			PlayerHit(CollsionEventData());
		}
	}
	pAttack = pState->GetState<PlayerAttackStateBase>();
	if (pAttack != nullptr) {
		if (pAttack->GetCollsionCreate()) {
			/*name = pAttack->GetString().c_str();*/
			/*if (pState->GetState<PlayerAttack2>() != nullptr) {
				Debug::DebugLogPrintfArgs("BossHitAttack2Reset = %s", name);
			}*/
			if (pState->GetState<PlayerAttack3>() != nullptr) {
				Debug::DebugLogPrintfArgs("BossHitAttack3Reset = %s", name);
			}
			
			hit = false;
		}
		
	}
	/*const Color::Rgb r = enemyBaseComponent.color->GetRgb();
	enemyBaseComponent.color->setRGB(Color::Rgb(r.r, r.g, r.b, (float)alpha));*/
}

void EnemyBase::Draw()
{
	if (!lastTarget) {
		return;
	}

	//ロックオンコーンの表示

	//ライトをOFFにして、影の描画をなくす
	SetUseLighting(FALSE);
	DrawCone3D(obj->GetTransform()->position + upPos, obj->GetTransform()->position + upPos + VECTOR3(0.0f,50.0f,0.0f) * MGetRotY(obj->GetTransform()->rotation.y), 45, 2, 0xffff00, 0xffff00, true);
	SetUseLighting(TRUE);
}

void EnemyBase::DrawTrail() {

	//剣の軌跡を作成
	enemyBaseComponent.weapon->CreateTrailEnemy(VECTOR3(0, 0, 0), VECTOR3(-70, 200, -230), 100.0f, 10.0f, 200.0f, 255.0f, 28, 0.5f);
}

void EnemyBase::EnemyDamageMove(const EnemyDamage::EnemyDamageInfo& _info)
{
	//敵がダメージを受けた時の吹っ飛ばし量の設定
	enemyBaseComponent.physics->SetVelocity(_info.speed * MGetRotY(enemyBaseComponent.playerObj->GetTransform()->rotation.y));
}

float EnemyBase::DamageCalculation(const VECTOR3& _pos, float _damage,float _defense, float deviation)
{
	float damage = _damage + (_damage * 2) / (_defense / 4);

	damageNum = new Object2D();

	damageNum->Init(Transform(VZero, VZero, VOne), "damageNum");
	damageNum->Component()->AddComponent<EnemyDamageUI>()->
		SetInformation(_pos,
		(int)damage + GetRand((int)deviation), VECTOR3(0.0f, -0.4f, 0.0f), 0.5f,
		ResourceLoad::GetHandle(ID::DAMAGE_UI_BUTTON), GetEnemyObj()->GetTransform(), VECTOR2I(81, 90));
	
	return damage;
}