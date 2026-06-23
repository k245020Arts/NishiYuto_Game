#include "camera.h"
#include "../Component/Transform/Transform.h"
#include "../../ImGui/imgui.h"
#include "../Common/InputManager/PadInput.h"
#include <cmath>
#include "../Component/ComponentManager.h"
#include "../Component/Shaker/Shaker.h"
#include "../Common/Easing.h"
#include "../Camera/CameraState/CameraStateManager.h"
#include "../Camera/CameraState/FollowCamera.h"
#include "../Camera/CameraState/JustAvoidCamera.h"
#include "../Camera/CameraState/JustAvoidAttackCamera.h"
#include "../Camera/CameraState/JustAvoidAttackHitCamera.h"
#include "../Camera/CameraState/CutSceneCamera.h"
#include "../Common/InputManager/InputManager.h"
#include "../Camera/CameraState/FreeCamera.h"
#include "../Enemy/EnemyManager.h"
#include "../Component/Hierarchy/Hierarchy.h"
#include "CameraEditorGui.h"
#include "CameraState/PlayerSpecialAttackCamera.h"
#include "CutSceneBox.h"
#include "../Component/UI/UIManager/UIManager.h"
#include "CameraState/PlayerHeavyAttackCamera.h"
#include "../Common/Random.h"
#include "../GameControler/GameControler.h"
#include "../Camera/CameraState/RangedEnemyCamera.h"
#include "../Camera/CameraState/RangedEnemyFinishCamera.h"

Camera::Camera()
{
	cameraComponent.target.transform	= nullptr;
	cameraComponent.player.transform	= nullptr;
	cameraComponent.cameraTransform		= nullptr;
	timeTest							= 1.0f;
	normalCamera						= true;
	targetChangeButton					= 1;
	reap								= 0.0f;
	reap								= CameraInformation::DEFALUT_RAPE;

	defalutDistance						= VECTOR3(0.0f,0.0f, -1500.0f);
	currentDistance						= defalutDistance;
	debugButton							= 0;
	debugId								= 12;
	tag									= Function::GetClassNameC<Camera>();
	fov									= 60.0f * DegToRad;

	rockOn								= false;
	beforePos							= 0.0f;
	nearFog								= 10000.0f;
	farFog								= 2300000.0f;
	direction							= EnemyAttackChangeCameraDirection::NONE;
	moveTimer							= 0.0f;
	angleMaxSpeed						= 0.0f;
	counter								= 0.0f;
	editor								= nullptr;
	hit									= false;
	hitDist								= 0.0f;
	moveAmout							= 0.0f;
	rockOn								= false;
	targetEnemyTransform				= nullptr;
	diffTarget							= VZero;
	cutSceneIndex						= 0;
	cutSceneBox							= new CutSceneBox();
	cutSceneBoxDraw						= false;
	uiManager							= FindGameObject<UIManager>();
	keepFov								= fov;
	cutStopChara						= false;
	isCutScene							= false;
	shakePower							= 0.0f;
	shakeTime							= 0.0f;
	rokPos								= 0.0f;
}

Camera::~Camera()
{
	SafeDelete<Transform>(cameraComponent.cameraTransform);
	SafeDelete<CameraEditorGui>(editor);

	//delete cameraComponent.state;
}

void Camera::Update()
{	

	cameraComponent.enemyManager->NearEnemyAlpha(cameraComponent.cameraTransform->position);
	
	
	//if (hit) {
	//	hit = false;
	//	//cameraComponent.cameraTransform->position.y += moveAmout;
	//}
	//else {
	//	counter = 0.0f;
	//}
	if (InputManager::GetInstance()->KeyInputDown("camera")) {
		if (!IsCutScene()) {
			if (!rockOn) {
				bool result = cameraComponent.enemyManager->CameraRockOnStart(this);
				if (result) {
					cameraComponent.state->NowChangeState(StateID::FOLLOW_CAMERA_S);
					rockOn = true;
				}
			}
			else {
				cameraComponent.enemyManager->TargetCancel(this);
				cameraComponent.state->NowChangeState(StateID::FREE_CAMERA_S);
				rockOn = false;
			}
		}
	}
	if (shakeTime > 0.0f) {
		shakeTime -= Time::DeltaTimeRate();
		fov = Random::GetFloat(-shakePower, shakePower) + keepFov;
	}
	else {
		fov = keepFov;
	}
}

