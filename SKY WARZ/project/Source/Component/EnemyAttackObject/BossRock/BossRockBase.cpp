#include "BossRockBase.h"
#include "BossRockManager.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Common/ResourceLoader.h"
#include "../../Collider/SphereCollider.h"
#include "../../Collider/RayCollider.h"
#include "../../Collider/ModelCollider.h"
#include "../../Collider/DountCollider.h"
#include "../../Collider/CapsuleCollider.h"
#include "../../ComponentManager.h"
#include "../../Physics/Physics.h"
#include "../../Shaker/Shaker.h"
#include "../../MeshRenderer/MeshRenderer.h"
#include "../../../Common/Debug/DebugNew.h"
#include "../../MeshRenderer2D/MeshRenderer2D.h"

BossRockBase::BossRockBase()
{
	randColl = nullptr;
	playerHitColl = nullptr;
	bossHitColl = nullptr;
	uiColl = nullptr;
	pushColl = nullptr;
	playerAttackHitColl = nullptr;
	bossRushHitColl = nullptr;
	blastColl = nullptr;
	physics = nullptr; 
	blastJustAvoidColl = nullptr;
	justAvoidCapselCollider = nullptr;
	playerHitCapsuleColl = nullptr;

	blast = false;
	color = nullptr;
	blinkCounter = 0.0f;
	blinkBaseMax = 0.5f;
	ChangeColorMode = false;
	nowBlast = false;
	flyCounter = 0.0f;

	blastCan = false;
	fly = false;
	groundInit = false;

	rockManager = FindGameObject<BossRockManager>();

	useHandleNumber = -1;

	justAvoidCollider = nullptr;

	preDraw = false;
	preTransform = Transform();
	preModel = ResourceLoad::LoadModel("BossRockPre", ID::BOSS_PRE_MODEL);

	throwRock = false;
	throwReady = false;
	throwRockBaseModel = -1;
	dir = VZero;
	boneNum = 0;
	addPos = VZero;

	debugId = 34;
	tag = Function::GetClassNameC<BossRockBase>();
	groundTime = 0.0f;
	playerAttackHit = false;
	velocityAdd = false;
	handMatrix = nullptr;
	start = false;
	capsuleEndPos = VZero;
	capsuleJustAvoidEndPos = VZero;
	colliderAddStart = false;
	alotHitCounter = 0.0f;
	flyingUIDraw = false;

	playerTransform = nullptr;
}

BossRockBase::~BossRockBase()
{
	randColl = nullptr;
	playerHitColl = nullptr;
	bossHitColl = nullptr;
	uiColl = nullptr;
	pushColl = nullptr;
	blastColl = nullptr;
	playerAttackHitColl = nullptr;
	physics = nullptr;
	bossRushHitColl = nullptr;
	useHandleNumber = -1;

	useHandleNumber = -1;
	physics = nullptr;
	rockManager->RemoveList(this);
	justAvoidCollider = nullptr;
	blastJustAvoidColl = nullptr;
	hitObjects.clear();
	preDraw = false;
	throwReady = false;
	playerAttackHit = false;
	handMatrix = nullptr;
	playerFlyAwayBButtonUI = -1;
	playerFlyAwayYButtonUI = -1;
}

void BossRockBase::Awake()
{
	rockManager->PushList(this);
}

