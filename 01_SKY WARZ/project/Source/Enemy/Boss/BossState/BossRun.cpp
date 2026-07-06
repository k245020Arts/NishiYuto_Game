#include "BossRun.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
#include "AttackSorting.h"
#include "BossStatus.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Component/EnemyAttackObject/BossRock/BossRockManager.h"

BossRun::BossRun()
{
	//id = ID::B_RUN;
	animId	= ID::B_RUN;
	string	= Function::GetClassNameC<BossRun>();
	bs		= nullptr;
	islooping = true;

	targetPos = VZero;

	counter = 0.0f;
	speed = 0.0f;
}

BossRun::~BossRun()
{
}

void BossRun::Update()
{
	Boss* b = GetBase<Boss>();
	b->LookPlayer(targetPos, 0.1f);
	HitLine();

	if (bs == nullptr)
	{
		bs = b->bs;
		speed = bs->GetStatus().runSpeed;
	}

	//rotation = b->obj->GetTransform()->rotation;
	//const float ROTY = -rotation.y - 0.5f * DX_PI_F;

	////移動の計算
	//velocity.x = speed * cosf(ROTY);
	//velocity.z = speed * sinf(ROTY);

	////計算したものをボスのポジションに足す
	//b->obj->GetTransform()->position += velocity;

	float speed = 0.0f;
	float max	= 0.0f;

	//半分以下になったらスピードアップ
	if (b->hpRate == Boss::FIVE || b->hpRate == Boss::THREE) {
		speed = 2000.0f;
		max = 2500.0f;
	}
	else {
		speed	= 1000.0f;
		max		= 1500.0f;
	}
	b->MoveBoss(speed, max);

	VECTOR3 targetVec = b->bossTransform->position - b->enemyBaseComponent.playerObj->GetTransform()->position;
	//プレイヤーと離れたらアイドルになる
	if (targetVec.Size() >= bs->GetStatus().chaseRange)
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_IDOL_S);
	//プレイヤーに近づいたら攻撃に移行
	if (b->RunChangeAttack()) {
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_COOL_TIME_S);
	}
}

void BossRun::Draw()
{
}

void BossRun::Start()
{
	Boss* b = GetBase<Boss>();
	//ボスのHPが半分を切ったらモーションを変える
	if (b->hpRate == Boss::FIVE || b->hpRate == Boss::THREE) {
		animId = ID::B_DUSH_ANIM;
	}
	else {
		animId = ID::B_RUN;
	}
	EnemyStateBase::Start();
	targetPos = b->enemyBaseComponent.playerObj->GetTransform()->position;
	islooping = true;
}

void BossRun::Finish()
{
}

void BossRun::HitLine()
{
	std::list<VECTOR3> copy = RayCastHits();
	Boss* b = GetBase<Boss>();
	VECTOR3 rockPos = VECTOR3(INFINITY, INFINITY, INFINITY);
	VECTOR3 pPos = b->enemyBaseComponent.playerObj->GetTransform()->position;
	float size = 1500;
	counter += Time::DeltaTimeRate();

	if (VSize(targetPos - b->GetEnemyObj()->GetTransform()->position) < 400 || counter >= 1.0f)
	{
		islooping = true;
		targetPos = pPos;
		counter = 0.0f;
	}
		
	if (!islooping)
		return;

	if (copy.size() == 0)
		return;

	for (auto rock : copy)
	{
		VECTOR3 copyVec = b->bossTransform->position - rock;
		if (rockPos.Size() > copyVec.Size())
			rockPos = rock;
	}

	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(b->GetEnemyObj()->GetTransform()->rotation.y);
	//プレイヤーとのベクトル
	VECTOR3 vec = pPos - b->GetEnemyObj()->GetTransform()->position;
	// 右ベクトル
	VECTOR3 rightVec = VECTOR3(frontVec.z, 0.0f, -frontVec.x);

	// 左右判定
	float side = VDot(rightVec, vec);

	//0以上なら右側、0以上なら左側
	float dir = (side >= 0) ? 1.0f : -1.0f;

	targetPos = rockPos + rightVec * dir * size;

	islooping = false;
}

std::list<VECTOR3> BossRun::RayCastHits()
{
	Boss* b = GetBase<Boss>();
	std::list<VECTOR3> rockPositions = b->rockManager->GetAllRockPos();
	std::list<VECTOR3> hitRocks;

	const float ROCK_RADIUS = 200;

	VECTOR3 playerPos = b->enemyBaseComponent.playerObj->GetTransform()->position;
	VECTOR3 myPos = b->GetEnemyObj()->GetTransform()->position;

	VECTOR3 toPlayer = playerPos - myPos;

	for (auto rockPosition : rockPositions)
	{
		VECTOR3 toRock = rockPosition - myPos;

		float dot = VDot(toRock, toPlayer);

		if (dot < 0.0f) continue;
		if (dot > VDot(toPlayer, toPlayer)) continue;

		float dx = toPlayer.x;
		float dz = toPlayer.z;

		float distLine = abs(
			dx * (myPos.z - rockPosition.z) -
			dz * (myPos.x - rockPosition.x)
		) / sqrt(dx * dx + dz * dz);

		if (distLine < ROCK_RADIUS)
		{
			hitRocks.push_back(rockPosition);
		}
	}

	return hitRocks;
}