#include "PlayerHeavyAttackCamera.h"
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

PlayerHeavyAttackCamera::PlayerHeavyAttackCamera()
{
	player = nullptr;
	backCounter = 0.0f;

	targetPos = VZero;
	currentTarget = VZero;
	beforeTarget = 0.0f;
}

PlayerHeavyAttackCamera::~PlayerHeavyAttackCamera()
{
}

void PlayerHeavyAttackCamera::Update()
{
	Camera* c = GetBase<Camera>();
	const VECTOR3 ADD_POS = VECTOR3(0, 400, 0);
	//強攻撃のチャージをしているとき
	if (player->GetCharge()) {
		//他の処理から帰ってきたときにすぐにカメラの位置が戻らないように補完を掛けている
		if (backCounter >= 0.0f) {

			float t = 1.0f - backCounter / 2.0f;
			VECTOR3 easedT = Easing::Lerp(c->defalutDistance, VECTOR3(0, 0, -1200), t);
			c->currentDistance = easedT;
			backCounter -= Time::DeltaTimeRate();
			VECTOR3 targetp = c->cameraComponent.player.transform->position + ADD_POS;
			c->target = Easing::Lerp(currentTarget, targetp, t);
		}
		else {
			const VECTOR3 ZOOM_POS = VECTOR3(0, 0, -1200);
			c->currentDistance = ZOOM_POS;
			VECTOR3 targetp = c->cameraComponent.player.transform->position + ADD_POS;
			c->target = targetp;
		}
	}
	else {
		//ズームの処理
		if (zoomTimer >= 0.0f) {

			float t = 1.0f - zoomTimer / 0.2f;
			VECTOR3 easedT = Easing::Lerp(c->currentDistance, c->defalutDistance, t);
			c->currentDistance = easedT;
			zoomTimer -= Time::DeltaTimeRate();
			VECTOR3 targetp = c->cameraComponent.player.transform->position + ADD_POS;
			c->target = Easing::Lerp(currentTarget, targetp, t);
		}
		else {
			c->currentDistance = c->defalutDistance;
			VECTOR3 targetp = c->cameraComponent.player.transform->position + ADD_POS;
			c->target = targetp;
		}
		
	}
	
	//追従処理
	c->cameraComponent.camera->Follow();
	c->cameraComponent.camera->CameraRotationSet();
}

void PlayerHeavyAttackCamera::Draw()
{
}

void PlayerHeavyAttackCamera::Start()
{
	Camera* c = GetBase<Camera>();
	player = c->cameraComponent.player.obj->Component()->GetComponent<Player>();

	//backCounter = TIMER_MAX;
	currentTarget = c->target;
	//補完処理の時間
	backCounter = 2.0f;
	zoomTimer = 0.2f;
	//c->cameraComponent.cameraTransform->rotation.y = player->GetPlayerTransform()->rotation.y;
}

void PlayerHeavyAttackCamera::Finish()
{
}
