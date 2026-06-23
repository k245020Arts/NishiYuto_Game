#pragma once
#include "../../../Library/myDxLib.h"

class StateManager;
class Boss;
class Animator;
class Physics;
class PadInput;
class Color;
class Shaker;
class BaseObject;
class Camera;
class EffectManager;
class SoundManager;
class WeaponManager;
class GameManager;
class ColliderBase;

namespace BossInformation {

	struct BossComponent
	{
		BossComponent() {
			state = nullptr;
			boss = nullptr;
			physics = nullptr;
			camera = nullptr;
			control = nullptr;
			playerObj = nullptr;
			anim = nullptr;
			color = nullptr;
			shaker = nullptr;
			effect = nullptr;
			sound = nullptr;
			//weapon = nullptr;
			gameManager = nullptr;
		}
		StateManager* state;
		Boss* boss;
		Physics* physics;
		Animator* anim;
		PadInput* control;
		Color* color;
		Shaker* shaker;
		BaseObject* playerObj;
		Camera* camera;
		EffectManager* effect;
		SoundManager* sound;
		//WeaponManager* weapon;
		GameManager* gameManager;
	};

	//ç≈èâÇÃÉ|ÉWÉVÉáÉì
	const VECTOR3 BASE_POS = VECTOR3(500, 0, 0);
	const VECTOR3 BASE_GRAVITY = VECTOR3(0, -1500, 0);

	const VECTOR3 BASE_FIRCTION = VECTOR3(3000.0f, 3000.0f, 3000.0f);

};