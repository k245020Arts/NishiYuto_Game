#include "CameraBase.h"
#include "../Common/Easing.h"

CameraBase::CameraBase()
{
	cameraComponent = CameraInformation::CameraComponent();;

	reap = 0.1f;
	defalutDistance = VZero;
	currentDistance = VZero;

	fov = 60.0f * DegToRad;
		nearFog = 0.0f;;

	farFog = 0.0f;

	target = VZero;

	diffTarget = VZero;
	debugButton = 0;;
}

CameraBase::~CameraBase()
{
}

void CameraBase::Update()
{
}

void CameraBase::Draw()
{
}

void CameraBase::CameraShake(VECTOR3 _power, Shaker::ShakePattern _pattern, bool _stop, float _second)
{
	//カメラを震わせるときはこれを使う
	cameraComponent.shaker->ShakeStart(_power, _pattern, _stop, _second, cameraComponent.cameraTransform);
}

void CameraBase::CameraShakeStop()
{
	cameraComponent.shaker->ShakeFinish();
}

void CameraBase::CameraRotationSet()
{
	//敵に追従したカメラの角度のセットをしている関数
	VECTOR3 dir;

	dir = target - cameraComponent.cameraTransform->position;

	dir = dir.Normalize();
	float angle = atan2f(dir.x, dir.z);
	cameraComponent.cameraTransform->rotation.y = angle;
	// ======== 2. カメラの向くべき方向 ========
	//VECTOR3 p = VECTOR3(0, 500, -1500);
	//cameraComponent.cameraTransform->position = cameraComponent.player.transform->position + p;
	//VECTOR3 dir = cameraComponent.target.transform->position - cameraComponent.cameraTransform->position;
	//dir = dir.Normalize();
	//VECTOR3 up = VGet(0, 1, 0);

	//MATRIX lookM = MyLookAt(cameraComponent.cameraTransform->position, cameraComponent.cameraTransform->position + dir, up);

	//Quaternion targetQ = MToQ(lookM);

	//// ======== 3. 今の回転へ自然に補間 (SLERP) ========
	//float t = 1.0f - std::pow(0.0001f, Time::DeltaTimeRate() * 6.0f);
	//cameraQuaternion = QSlerp(cameraQuaternion, targetQ, t);

	//// ======== 4. DxLibカメラに反映 ========
	//target = cameraComponent.cameraTransform->position + QApply(cameraQuaternion, VGet(0, 0, 1));  // クォータニオン回転後の前方ベクトル
}

void CameraBase::Follow()
{
	//追従処理、カメラの距離と回転行列を掛けて今の位置を算出
	VECTOR3 offset = currentDistance * cameraComponent.cameraTransform->GetRotationMatrix();
	VECTOR3 desiredCamPos;
	desiredCamPos = cameraComponent.player.transform->position + offset;
	//カメラのポジションをイージングの割合でセットしている
	cameraComponent.cameraTransform->position = Easing::Lerp(cameraComponent.cameraTransform->position, desiredCamPos, reap);
	//そのあとにカメラshakeをかける
	cameraComponent.cameraTransform->position += cameraComponent.shaker->GetShakePower();

	/*if (fabs(beforePos - cameraComponent.cameraTransform->position.y) <= 1.5f) {
		cameraComponent.cameraTransform->position.y = beforePos;
	}

	beforePos = cameraComponent.cameraTransform->position.y;*/
}