#include "PlayerSpecialAttack.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/Physics/Physics.h"
#include "../playerStateManager.h"
#include "../../../Common/InputManager/PadInput.h"
#include "../../player.h"
#include "../../../Common/function.h"
#include "../../../Component/MotionBlur/MotionBlur.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Component/Color/Color.h"
#include "../../../Common/InputManager/InputManager.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Component/Collider/SphereCollider.h"
#include "../../../Enemy/EnemyManager.h"
#include "../../../Camera/Camera.h"
#include "../../../Stage/Sky.h"
#include "../../../Common/Easing.h"
#include "../../../Common/Random.h"
#include "../../../Screen.h"
#include "../../../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../../../Component/Animator/Anim2D.h"
#include "../../../Stage/StageManager.h"

PlayerSpecialAttack::PlayerSpecialAttack()
{
	string = Function::GetClassNameC<PlayerSpecialAttack>();
	//id = ID::P_ANIM_JUST_AVOID_ATTACK4;
	animId = ID::P_SPECIAL_ATTACK_ANIM;
	radius = 2000.0f;
	//TODO 当たり判定を動かないように新しい当たり判定の生成
	//playerAttackData.collTrans = Transform(VECTOR3(0, 100, 200), VZero, VECTOR3(radius, 0, 0));
	////frontSpeed = 1000.0f;
	//playerAttackData.hitDamage = 200.0f;
	

	moveNum = 0;
	state = NO_ATTACK;

	waitCounter = -1.0f;
	chargeCounter = -1.0f;

	keepPos = VZero;
	specialAngle = 0.0f;
	angle = 0.0f;
	currentAngle = 0.0f;
	angleMin = 20.0f * DegToRad;
	angleMax = 60.0f * DegToRad;
	lineStart = VZero;
	lineEnd = VZero;

	moveCounter = 0.0f;
	moveSpeed = 15.0f;
	centerTo = false;
	randAngle = 0.0f;
	playerHandle = LoadGraph("data/image/Cutin.png");
	boxHandle = LoadGraph("data/image/visionEffect.png");

	zoomCounterBase = 1.0f;

	zoomCounter = zoomCounterBase;
	zoom = true;
	zoomRate = 0.0f;
	zoomSize = 0.0f;

	attackDamage = false;
	beforeWaitCounter = 0.0f;
	moveStart = false;
	
	/*playerAttackData.state = StateID::PLAYER_SPECIAL_ATTACK_S;*/
}

PlayerSpecialAttack::~PlayerSpecialAttack()
{
	if (playerHandle >= 0) {
		DeleteGraph(playerHandle);
		playerHandle = -1;
	}

	if (boxHandle >= 0) {
		DeleteGraph(boxHandle);
		boxHandle = -1;
	}
	
	
}

void PlayerSpecialAttack::Update()
{
	switch (state)
	{
	case PlayerSpecialAttack::NO_ATTACK:
		return;
		break;
	case PlayerSpecialAttack::BEFORE:
		BeforeUpdate();
		break;
	case PlayerSpecialAttack::GROUND_ATTACK:
		GroundUpdate();
		break;
	case PlayerSpecialAttack::CHARGE:
		ChargeUpdate();
		break;
	case PlayerSpecialAttack::FINAL_ATTACK:
		FinalAttackUpdate();
		break;
	default:
		break;
	}
	
}

