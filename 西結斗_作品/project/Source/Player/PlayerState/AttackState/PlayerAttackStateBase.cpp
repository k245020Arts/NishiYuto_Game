#include "PlayerAttackStateBase.h"
#include "../../player.h"
#include "../../../Common/Easing.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Common/InputManager/InputManager.h"
#include "../playerStateManager.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Weapon/WeaponManager.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/MotionBlur/MotionBlur.h"
#include "../../../Enemy/EnemyManager.h"
#include "../../../Camera/Camera.h"
#include "../../../Component/Collider/SphereCollider.h"

PlayerAttackStateBase::PlayerAttackStateBase()
{
	easingCount		= 0.0f;
	beforeAngle		= 0.0f;
	angle			= 0.0f;
	nextAttack		= 0.0f;
	distSize		= 0.0f;
	playerAttackData.normalAttackNextID = StateID::PLAYER_ATTACK1_S;
	rotation		= 0.0f;
	runTimer		= 0.0f;
	time			= 0.0f;
	beforeAngle		= 0.0f;
	beforeAttack	= false;
	defalutTrail	= true;
	nextAvoid		= false;
	rockOn			= false;
	playerAttackData.hitDamage		= 0.0f;

	dist			= VZero;
	norm			= VZero;

	playerAttackData.attackMove		= VZero;

	rotation		= false;
	defalutTrail	= true;
	attackAgainStartCounter = 0.0f;
	playerAttackData.attackAgainStartCounterMax = 0.0f;

	playerAttackData.attackNum = 0;
	attackCount = 0;
	collsionCreate = false;

	playerAttackData.attackNum = 0;
	playerAttackData.attackAgainStartCounterMax = 0.0f;

	speedChange = true;
	normal = false;
	special = false;
	frontMove = false;
}

PlayerAttackStateBase::~PlayerAttackStateBase()
{
}

void PlayerAttackStateBase::Update()
{
	Player* p = GetBase<Player>();
	//collsionCreate = false;
	AttackCollsion();
	SpecialAttackStart();
	//AttackCommonUpdate();
	
	//p->RotationChange(p->GetWalkAngle(), 5.0f);
	//p->playerCom.player->DrawTrail();
	//後隙の設定
	if (nextAvoid) {
		p->playerCom.player->AvoidReady();
		noStateChange = true;
		nextAvoid = false;
	}
	if (runTimer >= 0.0f) {
		runTimer -= Time::DeltaTimeRate();
		//後隙が終わったら次の状態に遷移
		if (runTimer <= 0.0f) {
			if (nextAttack) {
				if (normal) {
					//次の攻撃状態が記録されているなら派生攻撃へと移行、弱攻撃状態
					if (playerAttackData.normalAttackNextID != StateID::STATE_MAX) {
						p->playerCom.stateManager->ChangeState(playerAttackData.normalAttackNextID);
					}
					else {
						p->playerCom.player->AvoidFinishState();
					}
				}
				else {
					//次の攻撃状態が記録されているなら派生攻撃へと移行、強攻撃状態
					if (playerAttackData.specialAttackNextID != StateID::STATE_MAX) {
						p->playerCom.stateManager->ChangeState(playerAttackData.specialAttackNextID);
					}
					else {
						p->playerCom.player->AvoidFinishState();
					}
					
				}
				
			}
			
			else {
				p->playerCom.player->AvoidFinishState();
			}
			
		}
	}
}

void PlayerAttackStateBase::EnemyRotation()
{
	if (!rockOn) {
		return;
	}
	Player* p = GetBase<Player>();
	//敵が一定以上より遠いか回転が終わったらここの処理をスルーする
	if (easingCount > 1.0f || !rotation) {
		return;
	}
	//敵の方向に向ける
	easingCount += p->playerCom.player->GetPlayerObj()->GetObjectTimeRate() * 6.0f;
	easingCount = min(1.0f, easingCount);

	p->playerCom.player->GetPlayerTransform()->rotation.y = Easing::Lerp(beforeAngle, angle, easingCount);
}

