#include "PlayerAttack5.h"
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


PlayerAttack5::PlayerAttack5()
{
	string = Function::GetClassNameC<PlayerAttack5>();
	//id = ID::P_ANIM_ATTACK_5;
	animId = ID::P_ANIM_ATTACK_5;
	speedChange = false;
	/*playerAttackData.collTrans = Transform(VECTOR3(0, 80, 100), VZero, VECTOR3(300, 0, 0));
	playerAttackData.normalAttackNextID = StateID::PLAYER_ATTACK1_S;*/
	//frontSpeed = 500.0f;
	/*playerAttackData.frontSpeed = 0.0f;
	playerAttackData.hitDamage = 150.0f;*/
	defalutTrail = true;
	timer = 0.0f;
	avoidReady = false;

	/*playerAttackData.state = StateID::PLAYER_ATTACK5_S;
	playerAttackData.attackNum = 4;
	playerAttackData.attackAgainStartCounterMax = 0.1f;*/
}

PlayerAttack5::~PlayerAttack5()
{
}

void PlayerAttack5::Update()
{

	Player* p = GetBase<Player>();
	collsionCreate = false;
	//AttackCollsion();
	PlayerAttackStateBase::Update();
	//PlayerAttackStateBase::AttackCommonUpdate();
	float frame = p->playerCom.anim->GetCurrentFrame();
	bool collsion = (playerAttackData.collsionStartTime <= frame && playerAttackData.collsionFinishTime >= frame);
	if (collsion) {
		if (defalutTrail) {
			p->playerCom.player->DrawTrail();
		}
		attackAgainStartCounter -= Time::DeltaTimeRate();
		if (attackAgainStartCounter <= 0.0f) { //V‚µ‚­“–‚½‚è”»’è‚Ì¶¬‚ªo—ˆ‚é‚æ‚¤‚É‚È‚é‚È‚ç
			attackAgainStartCounter = playerAttackData.attackAgainStartCounterMax;
			AgainAttackCollsion();
		}
		/*p->playerCom.blur->MosionStart(0.04f, 0.1f, animId, 1);;*/

	}

	if (!noStateChange) {
		EnemyRotation(); //“G‚Ì•ûŒü‚ÉŒü‚­
		if (InputManager::GetInstance()->KeyInputDown("avoid")) {
			//p->playerCom.player->AvoidReady();
			avoidReady = true; //‰ñ”ðó‘Ô‚ÉˆÚs
			//noStateChange = true;
		}
		if (InputManager::GetInstance()->KeyInputDown("attack")) {
			nextAttack = true; //ŽŸ‚ÌUŒ‚ó‘Ô‚ÉˆÚs
		}
		timer -= Time::DeltaTimeRate();
		if (p->playerCom.physics->GetGravity().y <= -30000.0f) {

		}
		else {
			p->playerCom.physics->AddGravity(VECTOR3(0, -1500, 0)); //d—Í‚Ì‰ÁŽZ
		}
		//if (p->playerCom.anim->GetCurrentFrame() >= p->playerCom.anim->EventStartTime(animId) - 3.0f) {

		//}
		if (p->playerCom.anim->AnimEventCan()) {
			if (beforeAttack) {
				AttackMoveStart();
				p->playerCom.anim->SetPlaySpeed(2.0f);
			}
			beforeAttack = false;
		}
		else {
			if (!beforeAttack) {
				//p->playerCom.anim->SetFrame(p->playerCom.anim->EventStartTime(animId));
				//firstColl = true;
			}
		}
		if (p->playerCom.physics->GetGround()) {
			if (avoidReady) {
				nextAvoid = true;
			}
			else if (beforeAttack) {
				//p->playerCom.anim->SetPlaySpeed(1.5f);
			}
			else {
				runTimer = 0.4f;
				noStateChange = true;
				p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
			}
		}
	}
}

void PlayerAttack5::Draw()
{
}

void PlayerAttack5::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	PlayerAttackStateBase::Start();
	
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.physics->AddGravity(VECTOR3(0, 0, 0));
	p->playerCom.physics->SetGravity(VZero);
	//timer = 2.0f;
	//count = ATTACK_NUMMAX;
	//AgainTimerSet(0.1f, 4);
	avoidReady = false;

}

void PlayerAttack5::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	p->playerCom.anim->AnimEventReset();
	p->playerCom.physics->SetGravity(PlayerInformation::BASE_GRAVITY);
	PlayerAttackStateBase::Finish();
}

void PlayerAttack5::Again()
{
	
	PlayerAttackStateBase::Start();
	/*if (distSize <= ATTACK_MOVE_DIST) {
		p->playerCom.physics->SetVelocity(norm * distSize * 5.0f);
	}*/
	
}