void PlayerSpecialAttack::Draw()
{
	Player* p = GetBase<Player>();
	//DrawSphere3D(p->specialAttackCenterPos, 10, 1, 0xffffff, 0xffffff, true);
	int alpha = p->playerCom.meshRenderer2D->GetAlpha();
	int add = alpha;
	if (add >= 60) {
		add = 60;
	}
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	//DrawRotaGraph(Screen::WIDTH / 2.0f, Screen::HEIGHT / 2.0f, zoomRate, 0.0f * DegToRad, playerHandle, true);
	DrawRectRotaGraph((int)Screen::WIDTH / 2, (int)Screen::HEIGHT / 2, 0, (int)zoomSize / 2, (int)Screen::WIDTH, (int)(200.0f - zoomSize), (double)zoomRate, 0.0 * DegToRad, playerHandle, true);
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void PlayerSpecialAttack::Start()
{
	Player* p = GetBase<Player>();
	PlayerAttackStateBase::Start();
	p->playerCom.anim->Play(ID::P_ANIM_IDOL);
	firstColl = true;
	
	//p->playerCom.camera->CutSceneChangeState("playerSpecialAttackBefore", CutSceneSpece::ALL_ENEMY);
	//AttackCollsion();

	const int ROTATION_CHANGE_LOOP = 8;
	int max = 0;
	float finalAngle = p->playerTransform->rotation.y;

	//------------------プレイヤーの視野に何人いるかを360に分けて判定、一番群れがいる角度に向かって必殺技を打つ---------

	for (int i = 0; i < ROTATION_CHANGE_LOOP; i++) {
		int n = p->playerCom.enemyManager->PlayerFovEnemyNum(obj->GetTransform(), 45.0f);
		if (max <= n) {
			max = n;
			finalAngle = p->playerTransform->rotation.y;
		}
		p->playerTransform->rotation.y += 45.0f * DegToRad;
	}

	p->playerTransform->rotation.y = finalAngle;
	Transform nearPos = p->playerCom.enemyManager->NearFovEnemyPos(*p->playerTransform,45.0f * DegToRad);

	VECTOR3 nearEnemyDir = nearPos.position - p->playerTransform->position;

	p->playerTransform->rotation.y = atan2(nearEnemyDir.x, nearEnemyDir.z);

	defalutRotation = p->playerTransform->rotation;

	//-----------------------------------------------------------------------------------------------------------

	state = BEFORE;
	collsionCreate = true;
	p->hitObjects.clear();
	p->playerCom.anim->SetPlaySpeed(1.0f);

	waitCounter = -1.0f;
	chargeCounter = -1.0f;
	p->noDamage = true;

	p->playerCom.meshRenderer2D->FeedInDraw(0.5f);
	

	keepPos = p->playerTransform->position;

	//p->playerTransform->position = CUT_SCENE_POS;
	specialAngle = 0.0f;
	angle = 0.0f;
	currentAngle = 0.0f;

	//線の開始から終了地点の取得
	VECTOR3 dir = VECTOR3(cosf(currentAngle),0.0f,sinf(currentAngle));

	lineStart = p->specialAttackCenterPos + dir * radius;
	lineEnd = p->specialAttackCenterPos - dir * radius;
	moveCounter = 0.0f;
	centerTo = true;

	zoomCounterBase = 1.0f;
	p->playerCom.anim2D->AnimPlay();
	p->playerCom.anim2D->AnimReset();
	zoomCounter = zoomCounterBase;
	zoom = true;
	zoomRate = 1.0f;
	zoomSize = 0.0f;
	p->playerCom.camera->SleepTargetSet(CutSceneSpece::ALL_ENEMY, true);
	SoundManager::GetInstance()->PlaySe(Sound_ID::CUTIN_START);
	SoundManager::GetInstance()->PlaySe(Sound_ID::PLAYER_SPECIAL_ATTACK_V);
	attackDamage = false;
	moveStart = false;
	playerAttackData.hitDamage = 200.0f;
	p->specialAttackGuageMax = false;
}

void PlayerSpecialAttack::Finish()
{
	Player* p = GetBase<Player>();
	ColliderBase* collider = p->obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("special");
	//p->playerCom.camera->ChangeStateCamera(StateID::FREE_CAMERA_S);
	EffectManager::GetInstance()->StopEffekseer(Effect_ID::PLAYER_SPECIAL_PLACE);
	EffectManager::GetInstance()->StopEffekseer(Effect_ID::PLAYER_SPECIAL_SLASH);
	state = NO_ATTACK;
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->noDamage = false;
	p->obj->Component()->GetComponent<SphereCollider>()->CollsionRespown();
	p->playerCom.meshRenderer2D->FeedOutDraw(0.5f);
}

void PlayerSpecialAttack::SpecialRotationChange()
{
	/*Player* p = GetBase<Player>();
	if (goingToCenter)
	{
		dashDir =(p->specialAttackCenterPos - p->playerTransform->position);
		dashDir = dashDir.Normalize();
	}
	else
	{
		currentAngle += angleStep;

		VECTOR3 dir(cosf(currentAngle),0.0f,sinf(currentAngle));

		dashDir = dir.Normalize();
	}*/
}

void PlayerSpecialAttack::MoveStart(float _angle)
{
	Player* p = GetBase<Player>();
	ColliderBase* collider = p->obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("special");
	AddCollsion();
	SoundManager::GetInstance()->RandamSe("swordWind", 5);
	
	p->playerTransform->position = p->specialAttackCenterPos + VECTOR3(0.0f,0.0f,radius) * MGetRotY(p->playerTransform->rotation.y);

	float dist = VECTOR3(p->specialAttackCenterPos - p->playerTransform->position).Size();
	p->playerCom.physics->SetVelocity(VZero);

	/*VECTOR3 forward = p->playerTransform->Forward() * MGetRotY(p->playerTransform->rotation.y);
	p->playerCom.physics->AddVelocity(forward * 50000.0f, true);*/

	//VECTOR3 forward = p->playerTransform->Forward() * MGetRotY(p->playerTransform->rotation.y);
	//p->playerCom.physics->SetVelocity(forward * 30000.0f);
}

void PlayerSpecialAttack::AddCollsion()
{
	Player* p = GetBase<Player>();
	ColliderBase* collider = p->obj->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom = nullptr;
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::P_SPECIAL_ATTACK;
	info.size = 1.0f;
	std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) {dynamic_cast<Player*>(com)->CollsionAttackEvent(_data); };
	collider->CollsionAdd(info, Transform(p->specialAttackCenterPos, VZero, VECTOR3(radius, 0, 0)), func,"special");
	p->playerCom.enemyManager->CanPlayerSpecialHit();
	p->hitObjects.clear();
}

