#include "T_EnemyDead.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Collider/ModelCollider.h"
#include "../../../Component/Collider/SphereCollider.h"
#include "../../../Common/Easing.h"
#include "../../../Component/Color/Color.h"
#include "../../EnemyManager.h"

T_EnemyDead::T_EnemyDead()
{
	string = Function::GetClassNameC<T_EnemyDead>();
	counter = 0;
	fadeCounter = FADE_SPEED;

}

T_EnemyDead::~T_EnemyDead()
{
}

void T_EnemyDead::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	if (enemy->IsPlayerSpecialMove())
	{
		enemy->enemyBaseComponent.anim->SetPlaySpeed(0);
		return;
	}
	else
		enemy->enemyBaseComponent.anim->SetPlaySpeed(motionSpeed);

	KnockbackMove(enemy, enemy->deadPreset);

	if (enemy->enemyBaseComponent.anim->IsFinish() && enemy->GetEnemyObj()->GetTransform()->position.y <= 0)
	{
		fadeCounter -= Time::DeltaTimeRate();
		float reet = fadeCounter / FADE_SPEED;
		float alph = Easing::EaseIn(0.0f, 255.0f, reet);

		float color = 255.0f;
		enemy->enemyBaseComponent.color->setRGB(Color::Rgb(255, 255, 255, alph));
		if (fadeCounter <= 0)
			enemy->active = false;
	}
}

void T_EnemyDead::Draw()
{
}

void T_EnemyDead::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	//‹ß‹——£‚Æ‰“‹——£‚Åƒ‚[ƒVƒ‡ƒ“‚ª•Ï‚í‚é‚©‚ç‚»‚Ì‚½‚ß‚ÌÝ’è
	switch (enemy->enemyType)
	{
	case EnemyType::MELEE:
		animId = ID::TE_DEAD;
		break;
	case EnemyType::RANGED_LEADER: 
	case EnemyType::RANGED:
		animId = ID::TE_R_DEAD;
		break;
	default:
		break;
	}

	enemy->enemyBaseComponent.anim->SetPlaySpeed(0.8f);
	if (!enemy->IsPlayerSpecialMove())
	{
		obj->Component()->RemoveAllComponent<SphereCollider>();
		obj->Component()->RemoveAllComponent<ModelCollider>();
	}
	
	motionSpeed = enemy->enemyBaseComponent.anim->GetPlaySpeed();
	counter = 0;
	FindGameObject<EnemyManager>()->RemoveList(enemy, obj);

	enemyPosY = enemy->GetPos().y;

	EnemyStateBase::Start();
}

void T_EnemyDead::Finish()
{
}
