#include "BossWalk.h"
#include "../../../State/StateManager.h"
#include "../Boss.h"
#include "../../../Common/Random.h"

BossWalk::BossWalk()
{
	animId = ID::B_WAIT_SEE;
	string = Function::GetClassNameC<BossWalk>();
}

BossWalk::~BossWalk()
{
}

void BossWalk::Update()
{
	Boss* b = GetBase<Boss>();
	b-> LookPlayer();

	VECTOR3 enemyPos = b->obj->GetTransform()->position;
	float MAX = 100;

	if (isRedefinition)
	{
		float _rangeX = MAX * (float)Random::GetReal() - MAX * (float)Random::GetReal();
		float _rangeZ = MAX * (float)Random::GetReal() - MAX * (float)Random::GetReal();
		newPos = VECTOR3(_rangeX, 0, _rangeZ);
		isRedefinition = false;
	}

	VECTOR3 vec = b->GetEnemyObj()->GetTransform()->position - newPos;

	if (vec.Size() >= 300)// ˆÚ“®
		b->GetEnemyObj()->GetTransform()->position += 5 * newPos.Normalize();

	timeCounter += Time::DeltaTimeRate();
	if (timeCounter >= maxCounter)
		b->enemyBaseComponent.state->ChangeState(StateID::ATTACK_SORTING_S);
}

void BossWalk::Start()
{
	EnemyStateBase::Start();
	timeCounter = 0;
	maxCounter = GetRand(3);
}

void BossWalk::Finish()
{
	isRedefinition = true;
}