void PlayerSpecialAttack::StateImguiDraw()
{
	ImGui::Text("attackNum = %d", moveNum);
}

void PlayerSpecialAttack::BeforeUpdate()
{
	Player* p = GetBase<Player>();
	int alpha = p->playerCom.meshRenderer2D->GetAlpha();
	if (alpha >= 255) {
		if (zoom) { //ズームをする
			zoomRate = Easing::EasingFlow<float>(&zoomCounter, zoomCounterBase, 2.0f, 1.0f, Easing::EaseIn<float>);
			zoomSize = Easing::EasingFlow<float>(&zoomCounter, zoomCounterBase, 120.0f, 0.0f, Easing::EaseIn<float>);
			if (zoomCounter <= 0.0f) {
				beforeWaitCounter = 1.0f;
				zoom = false;
				EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(Screen::WIDTH / 2.0f - 100.0f, Screen::HEIGHT / 2.0f, 0.0f), VZero, VOne), nullptr, Effect_ID::PLAYER_SPECIAL_FLASH, 1.0f, false);
				EffectManager::GetInstance()->SetSpeedEffekseer(Effect_ID::PLAYER_SPECIAL_FLASH, 2.0f);
				
			}
		}
		if (beforeWaitCounter > 0.0f) {
			beforeWaitCounter -= Time::DeltaTimeRate();
			if (beforeWaitCounter <= 0.0f) { //攻撃の開始
				state = GROUND_ATTACK;
				p->specialAttackStartPos = p->playerTransform->position;
				VECTOR3 forward = p->playerTransform->Forward();
				p->specialAttackCenterPos = p->specialAttackStartPos + forward * radius;
				MoveStart(0.0f);
				moveNum = 20;
				moveStart = true;
				p->playerCom.camera->CutSceneChangeState("PlayerSpecialAttack", false);
				p->playerCom.meshRenderer2D->FeedOutDraw(0.5f);
				p->playerCom.camera->SleepTargetSet(CutSceneSpece::ALL_ENEMY, false);
			}
		}
	}
	
}
#define MODE_1
void PlayerSpecialAttack::GroundUpdate()
{
#ifdef MODE_1
	Player* p = GetBase<Player>();
	if (moveStart) {
		obj->SetObjectTimeRate(1.0f);
		moveStart = false;
		return;
	}
	float dt = Time::DeltaTimeRate();

	p->playerCom.player->DrawTrail(VECTOR3(0, 0, 100),VECTOR3(0, 0, -350),250.0f, 235.0f,0.0f, 150.0f,28, 0.4f);

	moveCounter += moveSpeed * dt;

	// 線形補間で位置を決定
	p->playerTransform->position =
		Easing::EaseInOut(lineStart, lineEnd, moveCounter);

	// 向き直線方向
	VECTOR3 dir = lineEnd - lineStart;
	dir = dir.Normalize();
	float yaw = atan2f(dir.z, dir.x);
	p->playerTransform->rotation.y = yaw;

	if (moveCounter >= 1.0f)
	{
		centerTo = !centerTo;
		if (centerTo) {
			//下でセットした最初の位置についたので中央に向かう
			currentAngle += randAngle;

			VECTOR3 outward(cosf(currentAngle), 0.0f, sinf(currentAngle));
			
			lineStart = p->specialAttackCenterPos + outward * radius;
			lineEnd = p->specialAttackCenterPos - outward * radius;
		}
		else {
			//中央に向かわずに次の角度の最初の位置に移動する
			randAngle = Random::GetFloat(angleMin, angleMax);
			float nextAngle = currentAngle + randAngle;
			//次の角度に向かって開始か終了か取得
			VECTOR3 outward(cosf(nextAngle), 0.0f, sinf(nextAngle));
			lineStart = lineEnd;
			lineEnd = p->specialAttackCenterPos + outward * radius;
		}
		moveNum--;
		moveCounter = 0.0f;
		ColliderBase* collider = p->obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("special");
		AddCollsion();
		SoundManager::GetInstance()->RandamSe("swordWind", 5);
		SoundManager::GetInstance()->RandamSe("PlayerSpecialAttackAttackV", 2);
	}

	
	if (!p->playerCom.camera->IsCutScene())
	{
		// 次フェーズへ
		p->playerTransform->position = p->specialAttackStartPos;
		VECTOR3 specialCenterPos = p->specialAttackCenterPos;
		//壁近くならプレイヤーが壁にめり込まないように位置を補完
		if (fabs(specialCenterPos.z) >= WALL_EDGE_POS - 5000.0f) {
			if (specialCenterPos.z > 0.0f) {
				p->playerTransform->position.z -= radius * 2.0f;
				p->specialAttackCenterPos.z -= radius * 2.0f;
			}
			else {
				p->playerTransform->position.z += radius * 2.0f;
				p->specialAttackCenterPos.z += radius * 2.0f;
			}
		}
		else if (fabs(specialCenterPos.x) >= WALL_EDGE_POS - 5000.0f) {
			if (specialCenterPos.x > 0.0f) {
				p->playerTransform->position.x -= radius * 2.0f;
				p->specialAttackCenterPos.x -= radius * 2.0f;
			}
			else {
				p->playerTransform->position.x += radius;
				p->specialAttackCenterPos.x += radius;
			}
		}
			
		
		//p->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
		ColliderBase* collider = p->obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("special");
		float angle = 36.0f * DegToRad;
		p->playerTransform->rotation = defalutRotation;
		p->playerCom.physics->SetVelocity(VZero);
		state = CHARGE;
		chargeCounter = 1.5f;
		AddCollsion();
		p->playerCom.anim->Play(animId);
		p->playerCom.shaker->ShakeStart(VOne * 5.0f, Shaker::MIX_SHAKE, false, -1);
		p->playerCom.camera->CutSceneChangeState("playerSpecialCut",false);
		p->obj->Component()->GetComponent<SphereCollider>()->CollsionFinish();
		//ToDO 必殺技のチャージのエフェクトの実装
		//p->playerCom.effect->CreateEffekseer(Transform(VECTOR3(-100, 100, 100), VZero, VOne * 0.5f), p->obj, Effect_ID::PLAYER_SPECIAL_CHARGE, 2.0f);
	}

	

#else
	Player* p = GetBase<Player>();
	p->playerCom.player->DrawTrail(VECTOR3(0, 0, 100), VECTOR3(0, 0, -350), 0.0f, 0.0f, 255.0f, 150.0f, 28, 0.8f);
	if (p->playerCom.keyboard->GetIsKeyboardPut(KEY_INPUT_3)) {
		p->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
	}

	float  distance = VECTOR3(p->specialAttackCenterPos - p->playerTransform->position).Size();
	VECTOR3 forward = p->playerTransform->Forward() * MGetRotY(p->playerTransform->rotation.y);
	p->playerCom.physics->AddVelocity(forward * 50000.0f, true);
	p->playerCom.blur->MosionStart(0.5f, 0.000001f, animId, 0);
	if (distance > radius + 200.0f) {
		if (moveNum > 0) {
			float angle = 0.0f;
			//if (moveNum % 2 == 0) {
				angle = 45.0f * DegToRad;
			//}
			//else {
				//angle = -90.0f * DegToRad;
			//}
			VECTOR3 norm = forward * radius;
			//p->playerTransform->position = norm;
			MoveStart(angle * DegToRad);
			p->playerTransform->rotation.y += angle;
			
			moveNum--;
		}
		else {
			p->playerTransform->position = p->specialAttackStartPos;
			//p->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
			ColliderBase* collider = p->obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>("special");
			float angle = 36.0f * DegToRad;
			p->playerTransform->rotation = defalutRotation;
			p->playerCom.physics->SetVelocity(VZero);
			state = CHARGE;
			chargeCounter = 1.5f;
			AddCollsion();
			p->playerCom.anim->Play(animId);
			p->playerCom.shaker->ShakeStart(VOne * 5.0f, Shaker::MIX_SHAKE, false, -1);
			p->playerCom.camera->CutSceneChangeState("playerSpecialCut");
			p->obj->Component()->GetComponent<SphereCollider>()->CollsionFinish();
		}
	}
#endif // DEBUG
}

