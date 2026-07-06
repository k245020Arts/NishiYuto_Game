#pragma once
#include "../Common/ID/EffectID.h"
#include "EnemyState/EnemyBlowAway.h"
#include "EnemyState/EnemyDamage.h"

class StateManager;
//class Enemy;
class TrashEnemy;
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
class GameControler;
class ColliderBase;

namespace EnemyInformation {

	struct EnemyComponent
	{
		EnemyComponent() {
			state = nullptr;
			//enemy = nullptr;
			physics = nullptr;
			camera = nullptr;
			playerObj = nullptr;
			anim = nullptr;
			color = nullptr;
			shaker = nullptr;
			weapon = nullptr;
			gameManager = nullptr;
			trashEnemy = nullptr;
		}
		StateManager* state;
		//Enemy* enemy;
		TrashEnemy* trashEnemy;
		Physics* physics;
		Animator* anim;
		//PadInput* control;
		Color* color;
		Shaker* shaker;
		BaseObject* playerObj;
		Camera* camera;
		//EffectManager* effect;
		//SoundManager* sound;
		WeaponManager* weapon;
		GameControler* gameManager;
		std::list<ColliderBase*> collider;
	};

	//最初のポジション
	const VECTOR3 BASE_POS = VECTOR3(500, 1000, 0);
	const float HIT_EFFECT_TIME = 0.2f;
	const float HIT_EFFECT_SCALE_RATE = 0.1f;

	enum AttackType {
		Normal,         // 通常攻撃
		JustAvoid,      // ジャスト回避攻撃
		BlowAway,       // 吹っ飛ばし
		LoopCombo       // 多段ヒット・特殊コンボ
	};

	struct EnemyReaction {

		EnemyReaction() {
			attackID = StateID::State_ID::STATE_MAX;
			dInfo = EnemyDamage::EnemyDamageInfo{};
			bInfo = EnemyBlowAway::EnemyBlowAwayInfo{};

			attackType = Type::Normal;
			changeStateID = StateID::State_ID::STATE_MAX;

			vibrationPower = 0;
			vibrationType = 0;

			hitEffectID = Effect_ID::EFFECT_ID::EFFECT_MAX;
			hitEffectTime = 0.0f;
			hitEffectScaleRate = 1.0f;

			hit = false;
			slashAngleRad = 0.0f;
			slashEffectID = Effect_ID::EFFECT_ID::EFFECT_MAX;
			useSlashEffect = false;

			loopMax = -1;
			loopInterval = 0.0f;
		}
		enum class Type {
			None = -1,
			Normal = 0,
			BlowAway,
			LoopCombo,
			Special
		};
		EnemyReaction(
			StateID::State_ID _attackID,
			const EnemyDamage::EnemyDamageInfo& _dInfo,
			const EnemyBlowAway::EnemyBlowAwayInfo& _bInfo,
			Type _attackType,StateID::State_ID _changeStateID,
			int _vibrationPower,int _vibrationType,Effect_ID::EFFECT_ID _hitEffectID,float _hitEffectTime,
			float _hitEffectScaleRate,bool _hit,float _slashAngleRad,Effect_ID::EFFECT_ID _slashEffectID,
			bool _useSlashEffect,int _loopMax,float _loopInterval
		)
		{
			attackID = _attackID;
			dInfo = _dInfo;
			bInfo = _bInfo;

			attackType = _attackType;
			changeStateID = _changeStateID;

			vibrationPower = _vibrationPower;
			vibrationType = _vibrationType;

			hitEffectID = _hitEffectID;
			hitEffectTime = _hitEffectTime;
			hitEffectScaleRate = _hitEffectScaleRate;

			hit = _hit;
			slashAngleRad = _slashAngleRad;
			slashEffectID = _slashEffectID;
			useSlashEffect = _useSlashEffect;

			loopMax = _loopMax;
			loopInterval = _loopInterval;
		}
		StateID::State_ID attackID;

		EnemyDamage::EnemyDamageInfo dInfo;
		EnemyBlowAway::EnemyBlowAwayInfo bInfo;

		
		
		Type attackType;
		StateID::State_ID changeStateID;

		int vibrationPower;
		int vibrationType;

		Effect_ID::EFFECT_ID hitEffectID;
		float hitEffectTime;
		float hitEffectScaleRate;

		bool hit;
		float slashAngleRad;
		Effect_ID::EFFECT_ID slashEffectID;
		bool useSlashEffect;

		int loopMax;          // -1でループなし
		float loopInterval;   // ループ間隔
	};	
};