void PlayerAttackStateBase::Start()
{
	Player* p			= GetBase<Player>();
	//p->PlayerStickInput();
	nextAttack			= false;
	nextAvoid			= false;
	p->playerCom.player->SetAvoidStart(false);
	noStateChange		= false;

	if (p->playerCom.targetObj != nullptr) {
		p->attackTargetTrans		= *(p->playerCom.targetObj->GetTransform());
	}
	else {
		Transform nearEnemyPos = p->playerCom.enemyManager->NearEnemyPos(p->playerTransform->position);
		p->attackTargetTrans				= nearEnemyPos;
		//p->attackTargetTrans.position = VECTOR3(0, 0, 1) * p->playerTransform->rotation;
	}
	
	//敵とプレイヤーの距離をとる
	dist				= p->attackTargetTrans.position - p->playerCom.player->GetPlayerTransform()->position;
	
	VECTOR3 frontVector = VECTOR3(0.0f, 0.0f, 1.0f) * MGetRotY(p->playerTransform->rotation.y);
	rockOn = false;
	beforeAngle			= p->playerCom.player->GetPlayerTransform()->rotation.y;

	//角度計算
	angle = atan2f(dist.x, dist.z);
	rockOn			= true;
	
	easingCount			= 0.0f;
	firstColl			= true;
	distSize			= dist.Size();
	norm				= dist.Normalize();
	SoundManager::GetInstance()->RandamSe("P_AttackV", 3);
	beforeAttack		= true;
	runTimer			= 0.0f;
	attackCount = playerAttackData.attackNum;
	attackAgainStartCounter = playerAttackData.attackAgainStartCounterMax;
	runTimer = -1.0f;
	normal = false;
	special = false;
}

void PlayerAttackStateBase::Finish()
{
	Player* p = GetBase<Player>();
    p->DeleteCollision(&p->attackColl);
	if (!nextAttack) {
		p->playerCom.camera->CameraLeapSet(0.2f);
		p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	}
}

bool PlayerAttackStateBase::IsAttack()
{
	Player* p = GetBase<Player>();
	float frame = p->playerCom.anim->GetCurrentFrame();
	bool collsion = (playerAttackData.collsionStartTime <= frame && playerAttackData.collsionFinishTime >= frame);
	/*if (id == StateID::PLAYER_ATTACK3_S) {
		int dljdjlf = 0;
	}*/
	return collsion;
}

void PlayerAttackStateBase::AttackMoveStart()
{
	Player* p = GetBase<Player>();
	//近いと敵の方向に向かって攻撃の移動処理をいれる
	rotation = true;
	if (frontMove) {
		//前に行くときにはz軸のみを使うのでzを参照
		p->playerCom.physics->SetVelocity(p->playerTransform->Forward() * playerAttackData.attackMove.z);
	}
	else {
		p->playerCom.physics->SetVelocity(playerAttackData.attackMove * MGetRotY(angle));
	}
	
}

