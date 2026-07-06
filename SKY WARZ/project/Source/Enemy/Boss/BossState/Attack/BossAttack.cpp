#include "BossAttackJsonParse.h"
#include "../../Boss.h"
#include "../../../../Component/Animator/Animator.h"
#include "../../../../CharaBase/CharaBase.h"
//#include "../Enemy.h"
#include "../../../../Component/Transform/transform.h"
#include "../../../../Component/Collider/sphereCollider.h"
#include "../../../EnemyBase.h"
#include "../../../Boss/Boss.h"
#include "../../../../Common/ResourceLoader.h"
#include "../../../EnemyState/EnemyStateManager.h"
#include "../../../../Common/Effect/EffectManager.h"
#include "../../../../Common/Sound/SoundManager.h"
#include "../../../../Weapon/WeaponManager.h"
#include "../../../../Camera/Camera.h"
#include "../../../TrashEnemy/TrashEnemy.h"
#include "../../../../Common/Debug/DebugLogText.h"
#include "../../../../State/StateManager.h"
#include "../../../../Common/Easing.h"
#include "../BossStatus.h"
#include "../../../../Component/Physics/Physics.h"
#include "../../../../Component/Collider/DountCollider.h"
#include "../../../../Component/EnemyAttackObject/ShockWave/ShockWave.h"
#include "../../../../Component/EnemyAttackObject/BossRock/BossRockManager.h"
#include "../AttackSorting.h"

BossAttack::BossAttack()
{
	averageSpeed = 0.0f;
	normal = VZero;
	aloowStop = false;
	firstJump = false;
	gravitySpeed = false;
	groundEffect = false;
	rockGet = false;
	throwRock = false;

	rushSound = false;
	firstOnes = false;
	secondOnes = false;
	firstMove = false;

	rockColl = nullptr;
	rushAttackCount = 0.0f;
	throwObjectAppearTime = 0.0f;
	throwObjectNumNow = 0;
	bossPtr = nullptr;
}

BossAttack::~BossAttack()
{
}

void BossAttack::Update()
{
	EnemyAttackBase::Update();

	//ボスの攻撃に必要なUpdateの処理を実行--------------------------

	AttackFinishFrame();
	AttackSound();
	BossAttackCollsion();
	BossJustAvoidCollsion();
	RotateEvent();
	LookEvent();
	ShackWaveEvent();
	MoveEvent();
	RushEvent();
	JumpEvent();
	ThrowObjectsEvent();
	SoundEvent();
	if (attackParam.useTrail) {
		BossTrail(attackParam.trailRightHand);
	}
	AttackFlash(ID::B_MODEL, attackParam.attackPositionFrameNum, attackParam.voiceName);

	//--------------------------------------------------------------------------------------
}

void BossAttack::Draw()
{
}

void BossAttack::Start()
{
	Boss* boss = GetBase<Boss>();
	bossPtr = boss;
	boss->threat = false;
	//attackParam.flash	= false;
	//DebugLogText::GetInstance()->Log(LogLevel::INFO, string + "攻撃が開始しました");

	//攻撃の前モーションが設定されてなかったら攻撃のモーションを流す
	if (attackParam.attackBeforeAnimID == ID::ID_MAX) {
		boss->enemyBaseComponent.anim->Play(attackParam.animID);
	}
	else {
		boss->enemyBaseComponent.anim->Play(attackParam.attackBeforeAnimID);
	}
	//空のオブジェクトを投擲物データ分生成
	boss->rockManager->CreateThrowEmptyObject(attackParam.throwAttackData);

	//攻撃に必要なデータの初期化
	firstColl = true;
	attackTime = boss->enemyBaseComponent.anim->EventFinishTime(attackParam.animID) - boss->enemyBaseComponent.anim->EventStartTime(attackParam.animID);
	sound = true;
	boss->enemyBaseComponent.anim->AnimEventReset();
	slowMode = false;
	aloowStop = false;
	firstJump = true;
	gravitySpeed = false;
	groundEffect = true;
	rockGet = false;
	throwRock = false;
	throwObjectAppearTime = attackParam.intervalTime;
	throwObjectNumNow = 0;

	rushSound = false;
	firstOnes = false;
	secondOnes = false;
	firstMove = true;

	rockColl = nullptr;
	boss->threat = false;

	if (attackParam.attackCameraBossLook) {
		//敵の攻撃時に敵の方向を向く攻撃なら敵の方向にカメラを向かせるs
		boss->enemyBaseComponent.camera->AttackEnemyFovChange(boss->bossTransform, attackParam.cameraChangeSpeed);
	}

	//-------------------------------------------------------

	//サウンドイベントで使う変数の初期化
	for (const auto& event : attackParam.soundEvent) {
		soundLoopPlaying[event.name] = false;
		soundPlayed[event.name] = false;
	}
}

