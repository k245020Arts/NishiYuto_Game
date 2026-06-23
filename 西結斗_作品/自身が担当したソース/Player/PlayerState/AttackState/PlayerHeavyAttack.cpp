#include "PlayerHeavyAttack.h"
#include "../../../Component/Animator/Animator.h"
#include "../playerStateManager.h"
#include <typeinfo>
#include "../../../Common/InputManager/PadInput.h"
#include "../../Player.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Camera/Camera.h"
#include "../../../Common/Easing.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../../../Common/Function.h"
#include "../../../Common/InputManager/InputManager.h"

PlayerHeavyAttack::PlayerHeavyAttack()
{
	string = Function::GetClassNameC<PlayerHeavyAttack>();
	//id = ID::P_ANIM_ATTACK1;
	nextAttack = false;
	animId = ID::P_HEAVY_ATTACK;
	/*playerAttackData.collTrans = Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	playerAttackData.normalAttackNextID = StateID::PLAYER_HEAVY_ATTACK_S;
	playerAttackData.frontSpeed = 4000.0f;*/
	time = 0.0f;
	/*playerAttackData.hitDamage = 300.0f;*/
	chargeCount = 0.0f;
	defalutTrail = true;
	speedChange = false;
}

PlayerHeavyAttack::~PlayerHeavyAttack()
{
}

void PlayerHeavyAttack::Update()
{
	Player* p = GetBase<Player>();
	collsionCreate = false;
	PlayerAttackStateBase::Update();
	PlayerAttackStateBase::AttackCommonUpdate();
}

void PlayerHeavyAttack::Draw()
{
}

void PlayerHeavyAttack::Start()
{
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	Player* p = GetBase<Player>();
	chargeCount = 1.0f;
	frontMove = true;
	p->playerCom.camera->ChangeStateCamera(StateID::PLAYER_HEAVY_CHARGE_CAMERA_S);
	const auto* chargeParam = playerAttackData.GetChargeLevel(p->attackLevel);
	//my_assert(chargeParam == nullptr,"チャージレベルがセットされていない");
	if (chargeParam == nullptr) {
		//チャージレベルがセットされていないときには削除
		my_error_assert("チャージレベルがセットされていない");
		return;
	}

	AgainTimerSet(chargeParam->againTimer, chargeParam->againTimerFlag);
	playerAttackData.attackMove = chargeParam->moveSpeed;
	playerAttackData.hitDamage = chargeParam->hitDamage;
	rockOn = false;
}

void PlayerHeavyAttack::Finish()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Finish();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
	p->playerCom.camera->ChangeStateCamera(StateID::FREE_CAMERA_S);

}
