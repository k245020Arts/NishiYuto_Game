#include "PlayerHeavyCharge.h"
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
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Common/Sound/SoundManager.h"

PlayerHeavyCharge::PlayerHeavyCharge()
{
	string			= Function::GetClassNameC<PlayerHeavyCharge>();
	nextAttack		= false;
	animId			= ID::P_HEAVY_CHARGE;
	playerAttackData.collTrans		= Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	playerAttackData.specialAttackNextID = StateID::PLAYER_HEAVY_ATTACK_S;
	playerAttackData.attackMove		= VECTOR3(0,0,5500.0f);
	time			= 0.0f;
	playerAttackData.hitDamage		= 30.0f;
	chargeCount		= 0.0f;
	defalutTrail	= false;
	chargeFinish	= false;
	baseFrequ		= 0;
	maxCharge		= false;

	playerAttackData.state = StateID::PLAYER_HEAVY_CHARGE_S;
}

PlayerHeavyCharge::~PlayerHeavyCharge()
{
}

void PlayerHeavyCharge::Update()
{
	Player* p = GetBase<Player>();
	//AttackCollsion();
	PlayerAttackStateBase::Update();
	
	if (InputManager::GetInstance()->KeyInput("heavyAttack")) {
		chargeCount -= Time::DeltaTimeRate();
		if (chargeCount <= 0.0f) {
			p->attackLevel = 2;
			p->playerCom.shaker->SetShakePower(VECTOR3(20, 20, 20));
			InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(150, -1);
			
			//オーラエフェクトが切れたらまた再生
			if (!EffectManager::GetInstance()->IsPlayIng(Effect_ID::PLAYER_CHARGE_FINAL)) {
				EffectManager::GetInstance()->CreateEffekseer(Transform(), obj, Effect_ID::PLAYER_CHARGE_FINAL, 1.0f);
			}
			baseFrequ = 90000;
			//チャージの音がなっていないならまた再生
			if (!SoundManager::GetInstance()->CheckSe(Sound_ID::PLAYER_MAX_CHARGE)) {
				SoundManager::GetInstance()->PlayRamdomChangeFrequencySe(Sound_ID::PLAYER_MAX_CHARGE, 0, baseFrequ);
			}
			//最大溜めになった瞬間エフェクトとSeを出す
			if (!maxCharge) {
				maxCharge = true;
				SoundManager::GetInstance()->PlaySe(Sound_ID::PLAYER_CHARGE_END);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0,100,0),VZero,VOne), obj, Effect_ID::PLAYER_CHARGE_END, 1.0f);
			}
		}
		else if (chargeCount <= 1.0f) {
			p->attackLevel = 1;
			p->playerCom.shaker->SetShakePower(VECTOR3(10, 10, 10));
			InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(100, -1);
			if (!EffectManager::GetInstance()->IsPlayIng(Effect_ID::PLAYER_CHARGE_SECOND)) {
				EffectManager::GetInstance()->CreateEffekseer(Transform(), obj, Effect_ID::PLAYER_CHARGE_SECOND, 1.0f);
			}
			baseFrequ = 80000;
			//
			if (!SoundManager::GetInstance()->CheckSe(Sound_ID::PLAYER_CHARGE)) {
				SoundManager::GetInstance()->PlayRamdomChangeFrequencySe(Sound_ID::PLAYER_CHARGE, 0, baseFrequ);
			}
		}
		p->PlayerStickInput();
		constexpr float DEAD_ZONE = 0.3f;
		if ((fabs(p->walkAngle.x) >= DEAD_ZONE || fabs(p->walkAngle.z) >= DEAD_ZONE) && p->hp > 0.0f) {
			p->RotationChange(p->walkAngle, 12.0f/*スピード*/);
		}
		
	}
	else {
		chargeFinish = true;
	}
	//チャージの状態が終わったら
	if (chargeFinish) {
		p->HeavyAttackChangeParam(p->attackLevel);
		p->playerCom.stateManager->ChangeState(playerAttackData.specialAttackNextID);
		p->playerCom.shaker->ShakeFinish();
		InputManager::GetInstance()->GetControllerInput()->StopControlVibrationStartFrame();
	}
}

void PlayerHeavyCharge::Draw()
{
}

void PlayerHeavyCharge::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	chargeCount = 2.0f;
	p->playerCom.camera->ChangeStateCamera(StateID::PLAYER_HEAVY_CHARGE_CAMERA_S);
	p->charge = true;
	//p->playerCom.sound->PlaySe(Sound_ID::PLAYER_CHARGE);
	baseFrequ = 50000;
	chargeFinish = false;
	p->playerCom.shaker->ShakeStart(VECTOR3(5, 5, 5), Shaker::MIX_SHAKE, false, -1);
	maxCharge = false;
	p->attackLevel = 0;
	InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(50, -1);
	EffectManager::GetInstance()->CreateEffekseer(Transform(), obj, Effect_ID::PLAYER_CHARGE_FIRST, 1.0f);
	p->playerCom.physics->SetVelocity(VZero);
}

void PlayerHeavyCharge::Finish()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Finish();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.anim->AnimEventReset();
	p->charge = false;
	p->playerCom.camera->ChangeStateCamera(StateID::FREE_CAMERA_S);
	p->playerCom.shaker->ShakeFinish();
	InputManager::GetInstance()->GetControllerInput()->StopControlVibrationStartFrame();
}