void BossAttack::Finish()
{
	Boss* boss = GetBase<Boss>();
	if (boss->maxAttack < 0) {
		boss->threat = true;
	}
	boss->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION);
	boss->enemyBaseComponent.physics->SetGravity(VECTOR3(0, -1500, 0));
	//敵の攻撃がまだ残っていたら削除
	if (boss->attackColl.instance != nullptr) {
		boss->DeleteCollision(&boss->attackColl);
	}
	boss->enemyBaseComponent.anim->AnimEventReset();
	//岩に当たった時にひるむ判定が残っていたら削除
	if (rockColl != nullptr) {
		rockColl->GetBaseObject()->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("Rush");
		rockColl = nullptr;
	}
	//攻撃終了時に投擲物をなくしたい場合は、投擲物の削除
	for (auto data : attackParam.throwAttackData) {
		if (data.playerAttackObjectDrop) {
			boss->rockManager->DropRockStart();
		}
	}
	firstColl = false;
	boss->rockManager->AttackFinishDelete();
	boss->rockManager->DeleteEmptyObject();
	boss->rockManager->BossDieDeleteObject(boss->hp);
}

void BossAttack::BossAttackCollsion()
{
	if (!CurrentAttackAnim()) {
		return;
	}
	Boss* b = GetBase<Boss>();
	//敵の攻撃判定の生成クラス
	if (b->enemyBaseComponent.anim->AnimEventCan()) {
		if (firstColl) {
			firstColl = false;
			b->CollsionStart<SphereCollider>(&b->attackColl, attackParam.attackCollTransform, b->attackFunk);
			b->SetShape(CollsionInformation::SPHERE, &b->attackColl);
		}
	}
}


void BossAttack::BossTrail(bool _right)
{
	Boss* e = GetBase<Boss>();
	float time = e->enemyBaseComponent.anim->EventStartTime(attackParam.animID);
	//剣の軌跡を発生
	if (time - 7.0f <= e->enemyBaseComponent.anim->GetCurrentFrame() && time + 5.0f >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		e->Drail(_right);
	}
}

void BossAttack::BossJustAvoidCollsion()
{
	if (!CurrentAttackAnim()) {
		return;
	}
	Boss* e = GetBase<Boss>();
	float time = e->enemyBaseComponent.anim->EventStartTime(attackParam.animID);
	//ジャスト回避判定の作成
	if (time - attackParam.justAvoidCollsionStartTime <= e->enemyBaseComponent.anim->GetCurrentFrame() && time >= e->enemyBaseComponent.anim->GetCurrentFrame()) {
		e->CollsionStart<SphereCollider>(&e->justAvoidColl, attackParam.justAvoidCollTransform, e->justAvoidAttackFunk);
		e->SetShape(CollsionInformation::SPHERE, &e->justAvoidColl);
		e->justAvoidCollTime = 3.0f;
	}
}

void BossAttack::RotateEvent()
{
	Boss* boss = GetBase<Boss>();
	if (!attackParam.rotateMove) {
		return; //回転イベントがない場合スルー
	}
	if (boss->enemyBaseComponent.anim->AnimEventCan()) {
		//攻撃にかかる時間で90°回したいので１フレームごとに進む角度を求めている。
		averageSpeed = attackParam.angleMoveAmout / attackTime;
		averageSpeed *= boss->obj->GetObjectTimeRate();

		boss->bossTransform->rotation.y += averageSpeed * DegToRad;
	}
}