void BossRockBase::Update()
{
	if (!start) {
		return;//まだ情報が入っていないのでリターン
	}
	if (throwReady && !attackData.armSwordHand) { //腕投げの時の座標
		Transform* transform = obj->GetTransform();
		MATRIX matrixWorld = MV1GetFrameLocalWorldMatrix(throwRockBaseModel, boneNum);
		// WORLD座標を取得
		VECTOR3 frameWorldPos = MV1GetFramePosition(throwRockBaseModel, boneNum);
		MATRIX objWorldMat = obj->GetParent()->GetTransform()->WorldTransform().GetMatrix();
		MATRIX invObjWorldMat = MInverse(objWorldMat);
		VECTOR3 frashPosLocal = frameWorldPos * invObjWorldMat;
		transform->position = frameWorldPos + addPos;
	}
	if (!attackData.groundDelete) { //削除時間
		groundTime -= Time::DeltaTimeRate();
		if (groundTime <= 0.0f) {
			obj->DestroyMe();
		}
	}
	if (throwRock) { //投げた後の挙動
		if (velocityAdd) {
			VECTOR3 direction = dir + attackData.diffusionAngle;
			physics->AddVelocity(direction * attackData.throwSpeed, true);
			
			
		}
		VECTOR3 dist = obj->GetTransform()->position - obj->GetParent()->GetTransform()->position;
		if (dist.Size() >= 50000.0f) {
			obj->DestroyMe();
		}
	}
	//自由な方向に飛ぶのを指定したらその方向に飛ばす
	if (attackData.freeDir) {
		//VECTOR3 direction = dir + attackData.diffusionAngle;
		if (velocityAdd) {
			physics->AddVelocity(attackData.thorwVelocity, true);
		}
	}
	if (playerAttackHit) { //プレイヤーが攻撃して飛ばしたときに一瞬で地面についた判定にならないためのカウンター

		flyCounter -= Time::DeltaTimeRate();
		if (flyCounter <= 0.0f) {
			fly = true;
		}
	}
	if (colliderAddStart) {//当たり判定が大きくなるか
		if (playerHitCapsuleColl != nullptr) {
			playerHitCapsuleColl->AddCapsuleEndPos(attackData.capsuleEndPosExtendOffset);
		}
		if (justAvoidCapselCollider != nullptr) {
			justAvoidCapselCollider->AddCapsuleEndPos(attackData.capsuleEndPosExtendOffset);
		}
	}
	if (attackData.alotCollsionHit) { //多段ヒット
		alotHitCounter += Time::DeltaTime();
		//再生成タイミングが来たら
		if (alotHitCounter > attackData.alotHitColliderCreateCounter) {
			AlotCollsionSet();
			alotHitCounter = 0.0f;
		}
	}
	if (nowBlast) { //爆発
		//50
		float waveSpeed = attackData.waveSpeed;
		blastColl->GetTransform()->scale.x += waveSpeed;
		blastColl->AddOutRadius(waveSpeed);
		blastJustAvoidColl->GetTransform()->scale.x += waveSpeed;
		blastJustAvoidColl->AddOutRadius(waveSpeed);
		//一定以上の爆発範囲になったら削除
		if (blastColl->GetOutRadius() >= attackData.maxRadius) {
			obj->DestroyMe();
		}
		if (blastJustAvoidColl->GetOutRadius() >= attackData.maxRadius) {
			obj->DestroyMe();
		}
	}
	else {
		if (blast) { //爆発の予測
			blinkCounter -= Time::DeltaTimeRate();
			if (blinkCounter <= 0.0f) {
				blinkCounter = blinkBaseMax;
				ChangeColorMode = !ChangeColorMode;
				blinkBaseMax -= blinkBaseMax / 8.0f;
			}
			if (ChangeColorMode) {
				color->setRGB(Color::Rgb(255, 0, 0, 255));
			}
			else {
				color->setRGB(Color::Rgb(255, 255, 255, 255));
			}
			if (blinkBaseMax <= 0.01f) {
				BlastCollsionCreate();
				
			}
		}
	}
	//はじき返せることをUIで示す
	if (flyingUIDraw) {
		VECTOR3 pos = obj->GetTransform()->position;
		VECTOR3 UIDrawPos = ConvWorldPosToScreenPos(pos);
		int i = 0;
		for (auto mesh : meshRenderer2DList) {
			
			//距離によってスケールを変えたり、座標を変える
			const float MAX_DIST = 15000.0f;
			VECTOR3 dist = (playerTransform->position - obj->GetTransform()->position);
			float size = dist.Size();
			if (size <= 0.0f) {
				size = 0.0f;
			}
			if (size >= MAX_DIST) {
				size = MAX_DIST;
			}
			float rate = size / MAX_DIST;
			VECTOR3 scale = VOne * (1 - rate);
			mesh->SetScale(scale);
			float POS_AMONG = 50.0f;
			mesh->SetPosition(VECTOR3(UIDrawPos.x + (i * POS_AMONG) * (1 - rate), UIDrawPos.y, pos.z));
			i++;
			bool draw = true;
			const float PLAYER_BACK_BASE_NUM = 1.0f;
			if (UIDrawPos.z > PLAYER_BACK_BASE_NUM) {
				draw = false;
			}
			mesh->SetDraw(draw);
		}
	}
	
}

