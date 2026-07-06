#include "PlayerSpecialAttackCamera.h"
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
#include <algorithm>

PlayerSpecialAttackCamera::PlayerSpecialAttackCamera()
{
	player = nullptr;
	backCounter = 0.0f;

	targetPos = VZero;
	currentTarget = VZero;
	beforeTarget = 0.0f;
}

PlayerSpecialAttackCamera::~PlayerSpecialAttackCamera()
{
}

void PlayerSpecialAttackCamera::Update()
{
	const VECTOR3 DIST = VECTOR3(0.0f, 1000.0f, -500.0f);
	Camera* c = GetBase<Camera>();
	if (backCounter >= 0.0f) {
		float t = 1.0f - backCounter / TIMER_MAX;
		VECTOR3 dist = c->defalutDistance + DIST;
		VECTOR3 easedT = Easing::EaseOut(c->currentDistance, dist, t);
		VECTOR3 targetp = player->GetSpecialAttackCenterPos();;
		c->target = Easing::EaseOut(currentTarget, targetp, t);
		c->currentDistance = easedT;
		backCounter -= Time::DeltaTimeRate();
	}
	else {
		c->currentDistance = c->defalutDistance + DIST;
		VECTOR3 targetp = player->GetSpecialAttackCenterPos();

		/*if (fabs(beforeTarget - c->cameraComponent.player.transform->position.y) <= 1.0f) {
			targetp.y = beforeTarget;
		}*/
		c->target = targetp;
		//beforeTarget = targetp.y;
	}
	VECTOR3 offset = c->currentDistance * c->cameraComponent.cameraTransform->GetRotationMatrix();
	VECTOR3 desiredCamPos;
	desiredCamPos = basePos + offset;
	//カメラのポジションをイージングの割合でセットしている
	c->cameraComponent.cameraTransform->position = Easing::Lerp(c->cameraComponent.cameraTransform->position, desiredCamPos, c->reap);
	c->CameraRotationSet();
}

void PlayerSpecialAttackCamera::Draw()
{
}

void PlayerSpecialAttackCamera::Start()
{
	Camera* c = GetBase<Camera>();
	player = c->cameraComponent.player.obj->Component()->GetComponent<Player>();

	backCounter = TIMER_MAX;
	currentTarget = c->target;
	beforeTarget = c->cameraComponent.player.transform->position.y;
	basePos = player->GetSpecialAttackStartPos();
}

void PlayerSpecialAttackCamera::Finish()
{
}
