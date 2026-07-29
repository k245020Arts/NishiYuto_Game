#include "PlayerAttack6.h"
#include "../../Player.h"
#include "../../../Component/Animator/Animator.h"

PlayerAttack6::PlayerAttack6()
{
	string = Function::GetClassNameC<PlayerAttack6>();
	//id = ID::P_ANIM_ATTACK1;
	nextAttack = false;
	animId = ID::P_ANIM_ATTACK_6;
}

PlayerAttack6::~PlayerAttack6()
{
}

void PlayerAttack6::Update()
{
	Player* p = GetBase<Player>();
	collsionCreate = false;


	PlayerAttackStateBase::Update();
	PlayerAttackStateBase::AttackCommonUpdate();
}

void PlayerAttack6::Draw()
{
}

void PlayerAttack6::Start()
{
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	Player* p = GetBase<Player>();
}

void PlayerAttack6::Finish()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Finish();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
}