void BossAttack::LookEvent()
{
	Boss* boss = GetBase<Boss>();
	if (!attackParam.lookPlayer) {
		return; //見るイベントがない場合スルー
	}
	if (attackParam.lookMaxCounter <= boss->enemyBaseComponent.anim->GetCurrentFrame()) {
		return; //プレイヤーを見るイベントの最大フレームを越したらスルー
	}

	//引数で値を増やしても良いがそれだと微調整が出来ないのでこの処理を何回走るかで振り向きの強度を決めている
	for (int i = 0; i < attackParam.lookNum; i++) {
		boss->LookPlayer();
	}
}

void BossAttack::MoveEvent()
{
	Boss* boss = GetBase<Boss>();
	if (!CurrentAttackAnim()) {
		return; //今再生されてるモーションが攻撃モーション以外ならスルー
	}
	if (!attackParam.playerAloowMove && !attackParam.frontMove) {
		return; //追従モードがまっすぐに進むかのイベントが両方入ってないならスルー
	}
	float animFrame = boss->enemyBaseComponent.anim->GetCurrentFrame();

	//移動するアニメーションの再生時間なら移動する

	const float FIRCTION_RATE = 24.0f;
	if (attackParam.moveStartTime <= animFrame && attackParam.moveFinishTime >= animFrame) {
		//プレイヤー追従モードなら
		if (attackParam.playerAloowMove) {
			VECTOR3 dis = boss->enemyBaseComponent.playerObj->GetTransform()->position - boss->bossTransform->position;
			normal = dis.Normalize();
			bool move = true;
			//y座標をいじりたくないので0にする。
			normal.y = 0.0f;
			//基準の距離よりボスとプレイヤーが近くなったら
			if (dis.Size() <= attackParam.playerBaseNear) {
				if (attackParam.playerNearStop) {
					boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * FIRCTION_RATE); //ストップさせる
					move = false;
				}
				else if (attackParam.playerNearAloowStop) {
					//move = false;
					aloowStop = true; //プレイヤーを通り越して走り出す
				}

			}
			if (firstMove) {
				//初速を代入
				if (move) {
					VECTOR3 dis = boss->enemyBaseComponent.playerObj->GetTransform()->position - boss->bossTransform->position;
					normal = dis.Normalize();
					boss->enemyBaseComponent.physics->AddVelocity(normal * attackParam.baseFirstSpeed, false);
				}
				firstMove = false;
				return;//最初の移動の時は返す
			}
			//プレイヤーを通過するモードになったら正面移動をし続ける
			if (aloowStop) {
				VECTOR3 dis = boss->bossTransform->Forward() * 1.0f;
				normal = dis.Normalize();
				normal.y = 0.0f;
				float speed = attackParam.baseSpeed;
				if (attackParam.addVelocity) {
					boss->enemyBaseComponent.physics->AddVelocity(normal * speed, true);
				}
				else {
					boss->enemyBaseComponent.physics->SetVelocity(normal * speed);
				}
				return;
			}
			if (move) {
				float speed = dis.Size();
				speed = std::clamp(speed, attackParam.minMoveSpeed, attackParam.maxMoveSpeed);
				if (attackParam.addVelocity) {
					boss->enemyBaseComponent.physics->AddVelocity(normal * speed, true);
				}
				else {
					boss->enemyBaseComponent.physics->SetVelocity(normal * speed);
				}

			}
			//追従するときは基本プレイヤーを見るのでlookEventも動かす
			LookEvent();
		}
		if (attackParam.frontMove) {
			if (firstMove) {
				VECTOR3 dis = boss->bossTransform->Forward() * 1.0f;
				normal = dis.Normalize();
				boss->enemyBaseComponent.physics->AddVelocity(normal * attackParam.baseFirstSpeed, false);
				firstMove = false;
				return;//最初の移動の時は返す
			}
			VECTOR3 dis = boss->bossTransform->Forward() * 1.0f;
			normal = dis.Normalize();
			normal.y = 0.0f;
			float speed = attackParam.baseSpeed;
			if (attackParam.addVelocity) {
				boss->enemyBaseComponent.physics->AddVelocity(normal * speed, true);
			}
			else {
				boss->enemyBaseComponent.physics->SetVelocity(normal * speed);
			}
		}

	}
	//移動が終了したら移動をストップする
	if (animFrame > attackParam.moveFinishTime) {
		boss->enemyBaseComponent.physics->SetFirction(BossInformation::BASE_FIRCTION * FIRCTION_RATE);
	}
}

