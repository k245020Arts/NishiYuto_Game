#include "BossBackStep.h"
#include "../../../Common/Easing.h"
#include "../../../State/StateManager.h"

BossBackStep::BossBackStep()
{
	animId = ID::B_BACKSTEP;
	//string = Function::GetClassNameC<BossCoolTime>();

	speed = 0.0f;
	time = 0.0f;
	counter = 0;
}

BossBackStep::~BossBackStep()
{
}

void BossBackStep::Update()
{
	//å„Ç≈èëÇ´íºÇ∑
	Boss* b = GetBase<Boss>();
	b->LookPlayer();
	speed = Easing::EaseIn(0.0f, MaxSpeed, MaxSpeed / 60);

	const float ROTY = -b->GetEnemyObj()->GetTransform()->rotation.y - 0.5f * DX_PI_F;
	VECTOR3 vec = pPos - b->GetEnemyObj()->GetTransform()->position;

	if (vec.Size() <= 2200.0f && counter == 1 || vec.Size() <= 4400 && counter == 2)
	{
		b->GetEnemyObj()->GetTransform()->position.x -= speed * cosf(ROTY);
		b->GetEnemyObj()->GetTransform()->position.z -= speed * sinf(ROTY);
	}
	else if (time < 0.5f)
		time += Time::DeltaTimeRate();
	else if (counter < 2)
	{
		time = 0.0f;
		counter++;
	}
	else
		time += Time::DeltaTimeRate();

	if (time >= 0.55f)
		b->enemyBaseComponent.state->ChangeState(StateID::ATTACK_SORTING_S);
	
	//time += Time::DeltaTimeRate();

	//if (vec.Size() <= 2200.0f && counter == 1|| vec.Size() <= 4400 && counter == 2)
	//{
	//	b->GetEnemyObj()->GetTransform()->position.x -= speed * cosf(ROTY);
	//	b->GetEnemyObj()->GetTransform()->position.z -= speed * sinf(ROTY);
	//}
	//else if (time >= 0.5f && counter < 2)
	//{
	//	time = 0.0f;
	//	counter++;
	//}
	///*else if(counter < 2)
	//	time = 0.0f;*/
	//else if(/*counter > 2 &&*/ time>=0.55f||time >= 3.0f)
	//	b->enemyBaseComponent.state->ChangeState(StateID::ATTACK_SORTING_S);
}

void BossBackStep::Start()
{
	Boss* b = GetBase<Boss>();
	EnemyStateBase::Start();
	speed = 0.0f;
	time = 0.0f;
	counter = 0;

	pPos = b->enemyBaseComponent.playerObj->GetTransform()->position;
}

void BossBackStep::Finish()
{
}
