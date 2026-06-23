#include "TrashEnemy.h"
#include "../../Component/Animator/Animator.h"
#include "../../Component/Physics/Physics.h"
#include "../../Common/InputManager/PadInput.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../Component/Collider/SphereCollider.h"
#include "../../Component/Color/Color.h"
#include "../../Component/Shaker/Shaker.h"
#include "../../../ImGui/imgui.h"
#include "../../Player/Player.h"
#include "../EnemyState/EnemyBlowAway.h"
#include "../../Camera/CameraManager.h"
#include "../../Camera/Camera.h"
#include "../../Common/Effect/EffectManager.h"
#include "../../Common/Sound/SoundManager.h"
#include "../../Common/ID/ID.h"
#include "../../Weapon/WeaponManager.h"
#include "../../State/StateManager.h"
#include "../../GameControler/GameControler.h"
#include "../../Player/PlayerState/AttackState/PlayerAttackStateBase.h"
#include "../../Weapon/SwordEffect.h"
#include "../../Weapon/CharaWeapon.h"
#include "../../Common/ResourceLoader.h"
#include "../../Player/PlayerState/AttackState/PlayerSpecialAttack.h"

#include "TrashEnemyState/T_EnemyStatus.h"
#include "TrashEnemyState/T_EnemyIdol.h"
#include "TrashEnemyState/T_EnemyRun.h"
#include "TrashEnemyState/T_EnemyAttack.h"
#include "TrashEnemyState/T_EnemyDead.h"
#include "TrashEnemyState/CooperateAttack1.h"
#include "TrashEnemyState/CooperateAttack2.h"
#include "TrashEnemyState/Standby.h"
#include "TrashEnemyState/T_EnemyDamage.h"
#include "TrashEnemyState/T_EnemyWaitSee.h"
#include "TrashEnemyState/T_EnemyStaySky.h"
#include "TrashEnemyState/T_EvadeState.h"

namespace
{
	const std::unordered_map<StateID::State_ID, EnemyInformation::EnemyReaction> enemyTable = {

	{ StateID::PLAYER_ATTACK1_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_ATTACK1_S,
			EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,1500), VECTOR3(100,100,100), 0.15f, 0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::Normal,
			StateID::B_THREAT_S,
			10, 10,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			40.0f,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			-1,
			0.0f
		)
	},

	{ StateID::PLAYER_ATTACK2_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_ATTACK2_S,
			EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,1500), VECTOR3(100,100,100), 0.25f, 0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::Normal,
			StateID::B_THREAT_S,
			40, 30,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			70.0f * DegToRad,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			-1,
			0.0f
		)
	},

	{ StateID::PLAYER_ATTACK3_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_ATTACK3_S,
			EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,1500), VECTOR3(100,100,100), 0.35f, 0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::Normal,
			StateID::B_THREAT_S,
			10, 10,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			80.0f * DegToRad,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			-1,
			0.0f
		)
	},

	{ StateID::PLAYER_ATTACK4_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_ATTACK4_S,
			EnemyDamage::EnemyDamageInfo(VECTOR3(0,400,0), VECTOR3(200,200,200), 0.85f, 1.0f),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::Normal,
			StateID::B_THREAT_S,
			50, 50,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			0.0f * DegToRad,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			-1,
			0.0f
		)
	},

	{ StateID::PLAYER_JUST_AVOID_ATTACK1_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_JUST_AVOID_ATTACK1_S,
			EnemyDamage::EnemyDamageInfo(),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::LoopCombo,
			StateID::B_THREAT_S,
			0, 0,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			90.0f * DegToRad,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			5,
			0.13f
		)
	},

	{ StateID::PLAYER_ATTACK5_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_ATTACK5_S,
			EnemyDamage::EnemyDamageInfo(VECTOR3(0,-2000,-2000), VECTOR3(200,200,200), 0.2f, 0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::Normal,
			StateID::B_THREAT_S,
			70, 40,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			180.0f * DegToRad,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			-1,
			0.0f
		)
	},

	{ StateID::PLAYER_SPECIAL_ATTACK_S,
		EnemyInformation::EnemyReaction(
			StateID::PLAYER_SPECIAL_ATTACK_S,
			EnemyDamage::EnemyDamageInfo(VECTOR3(0,0,1500), VECTOR3(100,100,100), 0.15f, 0.8f),
			EnemyBlowAway::EnemyBlowAwayInfo(),
			EnemyInformation::EnemyReaction::Type::Special,
			StateID::B_THREAT_S,
			10, 10,
			Effect_ID::HIT_EFFECT,
			EnemyInformation::HIT_EFFECT_TIME,
			EnemyInformation::HIT_EFFECT_SCALE_RATE,
			true,
			40.0f,
			Effect_ID::PLAYER_SLASH_ATTACK,
			true,
			-1,
			0.0f
		)
	},

	{ StateID::PLAYER_HEAVY_ATTACK_S,
	EnemyInformation::EnemyReaction(
		StateID::PLAYER_HEAVY_ATTACK_S,
		EnemyDamage::EnemyDamageInfo(VECTOR3(0,-2000,-2000), VECTOR3(200,200,200), 0.2f, 0.8f),
		EnemyBlowAway::EnemyBlowAwayInfo(),
		EnemyInformation::EnemyReaction::Type::Normal,
		StateID::B_THREAT_S,
		70, 40,
		Effect_ID::HIT_EFFECT,
		EnemyInformation::HIT_EFFECT_TIME,
		EnemyInformation::HIT_EFFECT_SCALE_RATE,
		true,
		180.0f * DegToRad,
		Effect_ID::PLAYER_SLASH_ATTACK,
		true,
		-1,
		0.0f
	)
	},

	};
}

