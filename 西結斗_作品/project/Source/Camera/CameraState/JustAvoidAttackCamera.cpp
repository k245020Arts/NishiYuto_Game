#include "JustAvoidAttackCamera.h"
#include "../../Common/Easing.h"
#include "../../Component/Transform/Transform.h"
#include "../cameraInformation.h"
#include "../../Component/Shaker/Shaker.h"
#include "cameraStateManager.h"
#include "../camera.h"

JustAvoidAttackCamera::JustAvoidAttackCamera()
{
	string			= Function::GetClassNameC<JustAvoidAttackCamera>();

	//id = ID::C_AVOID_ATTACK;
	firstRotation	= 0.0f;
	distanceTimer	= 0.0f;
	timer			= 0.0f;
}

JustAvoidAttackCamera::~JustAvoidAttackCamera()
{

}

void JustAvoidAttackCamera::Update()
{
	//敵からプレイヤーへの注視点の変更
	Camera* c = GetBase<Camera>();
	if (timer >= 0.0f) {
		float t = 1.0f - timer / MAX_TIMER;
		VECTOR3 easedT = Easing::EaseOut(c->cameraComponent.target.transform->position, c->cameraComponent.player.transform->position, t);
		c->cameraComponent.cameraTransform->rotation.y = Easing::EaseInOut(firstRotation, firstRotation + CHANGE_ROTATION_Y * DegToRad, t);
		targetPos = easedT;
		timer -= Time::DeltaTimeRate();
	}
	else {
		targetPos = c->cameraComponent.player.transform->position;
	}
	//カメラの引く距離の変更
	if (distanceTimer > 0.0f) {
		float t = 1.0f - distanceTimer / DISTANCE_TIMER_MAX;
		VECTOR3 easedT = Easing::EaseOut(c->cameraComponent.target.transform->position, c->cameraComponent.player.transform->position, t);
		c->currentDistance = Easing::SinCube(VECTOR3(0.0f,0.0f,-1500.0f), VECTOR3(0.0f, 0.0f, -2500.0f), t);
		targetPos = easedT;
		distanceTimer -= Time::DeltaTimeRate();
	}
	else {
		c->currentDistance = -1500.0f;
	}
	c->cameraComponent.camera->Follow();

	c->target = targetPos;

}

void JustAvoidAttackCamera::Draw()
{
}

void JustAvoidAttackCamera::Start()
{
	Camera* c = GetBase<Camera>();
	//c->cameraComponent.cameraTransform->rotation.x += 180.0f * DegToRad;
	distanceTimer									= DISTANCE_TIMER_MAX;
	c->cameraComponent.cameraTransform->rotation.x	= 10.0f * DegToRad;
	firstRotation									= c->cameraComponent.cameraTransform->rotation.y;
	timer											= MAX_TIMER;
}

void JustAvoidAttackCamera::Finish()
{
	//c->cameraComponent.cameraTransform->rotation.x -= 180.0f * DegToRad;
	Camera* c = GetBase<Camera>();
	c->cameraComponent.cameraTransform->rotation.x = 30.0f * DegToRad;
	c->cameraComponent.cameraTransform->rotation.y -= CHANGE_ROTATION_Y * DegToRad;
}
