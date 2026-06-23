#include "PlayerBlowAway.h"
#include "../../Component/Animator/Animator.h"
#include "playerStateManager.h"
#include "../../Component/Color/Color.h"
#include "../../Component/Physics/Physics.h"
#include "../player.h"
#include "../../Component/Shaker/Shaker.h"
#include "../../Common/Function.h"
#include "../../Common/Sound/SoundManager.h"
#include "../../Camera/Camera.h"

PlayerBlowAway::PlayerBlowAway()
{
	string		= string = Function::GetClassNameC<PlayerBlowAway>();;
	animId		= ID::P_DAMAGE;
	eRotation	= VZero;
	//id= ID::P_BLOWAWAY;
}

PlayerBlowAway::~PlayerBlowAway()
{
}

void PlayerBlowAway::Update()
{
	Player* p = GetBase<Player>();
	//—Ž‰º‚µ‚½‚çƒJƒƒ‰‚ðc—h‚ê
	if (p->playerCom.physics->GetGround()) {
		if (p->playerCom.anim->GetCurrentID() == ID::GetID(ID::P_FALL)) {
			p->playerCom.anim->Play(ID::P_GETUP);
			p->playerCom.shaker->ShakeFinish();
			SoundManager::GetInstance()->PlaySe(Sound_ID::ENEMY_FALL);
			p->playerCom.camera->CameraShake(VECTOR3(100, 100, 0), Shaker::HORIZONAL_SHAKE, false, 0.5f);
		}
	}
	//ƒ_ƒ[ƒWˆ—
	if (p->playerCom.anim->IsFinish()) {
		if (p->playerCom.anim->GetCurrentID() == ID::GetID(ID::P_DAMAGE)) {
			p->playerCom.anim->Play(ID::P_FALL);
			p->playerCom.anim->SetPlaySpeed(1.0f);
		}
		else if (p->playerCom.anim->GetCurrentID() == ID::GetID(ID::P_GETUP)) {
			//p->playerCom.stateManager->ChangeState(ID::P_ANIM_RUN);
			p->AvoidFinishState();
			
		}
	}
	//d—Í‚Ì‰Á‘¬
	p->playerCom.physics->AddGravity(VECTOR3(0, -20.0f, 0));
	if (p->playerCom.anim->GetCurrentID() == ID::GetID(ID::P_FALL)) {
		p->playerCom.shaker->AddShakePower(VECTOR3(0.1f, 0.1f, 0.1f));
	}
}

void PlayerBlowAway::Draw()
{
}

void PlayerBlowAway::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	//“G‚Ì•ûŒü‚ÉŒü‚­‚æ‚¤‚ÉŒ»Ý‚Ì“G‚ÌŠp“x‚ðŽæ“¾
	if (p->playerCom.hitObj != nullptr) {
		eRotation = p->playerCom.hitObj->GetTransform()->rotation;
	}
	const float FIRST_SPEED = 10000.0f;
	const float ADD_SPEED = 2000.0f;
	const float GRAVITY = -3000.0f;
	p->playerCom.physics->SetVelocity(VECTOR3(0, 0, FIRST_SPEED) * MGetRotY(eRotation.y));
	p->playerCom.physics->AddVelocity(VECTOR3(0, ADD_SPEED, 0), false);
	p->playerCom.physics->SetGravity(VECTOR3(0, GRAVITY, 0));
	p->playerCom.player->GetPlayerTransform()->position.y += 10.0f;
	p->playerCom.anim->SetPlaySpeed(2.5f);
	p->playerCom.shaker->ShakeStart(VECTOR3(10, 10, 10), Shaker::MIX_SHAKE, false, -1.0f);
}

void PlayerBlowAway::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.physics->SetGravity(PlayerInformation::BASE_GRAVITY);
	p->playerCom.shaker->ShakeFinish();
}
