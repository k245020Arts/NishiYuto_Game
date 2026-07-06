#include "playerAttack4.h"
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

PlayerAttack4::PlayerAttack4()
{
	string			= Function::GetClassNameC<PlayerAttack4>();
	animId			= ID::P_ANIM_ATTACK4;
	speedChange = false;
	/*playerAttackData.collTrans		= Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	playerAttackData.frontSpeed		= 5000.0f;*/
	//frontSpeed		= 0.0f;
	/*playerAttackData.hitDamage		= 200.0f;
	playerAttackData.normalAttackNextID = StateID::PLAYER_ATTACK5_S;*/

	avoidReady = false;
	timer = 0.0f;
	/*playerAttackData.state = StateID::PLAYER_ATTACK4_S;

	playerAttackData.attackNum = 3;
	playerAttackData.attackAgainStartCounterMax = 0.2f;*/
}

PlayerAttack4::~PlayerAttack4()
{
}

void PlayerAttack4::Update()
{
	Player* p = GetBase<Player>();
	/*collsionCreate = false;
	AttackCollsion();*/
	collsionCreate = false;
	PlayerAttackStateBase::Update();
	PlayerAttackStateBase::AttackCommonUpdate();
	timer -= Time::DeltaTimeRate();
	if (timer <= 0.0f) {
		p->playerCom.anim->SetPlaySpeed(1.0f);
	}
}

void PlayerAttack4::Draw()
{
}

void PlayerAttack4::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	timer = 0.01f;
	//p->playerCom.anim->SetPlaySpeed(0.1f);
	p->playerCom.physics->SetGravity(VZero);
	//UŒ‚‚Ì‘O‚É‚½‚ß‚ð‚¢‚ê‚é‚½‚ß‚Ìshake
	//p->playerCom.shaker->ShakeStart(VECTOR3(20.0f, 10.0f, 10.0f), Shaker::HORIZONAL_SHAKE, false, 0.4f);
	//p->playerCom.controller->ControlVibrationStartFrame(100, 20);
	p->playerCom.physics->SetGravity(VECTOR3(0,-5000,0));
	//AgainTimerSet(0.2f, 3);
	avoidReady = false;
}

void PlayerAttack4::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
	PlayerAttackStateBase::Finish();
}
