#include "playerAttack2.h"
#include "../../../Component/Animator/Animator.h"
#include "../playerStateManager.h"
#include "../../../Common/InputManager/PadInput.h"
#include "../../Player.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Camera/Camera.h"
#include "../../../Common/Easing.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../../../Common/Function.h"
#include "../../../Common/InputManager/InputManager.h"

PlayerAttack2::PlayerAttack2()
{
	string			= Function::GetClassNameC<PlayerAttack2>();
	animId			= ID::P_ANIM_ATTACK2;
	//playerAttackData.collTrans		= Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	//playerAttackData.normalAttackNextID = StateID::PLAYER_ATTACK3_S;
	//playerAttackData.frontSpeed		= 7500.0f;
	////frontSpeed = 0.0f;
	//playerAttackData.hitDamage		= 150.0f;
	//playerAttackData.state = StateID::PLAYER_ATTACK2_S;
	//playerAttackData.attackNum = 0;
	//playerAttackData.attackAgainStartCounterMax = 0.0f;
}

PlayerAttack2::~PlayerAttack2()
{
}

void PlayerAttack2::Update()
{
	Player* p = GetBase<Player>();
	/*collsionCreate = false;
	AttackCollsion();*/
	collsionCreate = false;
	PlayerAttackStateBase::Update();
	PlayerAttackStateBase::AttackCommonUpdate();
}

void PlayerAttack2::Draw()
{
}

void PlayerAttack2::Start()
{
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
}

void PlayerAttack2::Finish()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Finish();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();

}