void Camera::Draw()
{
	//カメラがフェード中も止まってしまうのが嫌だったので一時的にdrawのところに逃がしている
	if (rockOn) {
		//cameraComponent.state->Update();
	}
	else {
		
	}
	if (CheckHitKey(KEY_INPUT_Q)) {
		cameraComponent.enemyManager->ChangeCameraRockOn(this, false, true,true);
	}
	else {
		cameraComponent.enemyManager->ChangeCameraRockOn(this, true, true,true);
	}

	//Dxlibのカメラの設定(SetDrawScreenを使うと初期化されるため毎フレーム呼ぶ)。
	const float NEAR_CLIP = 10.0f;
	const float FAR_CLIP = 5000000.0f;
	SetCameraNearFar(NEAR_CLIP, FAR_CLIP);
	SetFogEnable(true);
	SetFogStartEnd(nearFog, farFog);
	const int R = 137;
	const int G = 189;
	const int B = 222;
	SetFogColor(R, G, B);
	//SetFogColor(255, 255, 255);
	SetupCamera_Perspective(fov);

	if (debugButton == 2) { //デバックウィンドウでこのモードのラジオボタンを押したら
		Transform transform = *obj->GetTransform();
		SetCameraPositionAndTarget_UpVecY(transform.position, diffTarget);
	}
	else if (rockOn) {
		SetCameraPositionAndTarget_UpVecY(cameraComponent.cameraTransform->position, target);
	}
	else if (!rockOn) {
		SetCameraPositionAndTarget_UpVecY(cameraComponent.cameraTransform->position, target);
	}
	
	//DrawSphere3D(target, 50, 1, 0x999999, 0x999999, true);
}

void Camera::Start(BaseObject* _eObj)
{
	const float NEAR_CLIP = 10.0f;
	const float FAR_CLIP = 10000.0f;
	SetCameraNearFar(NEAR_CLIP, FAR_CLIP);
	SetupCamera_Perspective(60.0f * DegToRad);
	//cameraComponent.target = &target;
	cameraComponent.shaker	= obj->Component()->AddComponent<Shaker>();
	
	cameraComponent.state	= obj->Component()->GetComponent<StateManager>();

	editor = new CameraEditorGui(this);
}

