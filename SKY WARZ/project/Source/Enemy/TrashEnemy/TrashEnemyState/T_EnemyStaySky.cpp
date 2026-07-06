#include "T_EnemyStaySky.h"
#include "../TrashEnemy.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Camera/Camera.h"
#include "../../../Player/Player.h"

T_EnemyStaySky::T_EnemyStaySky()
{
	animId = ID::TE_R_IDOL;
	string = Function::GetClassNameC<T_EnemyStaySky>();

	isLeader = false;
	setGravity = VZero;
}

T_EnemyStaySky::~T_EnemyStaySky()
{
}

void T_EnemyStaySky::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	if (enemy->IsPlayerSpecialMove())//必殺技時に止まらずに動くから停止させる
		return;

	if (!isLeader)
		RangedMove(enemy);
	else
		LeaderMove(enemy);
}

void T_EnemyStaySky::Start()
{
	const TrashEnemy* enemy = GetBase<TrashEnemy>();

	if (enemy->GetEnemyType() == EnemyType::RANGED_LEADER)
		isLeader = true;

	setGravity = enemy->enemyBaseComponent.physics->GetGravity();
	enemy->enemyBaseComponent.physics->SetGravity(VZero);

	EnemyStateBase::Start();
}

void T_EnemyStaySky::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	enemy->enemyBaseComponent.physics->SetGravity(setGravity);

	if (isLeader)
	{
		enemy->CooperateAtkFinish();
		enemy->enemyBaseComponent.camera->ChangeStateCamera(StateID::FREE_CAMERA_S);
	}
}

void T_EnemyStaySky::LeaderMove(TrashEnemy* _enemy)
{
	const float Speed = 50.0f;
	const float MaxPos = 1800.0f;

	if (_enemy->GetPos().y <= MaxPos)
		_enemy->GetEnemyObj()->GetTransform()->position.y += Speed;
	else
	{
		_enemy->isStandby = true;
		_enemy->enemyBaseComponent.camera->ChangeStateCamera(StateID::R_ENEMY_CAMERA_S);
	}

	//空を飛んでるため距離を測るために使う(2D状)
	VECTOR3 ePosRevision = _enemy->GetPos();
	ePosRevision.y = 0.0f;

	VECTOR3 vec = _enemy->enemyBaseComponent.playerObj->GetTransform()->position - ePosRevision;

	const float PlayerVecSize = 2000.0f;
	const float BackSpeed = 80.0f;

	//プレイヤーが近づかないようにする処理
	if (vec.Size() < PlayerVecSize)
		_enemy->enemyBaseComponent.playerObj->GetTransform()->position +=
		VECTOR3(0, 0, -1) * MGetRotY(_enemy->enemyBaseComponent.playerObj->GetTransform()->rotation.y) * BackSpeed;
}

void T_EnemyStaySky::RangedMove(TrashEnemy* _enemy)
{
	const VECTOR3 enePos = _enemy->GetPos();
	const VECTOR3 targetPos = _enemy->cooperateWayPoint;
	VECTOR3 dir = VZero;
	float speed = 0.0f;

	//リーダーの周りに移動
	if (VSize(targetPos - enePos) >= 30 && !_enemy->isStandby)
	{
		dir = VNorm(targetPos - enePos);
		speed = 30.0f;

		_enemy->GetEnemyObj()->GetTransform()->position += dir * speed;
	}
	else
		_enemy->isStandby = true;
}