void BossAttack::JumpEvent()
{
	if (!attackParam.jump) {
		return; //ジャンプイベント状態ではないならスルー
	}
	Boss* boss = GetBase<Boss>();
	if (boss->enemyBaseComponent.anim->GetCurrentFrame() <= attackParam.jumpStartTime) {
		return; //ジャンプスタートの時間が来るまではリターン
	}
	if (firstJump) {
		firstJump = false; //初速のジャンプ
		boss->enemyBaseComponent.physics->AddVelocity(VECTOR3(0, attackParam.jumpSpeed/*3000.0f*/, 0), false);
	}

	boss->enemyBaseComponent.physics->AddGravity(VECTOR3(0, gravitySpeed, 0)); //重力をどんどん増加させる
	gravitySpeed += attackParam.addGravity;

	//地面についたとき
	if (boss->enemyBaseComponent.physics->GetGround()) {
		if (boss->enemyBaseComponent.anim->GetCurrentFrame() >= attackParam.groundEffectStartTime) {

			if (groundEffect) {
				groundEffect = false;

				BaseObject* obj2 = EffectManager::GetInstance()->CreateEffekseer(*boss->GetBaseObject()->GetTransform(), boss->GetBaseObject(), attackParam.jumpGroundEffect, 1.0f);


				EffectManager::GetInstance()->ParentTransformRemove(obj2);

				SoundManager::GetInstance()->PlaySe(Sound_ID::GROUND);
				boss->enemyBaseComponent.camera->CameraPerspectiveShakeStart(/*3.0f, 0.4f*/ attackParam.groundShakeCamera, attackParam.groundShakeTime);
				//衝撃波を出すなら出す
				CreateWave();
			}
		}

		/*if (b->enemyBaseComponent.anim->IsFinish()) {
			b->BossAttackStateChange();
		}*/
	}
}

void BossAttack::ShackWaveEvent()
{
	if (!attackParam.shockWave || attackParam.jump) {
		return; //ジャンプイベントの場合はジャンプから衝撃波を出す
	}
	Boss* boss = GetBase<Boss>();
	if (boss->enemyBaseComponent.anim->AnimEventCan()) {
		if (groundEffect) {
			CreateWave();
			groundEffect = false;
		}

	}
}

void BossAttack::CreateWave()
{
	if (!attackParam.shockWave) {
		return; //Waveを出さないなら出さない
	}
	Boss* boss = GetBase<Boss>();
	BaseObject* obj1 = EffectManager::GetInstance()->CreateEffekseer(*boss->GetBaseObject()->GetTransform(), boss->GetBaseObject(), /*Effect_ID::BOSS_WAVE*/attackParam.shockMoveEffect, 1.0f);
	EffectManager::GetInstance()->ParentTransformRemove(obj1);
	ShockWave* w = obj1->Component()->AddComponent<ShockWave>();
	w->CreateWave(CollsionInformation::B_E_ATTACK, Transform(VZero, VZero, VOne), /*50.0f, 50.0f*/attackParam.startRange, attackParam.shockWaveSpeed);

}