void Camera::ImguiDraw()
{
	//cameraComponent.state->StateNodeDraw();
	ImGui::RadioButton("normalCamera",&debugButton ,0);
	ImGui::RadioButton("FreeCamera", &debugButton, 1);
	ImGui::RadioButton("DiffTarget", &debugButton, 2);
	ImGui::DragFloat3("deffTarget", &diffTarget.x, 1.0f, -100000.0f, 10000.0f);
	ImGui::DragFloat3("cameraPosition", &cameraComponent.cameraTransform->position.x,1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("cameraPlayerPosition", &cameraComponent.player.transform->position.x,1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("cameraRotation", &cameraComponent.cameraTransform->rotation.x,1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("target", &target.x,1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat3("currentDistance", &currentDistance.x,1.0f, 0.0f, 5000.0f);
	ImGui::DragFloat("time", &timeTest,1.0f,0.0f, 2.0f);
	//ImGui::DragFloat3("distance", cameraComponent.currentDistance.x,5.0f,-1500.0f, 1500.0f);
	ImGui::RadioButton("targetPlayer", &targetChangeButton, 0);
	ImGui::RadioButton("targetEnemy", &targetChangeButton, 1);
	ImGui::DragFloat("fov", &fov, 0.1f, 0.0f, 360.0f * DegToRad);
	ImGui::InputFloat("nearFog", &nearFog);
	ImGui::InputFloat("farFog", &farFog);

}

void Camera::PlayerSet(BaseObject* _obj)
{
	cameraComponent.player.obj					= _obj;
	cameraComponent.camera = this;
	cameraComponent.player.transform			= _obj->GetTransform();
	cameraComponent.cameraTransform				=  new Transform(*cameraComponent.player.transform);
	cameraComponent.cameraTransform->rotation.x = 30.0f * DegToRad;
	cameraComponent.player.shaker				= _obj->Component()->GetComponent<Shaker>();
	cameraComponent.enemyManager				= FindGameObject<EnemyManager>();

	//stateを登録
	cameraComponent.state->CreateState<FollowCamera>				("_FollowCamera", StateID::FOLLOW_CAMERA_S);
	cameraComponent.state->CreateState<JustAvoidCamera>				("_JustAvoidCamera", StateID::JUST_AVOID_CAMERA_S);
	cameraComponent.state->CreateState<JustAvoidAttackCamera>		("_JustAvoidAttackCamera", StateID::JUST_AVOID_ATTACK_CAMERA_S);
	cameraComponent.state->CreateState<JustAvoidAttackHitCamera>	("_JustAvoidAttackHitCamera", StateID::JUST_AVOID_ATTACK_HIT_CAMERA_S);
	cameraComponent.state->CreateState<FreeCamera>					("_FreeCamera", StateID::FREE_CAMERA_S);
	cameraComponent.state->CreateState<PlayerSpecialAttackCamera>	("_PlayerSpecialAttackCamera", StateID::PLAYER_SPECIAL_ATTACK_CAMERA_S);
	cameraComponent.state->CreateState<CutSceneCamera>				("_CutSceneCamera", StateID::CUT_SCENE_CAMERA_S);
	cameraComponent.state->CreateState<PlayerHeavyAttackCamera>		("_PlayerHeavyAttackCamera", StateID::PLAYER_HEAVY_CHARGE_CAMERA_S);
	cameraComponent.state->CreateState<RangedEnemyCamera>			("RangedEnemyCamera", StateID:: R_ENEMY_CAMERA_S);
	cameraComponent.state->CreateState<RangedEnemyFinishCamera>		("RangedEnemyFinishCamera", StateID::R_ENEMY_FINISH_CAMERA_S);

	cameraComponent.state->NodeDrawReady();
	cameraComponent.target.shaker = _obj->Component()->GetComponent<Shaker>();

	cameraComponent.state->SetComponent<Camera>(this);
	cameraComponent.state->StartState(StateID::FREE_CAMERA_S);

	Hierachy* h = FindGameObject<Hierachy>();
	if (h != nullptr)
	{
		h->SetCameraEditor(this);
	}
	CutSceneChangeState("PlayingBefore",true);
	//CameraRotationSet();
}

void Camera::TargetSet(BaseObject* _obj)
{
	if (_obj == nullptr) {
		
		cameraComponent.target.obj	= _obj;
		rockOn						= false;
		cameraComponent.state->ChangeState(StateID::FREE_CAMERA_S);
		return;
	}
	if (cameraComponent.enemyManager->ObjectIsEnemy(_obj)) {
		cameraComponent.target.obj			= _obj;
		cameraComponent.target.transform	= cameraComponent.target.obj->GetTransform();
		rockOn								= true;
	}
	
}

void Camera::TargetEnemySet()
{
	target = cameraComponent.target.transform->position;
}

void Camera::CameraLeapSet(float _rape)
{
	//カメラ追従していくスピードをセットしている
	reap = _rape;
}

void Camera::ChangeStateCamera(StateID::State_ID _id)
{
	cameraComponent.state->ChangeState(_id);
}

void Camera::CollsionPosHit(const VECTOR3& _norm, float _size, const VECTOR3& _groundPos)
{
	hitPos		= _groundPos;
	hitNormal	= _norm;
	hitDist		= _size;
	hit			= true;
}

void Camera::PushCamera(const CollsionEventData& _data)
{
	PushInfo info = _data.pushes[0];
	VECTOR3 norm = info.normal;
	float size = info.penetration;
	VECTOR3 groundPos = info.targetPos;

	float offset	= 5.0f; 
	float newDist	= max(0, size - offset);
	//1より上なら1にする
	if (counter > 1.0f) {
		counter = 1.0f;
	}
	else {
		counter += Time::DeltaTimeRate() * 3.0f;
	}
	float rate		= counter / 1.0f;
	hit				= true;

	cameraComponent.cameraTransform->position = VECTOR3(cameraComponent.cameraTransform->position.x ,groundPos.y + 280.0f, cameraComponent.cameraTransform->position.z);
}

void Camera::AttackEnemyFovChange(Transform* _targetTransform,float _maxspeed)
{
	std::shared_ptr<StateBase> freeCamera = cameraComponent.state->GetState<StateBase>();
	//カメラが動いているときはこの関数は作動しない
	if (moveTimer > 0.0f || freeCamera->GetID() != StateID::FREE_CAMERA_S || CameraRotationMove()) {
		return;
	}
	direction				= EnemyAttackChangeCameraDirection::RIGHT;
	targetEnemyTransform	= _targetTransform;
	moveTimer				= MOVE_TIMER_MAX;
	angleMaxSpeed			= _maxspeed;
}

bool Camera::IsFovIn(const Transform& _targetTransform, float maxFov)
{
	VECTOR3 dir = target - cameraComponent.cameraTransform->position;
	dir				= dir.Normalize();

	VECTOR3 target	= _targetTransform.position - cameraComponent.player.obj->GetTransform()->position;
	float dist = target.Size();
	if (VDot(dir.Normalize(), target.Normalize()) < cosf(maxFov)) {
		return false;
	}
	else {
		return true;
	}

}

void Camera::RotationChange(const Transform& _targetTransform, float _speed)
{
	VECTOR3 forward = VECTOR3(0, 0, 1) * MGetRotY(cameraComponent.cameraTransform->rotation.y);
	VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(cameraComponent.cameraTransform->rotation.y);
	VECTOR3 toTarget = cameraComponent.cameraTransform->position - _targetTransform.position;
	VECTOR3 target = toTarget * MGetRotY(cameraComponent.cameraTransform->rotation.y);
	float dot = VDot(toTarget.Normalize(), forward.Normalize());	//コサインの値が正面ベクトルとカメラの角度を計算
	//内積を使って補正
	if (dot >= cosf(_speed * DegToRad)) {
		float inRot = atan2f(toTarget.x, toTarget.z);
		cameraComponent.cameraTransform->rotation.y = inRot;
	}
	else {
		if (VDot(right, toTarget) > 0) {
			cameraComponent.cameraTransform->rotation.y += _speed * DegToRad;
		}
		else {
			cameraComponent.cameraTransform->rotation.y -= _speed * DegToRad;
		}
	}
}

bool Camera::CameraRotationMove()
{
	bool cancel = false;
	const float DEAD_ZONE = 0.3f;
	//コントローラーの入力があったら移動中と判定
	if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().rightStick.x >= DEAD_ZONE || CheckHitKey(KEY_INPUT_RIGHT)) {
		cancel = true;
	}
	if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().rightStick.x <= -DEAD_ZONE || CheckHitKey(KEY_INPUT_LEFT)) {
		cancel = true;
	}

	if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().rightStick.y >= DEAD_ZONE || CheckHitKey(KEY_INPUT_UP)) {
		cancel = true;
	}

	if (InputManager::GetInstance()->GetControllerInput()->GetStickInput().rightStick.y <= -DEAD_ZONE || CheckHitKey(KEY_INPUT_DOWN)) {
		cancel = true;
	}
	return cancel;
}

void Camera::CameraEditor()
{
	editor->EditorWindow();
}

void Camera::CutSceneChangeState(std::string _name, bool _cutScene)
{
	CutSceneChangeState(_name, _cutScene,CutSceneSpece::NONE);
}

void Camera::CutSceneChangeState(std::string _name, bool _cutScene, int _stop)
{
	JsonReader json;
	std::string name = "data/json/CutScene/" + _name + ".json";
	json.Load(name);

	cutSceneData.clear();
	for (auto& j : json.Data()["cutScenes"]) {
		CutSceneSpece::CutScene cut;
		j.get_to(cut);
		cutSceneData.push_back(cut);
	}

	cameraComponent.state->ChangeState(StateID::CUT_SCENE_CAMERA_S);
	isCutScene = true;

	cutStopChara = _stop;
	SleepTargetSet(cutStopChara, true);
	if (_cutScene) {
		cutSceneBox->StartBox(1.0f, 0x00000, Easing::EaseInOut<int>);
		uiManager->SetUIDraw(false);
	}
	cutSceneBoxDraw = _cutScene;
}

void Camera::SleepTargetSet(int _stop, bool _sleep)
{
	if (_stop == CutSceneSpece::NONE) {
		return;
	}
	//ビット演算を使い判定
	if (_stop & CutSceneSpece::PLAYER) {
		cameraComponent.player.obj->SetSleep(_sleep);
	}
	if (_stop & CutSceneSpece::ALL_ENEMY) {
		cameraComponent.enemyManager->SleepAllEnemy(_sleep);
	}
}

void Camera::CameraPerspectiveShakeStart(float _power, float _time)
{
	shakePower = _power * DegToRad;
	shakeTime = _time;
}

void Camera::CameraPerspectiveShakeFinish()
{
	shakeTime = 0.0f;
}
