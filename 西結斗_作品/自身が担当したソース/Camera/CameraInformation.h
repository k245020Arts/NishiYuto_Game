#pragma once

class Shaker;
class BaseObject;
class Transform;
class Camera;
class StateManager;
class PadInput;
class EnemyManager;

enum class EnemyAttackChangeCameraDirection {
	NONE  = -1,
	RIGHT = 0,
	LEFT,
};

namespace CameraInformation {
	//カメラのプレイヤーやエネミーのほしい情報をまとめた構造体
	struct CharaInformation
	{
		CharaInformation() {
			transform = nullptr;
			shaker = nullptr;
			obj = nullptr;
		}
		Transform* transform;
		Shaker* shaker;
		BaseObject* obj;
	};
	//カメラのほしい情報をまとめた構造体
	struct CameraComponent
	{
		CameraComponent() {
			player = CharaInformation();
			target = CharaInformation();
			camera = nullptr;
			shaker = nullptr;
			cameraTransform = nullptr;
			state = nullptr;
			enemyManager = nullptr;
			//target = nullptr;
			//reap = nullptr;
			//currentDistance = nullptr;
			//defalutDistance = nullptr;
		}
		CharaInformation player;
		CharaInformation target;
		Camera* camera;
		Shaker* shaker;
		Transform* cameraTransform;
		StateManager* state;
		EnemyManager* enemyManager;
		//VECTOR3* target;
		//float* reap;
		//VECTOR3* currentDistance;
		//VECTOR3* defalutDistance;
	};
	const float DEFALUT_RAPE = 0.2f;
}