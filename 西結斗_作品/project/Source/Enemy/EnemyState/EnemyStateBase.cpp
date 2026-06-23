#include "EnemyStateBase.h"
#include "../../Component/Animator/Animator.h"
//#include "../Enemy.h"
#include "../../Component/Transform/transform.h"
#include "../../Component/Collider/sphereCollider.h"
#include "../EnemyBase.h"
#include "../Boss/Boss.h"
#include "../../Common/ResourceLoader.h"
#include "EnemyStateManager.h"
#include "../../Common/Effect/EffectManager.h"
#include "../../Common/Sound/SoundManager.h"
#include "../../Weapon/WeaponManager.h"
#include "../../Camera/Camera.h"
#include "../TrashEnemy/TrashEnemy.h"

#define PATTERN1

EnemyStateBase::EnemyStateBase()
{
	firstColl = false;
	animId = ID::E_GETUP;
	attackTime = 0.0f;
	keepAnimSpeed = 0.0f;



	collTrans = Transform();
	sound = false;;

	fallFrame = 0.0f;
	animSlowCounter = 0.0f;
	slowMode = false;
}

EnemyStateBase::~EnemyStateBase()
{
}

void EnemyStateBase::Update()
{
	EnemyBase* e = GetBase<EnemyBase>();
	if (animSlowCounter > 0.0f) {
		animSlowCounter -= Time::DeltaTimeRate();
		if (animSlowCounter < 0.0f) {
#ifdef PATTERN1
			e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed);
			slowMode = true;
#else
			e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed - 0.7f);
			/*if (!e->enemyBaseComponent.anim->AnimEventCan()) {
				e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed + 0.3f);
			}*/
#endif
		}
	}
	if (slowMode) {
		if (e->enemyBaseComponent.anim->EventFinishTime(animId) - 5.0f <= e->enemyBaseComponent.anim->GetCurrentFrame()) {
			e->enemyBaseComponent.anim->SetPlaySpeed(keepAnimSpeed + 0.3f);
			slowMode = false;
		}
	}
}

void EnemyStateBase::Start()
{
	EnemyBase* e = GetBase<EnemyBase>();
	e->enemyBaseComponent.anim->Play(animId);
	attackTime = e->enemyBaseComponent.anim->EventFinishTime(animId) - e->enemyBaseComponent.anim->EventStartTime(animId);
	sound = true;
	e->enemyBaseComponent.anim->AnimEventReset();
	slowMode = false;
}

