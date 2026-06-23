#include "playerAttack3.h"
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

namespace {
	int createCounter;
}

PlayerAttack3::PlayerAttack3()
{
	string			= Function::GetClassNameC<PlayerAttack3>();
	animId			= ID::P_ANIM_ATTACK3;
	createCounter = 0;
	//playerAttackData.collTrans		= Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	//playerAttackData.normalAttackNextID = StateID::PLAYER_ATTACK4_S;
	//playerAttackData.frontSpeed		= 7500.0f;
	////frontSpeed = 0.0f;
	//playerAttackData.hitDamage		= 150.0f;
	//playerAttackData.state = StateID::PLAYER_ATTACK3_S;
	//playerAttackData.attackNum = 0;
	//playerAttackData.attackAgainStartCounterMax = 0.0f;
	speedChange = false;
}

PlayerAttack3::~PlayerAttack3()
{
}

void PlayerAttack3::Update()
{
	Player* p = GetBase<Player>();
	//Debug::DebugLogPrintfArgs("Create = %d", (int)collsionCreate);
	/*if (collsionCreate) {
		createCounter++;
		if (createCounter > 18000) {
			collsionCreate = false;
		}
	}*/
	collsionCreate = false;
	PlayerAttackStateBase::Update();
	PlayerAttackStateBase::AttackCommonUpdate();
	
}

void PlayerAttack3::Draw()
{
}

void PlayerAttack3::Start()
{
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	createCounter = 0;
}

void PlayerAttack3::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
	PlayerAttackStateBase::Finish();
}

void PlayerAttack3::StateImguiDraw()
{
	if (collsionCreate) {
		ImGui::Text("collsionCreate = true");
	}
}
