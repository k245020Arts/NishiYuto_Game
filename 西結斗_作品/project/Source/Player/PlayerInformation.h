#pragma once
#include "../../Library/myDxLib.h"
#include "../Component/Shaker/Shaker.h"
#include "../Common/JsonReader.h"
#include "../Common/ID/StateID.h"

class MeshRenderer;
class MeshRenderer2D;
class Physics;
class StateManager;
class Camera;
class InputManager;
class KeyboardInput;
class PadInput;
class Player;
class Animator;
class Color;
class BaseObject;
class Shaker;
class EffectManager;
class SoundManager;
class WeaponManager;
class MotionBlur;
class GameControler;
class EnemyManager;
class Anim2D;
class ButtonUI;

namespace PlayerInformation {

	enum PlayerState
	{
		WAIT,
		WALK,
	};

	struct CharaComponent
	{
		CharaComponent() {
			stateManager = nullptr;
			renderer = nullptr;
			physics = nullptr;
			camera = nullptr;
			player = nullptr;
			anim = nullptr;
			color = nullptr;
			targetObj = nullptr;
			shaker = nullptr;
			weapon = nullptr;
			blur = nullptr;
			gameManager = nullptr;
			enemyManager = nullptr;
			hitObj = nullptr;
			meshRenderer2D = nullptr;
			anim2D = nullptr;
			specialAttackButton = nullptr;
		}
		StateManager* stateManager;
		MeshRenderer* renderer;
		Physics* physics;
		Camera* camera;
		Player* player;
		Animator* anim;
		Color* color;
		BaseObject* targetObj;
		BaseObject* hitObj;
		Shaker* shaker;
		WeaponManager* weapon;
		MotionBlur* blur;
		GameControler* gameManager;
		EnemyManager* enemyManager;
		MeshRenderer2D* meshRenderer2D;
		Anim2D* anim2D;
		ButtonUI* specialAttackButton;

	};
	/// <summary>
	/// 基準の空気抵抗値
	/// </summary>
	static const VECTOR3 BASE_INTERIA = VECTOR3(7500.0f, -7500.0f, 7500.0f);
	/// <summary>
	/// 基準の重力値
	/// </summary>
	static const VECTOR3 BASE_GRAVITY = VECTOR3(0.0f, -4000.0f, 0.0f);
	/// <summary>
	/// ジャスト回避をした時にどれくらい敵をスローさせるか
	/// </summary>
	static const float JUST_AVOID_ENEMY_TIME_SCALE = 0.3f;

	// チャージレベル1つ分のリアクションパラメータ
	struct ChargeReactionLevelData
	{
		VECTOR3 shakePower;		//ヒットストップのパワー
		float   shakeTime;		//ヒットストップの時間
		VECTOR3 cameraShakePower;//カメラのシェイクのパワー
		float   cameraShakeTime;//カメラのシェイクのパワー

		ChargeReactionLevelData()
			: shakePower(VZero), shakeTime(0.0f)
			, cameraShakePower(VZero), cameraShakeTime(0.0f)
		{
		}

		ChargeReactionLevelData(const VECTOR3& _shakePower, float _shakeTime,
			const VECTOR3& _cameraShakePower, float _cameraShakeTime)
			: shakePower(_shakePower), shakeTime(_shakeTime)
			, cameraShakePower(_cameraShakePower), cameraShakeTime(_cameraShakeTime)
		{
		}
	};

