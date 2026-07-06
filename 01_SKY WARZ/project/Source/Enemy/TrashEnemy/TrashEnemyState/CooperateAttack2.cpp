#include "CooperateAttack2.h"
#include "../TrashEnemy.h"
#include "T_EnemyStatus.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/Collider/ColliderBase.h"
#include "../../../Common/Easing.h"
#include "../../../Common/InputManager/PadInput.h"
#include "../../../Common/InputManager/InputManager.h"
#include "../../../Camera/Camera.h"

CooperateAttack2::CooperateAttack2()
{
	string = Function::GetClassNameC<CooperateAttack2>();
	animId = ID::TE_R_C_ATTACK;
	attackParam.animID = ID::TE_R_C_ATTACK;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern = EnemyAttackBase::BACK;

	attackParam.hitDamage = 50;
	attackParam.useFlash = true;
	attackParam.attackFlashStartTime = 0.7f;
	attackParam.slowAmout = 0.1f;
	attackParam.slowTime = 0.3f;
	attackParam.speedUpMotionSpeed = 0.3f;

	damageMove = false;

	speedDownCounter = 0;

	hitStopCounter = 0.0f;

	pPos = VZero;

	isDamageMove = true;

	motionSpeed = 0;
}

CooperateAttack2::~CooperateAttack2()
{
}

void CooperateAttack2::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	if (enemy->IsPlayerSpecialMove())//•KŽE‹ZŽž‚ÉŽ~‚Ü‚ç‚¸‚É“®‚­‚©‚ç’âŽ~‚³‚¹‚é
		return;

	if (!damageMove)
			RangedMove(enemy);
		else
			DamageMove(enemy);
}

void CooperateAttack2::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	firstColl = true;

	EnemyAttackBase::collTrans.position	= CollPos;
	EnemyAttackBase::collTrans.scale		= Collscale;

	enemy->isMovingToPlayer = true;

	EnemyStateBase::Start();
}

void CooperateAttack2::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	
	enemy->CooperateAtkFinish();
}

void CooperateAttack2::RangedMove(TrashEnemy* _enemy)
{
	const VECTOR3 enePos = _enemy->GetPos();
	const VECTOR3 targetPos = _enemy->cooperateWayPoint;
	VECTOR3 dir = VZero;
	const float Speed = 50.0f;
	const float SearchPosMax = 300.0f;
	const float Max = 50.0f;

	if (VSize(pPos - enePos) > SearchPosMax)
		pPos = _enemy->enemyBaseComponent.playerObj->GetTransform()->position;

	_enemy->LookTarget(pPos);

	if (_enemy->cooperateDamageMove && isDamageMove)//ƒ_ƒ[ƒW‚ð‚à‚ç‚Á‚½Žž‚Ìˆ—
	{
		damageMove = true;
		_enemy->DeleteCollision(&_enemy->attackColl);
		InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartTime(ControllerPower, SceondTime);
		return;
	}
	else if (VSize(pPos - enePos) < Max )//’n–Ê‚É’…’n‚µ‚½Žž
	{
		if (!_enemy->cooperateDamageMove)//UŒ‚‚ðH‚ç‚Á‚Ä‚È‚©‚Á‚½‚Æ‚«
		{
			_enemy->enemyBaseComponent.anim->Play(ID::TE_R_IDOL);
			_enemy->enemyBaseComponent.anim->SetPlaySpeed(1.0f);//ƒ‚[ƒVƒ‡ƒ“‘¬“x‚ª0‚É‚È‚é‚©‚ç“ü‚ê‚é

			isDamageMove = false;
			_enemy->isStandby = false;

		}
		else if (!isDamageMove)//UŒ‚‚ðH‚ç‚¤‚Ì‚Æ’…’n‚ª“¯Žž‚¾‚Á‚½Žž
			isDamageMove = true;

		return;
	}

	const float ROTY = -_enemy->enemyBaseComponent.playerObj->GetTransform()->rotation.y - 0.5f * DX_PI_F;
	dir = VNorm(pPos - enePos);
	
	_enemy->GetEnemyObj()->GetTransform()->position += dir * Speed; 
	
	AttackCollsion();
	AttackSound();
	AttackFlash(ID::E_MODEL, 35, "E_AttackV");
	Trail();
}

void CooperateAttack2::DamageMove(TrashEnemy* _enemy)
{
	const float CounterMax = 1.0f;

	//_enemy->enemyBaseComponent.camera->ChangeStateCamera(StateID::R_ENEMY_FINISH_CAMERA_S);

	hitStopCounter += Time::DeltaTimeRate();

	if (hitStopCounter < CounterMax)
	{
		_enemy->GetEnemyObj()->GetTransform()->position += sinf(hitStopCounter * 60) * VECTOR3(10, 0, 10);
		return;
	}
	
	const VECTOR3 enePos = _enemy->GetPos();
	const VECTOR3 targetPos = _enemy->cooperateWayPoint;
	VECTOR3 dir = VNorm(targetPos - enePos);
	const float Speed = 100.0f;
	
	speedDownCounter += Time::DeltaTimeRate();

	_enemy->GetEnemyObj()->GetTransform()->position += dir * (Speed - speedDownCounter);
}