void BossRockBase::Draw()
{
	if (preDraw) { //予測線の描画
		//MV1SetDifColorScale(preModel, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		MV1SetMatrix(preModel, preTransform.GetMatrix());
		MV1DrawModel(preModel);
	}
	
}

void BossRockBase::Ground(const CollsionEventData& _data)
{
	if (fly || attackData.groundDelete) { //二回地面につくか、地面についたら消えるようになるパラメータをセットしたら
		if (attackData.blastCan) { //爆発するなら爆発
			BlastCollsionCreate();
		}
		else { //しないなら削除
			obj->DestroyMe();
			SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_BREAK);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VZero, VZero, VOne * 4.0f), obj, Effect_ID::ROCK_BREAK, 1.0f);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0, -100, 0), VZero, VOne * 4.0f), obj, Effect_ID::BOSS_GROUND, 6.0f);
		}
		return;
	}
	if (groundInit || throwReady) {
		return;
	}
	SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_GROUND);
	EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0,-100,0), VZero, VOne * 4.0f), obj, Effect_ID::BOSS_GROUND, 1.0f);
	//effectManager->StopEffekseer(Effect_ID::ROCK_FALL);
	groundInit = true;
	//当たり判定が存在していたら削除
	if (playerHitColl != nullptr) {
		obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("_rockAttack");
		playerHitColl = nullptr;
	}
	if (playerHitCapsuleColl != nullptr) {
		obj->Component()->RemoveComponentWithTagIsCollsion<CapsuleCollider>("_rockAttack");
		playerHitCapsuleColl = nullptr;
	}
	
	//地面についたときの残留ダメージがあるならその当たり判定の生成
	if (attackData.playerGroundHit) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();

		info.oneColl = false;
		//カプセルの形ならカプセルで生成
		if (attackData.capsule) {
			info.shape = CollsionInformation::CAPSULE;
			if (attackData.playerGroundOneHit) {
				info.oneColl = true;
				info.tag = CollsionInformation::THROW_OBJECT_GROUND_ONE_HIT;
			}
			else if (attackData.playerGroundNoDamageReactionHit) {
				info.tag = CollsionInformation::THROW_OBJECT_GROUND_NO_DAMAGE_REACTION;
			}
			else {
				info.tag = CollsionInformation::THROW_OBJECT_GROUND;
			}

			playerHitCapsuleColl = obj->Component()->AddComponent<CapsuleCollider>();
			//200
			playerHitCapsuleColl->CapsuleSet(info, Transform(attackData.capselEndPos, VZero, VOne), Transform(attackData.capselStartPos, VZero, VZero), attackData.playerHitCollRadius, true, handMatrix,nullptr, "_rockGroundAttack");
		}
		else {
			//それ以外なら丸で生成
			info.shape = CollsionInformation::SPHERE;
			if (attackData.playerGroundOneHit) {
				info.oneColl = true;
				info.tag = CollsionInformation::THROW_OBJECT_GROUND_ONE_HIT;
			}
			else if (attackData.playerGroundNoDamageReactionHit) {
				info.oneColl = true;
				info.tag = CollsionInformation::THROW_OBJECT_GROUND_NO_DAMAGE_REACTION;
			}
			else {
				info.oneColl = true;
				info.tag = CollsionInformation::THROW_OBJECT_GROUND;
			}

			playerHitColl = obj->Component()->AddComponent<SphereCollider>();
			//200
			playerHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.playerGroundCollRadius, 1.0f, 1.0f)), nullptr, "_rockGroundAttack");
		}
		
	
	}
	
	obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("justAvoid_rock");
	justAvoidCollider = nullptr;
	//randColl = nullptr;
	//flyCounter = 0.5f;
	if (attackData.capsuleEndPosExtendFromThrowStart) {
		CapsuleColliderPosAddStart();
	}
	
	velocityAdd = false;
	//押し返し判定があるなら押し返し判定をする
	if (attackData.pushCollCan) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::MODEL;
		info.oneColl = false;
		info.tag = CollsionInformation::BOSS_ROCK;
		if (pushColl == nullptr) {
			VECTOR3 size = rockManager->GetPushCollSize(attackData);
			pushColl = obj->Component()->AddComponent<ModelCollider>();
			
			pushColl->ModelColliderSet(info, Transform(VZero, VZero, size), MV1DuplicateModel(ResourceLoad::GetHandle(ID::BOSS_PUSH)), nullptr);
		}
	}

	//はじき返し判定があるなら生成
	if (attackData.playerAttackFlying) {
		meshRenderer2DList = obj->Component()->GetComponents<MeshRenderer2D>();
		flyingUIDraw = true;
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE; 
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_ROCK_PLAYER_ATTACK;
		std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) { PlayerAttackRockFlyAway(_data); };
		if (playerAttackHitColl == nullptr) {
			playerAttackHitColl = obj->Component()->AddComponent<SphereCollider>();
			//200
			playerAttackHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.playerAttackFlyingCollRadius, 1.0f, 1.0f)), func, "bossplayerAttack");
		}
	}

	//突進攻撃時にひるませる判定があるなら生成
	if (attackData.bossRushHit) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_ROCK_RUSH;
		std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) { RockBossHit(_data); };
		if (bossRushHitColl == nullptr) {
			bossRushHitColl = obj->Component()->AddComponent<SphereCollider>();
			bossRushHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.bossRushHitCollRadius, 1.0f, 1.0f)), func,"bossRushAttack");
		}
	}

	rockManager->ShakeCamera();
	
	if (attackData.blastCan) {
		
		if (blastCan) {
			blast = true;
		}
	}

	//予測円があるなら、予測円を削除
	if (attackData.predictionCicleCan) {
		preDraw = false;
		obj->Component()->RemoveComponentWithTagIsCollsion<RayCollider>("uiColl");
		uiColl = nullptr;
	}

	/*if (pushColl == nullptr) {
		pushColl = obj->Component()->AddComponent<ModelCollider>();
		switch (useHandleNumber)
		{
		case 0:
			pushColl->ModelColliderSet(info, Transform(VZero, VZero, VOne * 4.0f), MV1DuplicateModel(ResourceLoad::GetHandle(ID::BOSS_PUSH)));
			break;
		case 1:
			pushColl->ModelColliderSet(info, Transform(VZero, VZero, VOne * 3.0f), MV1DuplicateModel(ResourceLoad::GetHandle(ID::BOSS_PUSH)));
			break;
		case 2:
			pushColl->ModelColliderSet(info, Transform(VZero, VZero, VOne * 10.0f), MV1DuplicateModel(ResourceLoad::GetHandle(ID::BOSS_PUSH)));
			break;
		}
	}*/
	
	//物理クラスがあるなら、移動を完全に静止
	if(physics != nullptr)
		physics->SetVelocity(VZero);
	

	

	
	
	
}


