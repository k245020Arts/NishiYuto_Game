#include "BossAppear.h"
#include "../Boss.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "BossStatus.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Camera/Camera.h"
#include "../../../GameControler/GameControler.h"
#include "../../../Common/Fead/Fead.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Common/InputManager/InputManager.h"
#include "../../../Common/InputManager/PadInput.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Component/UI/UIManager/UIManager.h"
#include "../../../Component/Collider/SphereCollider.h"

BossAppear::BossAppear()
{
	animId = ID::B_APPEAR_FALL;
	//id = ID::B_IDOL;
	string = Function::GetClassNameC<BossAppear>();
	fead = FindGameObject<Fead>();
	feedInFinish = false;
	uiManager = FindGameObject<UIManager>();
	bossId = -1; 
	runStop = false;
	stopCounter = 1.0f;
	rightDushSound = false;;
	leftDushSound = false;;
}

BossAppear::~BossAppear()
{
}

void BossAppear::Update()
{
	Boss* b = GetBase<Boss>();
	if (bossId == 0) {
		if (b->enemyBaseComponent.camera->GetCutNum() == 3) { //カットシーンが3カット目に入ったらジャンプモーションを再生
			if (!feedInFinish) {
				b->enemyBaseComponent.physics->GetBaseObject()->SetObjectTimeRate(1.0f);
				feedInFinish = true;
				SoundManager::GetInstance()->PlaySe(Sound_ID::BOSS_JUMP_WIND);
			}

		}
		if (b->enemyBaseComponent.camera->GetCutNum() == 5) {//カットシーンが3カット目に入ったら咆哮をする
			if (!roar) {
				b->enemyBaseComponent.anim->Play(ID::B_ROAR_2, 0.2f);
				roar = true;
				b->enemyBaseComponent.anim->AnimEventReset();
			}

		}
		RoarEvent();
		//フェードが開けたら重力を加算
		if (feedInFinish) {
			const VECTOR3 GRAVITY_ADD = VECTOR3(0, -80.0f, 0);
			b->enemyBaseComponent.physics->AddGravity(GRAVITY_ADD);
			//着地したら
			if (b->enemyBaseComponent.physics->GetGround()) {
				b->enemyBaseComponent.anim->Play(ID::B_APPEAR_LAND);
				b->enemyBaseComponent.camera->CameraShake(VECTOR3(100, 100, 100), Shaker::MIX_SHAKE, false, 1.0f);
				feedInFinish = false;
				SoundManager::GetInstance()->PlaySe(Sound_ID::ENEMY_FALL);
				const int POWER = 200;
				const int FRAME = 60;
				InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(POWER, FRAME);
			}
			if (!b->enemyBaseComponent.camera->IsCutScene()) {

			}
		}
	}
	else {
		if (b->enemyBaseComponent.camera->GetCutNum() == 2) { //カットシーンが2カット目に入ったら走り出す
			Animator* anim = b->enemyBaseComponent.anim;
			anim->Play(ID::B_APPEAR_FALL);
			b->enemyBaseComponent.physics->AddVelocity(b->bossTransform->Forward() * -11000.0f, true);
			//走るときのサウンド
			if (anim->GetCurrentFrame() >= 3.0f && anim->GetCurrentFrame() <= 4.0f) {
				if (rightDushSound) {
					SoundManager::GetInstance()->PlayRamdomChangeFrequencySe(Sound_ID::BOSS_WALK, 30000, 1000);
					EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0, -100, 0), VZero, VOne), obj, Effect_ID::BOSS_GROUND, 1.0f);
				}
				rightDushSound = false;
			}
			else if (anim->GetCurrentFrame() >= 13.0f && anim->GetCurrentFrame() <= 14.0f) {
				if (leftDushSound) {
					SoundManager::GetInstance()->PlayRamdomChangeFrequencySe(Sound_ID::BOSS_WALK, 30000, 1000);
					EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0, -100, 0), VZero, VOne), obj, Effect_ID::BOSS_GROUND, 1.0f);
				}
				leftDushSound = false;
			}

			float resetTime = 14.0f;
			//最後に音がなった時からしばらくたったら
			if (b->enemyBaseComponent.anim->GetCurrentFrame() >= resetTime) {
				rightDushSound = true;
				leftDushSound = true;
			}
		}
		if (b->enemyBaseComponent.camera->GetCutNum() == 3) { //カットシーンが3カット目に入ったら
			if (!roar) {
				b->enemyBaseComponent.anim->Play(ID::B_APPEAR_LAND);
				runStop = true;
				b->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * 2.0f);
			}
		}
		RoarEvent();
		if (runStop) { //走り終わったら咆哮
			if (b->enemyBaseComponent.anim->IsFinish()) {
				stopCounter -= Time::DeltaTimeRate();
				if (stopCounter <= 0.0f) {
					b->enemyBaseComponent.anim->Play(ID::B_ROAR_ANIM);
					runStop = false;
					roar = true;
				}
			}
		}
	}
	
}