void PlayerAttackStateBase::AttackCommonUpdate()
{
	if (noStateChange) {
		return; //ステートがチェンジするならリターン
	}
	//collsionCreate = false;
	Player* p = GetBase<Player>();
	float frame = p->playerCom.anim->GetCurrentFrame();
	if (InputManager::GetInstance()->KeyInputDown("attack")) {
		//アニメーション一定フレーム以降なら
		if (playerAttackData.attackInputStartTime <= frame) {
			nextAttack = true;
			normal = true;
			special = false;
		}
	}
	if (InputManager::GetInstance()->KeyInputDown("heavyAttack")) {
		//アニメーション一定フレーム以降なら
		if (playerAttackData.attackInputStartTime <= frame) {
			nextAttack = true;
			special = true;
			normal = false;
		}
	}
	//攻撃の時に回避行動をいれたら回避状態に移行
	if (InputManager::GetInstance()->KeyInputDown("avoid")) {
		if (playerAttackData.motionCancelStartTime <= frame) {
			nextAvoid = true;
		}
	}
	
	bool collsion = (playerAttackData.collsionStartTime <= frame && playerAttackData.collsionFinishTime >= frame);
	if (collsion) {
		if (defalutTrail) {
			p->playerCom.player->DrawTrail();
		}
		attackAgainStartCounter -= Time::DeltaTimeRate();
		if (attackAgainStartCounter <= 0.0f) { //複数当たり判定が入っているなら
			attackAgainStartCounter = playerAttackData.attackAgainStartCounterMax;
			AgainAttackCollsion();
		}
	}

	EnemyRotation();
	//攻撃の時に次のボタンが押されていたら次の攻撃

	//当たり判定がある間はスピードを早く
	if (collsion) {
		if (beforeAttack) {
			AttackMoveStart();
		}
		if (speedChange) {
			p->playerCom.anim->SetPlaySpeed(3.5f);
		}
		beforeAttack = false;

	}
	else {
		if (beforeAttack) {
			//p->playerCom.anim->SetPlaySpeed(1.0f);
		}
		else {
			//p->playerCom.physics->SetVelocity(VZero);
			p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA + VECTOR3(40000.0f, 0.0f, 40000.0f));
			if (nextAttack) {
				//次の攻撃に行くときの設定
				runTimer = playerAttackData.nextAttackRunTimer;
				noStateChange = true;
				p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
			}
			else {
				//攻撃が終わるときの設定
				runTimer = playerAttackData.noAttackRunTimer;
				noStateChange = true;
				p->playerCom.anim->SetPlaySpeed(ATTACK_FINISH_ANIM_SPEED);
			}
		}
	}
}

void PlayerAttackStateBase::AgainAttackCollsion()
{
	Player* p = GetBase<Player>();

	if (attackCount <= 0) {
		return; //攻撃の回数が0なら返す
	}

	firstColl = true;
	attackCount--;

	
	Debug::DebugLog("collsionCreate");
	p->DeleteCollision(&p->attackColl);
	firstColl = false;
	BaseAttackCollsion();
}

void PlayerAttackStateBase::AgainTimerSet(float _time, int _attackNum)
{
	//複数タイムのセット
	playerAttackData.attackNum = _attackNum;
	attackCount = playerAttackData.attackNum;
	attackAgainStartCounter = _time;
	playerAttackData.attackAgainStartCounterMax = attackAgainStartCounter;
}

void PlayerAttackStateBase::AttackCollsion()
{
	Player* p = GetBase<Player>();
	//アニメーションのイベントが始まったら攻撃の当たり判定を生成する。
	float frame = p->playerCom.anim->GetCurrentFrame();
	bool collsion = (playerAttackData.collsionStartTime <= frame && playerAttackData.collsionFinishTime >= frame);
	if (collsion) {
		//最初の一回のみ生成したい
		if (firstColl) {
			firstColl = false;
			BaseAttackCollsion();
			SoundManager::GetInstance()->RandamSe("swordWind", 5);
		}
	}
}

void PlayerAttackStateBase::SpecialAttackStart()
{
	Player* p = GetBase<Player>();
	if (InputManager::GetInstance()->KeyInputDown("SpecialAttack")) {
		if (p->CanSpecialAttack()) { //スペシャルゲージがたまっているなら
			p->playerCom.stateManager->ChangeState(StateID::PLAYER_SPECIAL_ATTACK_S);
			p->specialAttackBar = 0.0f;
		}

	}
}

void PlayerAttackStateBase::SetAttackData()
{
	Player* p = GetBase<Player>();
	playerAttackData = p->GetAttackDataMap(id);
}

void PlayerAttackStateBase::BaseAttackCollsion()
{
	Player* p = GetBase<Player>();
	p->playerCom.player->CollsionStart<SphereCollider>(&p->attackColl, playerAttackData.collTrans);
	p->playerCom.player->SetShape(CollsionInformation::SPHERE, &p->attackColl);
	collsionCreate = true;
	//新しく攻撃を生成する際敵のヒットリストを空にする
	p->hitObjects.clear();
}
