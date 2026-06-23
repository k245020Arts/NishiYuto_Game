#include "T_EvadeState.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"

T_Evade::T_Evade()
{
	animId = ID::TE_STANCE;
	string = Function::GetClassNameC<T_Evade>();

	targetRotY = 0;
	targetPos = VZero;
	copyePos = VZero;

	isIdolAmim = false;
}

T_Evade::~T_Evade()
{
}

void T_Evade::Update()
{
	//このステートを呼ぶタイミングと戻すタイミングを作る
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	const VECTOR3 ePos = enemy->GetEnemyObj()->GetTransform()->position;

	if (VSize(enemy->enemyBaseComponent.playerObj->GetTransform()->position - ePos) >= 2000 && VSize(copyePos - ePos) >= 2000)
	{
		if (!isIdolAmim)
		{
			enemy->enemyBaseComponent.anim->Play(ID::TE_IDOL);
			isIdolAmim = true;
		}
		
		return;
	}

	if (isIdolAmim)
	{
		enemy->enemyBaseComponent.anim->Play(ID::TE_STANCE);
		isIdolAmim = false;
	}
		
	//正面べく
	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(targetRotY);
	//プレイヤーとのベクトル
	VECTOR3 vec = targetPos - ePos;
	// 右ベクトル
	VECTOR3 rightVec = VECTOR3(frontVec.z, 0.0f, -frontVec.x);
	// 左右判定
	float side = VDot(rightVec, vec);

	//0以上なら右側、0以上なら左側
	float dir = (side >= 0) ? 1.0f : -1.0f;

	enemy->GetEnemyObj()->GetTransform()->position -= rightVec * dir * BackSpeed;
}

void T_Evade::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	targetRotY = enemy->leaderRotY;
	targetPos = enemy->leaderPos;
	targetPos.y = 0.0f;

	copyePos = enemy->GetPos();

	EnemyStateBase::Start();
}

void T_Evade::Finish()
{
}
