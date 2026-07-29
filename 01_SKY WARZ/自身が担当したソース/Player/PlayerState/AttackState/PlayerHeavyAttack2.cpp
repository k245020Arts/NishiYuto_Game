#include "PlayerHeavyAttack2.h"
#include "../../Player.h"
#include "../../../Component/Animator/Animator.h"

PlayerHeavyAttack2::PlayerHeavyAttack2()
{
	string = Function::GetClassNameC<PlayerHeavyAttack2>();
	//id = ID::P_ANIM_ATTACK1;
	nextAttack = false;
	animId = ID::P_HEAVY_ATTACK2;
}

PlayerHeavyAttack2::~PlayerHeavyAttack2()
{
}

void PlayerHeavyAttack2::Update()
{
	Player* p = GetBase<Player>();
	collsionCreate = false;


	PlayerAttackStateBase::Update();
	PlayerAttackStateBase::AttackCommonUpdate();
}

void PlayerHeavyAttack2::Draw()
{
}

void PlayerHeavyAttack2::Start()
{
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	Player* p = GetBase<Player>();
	frontMove = true;
	rockOn = false;
}

void PlayerHeavyAttack2::Finish()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Finish();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
	frontMove = false;
}