TrashEnemy::TrashEnemy()
{
	tag = Function::GetClassNameC<TrashEnemy>();
	eStatus = new T_EnemyStatus;

	chara = nullptr;

	speed = 0;
	defense = 0;

	isAttack = false;
	isStandby = false;
	isCooperateAtk = false;

	cooperateWayPoint = VZero;

	isMovingToPlayer = false;

	slowCounter = 0;

	mStopCounter = 0;

	active = true;

	deadPreset = deadPresets[0];

	nextCooperateID = StateID::State_ID::STATE_MAX;

	isRunState = false;

	pointNumber = 0;

	leaderPos = VZero;

	cooperateDamageMove = false;

	deadMove = false;
	attackFunk = nullptr;
	justAvoidAttackFunk = nullptr;

	cAttack = false;

	leaderRotY = 0;
	guage = nullptr;
}

TrashEnemy::~TrashEnemy()
{
	delete eStatus;
	eStatus = nullptr;
	guage = nullptr;
}

void TrashEnemy::Update()
{
	slowCounter += Time::DeltaTimeRate();

	if (IsPlayerSpecialMove())
		return;
		
	EnemyBase::Update();

	if (hp <= 0)
	{
		enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_DEAD);
		deadMove = true;
		if (guage != nullptr) {
			guage->DestroyMe();
		}
		
	}
		
	if (CheckHitKey(KEY_INPUT_9))
	{
		hp -= maxHp;
		deadPreset = deadPresets[2];
	}
}

void TrashEnemy::Draw()
{
	EnemyBase::Draw();
}

