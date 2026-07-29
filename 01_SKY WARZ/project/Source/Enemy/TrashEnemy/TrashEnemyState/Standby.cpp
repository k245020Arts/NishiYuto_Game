#include "Standby.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "../../../Common/Random/Random.h"
#include "../../../Camera/Camera.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Component/Physics/Physics.h"

Standby::Standby()
{
	animId = ID::TE_STANCE;
	string = Function::GetClassNameC<Standby>();
	cooperateCounter = 0;

	range = 0.0f;
	randomSpeed = 0.0f;
	redefinitionCounter = 0.0f;
	isRedefinition = true;

	runTime = 0.0f;
}

Standby::~Standby()
{
}

void Standby::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	pPos = enemy->enemyBaseComponent.playerObj->GetTransform()->position;
	enemy->LookTarget(pPos);

	vec = enemy->enemyBaseComponent.playerObj->GetTransform()->position - enemy->GetPos();
	if (!enemy->isCooperateAtk)
	{
		InCameraView();

		if (vec.Size() <= range)//プレイヤーとの距離を見て後ろに下がる
		{
			isRedefinition = true;
			enemy->NormalMove(BACKSPEED);
		}
		else
		{
			RotateMove(enemy);
			if (vec.Size() <= range / 2)
			{
				pPos = enemy->enemyBaseComponent.playerObj->GetTransform()->position;
				cooperateCounter = 0;
			}
		}

		if (vec.Size() >= enemy->GetStatus().chaseRange)
			runTime += Time::DeltaTimeRate();
		else
			runTime = 0.0f;

		//runと切り替わるときにすぐ切り替わらないようにクールタイムを設けた
		if (runTime >= RunTimeMac)
			enemy->ChangeState(StateID::T_ENEMY_RUN_S);
	}
	else
	{
		CooperateMove(enemy);
	}
}

void Standby::Draw()
{
}

void Standby::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	range = enemy->GetStatus().runRange;
	
	if (enemy->isCooperateAtk)
		enemy->isMovingToPlayer = true;
		
	pPos = enemy->enemyBaseComponent.playerObj->GetTransform()->position;

	randomSpeed = (float)Random::GetReal();

	runTime = 0.0f;

	enemy->isAtkStandby = true;

	EnemyStateBase::Start();
}

void Standby::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	cooperateCounter = 0;
	enemy->isStandby = false;
	enemy->isAttack = false;
	isRedefinition = true;

	enemy->isAtkStandby = false;
}

void Standby::RotateMove(TrashEnemy* _enemy)
{
	const VECTOR3 EPos = _enemy->GetPos();
	const float vecMax = 200.0f;

	if (isRedefinition)
	{
		float MaxPos = 1500;
		_enemy->enemyBaseComponent.anim->Play(ID::TE_STANCE);
		float _rangeX = MaxPos * (float)Random::GetReal() - MaxPos * (float)Random::GetReal();
		float _rangeZ = MaxPos * (float)Random::GetReal() - MaxPos * (float)Random::GetReal();
		newPos = VECTOR3(_rangeX, 0, _rangeZ) + EPos;
		isRedefinition = false;
	}
	else if (VSize(newPos - EPos) >= vecMax)
	{
		// 移動
		const float Speed = 10.0f;
		VECTOR3 dir = VNorm(newPos - EPos);
		dir.y = 0.0f;
		_enemy->Move(dir, Speed);
	}
	else
	{
		redefinitionCounter += Time::DeltaTimeRate();
		_enemy->enemyBaseComponent.anim->Play(ID::TE_IDOL);
		const float MaxTime = 1.0f;

		if (redefinitionCounter >= MaxTime)
		{
			isRedefinition = true;
			redefinitionCounter = 0;
		}
	}
}

void Standby::CooperateMove(TrashEnemy* _enemy)
{
	const float C_CoolTime = 1.0f;
	const float C_CounterMax = 2.0f;

	if (cooperateCounter <= C_CoolTime)//定位置についてから少しクールタイムをもうける
		cooperateCounter += Time::DeltaTimeRate();
	else if (cooperateCounter != C_CounterMax)//マネージャーに準備完了したことを伝える
	{
		_enemy->isStandby = true;
		EffectManager::GetInstance()->CreateEffekseer(Transform(_enemy->GetPos(), VZero, VOne), nullptr, Effect_ID::COOPERATEATTACK, 1.0f);
		cooperateCounter = C_CounterMax;
	}
	else//falseにしないとマネージャー側で一体ずつカウントがやりずらい
		_enemy->isStandby = false;
}

void Standby::InCameraView()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	const float Viewing = 45.0f;
	VECTOR3 cameraPos = enemy->enemyBaseComponent.camera->GetCameraTransform()->position;
	cameraPos.y = 0;
	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(enemy->enemyBaseComponent.camera->GetCameraTransform()->rotation.y);
	VECTOR3 vec = enemy->GetEnemyObj()->GetTransform()->position - enemy->enemyBaseComponent.playerObj->GetTransform()->position;
	
	//内積
	float dotProduct = VDot(frontVec, vec.Normalize());
	
	if (dotProduct > cosf(Viewing * DegToRad) )//カメラに写っているかつプレイヤーの前
		enemy->isAttack = true;
	else
		enemy->isAttack = false;
}
