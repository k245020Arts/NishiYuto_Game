#include "playerJustAvoidAttack1.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/Physics/Physics.h"
#include "../playerStateManager.h"
#include "../../../Common/InputManager/PadInput.h"
#include "../../player.h"
#include "../../../Common/function.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Component/Color/Color.h"
#include "../../../Common/InputManager/InputManager.h"
#include "../../../Common/Sound/SoundManager.h"

PlayerJustAvoidAttack1::PlayerJustAvoidAttack1()
{
	string = Function::GetClassNameC<PlayerJustAvoidAttack1>();
	//id =  ID::P_ANIM_JUST_AVOID_ATTACK1;
	animId = ID::P_ANIM_JUST_AVOID_ATTACK1;
	//playerAttackData.collTrans = Transform(VECTOR3(0, 100, 200), VZero, VECTOR3(300, 0, 0));
	////nextAttackID = ID::P_ANIM_ATTACK1;
	//playerAttackData.normalAttackNextID = StateID::PLAYER_ATTACK1_S;
	//playerAttackData.frontSpeed = 10000.0f;
	//playerAttackData.hitDamage = 200.0f;
	defalutTrail = false;
	attack = false;
	count = 0;
	timer = 0.0f;
	distSize = 0.0f;

	/*playerAttackData.state = StateID::PLAYER_JUST_AVOID_ATTACK1_S;*/
}

PlayerJustAvoidAttack1::~PlayerJustAvoidAttack1()
{
}

void PlayerJustAvoidAttack1::Update()
{
	Player* p = GetBase<Player>();
	collsionCreate = false;
	/*AttackCollsion();*/
	PlayerAttackStateBase::Update();
	if (p->playerCom.anim->AnimEventCan()) {
		const float R = 0.0f;
		const float G = 0.0f;
		const float B = 255.0f;
		const float A = 150.0f;
		const int FRAME = 28;
		const float TIME = 0.8f;
		p->playerCom.player->DrawTrail(VECTOR3(0, 0, -100)/*êÊí[ç¿ïW*/, VECTOR3(0, 0, -350)/*ññí[ç¿ïW*/,
			R, G, B, A, FRAME, TIME);
	}
	if (!noStateChange) {
		if (distSize <= ATTACK_MOVE_DIST) {
			EnemyRotation();
		}
		if (InputManager::GetInstance()->KeyInputDown("avoid")) {
			p->playerCom.player->AvoidReady();
			noStateChange = true;
		}
		if (InputManager::GetInstance()->KeyInputDown("attack")) {
			nextAttack = true;
		}
		dist = p->attackTargetTrans.position - p->playerCom.player->GetPlayerTransform()->position;
		distSize = dist.Size();
		if (distSize <= DISTANCE_MOVE && beforeAttack) {
			p->playerCom.anim->SetPlaySpeed(0.8f);
			p->playerCom.physics->SetVelocity(VZero);
		}
		else {
			angle = atan2f(dist.x, dist.z);
			p->playerCom.physics->AddVelocity(playerAttackData.attackMove * MGetRotY(angle), false);
		}
		if (p->playerCom.anim->AnimEventCan()) {
			p->playerCom.anim->SetPlaySpeed(1.6f);
			beforeAttack = false;
			rotation = true;
			playerAttackData.attackMove = VECTOR3(0,0, 2000.0f);
			p->playerCom.physics->SetVelocity(playerAttackData.attackMove * MGetRotY(angle));

		}
		else {
			if (beforeAttack)
				p->playerCom.anim->SetPlaySpeed(0.3f);
			else {
				p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA + VECTOR3(30000.0f, 30000.0f, 30000.0f));
				if (nextAttack) {
					runTimer = 1.0f;
					noStateChange = true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
				}
				else {
					runTimer = 1.0f;
					noStateChange = true;
					p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
					p->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
				}
			}
		}
	}
}

void PlayerJustAvoidAttack1::Draw()
{
}

void PlayerJustAvoidAttack1::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();

	nextAttack = false;
	nextAvoid = false;
	p->playerCom.player->SetAvoidStart(false);
	p->playerCom.anim->AnimEventReset();

	noStateChange = false;
	
	if (p->playerCom.hitObj != nullptr) {
		p->attackTargetTrans = *(p->playerCom.hitObj->GetTransform());
	}
	else {
		p->attackTargetTrans = Transform();
	}
	rockOn = true;

	dist = p->attackTargetTrans.position - p->playerCom.player->GetPlayerTransform()->position;

	//äpìxåvéZ
	angle = atan2f(dist.x, dist.z);
	easingCount = 0.0f;
	beforeAngle = p->playerCom.player->GetPlayerTransform()->rotation.y;
	firstColl = true;
	distSize = dist.Size();
	norm = dist.Normalize();
	beforeAttack = true;
	runTimer = 0.0f;
	if (dist.Size() >= 2500 && p->playerCom.hitObj != nullptr) {
		//ãóó£Ç™âìÇ¢Ç∆Ç‡Ç∆Ç‡Ç∆ÇÃäpìxÇ‘ÇÒçUåÇÇÃà⁄ìÆèàóùÇÇ¢ÇÍÇÈ
		rotation = false;;
		p->playerCom.physics->SetVelocity(playerAttackData.attackMove * MGetRotY(beforeAngle));
	}
	else {
		//ãﬂÇ¢Ç∆ìGÇÃï˚å¸Ç…å¸Ç©Ç¡ÇƒçUåÇÇÃà⁄ìÆèàóùÇÇ¢ÇÍÇÈ
		rotation = true;
		p->playerCom.physics->SetVelocity(playerAttackData.attackMove * MGetRotY(angle));
	}


	if (distSize <= ATTACK_MOVE_DIST) {
		p->playerCom.physics->SetVelocity(norm * distSize * 3.5f);
	}
	p->playerCom.anim->SetPlaySpeed(0.01f);
	firstColl = true;
	attack = false;
	p->playerCom.shaker->ShakeStart(VECTOR3(20.0f, 10.0f, 10.0f), Shaker::HORIZONAL_SHAKE, false, 1.0f);
	InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(100, 20);
	if (p->largeJustAvoid) {
		if (p->playerCom.hitObj != nullptr) {
			p->playerCom.hitObj->SetObjectTimeRate(PlayerInformation::JUST_AVOID_ENEMY_TIME_SCALE + 0.5f);
		}
	}
	runTimer = -1.0f;
	normal = false;
	special = false;
	/*p->playerCom.anim->SetPlaySpeed(0.1f);
	timer = 0.5f;
	p->playerCom.shaker->ShakeStart(VECTOR3(30.0f, 10.0f, 10.0f), Shaker::HORIZONAL_SHAKE, false, 0.4f);*/
	//p->playerCom.controller->ControlVibrationStartFrame(100, 20);
}

void PlayerJustAvoidAttack1::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	p->playerCom.anim->AnimEventReset();
	p->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
	PlayerAttackStateBase::Finish();
	if (p->playerCom.hitObj != nullptr) {
		p->playerCom.hitObj->SetObjectTimeRate();
	}
	p->largeJustAvoid = false;
	/*p->playerCom.sound->FeedInStart(Sound_ID::PLAY_BGM, 1.0f);
	p->justAvoid = false;
	p->justFeedOutTime = p->JUST_FEED_OUT_TIME;*/
}

void PlayerJustAvoidAttack1::StateImguiDraw()
{
	ImGui::Text("distSize = %.f1", distSize);
	ImGui::Text("distSize = %d", collsionCreate);
}