void TrashEnemy::Start(Object3D* _obj, EnemyType _type, Object2D* _guage)
{
	SphereCollider* collider = _obj->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom = _obj->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::ENEMY;
	info.size = 1.0f;//
	std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) {DamageCollsionEvent(_data); };
	collider->CollsionAdd(info, Transform(VECTOR3(0, 100, 0), VZero, VECTOR3(350.0f, 1.0f, 1.0f)), func);

	enemyBaseComponent.state = obj->Component()->AddComponent<StateManager>();
	enemyBaseComponent.playerObj = FindGameObjectWithTag<Object3D>("PLAYER");
	pState = enemyBaseComponent.playerObj->Component()->GetComponent<Player>()->GetPlayerStateManager();

	enemyBaseComponent.collider = FindGameObjects<ColliderBase>();

	attackColl		= CharaBase::CollsionSet(nullptr, CollsionInformation::SPHERE, "e_attack", true, CollsionInformation::E_ATTACK);
	justAvoidColl	= CharaBase::CollsionSet(nullptr, CollsionInformation::SPHERE, "e_just_avoid", false, CollsionInformation::JUST_AVOID_ENEMY);

	enemyBaseComponent.anim = obj->Component()->GetComponent<Animator>();
	enemyBaseComponent.physics = obj->Component()->GetComponent<Physics>();
	
	enemyBaseComponent.color = obj->Component()->GetComponent<Color>();
	enemyBaseComponent.color->setRGB(Color::Rgb(255.0f, 255.0f, 255.0f, 255.0f));
	enemyBaseComponent.shaker = obj->Component()->GetComponent<Shaker>();
	
	enemyBaseComponent.physics = obj->Component()->GetComponent<Physics>();
	enemyBaseComponent.camera = FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<Camera>();

	using namespace ID;

	enemyBaseComponent.state->CreateState<T_EnemyIdol>("_T_EnemyIdol", StateID::T_ENEMY_IDOL_S);
	enemyBaseComponent.state->CreateState<T_EnemyRun>("_T_EnemyRun", StateID::T_ENEMY_RUN_S);
	enemyBaseComponent.state->CreateState<T_EnemyAttack>("_T_EnemyAttack", StateID::T_ENEMY_ATTACK_S);
	enemyBaseComponent.state->CreateState<T_EnemyDead>("_T_EnemyDead", StateID::T_ENEMY_DEAD);
	enemyBaseComponent.state->CreateState<CooperateAttack1>("_CooperateAttack1", StateID::COOPERATEATTACK1);
	enemyBaseComponent.state->CreateState<CooperateAttack2>("_CooperateAttack2", StateID::COOPERATEATTACK2);
	enemyBaseComponent.state->CreateState<Standby>("_Standby", StateID::T_ENEMY_STANDBY);
	enemyBaseComponent.state->CreateState<T_EnemyDamage>("_T_EnemyDamage", StateID::T_ENEMY_DAMAGE);
	enemyBaseComponent.state->CreateState<T_EnemyWaitSee>("_T_EnemyWaitSee", StateID::T_ENEMY_WAITSEE);
	enemyBaseComponent.state->CreateState <T_EnemyStaySky>("T_EnemyStaySky", StateID::T_ENEMY_STAYSKY);
	enemyBaseComponent.state->CreateState<T_Evade>("T_Evade", StateID::T_ENEMY_EVADE);

	enemyBaseComponent.state->SetComponent<TrashEnemy>(this);

	enemyType = _type;
	// スタートステートも StateID 化
	enemyBaseComponent.state->StartState(StateID::T_ENEMY_IDOL_S);
	enemyBaseComponent.weapon = FindGameObject<WeaponManager>();

	chara = obj->Component()->AddComponent<CharaWeapon>();
	chara->ObjectPointer(_obj, 10, ID::E_MODEL, -1);
	chara->SetImage(ResourceLoad::GetHandle(ID::SWORD_EFFECT_B));	

	guage = _guage;
}

void TrashEnemy::CreateTrashEnemy(VECTOR3 _pos, int kinds, int _number)
{
	obj->GetTransform()->position = _pos;
	pointNumber = _number;

	const float MAX = 1.5f;
	const float MID = 1.25f;
	const float MIN = 1.0f;

	switch (kinds)
	{
	case 0://すばしっこい敵
		hp = eStatus->GetStatus().maxHp * MIN;
		maxHp = hp;
		speed = eStatus->GetStatus().runSpeed * MAX;
		defense = eStatus->GetStatus().defense * MIN;
		GetEnemyObj()->GetTransform()->scale = GetEnemyObj()->GetTransform()->scale * MIN;
		
		break;
	case 1:
		hp = eStatus->GetStatus().maxHp * MID;
		maxHp = hp;
		speed = eStatus->GetStatus().runSpeed * MID;
		defense = eStatus->GetStatus().defense * MID;
		break;
	default://重い敵
		hp = eStatus->GetStatus().maxHp * MAX;
		maxHp = hp;
		speed = eStatus->GetStatus().runSpeed * MIN;
		defense = eStatus->GetStatus().defense * MAX;
		GetEnemyObj()->GetTransform()->scale = GetEnemyObj()->GetTransform()->scale * MAX;
		break;
	}
}

void TrashEnemy::SetEnemyType(EnemyType type)
{
	enemyType = type;
}

void TrashEnemy::SetLeaderPos(VECTOR3 _pos)
{
	leaderPos = _pos;
}

void TrashEnemy::RangedAttack()
{
	if (enemyType == EnemyType::MELEE || enemyType == EnemyType::MAX)
		return;

	enemyBaseComponent.state->ChangeState(StateID::COOPERATEATTACK2);

	isCooperateAtk = true;
}

void TrashEnemy::ChangeHp(float _damage)
{
	hp += _damage;
	//後で修正
	deadPreset = deadPresets[4];
}

void TrashEnemy::DamageCollsionEvent(const CollsionEventData& _data)
{
	if (_data.targetTag != CollsionInformation::WALL) {
		PlayerHit(_data);
	}
}