void BossRockBase::PlayerAttackRockFlyAway(const CollsionEventData& _data)
{
	CollsionInfo info;

	//はじき返したオブジェクトがボスに当たる当たり判定
	if (attackData.bossHit) {
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_ROCK_DAMAGE;

		bossHitColl = obj->Component()->AddComponent<SphereCollider>();
		std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) { RockBossHit(_data); };
		bossHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.bossHitCollRadius, 1.0f, 1.0f)),func, "rockBossHit");
	}
	

	physics->SetVelocity(_data.targetTransform->Forward() * attackData.flyingSpeed);
	physics->AddVelocity(VECTOR3(0, attackData.flyingHeight, 0), false);
	//physics->SetGravity(VZero);
	flyCounter = 0.5f;
	EffectManager::GetInstance()->CreateEffekseer(Transform(obj->GetTransform()->position + VECTOR3(0, 0, -100), VZero, VOne * 2.0f), nullptr, Effect_ID::PLAYER_ATTACK_ROCK, 1.0f);
	obj->Component()->GetComponent<Shaker>()->ShakeStart(VOne * 50.0f, Shaker::MIX_SHAKE, true, 0.3f);
	
	playerAttackHit = true;
	velocityAdd = false;
	//当たり判定が操作したら削除
	if (playerHitColl != nullptr) {
		playerHitColl = nullptr;
		obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("_rockAttack");
	}
	if (playerHitCapsuleColl != nullptr) {
		playerHitCapsuleColl = nullptr;
		obj->Component()->RemoveComponentWithTagIsCollsion<CapsuleCollider>("_rockAttack");
	}
	/*obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("bossplayerAttack");
	playerAttackHitColl = nullptr;*/
	flyingUIDraw = false;
	for (auto mesh : meshRenderer2DList) {
		mesh->SetDraw(false);
	}
}

