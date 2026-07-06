#include "FreeCamera.h"
#include "../camera.h"
#include "../../Common/InputManager/PadInput.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../Common/Easing.h"
#include <algorithm>
#include "../../GameControler/GameControler.h"

FreeCamera::FreeCamera()
{
	string			= Function::GetClassNameC<FreeCamera>();
	//id = ID::C_FREE;
	backCounter		= 0.0f;
	beforeTarget	= 0.0f;
}

FreeCamera::~FreeCamera()
{
}

void FreeCamera::Update()
{
	Camera* c = GetBase<Camera>();

	const VECTOR3 ADD_POS = VECTOR3(0, 400, 0);
	//滑らかに他のstateから移行した時に移動するようにしている。
	if (backCounter >= 0.0f) {
		float t = 1.0f - backCounter / TIMER_MAX;
		VECTOR3 easedT		= Easing::EaseOut(c->currentDistance, c->defalutDistance, t);
		VECTOR3 targetp = c->cameraComponent.player.transform->position + ADD_POS;
		c->target			= Easing::EaseOut(currentTarget, targetp, t);
		c->currentDistance	= easedT;
		backCounter			-= Time::DeltaTimeRate();
	}
	else {
		c->currentDistance	= c->defalutDistance;
		VECTOR3 targetp		= c->cameraComponent.player.transform->position + ADD_POS;
		c->target			= targetp ;
	}
	//敵にカメラを向くモードにしていなかったらスティック移動を有効にする
	if (c->direction == EnemyAttackChangeCameraDirection::NONE) {
		StickMove();
	}
	else {
		EnemyChangeDir();
	}
	

	//プレイヤーの追従処理
	c->Follow();
	/*if (c->hit) {
		return;
	}*/
	
}

void FreeCamera::Draw()
{
}

void FreeCamera::Start()
{
	Camera* c = GetBase<Camera>();

	backCounter		= TIMER_MAX;
	currentTarget	= c->target;
	beforeTarget	= c->cameraComponent.player.transform->position.y;
	
}

void FreeCamera::Finish()
{
}

void FreeCamera::EnemyChangeDir()
{
	
	Camera* c = GetBase<Camera>();
	
	if (c->CameraRotationMove()) {
		c->direction = EnemyAttackChangeCameraDirection::NONE;
		c->moveTimer = 0.0f;
	}

	
	float rate = 1 - (c->moveTimer / c->MOVE_TIMER_MAX);

	VECTOR3 toEnemy = c->targetEnemyTransform->position -c->cameraComponent.cameraTransform->position;

	float targetYaw = atan2f(toEnemy.x, toEnemy.z);

	float diff = AnglePI(c->cameraComponent.cameraTransform->rotation.y, targetYaw);

	float speed = diff;

	float rotationData = speed * rate;

	float maxStep = c->angleMaxSpeed * DegToRad * Time::DeltaTimeRate();
	rotationData = std::clamp(rotationData, -maxStep, maxStep);

	c->cameraComponent.cameraTransform->rotation.y += rotationData;

	c->moveTimer -= Time::DeltaTimeRate();

	if (c->moveTimer <= 0.0f) {
		c->direction = EnemyAttackChangeCameraDirection::NONE;
	}
}

void FreeCamera::StickMove()
{
	Camera* c = GetBase<Camera>();
	const float DEAD_ZONE = 0.3f;
	const float ROTATE_SPEED = 120.0f;
	//スティック移動
	if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().rightStick.x >= DEAD_ZONE || CheckHitKey(KEY_INPUT_RIGHT)) {
		c->cameraComponent.cameraTransform->rotation.y += ROTATE_SPEED * Time::DeltaTimeRate() * DegToRad;
	}
	if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().rightStick.x <= -DEAD_ZONE || CheckHitKey(KEY_INPUT_LEFT)) {
		c->cameraComponent.cameraTransform->rotation.y -= ROTATE_SPEED * Time::DeltaTimeRate() * DegToRad;
	}
	if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().rightStick.y >= DEAD_ZONE || CheckHitKey(KEY_INPUT_UP)) {
		//カメラのYに進む下限値
		const float CAMERA_LIMIT_ROTATE = 50.0f;
		if (c->cameraComponent.cameraTransform->rotation.x >= -CAMERA_LIMIT_ROTATE * DegToRad) {
			c->cameraComponent.cameraTransform->rotation.x -= ROTATE_SPEED * Time::DeltaTimeRate() * DegToRad;
		}

	}
	if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().rightStick.y <= -DEAD_ZONE || CheckHitKey(KEY_INPUT_DOWN)) {
		//カメラのYに進む上限値
		const float CAMERA_LIMIT_ROTATE = 70.0f;
		if (c->cameraComponent.cameraTransform->rotation.x <= CAMERA_LIMIT_ROTATE * DegToRad) {
			c->cameraComponent.cameraTransform->rotation.x += ROTATE_SPEED * Time::DeltaTimeRate() * DegToRad;
		}
	}
}

float FreeCamera::AnglePI(float _current, float _target)
{
	float diff = _target - _current;

	while (diff > 180.0f * DegToRad) diff -= 2.0f * 180.0f * DegToRad;
	while (diff < -180.0f * DegToRad) diff += 2.0f * 180.0f * DegToRad;

	return diff;
}