void TrashEnemy::NormalMove(float _speed)
{
	Move(VECTOR3(0, 0, -1), _speed);
}

void TrashEnemy::Move(VECTOR3 _vector, float _speed)
{
	GetEnemyObj()->GetTransform()->position += _vector * MGetRotY(GetEnemyObj()->GetTransform()->rotation.y) * _speed;
	if (isCooperateAtk)
		return;
	WallStop();
}

void TrashEnemy::WallStop()
{
	VECTOR3& ePos = GetEnemyObj()->GetTransform()->position;
	const VECTOR3 WallPos = VECTOR3(10000.0f, 0.0f, 10000.0f);

	if (ePos.x >= WallPos.x)
		ePos.x = WallPos.x;
	else if(ePos.x <= -WallPos.x)
		ePos.x = -WallPos.x;

	if(ePos.z >= WallPos.z)
		ePos.z = WallPos.z;
	else if (ePos.z <= -WallPos.z)
		ePos.z = -WallPos.z;
}

void TrashEnemy::LookTarget(VECTOR3 _pos)
{
	VECTOR3 distance = _pos - obj->GetTransform()->position;
	//向くべき角度
	float direction = -atan2f(distance.z, distance.x) - 0.5f * DX_PI_F;
	//その角度とどれだけ差があるか
	float sign = direction - obj->GetTransform()->rotation.y;
	//切り捨てして180の境界線を無くす
	sign -= floorf(sign / DX_PI_F / 2) * DX_PI_F * 2.0f;
	if (sign > DX_PI_F)
		sign -= 2 * DX_PI_F;
	//向くスピード(ラジアン)
	const float LOOK_SPEED = 0.5;
	//Playerの方をゆっくり向く
	if (sign > LOOK_SPEED)
		obj->GetTransform()->rotation.y += LOOK_SPEED;
	else if (sign < -LOOK_SPEED)
		obj->GetTransform()->rotation.y -= LOOK_SPEED;
	else
		obj->GetTransform()->rotation.y = direction;
}

bool TrashEnemy::IsPlayerSpecialMove()
{
	if (pState->GetState<PlayerSpecialAttack>() == nullptr)
		return false;
	else
	{
		if (pState->GetState<PlayerSpecialAttack>()->GetAttackDamage())
		{
			mStopCounter += Time::DeltaTimeRate();
			if (mStopCounter >= 0.2f)
				return false;
			else
				return true;
		}
		else
		{
			mStopCounter = 0;
			return true;
		}
	}		
}

void TrashEnemy::Trail()
{
	chara->CreateSwordEffect(VECTOR3(70, 0, -50), VECTOR3(120, 0, 50), 200.0f, 10.0f, 00.0f, 155.0f, 28, 0.5f);
}