void BossRockBase::RockBossHit(const CollsionEventData& _data)
{
	//ボスが岩に当たった時に削除
	if (attackData.blastCan) {
		SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_BLAST);
		BlastCollsionCreate();
	}
	obj->DestroyMe();
	/*blastColl = obj->Component()->AddComponent<DountCollider>();
	blastJustAvoidColl = obj->Component()->AddComponent<DountCollider>();
	nowBlast = true;
	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::DONUT;
	info.oneColl = false;
	info.tag = CollsionInformation::ROCK_BLAST_DAMAGE;
	blastColl->DountSet(info, Transform(VZero, VZero, VOne * 50.0f), 100.0f);
	info.oneColl = false;
	info.tag = CollsionInformation::JUST_AVOID;
	blastJustAvoidColl->DountSet(info, Transform(VZero, VZero, VOne * 500.0f), 850.0f);*/
	
	/*EffectManager::GetInstance()->CreateEffekseer(Transform(obj->GetTransform()->position , VZero, VOne * 3.0f), nullptr, Effect_ID::ROCK_BLAST, 1.0f);*/
}

void BossRockBase::Start(const EnemyAttackBase::ThrowObjectAttackData& _attack,MATRIX* _handMatrix)
{
	attackData = _attack;
	handMatrix = _handMatrix;
	
	//地面判定があるなら付ける
	if (attackData.randCan) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::RAY;
		info.oneColl = false;
		info.tag = CollsionInformation::BOSS_ROCK_F;
		randColl = obj->Component()->AddComponent<RayCollider>();
		//300，-300
		std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) { Ground(_data); };
		randColl->RaySet(info, Transform(VECTOR3(0, _attack.randCollInfo.rayStartPos, 0), VZero, VOne), Transform(VECTOR3(0, _attack.randCollInfo.rayFinishPos, 0), VZero, VOne),func);
	}
	//投擲物がプレイヤーに当たるなら付ける
	if (attackData.playerHit) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.oneColl = true;
		if (attackData.playerHitNoReaction) {
			info.tag = CollsionInformation::THROW_OBJECT_GROUND_NO_DAMAGE_REACTION;
		}
		else {
			info.tag = CollsionInformation::BOSS_ROCK_ATTACK;
		}
		//カプセルの時はカプセル型の当たり判定の生成
		if (attackData.capsule) {
			info.shape = CollsionInformation::CAPSULE;
			playerHitCapsuleColl = obj->Component()->AddComponent<CapsuleCollider>();
			//200
			playerHitCapsuleColl->CapsuleSet(info, Transform(attackData.capselEndPos,VZero,VOne),Transform(attackData.capselStartPos,VZero,VZero),attackData.playerHitCollRadius,true, handMatrix, nullptr,"_rockGroundAttack");
			info.oneColl = false;
			info.tag = CollsionInformation::JUST_AVOID;
			capsuleEndPos = attackData.capselEndPos;
			if (justAvoidCapselCollider == nullptr) {
				justAvoidCapselCollider = obj->Component()->AddComponent<CapsuleCollider>();
				//450
				justAvoidCapselCollider->CapsuleSet(info, Transform(attackData.capselEndPos, VZero, VOne), Transform(attackData.capselStartPos, VZero, VZero), attackData.playerHitJustAvoidCollRadius, true, handMatrix, nullptr,"justAvoid_rock");
			}
		}
		else {
			//それ以外は丸型
			info.shape = CollsionInformation::SPHERE;
			playerHitColl = obj->Component()->AddComponent<SphereCollider>();
			//200
			playerHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.playerHitCollRadius, 1.0f, 1.0f)),nullptr, "_rockGroundAttack");

			info.oneColl = false;
			info.tag = CollsionInformation::JUST_AVOID;
			if (justAvoidCollider == nullptr) {
				justAvoidCollider = obj->Component()->AddComponent<SphereCollider>();
				//450
				justAvoidCollider->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.playerHitJustAvoidCollRadius, 1.0f, 1.0f)),nullptr, "justAvoid_rock");
			}
		}
	}

	//予測円を出すなら生成
	if (attackData.predictionCicleCan) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::RAY;
		info.oneColl = false;
		info.tag = CollsionInformation::BOSS_ROCK_UI;
		uiColl = obj->Component()->AddComponent<RayCollider>();
		//10000,-10000
		std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) { SetPreInfo(_data); };
		uiColl->RaySet(info, Transform(VECTOR3(0, attackData.predictionCicleColliderInfo.rayStartPos, 0), VZero, VOne), Transform(VECTOR3(0, attackData.predictionCicleColliderInfo.rayFinishPos, 0), VZero, VOne), func, "uiColl");
	}

	preDraw = false;
	preModel = ResourceLoad::GetHandle(ID::BOSS_PRE_MODEL);
	groundInit = false;

	physics = obj->Component()->GetComponent<Physics>();
	fly = false;
	blast = false;
	color = obj->Component()->GetComponent<Color>();

	blinkBaseMax = attackData.blastBlinkMaxCounter;
	blinkCounter = blinkBaseMax;
	ChangeColorMode = false;
	nowBlast = false;
	//落ちる時専用のエフェクトを生成
	if (attackData.throwToFall || attackData.throwToFallToPlayer) {
		SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_FALL);

		EffectManager::GetInstance()->CreateEffekseer(Transform(VZero, VZero, VOne * 2.0f), obj, Effect_ID::ROCK_FALL, 3.0f);
	}
	//腕で投げられる時の位置を取得
	if (attackData.armThrow) {
		throwReady = true;

		throwRockBaseModel = ResourceLoad::GetHandle(ID::B_MODEL);
		//15
		boneNum = attackData.armFrameNum;
		addPos = attackData.armAddPos;
	}
	if (attackData.freeDir) {
		velocityAdd = true;
	}
	groundTime = attackData.randTime;
	start = true;
}