void BossAttack::ThrowObjectsEvent()
{
	//投擲物イベントがないなら
	if (!attackParam.throwObject) {
		return;
	}
	Boss* boss = GetBase<Boss>();
	//腕で投げるなら
	if (attackParam.armThrow) {
		float animFrame = boss->enemyBaseComponent.anim->GetCurrentFrame();
		boss->rockManager->RockContorler(attackParam, animFrame);
	}
	else {
		throwObjectAppearTime -= Time::DeltaTimeRate();
		//岩が登場する時間が0になっているときかつ、MAXの値まで出てないときは登場
		if (throwObjectAppearTime <= 0.0f && throwObjectNumNow < attackParam.throwObjectNum)
		{
			boss->rockManager->CreateThrowObject(attackParam.throwAttackData, throwObjectNumNow, attackParam.throwObjectNum, 0.0f);

			throwObjectNumNow++;

			// 徐々に出るスピードが早くなる
			if (attackParam.intervalTimeSub) {
				float rate = (float)throwObjectNumNow / (float)attackParam.throwObjectNum;
				throwObjectAppearTime = Easing::Lerp(/*0.45f, 0.15f*/attackParam.maxIntervalTime, attackParam.minIntervalTime, rate);
			}
			else {
				throwObjectAppearTime = attackParam.intervalTime;
			}

		}
		//投擲物が全部出きったら
		if (throwObjectNumNow >= attackParam.throwObjectNum)
		{
			boss->rockManager->ShakeCamera();
			boss->enemyBaseComponent.state->ChangeState(StateID::BOSS_IDOL_S);
		}
	}
}

void BossAttack::RushEvent()
{
	//突進イベントがないなら
	if (!attackParam.rushMove) {
		return;
	}
	Boss* boss = GetBase<Boss>();
	//後隙の時にアニメーションの再生が終わったら終了
	if (boss->enemyBaseComponent.anim->IsFinish()) {
		if (boss->enemyBaseComponent.anim->GetCurrentID() == ID::GetID(attackParam.rushAfterAnimID)) {
			boss->BossAttackStateChange();
		}
		else {
			AttackStart();
		}
	}
	//前隙の時にプレイヤーを見る
	if (boss->enemyBaseComponent.anim->GetCurrentID() == ID::GetID(attackParam.attackBeforeAnimID)) {
		boss->LookPlayer(0.09f);
		return;//前隙の時に後ろの処理に行かないように
	}
	BossDushSound();

	rushAttackCount -= obj->GetObjectTimeRate();
	//突進時間が過ぎたなら後隙へと移行
	if (rushAttackCount <= 0.0f) {
		boss->enemyBaseComponent.anim->Play(attackParam.rushAfterAnimID);
		VECTOR3 p = boss->enemyBaseComponent.physics->GetVelocity().Normalize();
		boss->enemyBaseComponent.physics->AddVelocity(p * -attackParam.rushAfterSpeed/*5000*/, true);
	}
}

void BossAttack::AttackFinishFrame()
{
	Boss* boss = GetBase<Boss>();
	//攻撃のアニメーションじゃなかったらリターン
	if (!CurrentAttackAnim()) {
		return;
	}
	if (boss->enemyBaseComponent.anim->IsFinish())
	{
		boss->BossAttackStateChange();
		boss->GetStateManager()->GetState<AttackSorting>()->AttackFinish();
	}
}

void BossAttack::SoundEvent()
{
	if (attackParam.soundEvent.empty()) { //サウンドイベントがないならリターン
		return;
	}

	Boss* boss = GetBase<Boss>();

	float currentFrame = boss->enemyBaseComponent.anim->GetCurrentFrame();

	for (const auto& event : attackParam.soundEvent)
	{
		const auto soundID = Sound_ID::StringToID(event.name);

		//再生区間内
		bool inRange = currentFrame >= event.soundStartFrame && currentFrame <= event.soundStopFrame;

		if (inRange)
		{
			//ループ再生
			if (event.loop)
			{
				if (!SoundManager::GetInstance()->CheckSe(soundID)) {
					SoundManager::GetInstance()->PlaySe(soundID);
					soundLoopPlaying[event.name] = true;
				}
			}
			//1回再生
			else
			{
				if (!soundPlayed[event.name])
				{
					SoundManager::GetInstance()->PlaySe(soundID);

					soundPlayed[event.name] = true;
				}
			}
		}
		//区間外
		else
		{
			//ループ停止
			if (event.loop && soundLoopPlaying[event.name])
			{
				SoundManager::GetInstance()->StopSE(soundID);
				soundLoopPlaying[event.name] = false;
			}

			//stopFrame後に停止
			if (currentFrame > event.soundStopFrame)
			{
				if (!event.soundStopFrameAfetrSoundPlay)
				{
					SoundManager::GetInstance()->StopSE(soundID);
				}
			}
		}
	}
}