void TrashEnemy::PlayerHit(const CollsionEventData& _data)
{
	Player* player = pState->GetBaseObject()->Component()->GetComponent<Player>();
	if (player->IsHitObject(_data.myObject)) {
		return;
	}
	StateID::State_ID attackID = pState->GetState<PlayerStateBase>()->GetID();
	float damage = 0;
	if (pState->GetState<PlayerAttackStateBase>() != nullptr)
		damage = pState->GetState<PlayerAttackStateBase>()->GetHitDamage();
	else
		loopNum = -1;

	if (isStandby && enemyType == EnemyType::RANGED)
	{
		//遠距離のリーダーに飛ばす処理を作る
		cooperateDamageMove = true;
		return;
	}

	EnemyDamage::EnemyDamageInfo dInfo;
	EnemyBlowAway::EnemyBlowAwayInfo bInfo;
	float random[3] = {};
	//エフェクトの位置をランダムに派生
	for (float& r : random)
	{
		r = (float)GetRand(80) - 40.0f;
	}
	//プレイヤーの攻撃の種類によってダメージや吹っ飛び方の種類を変える
	float angleRan = 0.0f;
	bool lastAttack = false;
	bool lastBeforeAttack = false;

	auto param = enemyTable.find(attackID);
	if (param != enemyTable.end())
	{
		const auto& e = param->second;
		switch (e.attackType)
		{
		case EnemyInformation::EnemyReaction::Type::Normal:
			InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(e.vibrationPower, e.vibrationType);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * e.hitEffectScaleRate), obj, e.hitEffectID, e.hitEffectTime);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, e.slashAngleRad), VOne), obj, e.slashEffectID, 1.0f);
			hit = true;
			deadPreset = deadPresets[1];
			break;
		case EnemyInformation::EnemyReaction::Type::BlowAway:

			break;
		case EnemyInformation::EnemyReaction::Type::LoopCombo:
			SoundManager::GetInstance()->RandamSe("swordHit00000", 7);
			if (loopNum == -1)
			{
				hitCounter = 0.23f;
				loopNum = 5;
			}
			else if (loopNum == 1)
			{
				hitCounter = 0.13f;
				loopNum--;
				lastBeforeAttack = true;
			}
			else if (loopNum == 0)
			{
				loopNum = -1;
				lastAttack = true;
			}
			else if (loopNum > 0)
			{
				hitCounter = 0.13f;
				loopNum--;
			}
			if (lastAttack)
			{
				dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 3500.0f), VECTOR3(100, 100, 100), 0.5f, 1.2f);
				InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(250, 60);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE * 14.5f), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, 90 * DegToRad), VOne * 1.5f), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
				hit = true;
			}
			else if (lastBeforeAttack)
			{
				dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(100, 100, 100), 0.5f, 0.5f);
				enemyBaseComponent.shaker->ShakeStart(VECTOR3(20, 20, 20), Shaker::HORIZONAL_SHAKE, true, 0.3f);
				InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(250, 60);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
				angleRan = (float)GetRand(360);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, 90.0f * DegToRad), VOne * 1.5f), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
				damage = damage * 5;
			}
			else
			{
				dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(20, 20, 20), 0.2f, 0.02f);
				InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(450, 20);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
				angleRan = (float)GetRand(360);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, angleRan * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
				enemyBaseComponent.playerObj->Component()->GetComponent<Shaker>()->ShakeStart(VECTOR3(200, 200, 200), Shaker::HORIZONAL_SHAKE, true, 0.05f);
			}
			hit = true;

			deadPreset = deadPresets[2];
			break;
		case EnemyInformation::EnemyReaction::Type::Special://必殺技
			if (!specialAttackHit) 
				return;
			
			InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(e.vibrationPower, e.vibrationType);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * e.hitEffectScaleRate), obj, e.hitEffectID, e.hitEffectTime);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, e.slashAngleRad), VOne), obj, e.slashEffectID, 1.0f);
			specialAttackHit = false;

			if (hp <= 0)
				enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_DAMAGE);
			damage = damage * 2;
			deadPreset = deadPresets[3];
			break;
		default:
			break;
		}
	}
	EnemyDamageMove(dInfo);
	//連携攻撃のときは耐性を付与
	if (isCooperateAtk)
		damage = damage / 5;
	VECTOR3 a = VECTOR3((float)GetRand(100), (float)(500 + GetRand(100)), (float)GetRand(100));
	hp -= DamageCalculation(a, damage, eStatus->GetStatus().defense,30);

	//ダメージか吹っ飛ばしの状態になっていたらダメージのパラメーターをいれる。
	std::shared_ptr<EnemyDamage> eD = enemyBaseComponent.state->GetState<EnemyDamage>();
	std::shared_ptr <EnemyBlowAway> eB = enemyBaseComponent.state->GetState<EnemyBlowAway>();
	SoundManager::GetInstance()->RandamSe("E_DamageV", 2);
	enemyBaseComponent.color->setRGB(Color::Rgb(255, 0, 0, 255));
	damageFlash = 0.5f;
}

void TrashEnemy::SetWayPoint(VECTOR3 _wayPoint)
{
	targetPoint = _wayPoint;
}

void TrashEnemy::SetCooperateWayPoint(VECTOR3 _pos, StateID::State_ID _id)
{
	if (hp <= 0)
		return;

	isCooperateAtk = true;
	cooperateWayPoint = _pos;
	nextCooperateID = _id;
	enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_RUN_S);
}

void TrashEnemy::ChangeState(StateID::State_ID _id)
{
	enemyBaseComponent.state->ChangeState(_id);
}

void TrashEnemy::AttackCommand()
{
	enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_ATTACK_S);
}

void TrashEnemy::CooperateAtkFinish()
{
	isCooperateAtk = false;
	isStandby = false;
	isMovingToPlayer = false;
	cAttack = false;
	
	enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_IDOL_S);
}

void TrashEnemy::AddPos(VECTOR3 _pos)
{
	obj->GetTransform()->position = obj->GetTransform()->position + _pos;
}