void BossAppear::Draw()
{
}

void BossAppear::Start()
{
	Boss* b = GetBase<Boss>();
	bossId = b->bossID;
	//Bossの種類によってカットシーンの入り方を変えている
	if (bossId == 0) {
		const VECTOR3 INIT_POS = VECTOR3(0, 10000, 2000);
		obj->GetTransform()->position = INIT_POS;
		b->enemyBaseComponent.physics->GetBaseObject()->SetObjectTimeRate(0.0f);
		animId = ID::B_APPEAR_FALL;
	}
	else {
		const VECTOR3 INIT_POS = VECTOR3(0, 0, 20000);
		obj->GetTransform()->position = INIT_POS;
		b->enemyBaseComponent.physics->GetBaseObject()->SetObjectTimeRate(1.0f);
		//当たり判定の一時ストップ(壁に埋もれないようにするために)
		for (SphereCollider* collider : b->bossHitCollider) {
			collider->CollsionFinish();
		}
		animId = ID::B_IDOL;
	}
	EnemyStateBase::Start();

	feedInFinish = false;
	roar = false;
	first = true;
	//UIの描画をなくす
	uiManager->SetUIDraw(false);
	SoundManager::GetInstance()->FeedInOut(Sound_ID::PLAY_BGM, 0.5f);
	SoundManager::GetInstance()->PlayBGM(Sound_ID::BOSS_BEFORE,true,true);
	SoundManager::GetInstance()->FeedInStart(Sound_ID::BOSS_BEFORE,0.5f);
	runStop = false;
	stopCounter = 1.0f;
	rightDushSound = true;
	leftDushSound = true;
}

void BossAppear::Finish()
{
	Boss* b = GetBase<Boss>();
	b->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION);
}

void BossAppear::RoarEvent()
{
	Boss* b = GetBase<Boss>();
	if (roar) {
		if (b->enemyBaseComponent.anim->AnimEventCan()) {
			if (first) {
				//当たり判定の一時ストップの解除
				for (SphereCollider* collider : b->bossHitCollider) {
					collider->CollsionRespown();
				}
				first = false;
				EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0.0f, 100.0f, 0.0f), VZero, VOne), b->GetBaseObject(), Effect_ID::BOSS_ROAR, 2.0f);
				SoundManager::GetInstance()->PlaySe(Sound_ID::BOSS_ROAR_VOICE);
				SoundManager::GetInstance()->FeedInOut(Sound_ID::BOSS_BEFORE, 0.5f);
				b->enemyBaseComponent.camera->CameraPerspectiveShakeStart(5.0f, 2.0f);
				InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(500, 120);
			}

		}
		if (b->enemyBaseComponent.anim->IsFinish()) {
			b->enemyBaseComponent.gameManager->ChangeState(GameControler::GameState::PLAY);
			SoundManager::GetInstance()->FeedInStart(Sound_ID::PLAY_BGM, 1.0f);
		}
	}
}