void BossAttack::BossDushSound()
{
	Boss* b = GetBase<Boss>();
	SoundManager::GetInstance()->Play3DSound(Sound_ID::BOSS_WALK, obj, 200000.0f, 30000.0f);
	//突進時の足跡の音(アニメーションフレームで再生)
	if (b->enemyBaseComponent.anim->GetCurrentFrame() >= attackParam.rushSoundLeftFoot &&
		b->enemyBaseComponent.anim->GetCurrentFrame() <= attackParam.rushSoundLeftFoot + 1.0f) {
		//一回だけ鳴らす
		if (firstOnes) {
			rushSound = true;
		}
		firstOnes = false;
	}
	//突進時の足跡の音(アニメーションフレームで再生)
	if (b->enemyBaseComponent.anim->GetCurrentFrame() >= attackParam.rushSoundRightFoot &&
		b->enemyBaseComponent.anim->GetCurrentFrame() <= attackParam.rushSoundRightFoot + 1.0f) {
		//一回だけ鳴らす
		if (secondOnes) {
			rushSound = true;
		}
		secondOnes = false;
	}
	//音がなった時に周波数を変える
	if (rushSound) {
		SoundManager::GetInstance()->PlayRamdomChangeFrequencySe(Sound_ID::BOSS_WALK, 30000, 1000);
		rushSound = false;
		//Debug::DebugLog("bossDushSound");
	}
	float resetTime = 0.0f;
	//右のタイムと左のタイムで比較をして大きい方をResetTimeとする
	if (attackParam.rushSoundRightFoot > attackParam.rushSoundLeftFoot) {
		resetTime = attackParam.rushSoundRightFoot;
	}
	else {
		resetTime = attackParam.rushSoundLeftFoot;
	}
	//最後に音がなった時からしばらくたったら
	if (b->enemyBaseComponent.anim->GetCurrentFrame() >= resetTime + 2.0f) {
		firstOnes = true;
		secondOnes = true;
	}
}

void BossAttack::AttackStart()
{
	Boss* b = GetBase<Boss>();
	b->enemyBaseComponent.anim->Play(attackParam.animID);

	VECTOR3 pos = b->enemyBaseComponent.playerObj->GetTransform()->position;

	VECTOR3 angle = pos - b->GetBaseObject()->GetTransform()->position;
	//rotation = angle.Normalize();
	firstColl = true;
	//look = true;
	//distance = pos.Size();


	//突進した時専用の当たり判定を付けるか
	if (attackParam.rushColl) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_RUSH;

		rockColl = obj->Component()->AddComponent<SphereCollider>();
		Transform rushColl = attackParam.attackCollTransform;
		rushColl.scale.x += attackParam.addRushCollScale;

		std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) {bossPtr->RockHitRushDamage(_data); };
		rockColl->CollsionAdd(info, rushColl, func, "Rush");
	}
	//連続攻撃をしないなら
	if (b->maxAttack <= 0) {
		AttackFlash(ID::B_MODEL, attackParam.attackPositionFrameNum, attackParam.voiceName);
		//attackParam.flash = true;
		rushAttackCount = attackParam.rushTime;
		b->enemyBaseComponent.anim->SetPlaySpeed(2.0f);
	}
	else {
		rushAttackCount = attackParam.rushTime;
		b->enemyBaseComponent.anim->SetPlaySpeed(2.0f);
	}
	firstOnes = true;
	secondOnes = true;
}