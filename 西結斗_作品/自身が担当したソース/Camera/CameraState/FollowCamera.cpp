#include "FollowCamera.h"
#include "../../Common/Easing.h"
#include "../../Component/Transform/Transform.h"
#include "../CameraInformation.h"
#include "./../../Component/Shaker/Shaker.h"
#include "../../Component/ComponentManager.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerState/PlayerStateManager.h"
#include "../../Component/Object/Object3D.h"
#include "../Camera.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../Enemy/EnemyManager.h"

FollowCamera::FollowCamera()
{
	enemyShake	= false;
	string		= Function::GetClassNameC<FollowCamera>();
	backCounter = 0.0f;
	addPos		= VZero;
	player = nullptr;
	playerShake = false;
	changeState = false;
}

FollowCamera::~FollowCamera()
{

}

void FollowCamera::Update()
{
	Camera* c = GetBase<Camera>();
	const VECTOR3 ADD_POS = VECTOR3(0, 300, 0);
	//他の処理から帰ってきたときにすぐにカメラの位置が戻らないように補完を掛けている
	if (backCounter >= 0.0f) {

		float t				= 1.0f - backCounter / TIMER_MAX;
		VECTOR3 easedT		= Easing::EaseOut(c->currentDistance, c->defalutDistance, t);
		VECTOR3 target		= SetTarget();
		addPos				= Easing::EaseOut(VECTOR3(VZero), ADD_POS, t);
		c->target			= Easing::EaseOut(currentTarget,target,t) + addPos;
		c->currentDistance	= easedT;
		backCounter			-= Time::DeltaTimeRate();
		
	}
	else {
		c->currentDistance	= c->defalutDistance;
		addPos = ADD_POS;
		VECTOR3 target = SetTarget();
		c->target			= target + addPos;
	}
	//追従処理
	c->cameraComponent.camera->Follow();
	//c->TargetEnemySet();
	/*if (p->GetPlayerStateManager()->GetState<PlayerStateBase>()->GetString() == "PlayerJustAvoid") {
		*c->cameraComponent.target = c->cameraComponent.enemy.transform->position + c->cameraComponent.player.transform->position * 0.5f;
	}
	else {*/

	//カメラの回転
	c->cameraComponent.camera->CameraRotationSet();
	StickDirections stickDir = InputManager::GetInstance()->GetControllerInput()->GetStickKnockingPut(0.5f).rightStick;
	//ロックオンカメラのターゲットの変更
	if (stickDir == S_RIGHT) {
		c->cameraComponent.enemyManager->ChangeCameraRockOn(c->cameraComponent.camera,true);
		Start();
	}
	else if (stickDir == S_LEFT) {
		c->cameraComponent.enemyManager->ChangeCameraRockOn(c->cameraComponent.camera,false);
		Start();
	}
}

void FollowCamera::Draw()
{

}

void FollowCamera::Start()
{
	Camera* c = GetBase<Camera>();
	player											= c->cameraComponent.player.obj->Component()->GetComponent<Player>();

	backCounter										= TIMER_MAX;
	currentTarget									= c->target;
	c->cameraComponent.cameraTransform->rotation.x	= 30.0f * DegToRad;
	addPos											= VZero;
	changeState = false;
}

void FollowCamera::Finish()
{
	
}

VECTOR3 FollowCamera::SetTarget()
{
	Camera* c = GetBase<Camera>();
	if (c->cameraComponent.target.transform == nullptr) { //敵の情報がnullなら追従カメラから変える
		changeState = true;
		return VZero;
	}
	VECTOR3 enemyPos = c->cameraComponent.target.transform->position;
	VECTOR3 playerPos = c->cameraComponent.player.transform->position + (c->cameraComponent.player.transform->Forward() * 30.0f);
	//playerとenemyの間の点を注視点にする.
	VECTOR3 target = (enemyPos + playerPos) / 2.0f;

	return target;
}
