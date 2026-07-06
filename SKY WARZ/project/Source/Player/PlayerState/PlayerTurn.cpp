#include "PlayerTurn.h"
#include "playerStateManager.h"
#include "../../Component/Physics/Physics.h"
#include "../../Component/Transform/Transform.h"
#include "../../Common/InputManager/PadInput.h"
#include "../player.h"
#include "../../Component/Animator/Animator.h"
#include "../../Component/Transform/Transform.h"

PlayerTurn::PlayerTurn()
{
	string = string = Function::GetClassNameC<PlayerTurn>();;
	animId = ID::P_TURN_ANIM;
	event = false;
}

PlayerTurn::~PlayerTurn()
{
}

void PlayerTurn::Update()
{
	Player* p = GetBase<Player>();
	
	if (p->playerCom.anim->IsFinish()) {
		p->AvoidFinishState();;
	}
	if (p->playerCom.anim->AnimEventCan()) {
		p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
		//event = true;
	}
}

void PlayerTurn::Draw()
{
}

void PlayerTurn::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	p->playerCom.physics->SetFirction(VZero);
	p->playerCom.physics->SetGravity(VZero);
	p->turn = true;
	event = false;
	p->PlayerStickInput();
	//blendSpeed = 0.4f;
}

void PlayerTurn::Finish()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	p->playerCom.physics->SetGravity(PlayerInformation::BASE_GRAVITY);
	//blendSpeed = 0.4f;
	//p->RotationChange(p->GetWalkAngle(), 180.0f);
	
}
