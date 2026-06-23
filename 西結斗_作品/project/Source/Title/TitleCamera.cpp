#include "TitleCamera.h"
#include "../Component/Transform/Transform.h"
#include "../../ImGui/imgui.h"
#include "../Common/InputManager/PadInput.h"
#include <cmath>
#include "../Component/ComponentManager.h"
#include "../Component/Shaker/Shaker.h"
#include "../Common/Easing.h"
#include "TitleScene.h"
#include "TitleControl.h"

TitleCamera::TitleCamera()
{
	reap = 0.0f;
	reap = CameraInformation::DEFALUT_RAPE;

	defalutDistance = VECTOR3(0.0f, 0.0f, -800.0f); // -800
	currentDistance = defalutDistance;
	
	debugId = 33;
	tag = Function::GetClassNameC<TitleCamera>();

	fov = 60.0f * DegToRad;

	debugButton = 0;
	cameraComponent.cameraTransform = nullptr;

	cameraNear = 10.0f;
	cameraFar = 5000000.0f;

	diffTarget = VZero;
	target = VZero;
	cameraComponent.cameraTransform = nullptr;
	rockOn = false;
	counter = 0;

	title = nullptr;
	titleCtrl = nullptr;

}

TitleCamera::~TitleCamera()
{
	delete cameraComponent.cameraTransform;
	cameraComponent.cameraTransform = nullptr;
}

void TitleCamera::Update()
{
	target = cameraComponent.player.transform->position;
	CameraRotationSet();
	Follow();
	
	SetCameraNearFar(cameraNear, cameraFar);
	SetupCamera_Perspective(fov);

	debugButton == 1 ? Update_Debug() : Update_Normal();
}

void TitleCamera::Update_Normal()
{
	const VECTOR3 POS_0 = VECTOR3(420, 190, 550); // 初期位置
	const VECTOR3 POS_1 = VECTOR3(500, 300, 1500); // 1つ目のターゲットポジション
	const VECTOR3 POS_2 = VECTOR3(0, 375, 1800); // 2つ目のターゲットポジション 1750

	const float RATIO = 0.2f; // 1つ目から2つ目のポジションに移動するまでの時間の比率(0～1)

	titleCtrl = FindGameObject<TitleControl>();

	float lerpProgress = titleCtrl->GetNowProgress();

	VECTOR3 cameraPos = VZero;
	if (lerpProgress < RATIO)
	{
		// 1つ目のポジションに移動
		cameraPos.x = Easing::Lerp(POS_0.x, POS_1.x, lerpProgress / RATIO);
		cameraPos.y = Easing::Lerp(POS_0.y, POS_1.y, lerpProgress / RATIO);
		cameraPos.z = Easing::Lerp(POS_0.z, POS_1.z, lerpProgress / RATIO);
	}
	else
	{
		// 2つ目のポジションに移動
		cameraPos.x = Easing::EaseOut(POS_1.x, POS_2.x, (lerpProgress - RATIO) / (1 - RATIO));
		cameraPos.y = Easing::EaseOut(POS_1.y, POS_2.y, (lerpProgress - RATIO) / (1 - RATIO));
		cameraPos.z = Easing::EaseOut(POS_1.z, POS_2.z, (lerpProgress - RATIO) / (1 - RATIO));
	}
	SetCameraPositionAndTarget_UpVecY(cameraPos, VECTOR3(0, 330, 0)/*target + VECTOR3(0, 300, 0)*/);
}

void TitleCamera::Update_Debug()
{
	Transform transform = *obj->GetTransform();
	//SetCameraPositionAndTarget_UpVecY(transform.position, diffTarget);
}

void TitleCamera::Draw()
{

}

void TitleCamera::ImguiDraw()
{
	//使用するTransformが違う
	//normalCameraの時はcameraComponent.CameraTransformを使用
	//DebugCameraの時は通常のTransformの使用
	ImGui::RadioButton("normalCamera", &debugButton, 0);
	ImGui::RadioButton("DebugCamera", &debugButton, 1);
	if (cameraComponent.cameraTransform != nullptr) {
		ImGui::DragFloat3("cameraPosition", &cameraComponent.cameraTransform->position.x, 1.0f, 0.0f, 5000.0f);
		ImGui::DragFloat3("cameraRotation", &cameraComponent.cameraTransform->rotation.x, 1.0f, 0.0f, 5000.0f);
	}
	
	ImGui::DragFloat3("target", &target.x, 1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("DebugTarget", &diffTarget.x, 1.0f, -100000.0f, 10000.0f);
	ImGui::DragFloat("CameraNear", &cameraNear, 1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat("CameraFar", &cameraFar, 1.0f, -100000.0f, 10000.0f);
}

//--------------------
//　ここから先は...
//--------------------

void TitleCamera::Start(BaseObject* _eObj)
{
	SetCameraNearFar(cameraNear, cameraFar);
	SetupCamera_Perspective(fov);
	SetCameraPositionAndTarget_UpVecY(VECTOR3(0, 0, -1500), VZero);
}

void TitleCamera::PlayerSet(BaseObject* _pObj)
{
	if (_pObj != nullptr) {
		cameraComponent.player.obj = _pObj;
		cameraComponent.player.transform = _pObj->GetTransform();

		cameraComponent.player.shaker = _pObj->Component()->GetComponent<Shaker>();

		cameraComponent.target.shaker = _pObj->Component()->GetComponent<Shaker>();
		//playerに追従させたいのでカメラのトランスフォームはプレイヤーを軸にする。
		cameraComponent.cameraTransform = new Transform(*cameraComponent.player.transform);
		cameraComponent.cameraTransform->rotation.x = 30.0f * DegToRad;
	}
	
	cameraComponent.shaker = obj->Component()->AddComponent<Shaker>();

	//タイトルシーンの情報はここからとれる。（ボタンを押したときなどの）
	title = GetScene<TitleScene>();

}