void BossRockBase::DropObject()
{
	if (throwRock) { //投げ終わった後なら落ちない
		return;
	}
	throwReady = false;
	//地面につくかどうかの判定
	if (attackData.randCan) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::RAY;
		info.oneColl = false;
		info.tag = CollsionInformation::BOSS_ROCK_F;
		if (randColl == nullptr) {
			std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) { Ground(_data); };
			randColl = obj->Component()->AddComponent<RayCollider>();
			randColl->RaySet(info, Transform(VECTOR3(0, 300, 0), VZero, VOne), Transform(VECTOR3(0, -300, 0), VZero, VOne), nullptr);
		}
	}
	//はじき返すかの判定
	if (attackData.playerAttackFlying) {
		meshRenderer2DList = obj->Component()->GetComponents<MeshRenderer2D>();
		flyingUIDraw = true;
		
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_ROCK_PLAYER_ATTACK;

		if (playerAttackHitColl == nullptr) {
			playerAttackHitColl = obj->Component()->AddComponent<SphereCollider>();
			std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) { PlayerAttackRockFlyAway(_data); };
			playerAttackHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(250.0f, 1.0f, 1.0f)), func, "bossplayerAttack");
		}
	}
	

	throwRock = true;
	colliderAddStart = false;
	float g = 2000.0f;
	//一定の重力の生成
	if (physics != nullptr) {
		physics->SetGravity(VECTOR3(0, -g, 0));
	}
	
}