void PlayerSpecialAttack::ChargeUpdate()
{
	Player* p = GetBase<Player>();
	chargeCounter -= Time::DeltaTimeRate();
	Debug::DebugOutPutPrintf("charge : velocity.x = %.1f : velocity.y = %.1f : velocity.z = %.1f", p->playerCom.physics->GetVelocity().x, p->playerCom.physics->GetVelocity().y, p->playerCom.physics->GetVelocity().z);
	Debug::DebugOutPutPrintf("charge : position.x = %.1f : position.y = %.1f : position.z = %.1f", p->playerCom.player->playerTransform->position.x, p->playerCom.player->playerTransform->position.y, p->playerCom.player->playerTransform->position.z);
	p->playerCom.physics->SetVelocity(VZero);
	if (chargeCounter <= 0.0f) {  //最後の攻撃の開始
		p->playerTransform->position = p->specialAttackCenterPos + VECTOR3(0.0f, 0.0f, radius) * MGetRotY(p->playerTransform->rotation.y);
		MoveStart(0.0f);
		chargeCounter = 0.0f;
		state = FINAL_ATTACK;
		//p->playerCom.anim->SetPlaySpeed(3.0f);
		p->playerCom.shaker->ShakeFinish();
		SoundManager::GetInstance()->PlaySe(Sound_ID::PLAYER_SPECIAL_ATTACK_FINAL_ATTACKV);
		playerAttackData.hitDamage = 5000.0f;
	}
}