	struct PlayerReaction
	{
		PlayerReaction() {
			state = StateID::STATE_MAX;
			shakePower = VZero;
			shakeTime = 0.0f;
			cameraShakePower = VZero;
			cameraShakeTime = 0.0f;
			soundName.clear();
			soundKind = 0;
			shakerLoop = false;
			shakePattern = Shaker::NONE;
			//chargeLevels.clear();
		}
		PlayerReaction(StateID::State_ID _state,const VECTOR3& _shakePower,float _shakeTime,const VECTOR3& _cameraShakePower,
			float _cameraShakeTime,const std::string& _soundName,int _soundKind,bool _shakerLoop,
			Shaker::ShakePattern _shakePattern,
			const std::vector<ChargeReactionLevelData>& _chargeLevels = {})
		{
			state = _state;
			shakePower = _shakePower;
			shakeTime = _shakeTime;
			cameraShakePower = _cameraShakePower;
			cameraShakeTime = _cameraShakeTime;
			soundName = _soundName;
			soundKind = _soundKind;
			shakerLoop = _shakerLoop;
			shakePattern = _shakePattern;
			chargeLevels = _chargeLevels; 
			
		}
		
		StateID::State_ID state;							//攻撃のステート
		VECTOR3 shakePower;									//ヒットストップのShakeのパワー
		float shakeTime;									//ヒットストップのShakeの時間
		VECTOR3 cameraShakePower;							//ヒットストップの時のカメラのShakeのパワー
		float cameraShakeTime;								//ヒットストップの時のカメラのShakeの時間
		std::string soundName;								//当たった時のヒット音
		int soundKind;										//サウンドが何種類あるか
		bool shakerLoop;									//ループするか
		Shaker::ShakePattern shakePattern;					//shakeのパターン
		std::vector<ChargeReactionLevelData> chargeLevels;  //チャージのレベル

		//チャージのレベルを取得
		const ChargeReactionLevelData* GetChargeLevel(int level) const
		{
			int idx = static_cast<int>(level);
			if (idx < 0 || idx >= (int)chargeLevels.size()) return nullptr;
			return &chargeLevels[idx];
		}
	};

	inline void to_json(nlohmann::json& j, const ChargeReactionLevelData& p) {
		j["shakePower"] = p.shakePower;
		j["shakeTime"] = p.shakeTime;
		j["cameraShakePower"] = p.cameraShakePower;
		j["cameraShakeTime"] = p.cameraShakeTime;
	}

	inline void from_json(const nlohmann::json& j, ChargeReactionLevelData& p) {
		j.at("shakePower").get_to(p.shakePower);
		j.at("shakeTime").get_to(p.shakeTime);
		j.at("cameraShakePower").get_to(p.cameraShakePower);
		j.at("cameraShakeTime").get_to(p.cameraShakeTime);
	}
	
	inline void to_json(nlohmann::json& j, const PlayerReaction& p) {
		j = {
			{"PlayerState",      StateID::GetID(p.state)},
			{"playerShakePower", p.shakePower},
			{"playerShakeTime",  p.shakeTime},
			{"cameraShakePower", p.cameraShakePower},
			{"cameraShakeTime",  p.cameraShakeTime},
			{"ShakePattern",     p.shakePattern},
			{"shakerType",       p.shakerLoop},
			{"soundName",        p.soundName},
			{"soundKind",        p.soundKind},
			{"chargeLevels",     p.chargeLevels},
		};
	}
	inline void from_json(const nlohmann::json& j, PlayerReaction& p) {
		p.state = StateID::StringToID(j["PlayerState"].get<std::string>());
		j.at("playerShakePower").get_to(p.shakePower);
		j.at("playerShakeTime").get_to(p.shakeTime);
		j.at("cameraShakePower").get_to(p.cameraShakePower);
		j.at("cameraShakeTime").get_to(p.cameraShakeTime);
		p.shakePattern = (Shaker::ShakePattern)j.at("ShakePattern").get_to(p.shakePattern);
		j.at("shakerType").get_to(p.shakerLoop);
		j.at("soundName").get_to(p.soundName);
		j.at("soundKind").get_to(p.soundKind);

		//// チャージレベルは存在しないStateもあるのでcontains()でチェック
		if (j.contains("chargeLevels")) {
			j.at("chargeLevels").get_to(p.chargeLevels);
		}
	}

	

}
