#include "TitleScene.h"
#include "TitleCamera.h"

#include "TitlePlayer.h"
#include "TitlePlayerIdol.h"
#include "TitlePlayerMove.h"

#include "../State/StateManager.h"
#include "../Player/PlayerState/PlayerStateManager.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../Component/Animator/Anim2D.h"
#include "../Component/Physics/Physics.h"
#include "../Camera/Camera.h"
#include "../Common/InputManager/InputManager.h"
#include "../Common/InputManager/PadInput.h"
#include "../Common/InputManager/KeyboardInput.h"
#include "../../ImGui/imgui.h"
#include "../Component/Transform/Transform.h"
#include "../Component/Animator/Animator.h"
#include "../Camera/cameraManager.h"
#include "../Player/PlayerState/PlayerStateBase.h"
#include "../Component/ComponentManager.h"
#include "../Component/Shaker/Shaker.h"
#include "../Common/Sound/SoundManager.h"
#include "../Common/Effect/EffectManager.h"
#include "../Weapon/WeaponManager.h"
#include "../Component/MotionBlur/MotionBlur.h"
#include "../Common/ResourceLoader.h"
#include "../Common/Transitor/FadeTransitor.h"
#include "../Common/Easing.h"

TitlePlayer::TitlePlayer()
{
	debugId = 34;
	tag = Function::GetClassNameC<TitlePlayer>();
}

TitlePlayer::~TitlePlayer()
{
}

void TitlePlayer::Update()
{
}

void TitlePlayer::Draw()
{
}

void TitlePlayer::Start(Object3D* _obj) {
	//初期化の値を設定
	//必要なコンポーネントを付けている。
	//使いたいコンポーネントがあればplayerInformation.hに情報が書いてあるのでそれを見てほしい
	obj = _obj;
	playerCom.stateManager = obj->Component()->GetComponent<StateManager>();

	// ステート関連
	playerCom.stateManager->CreateState<TitlePlayerIdol>("_TitleIdol", StateID::PLAYER_WAIT_S);
	playerCom.stateManager->CreateState<TitlePlayerMove>("_TitleMove", StateID::PLAYER_AVOID_S);

	//コンポーネントや参照関連
	ComponentManager* c = obj->Component();

	playerCom.renderer = c->GetComponent<MeshRenderer>();
	//playerCom.meshRenderer2D = c->GetComponent<MeshRenderer2D>();
	playerCom.color = obj->Component()->GetComponent<Color>();
	playerCom.anim = obj->Component()->GetComponent<Animator>();
	//playerCom.anim2D = obj->Component()->GetComponent<Anim2D>();
	playerCom.physics = c->GetComponent<Physics>();
	//playerCom.shaker = c->GetComponent<Shaker>();
	playerCom.blur = obj->Component()->GetComponent<MotionBlur>();

	camera = FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<TitleCamera>();

	//playerCom.keyboard = FindGameObject<KeyboardInput>();
	playerCom.weapon = FindGameObject<WeaponManager>(); //

	//重力を0に変更
	playerCom.physics->SetGravity(VZero);

	//初期位置を設定
	VECTOR3 firstPos = VECTOR3(0, 30, 0);
	obj->GetTransform()->position = firstPos;
	playerTransform = obj->GetTransform();

	//collName = "p_attack";

	using namespace ID;

	playerCom.stateManager->NodeDrawReady();
	playerCom.stateManager->SetComponent<TitlePlayer>(this);

	playerCom.stateManager->StartState(StateID::PLAYER_WAIT_S);
}

void TitlePlayer::RotationChange(const VECTOR3& _angle, float _speed)
{
	VECTOR3 forward = VECTOR3(0, 0, 1) * MGetRotY(playerTransform->rotation.y);
	VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(playerTransform->rotation.y);
	VECTOR3 target = _angle * MGetRotY(camera->GetCameraTransform()->rotation.y);
	float dot = VDot(target.Normalize(), forward.Normalize());	//コサインの値が正面ベクトルとカメラの角度を計算
	//内積を使って補正
	if (dot >= cosf(_speed * DegToRad))
	{
		float inRot = atan2f(target.x, target.z);
		playerTransform->rotation.y = inRot;
	}
	else
	{
		if (VDot(right, target) > 0) {
			playerTransform->rotation.y += _speed * DegToRad;
		}
		else {
			playerTransform->rotation.y -= _speed * DegToRad;
		}
	}
}

void TitlePlayer::RotationChange(const VECTOR3& _angle)
{
	RotationChange(_angle, 5.0f);
}