void PlayerSpecialAttack::FinalAttackUpdate()
{
	Player* p = GetBase<Player>();
	
	if (waitCounter >= 0.0f) { //攻撃後の余韻が終わったら終了
		waitCounter -= Time::DeltaTimeRate();
		if (!p->playerCom.camera->IsCutScene()) {
			p->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
		}
		return;
	}
	p->playerCom.player->DrawTrail(VECTOR3(0, 0, 100), VECTOR3(0, 0, -350), 0.0f, 0.0f, 255.0f, 150.0f, 28, 0.8f);
	float  distance = VECTOR3(p->specialAttackCenterPos - p->playerTransform->position).Size();
	VECTOR3 forward = p->playerTransform->Forward();
	p->playerCom.physics->AddVelocity(forward * 15000.0f, true);
	if (distance > radius) { //半径より外側に出たら、攻撃後の余韻に以降
		waitCounter = 10.0f;
		p->playerCom.physics->SetVelocity(VZero);
		EffectManager::GetInstance()->CreateEffekseer(Transform(p->specialAttackCenterPos + VECTOR3(0.0f,150.0f,0.0f), VECTOR3(0.0f, 0.0f, 180.0f * DegToRad), VOne * 8.0f), nullptr, Effect_ID::PLAYER_SPECIAL_FINAL, 1.5f);
		attackDamage = true;
		SoundManager::GetInstance()->PlaySe(Sound_ID::PLAYER_SPECIAL_ATTACK_BOM);
	}
}