void BossRockBase::ThrowRockStart(BaseObject* _player)
{
	if (throwRock) { //一回投げたらそれ以降は入らないようにする
		return;
	}
	dir = obj->GetParent()->GetTransform()->Forward() * -1.0f;
	throwReady = false;
	//地面につくかどうかの判定
	if (attackData.randCan) {
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::RAY;
		info.oneColl = false;
		info.tag = CollsionInformation::BOSS_ROCK_F;
		if (randColl == nullptr) {
			randColl = obj->Component()->AddComponent<RayCollider>();
			std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) { Ground(_data); };
			randColl->RaySet(info, Transform(VECTOR3(0, 300, 0), VZero, VOne), Transform(VECTOR3(0, -300, 0), VZero, VOne), func);
		}
	}
	//はじき返すかどうかの判定
	if (attackData.playerAttackFlying) {
		flyingUIDraw = true;
		meshRenderer2DList = obj->Component()->GetComponents<MeshRenderer2D>();
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.shape = CollsionInformation::SPHERE;
		info.oneColl = true;
		info.tag = CollsionInformation::BOSS_ROCK_PLAYER_ATTACK;
		playerFlyAwayBButtonUI = ResourceLoad::GetHandle(ID::B_BUTTON);
		playerFlyAwayYButtonUI = ResourceLoad::GetHandle(ID::Y_BUTTON);
		if (playerAttackHitColl == nullptr) {
			playerAttackHitColl = obj->Component()->AddComponent<SphereCollider>();
			std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) { PlayerAttackRockFlyAway(_data); };
			playerAttackHitColl->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(250.0f, 1.0f, 1.0f)), func, "bossplayerAttack");
		}
	}


	throwRock = true;
	if (attackData.throwToPlayer) {
		//プレイヤーに向かって飛ぶ
		VECTOR3 start = obj->GetTransform()->position;
		VECTOR3 target = _player->GetTransform()->position;

		VECTOR3 diff = target - start;

		float g = 2000.0f;
		physics->SetGravity(VECTOR3(0, -g, 0));

		VECTOR3 diffXZ = VECTOR3(diff.x, 0, diff.z);
		float distance = diffXZ.Size();

		float height = diff.y;

		float angle = 30.0f * DegToRad;

		float cosA = cosf(angle);
		float sinA = sinf(angle);

		//放物線の方程式にて速度を設定

		float num = g * distance * distance;
		float denominator = 2 * cosA * cosA * (distance * tanf(angle) - height);

		float speed = sqrtf(num / denominator);

		// 速度ベクトル
		VECTOR3 dir = diffXZ.Normalize();

		VECTOR3 velocity;
		velocity.x = dir.x * speed * cosA;
		velocity.z = dir.z * speed * cosA;
		velocity.y = speed * sinA;

		physics->AddVelocity(velocity, false);
	}
	//前に飛ばす処理
	if (attackData.thorwToFront) {
		//拡散方向
		VECTOR3 direction = dir + attackData.diffusionAngle;
		physics->AddVelocity(direction * attackData.throwFirstSpeed, true);
		physics->AddVelocity(VECTOR3(0.0f, attackData.upSpeed, 0.0f), false);

		physics->SetGravity(VECTOR3(0.0f, -attackData.throwFallGravity, 0.0f));
		velocityAdd = true;
	}

}


void BossRockBase::BlastCollsionCreate()
{
	if (blastColl != nullptr) {
		return; //爆発の当たり判定があったらない
	}
	CollsionInfo info;
	blastColl = obj->Component()->AddComponent<DountCollider>();
	blastJustAvoidColl = obj->Component()->AddComponent<DountCollider>();
	nowBlast = true;
	info.parentTransfrom = obj->GetTransform();
	info.shape = CollsionInformation::DONUT;
	info.oneColl = false;
	info.tag = CollsionInformation::ROCK_BLAST_DAMAGE;
	//50,100
	std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) { BlastCollsionEvent(_data); };
	blastColl->DountSet(info, Transform(VZero, VZero, VOne * attackData.blastColliderInfo.inRadius), attackData.blastColliderInfo.outRadius, nullptr);
	info.oneColl = false;
	info.tag = CollsionInformation::JUST_AVOID;
	//300,950
	blastJustAvoidColl->DountSet(info, Transform(VZero, VZero, VOne * attackData.blastColliderInfo.inRadius), attackData.blastColliderInfo.outRadius, nullptr);
	SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_BLAST);
	EffectManager::GetInstance()->CreateEffekseer(Transform(obj->GetTransform()->position, VZero, VOne * 3.0f), nullptr, Effect_ID::ROCK_BLAST, 1.0f);
}

