#include "JustAvoidAttackHitCamera.h"
#include "../../Common/Easing.h"
#include "../../Component/Transform/Transform.h"
#include "../cameraInformation.h"
#include "../../Component/Shaker/Shaker.h"
#include "cameraStateManager.h"
#include "../camera.h"

JustAvoidAttackHitCamera::JustAvoidAttackHitCamera()
{
	//id = ID::C_HIT;
	firstRotation	= 0.0f;
	timer			= 0.0f;
}

JustAvoidAttackHitCamera::~JustAvoidAttackHitCamera()
{
}

void JustAvoidAttackHitCamera::Update()
{
	Camera* c = GetBase<Camera>();
	//カメラを回転させている
	if (timer >= 0.0f) {
		float t = 1.0f - timer / MAX_TIMER;
		c->cameraComponent.cameraTransform->rotation.y = Easing::EaseInOut(firstRotation, firstRotation + CHANGE_ROTATION_Y * DegToRad, t);
		//ジャスト回避の時にカメラを近づける処理
		c->currentDistance	= Easing::Sin90Cube(VECTOR3(0.0f,0.0f,-500.0f) , VECTOR3(0.0f, 0.0f, -3000.0f), t);
		timer				-= Time::DeltaTimeRate();
		targetPos			= c->cameraComponent.player.transform->position;
	}
	else {
		targetPos			= c->cameraComponent.player.transform->position;
	}

	c->cameraComponent.camera->Follow();

	c->target				= targetPos;
}

void JustAvoidAttackHitCamera::Draw()
{
}

void JustAvoidAttackHitCamera::Start()
{
	Camera* c = GetBase<Camera>();
	firstRotation	= c->cameraComponent.cameraTransform->rotation.y;
	timer			= MAX_TIMER;
}

void JustAvoidAttackHitCamera::Finish()
{
	Camera* c = GetBase<Camera>();

	c->cameraComponent.cameraTransform->rotation.x = 30.0f * DegToRad;
	c->cameraComponent.cameraTransform->rotation.y -= CHANGE_ROTATION_Y * DegToRad;
}