void BossRockBase::AlotCollsionSet()
{
	CollsionInfo info;
	info.parentTransfrom = obj->GetTransform();
	info.oneColl = true;
	if (attackData.playerHitNoReaction) {
		info.tag = CollsionInformation::THROW_OBJECT_GROUND_NO_DAMAGE_REACTION;
	}
	else {
		info.tag = CollsionInformation::BOSS_ROCK_ATTACK;
	}
	if (attackData.capsule) {
		info.shape = CollsionInformation::CAPSULE;
	}
	else {
		info.shape = CollsionInformation::SPHERE;
	}
	//------------------各々の当たり判定を削除してから再生成することで多段ヒットを実現----------------------------------
	if (playerHitCapsuleColl != nullptr) {
		playerHitCapsuleColl = nullptr;
		obj->Component()->RemoveComponentWithTagIsCollsion<CapsuleCollider>("_rockAttack");
		playerHitCapsuleColl = obj->Component()->AddComponent<CapsuleCollider>();
		playerHitCapsuleColl->CapsuleSet(info, Transform(attackData.capselEndPos, VZero, VOne), 
			Transform(attackData.capselStartPos, VZero, VZero), attackData.playerHitCollRadius, 
			true, handMatrix, nullptr, "_rockGroundAttack"
		);
	}
	if (justAvoidCapselCollider != nullptr) {
		justAvoidCapselCollider = nullptr;
		obj->Component()->RemoveComponentWithTagIsCollsion<CapsuleCollider>("justAvoid_rock");
		playerHitCapsuleColl = obj->Component()->AddComponent<CapsuleCollider>();
		playerHitCapsuleColl->CapsuleSet(info, Transform(attackData.capselEndPos, VZero, VOne), 
			Transform(attackData.capselStartPos, VZero, VZero), attackData.playerHitJustAvoidCollRadius, 
			true, handMatrix, nullptr, "_rockGroundAttack"
		);
	}
	if (playerHitColl != nullptr) {
		playerHitColl = nullptr;
		obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("_rockAttack");
		playerHitColl = obj->Component()->AddComponent<SphereCollider>();
		if (attackData.playerHit) {
			playerHitColl->CollsionAdd(info, Transform(VZero, VZero, 
				VECTOR3(attackData.playerHitCollRadius, 1.0f, 1.0f)), nullptr, "_rockGroundAttack");
		}
		if (attackData.playerGroundHit) {
			playerHitColl->CollsionAdd(info, Transform(VZero, VZero, 
				VECTOR3(attackData.playerGroundCollRadius, 1.0f, 1.0f)), nullptr, "_rockGroundAttack");
		}
			
	}
	if (justAvoidCollider != nullptr) {
		justAvoidCollider = nullptr;
		obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("justAvoid_rock");
		justAvoidCollider = obj->Component()->AddComponent<SphereCollider>();
		if (attackData.playerHit) {
			justAvoidCollider->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.playerHitJustAvoidCollRadius, 1.0f, 1.0f))
				, nullptr, "_rockGroundAttack");
		}
		/*if (attackData.playerGroundHit) {
			justAvoidCollider->CollsionAdd(info, Transform(VZero, VZero, VECTOR3(attackData.playerGroundCollRadius, 1.0f, 1.0f)), "_rockGroundAttack");
		}*/

	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
}

void BossRockBase::SetPreInfo(const CollsionEventData& _data)
{
	preTransform = Transform(_data.hitPos, VZero, VECTOR3(5.0f, 0.1f, 5.0f));
	preDraw = true;
}

void BossRockBase::CapsuleColliderPosAddStart()
{
	if (attackData.capsuleEndPosExtend) {
		colliderAddStart = true;
	}
}

void BossRockBase::BlastCollsionEvent(const CollsionEventData& _data)
{
	if (HitObjects(_data.targetObject)) {
		return; //すでに当たってるオブジェクトならリターン
	}
	AddHitObj(_data.targetObject);
}

void BossRockBase::SetPlayerTransform(Transform* _transform)
{
	playerTransform = _transform;
}
