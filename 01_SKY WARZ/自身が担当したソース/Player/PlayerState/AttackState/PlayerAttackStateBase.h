#pragma once
#include "../PlayerStateBase.h"
#include "../../../Common/JsonReader.h"

class PlayerAttackStateBase : public PlayerStateBase
{
public:

	PlayerAttackStateBase();
	virtual ~PlayerAttackStateBase();

	virtual void Update()override;

	/// <summary>
	/// Enemyの方向に向く
	/// </summary>
	virtual void EnemyRotation();
	virtual void Start()override;
	virtual void Finish() override;
	/// <summary>
	/// playerの攻撃力を取得
	/// </summary>
	/// <returns></returns>
	float GetHitDamage()const { return playerAttackData.hitDamage; }
	
	/// <summary>
	/// 現在攻撃中かを取得
	/// </summary>
	/// <returns></returns>
	bool IsAttack();

	/// <summary>
	/// 攻撃の当たり判定の生成をある程度統一化させた関数
	/// </summary>
	void AttackCollsion();
	/// <summary>
	/// 当たり判定が直前に生成されたかを取得
	/// </summary>
	/// <returns>攻撃が生成されていたらtrue</returns>
	bool GetCollsionCreate() { return collsionCreate; }

	/// <summary>
	/// 必殺技を発動
	/// </summary>
	void SpecialAttackStart();

	struct ChargeAttackLevelData
	{
		float againTimer;        // 攻撃回数
		int   againTimerFlag;    // 再攻撃可能になるまでの時間
		VECTOR3 moveSpeed;		//移動スピード
		float hitDamage;		//攻撃力

		ChargeAttackLevelData()
			: againTimer(0.0f), againTimerFlag(0)
			, moveSpeed(0.0f), hitDamage(0.0f)
		{
		}

		ChargeAttackLevelData(float _againTimer, int _againTimerFlag,
			VECTOR3 _moveSpeed, float _hitDamage)
			: againTimer(_againTimer), againTimerFlag(_againTimerFlag)
			, moveSpeed(_moveSpeed), hitDamage(_hitDamage)
		{
		}
	};

	struct PlayerAttackData
	{
		StateID::State_ID state;							// 使用するステートID
		StateID::State_ID normalAttackNextID;				// 通常攻撃派生先ステートID
		StateID::State_ID specialAttackNextID;				// 特殊攻撃派生先ステートID

		float hitDamage;									// 攻撃ダメージ量
		float attackAgainStartCounterMax;					// 再攻撃可能になるまでの時間
		int attackNum;										// 攻撃回数
		float collsionStartTime;							// 攻撃判定開始時間
		float collsionFinishTime;							// 攻撃判定終了時間

		float motionCancelStartTime;						// モーションキャンセル開始時間
		float attackInputStartTime;							// 次攻撃入力受付開始時間
		float nextAttackRunTimer;							// 次段攻撃へ移行する入力猶予時間
		float noAttackRunTimer;								// 非攻撃状態へ戻るまでの時間

		VECTOR3 attackMove;									// 攻撃時移動量

		Transform collTrans;								// 攻撃判定Transform

		std::vector<ChargeAttackLevelData> chargeLevels;	// チャージ攻撃レベルデータ一覧

		const ChargeAttackLevelData* GetChargeLevel(int level) const
		{
			int idx = static_cast<int>(level); // LEVEL1=0, LEVEL2=1, LEVEL3=2
			if (idx < 0 || idx >= (int)chargeLevels.size()) return nullptr;
			return &chargeLevels[idx];
		}

		// デフォルトコンストラクタ
		PlayerAttackData()
			: state(StateID::State_ID::STATE_MAX)
			, normalAttackNextID(StateID::State_ID::STATE_MAX)
			, specialAttackNextID(StateID::State_ID::STATE_MAX)
			, hitDamage(0.0f)
			, attackAgainStartCounterMax(0.0f)
			, attackNum(0)
			, collsionStartTime(0.0f)
			, collsionFinishTime(0.0f)
			, motionCancelStartTime(0.0f)
			, attackInputStartTime(0.0f)
			, nextAttackRunTimer(0.0f)
			, noAttackRunTimer(0.0f)
			, attackMove(VZero)
			, collTrans()
			, chargeLevels()
		{
		}

		// 全引数コンストラクタ
		PlayerAttackData(
			StateID::State_ID state,
			StateID::State_ID normalAttackNextID,
			StateID::State_ID specialAttackNextID,
			float hitDamage,
			float attackAgainStartCounterMax,
			int attackNum,
			float collsionStartTime,
			float collsionFinishTime,
			float motionCancelStartTime,
			float _attackInput,
			float _nextAttackRunTimer,
			float _noAttackRunTimer,
			VECTOR3 _attackMove,
			const Transform& collTrans,
			const std::vector<ChargeAttackLevelData>& chargeLevels = {}
		)
			: state(state)
			, normalAttackNextID(normalAttackNextID)
			, specialAttackNextID(specialAttackNextID)
			, hitDamage(hitDamage)
			, attackAgainStartCounterMax(attackAgainStartCounterMax)
			, attackNum(attackNum)
			, collsionStartTime(collsionStartTime)
			, collsionFinishTime(collsionFinishTime)
			, motionCancelStartTime(motionCancelStartTime)
			, attackInputStartTime(_attackInput)
			, nextAttackRunTimer(_nextAttackRunTimer)
			, noAttackRunTimer(_noAttackRunTimer)
			, attackMove(_attackMove)
			, collTrans(collTrans)
			, chargeLevels(chargeLevels)
		{
		}
	};

	const PlayerAttackData& GetAttackData()const { return playerAttackData; }

	void SetAttackData();

protected:
	float easingCount;
	float beforeAngle;
	float angle;
	bool nextAttack;
	bool nextAvoid;
	bool frontMove;
	
	VECTOR3 dist;
	VECTOR3 norm;
	float distSize;
	float time;

	const float ATTACK_MOVE_DIST = 3000.0f;
	const float DISTANCE_MOVE = 800;

	bool beforeAttack;
	bool normal;
	bool special;

	float runTimer;

	const float ATTACK_FINISH_ANIM_SPEED = 0.1f;
	bool rotation;

	
	bool defalutTrail;
	bool rockOn;

	int attackCount;

	float attackAgainStartCounter;
	bool speedChange;
	//float attackAgainStartCounterMax;

	
	bool collsionCreate;

	void BaseAttackCollsion();
	void AttackMoveStart();
	void AttackCommonUpdate();

	void AgainAttackCollsion();
	void AgainTimerSet(float _time, int _attackNum);

	PlayerAttackData playerAttackData;
};

inline void to_json(nlohmann::json& j, const PlayerAttackStateBase::ChargeAttackLevelData& p) {
	j["againTimer"] = p.againTimer;
	j["againTimerFlag"] = p.againTimerFlag;
	j["MoveSpeed"] = p.moveSpeed;
	j["hitDamage"] = p.hitDamage;
}

inline void from_json(const nlohmann::json& j, PlayerAttackStateBase::ChargeAttackLevelData& p) {
	j.at("againTimer").get_to(p.againTimer);
	j.at("againTimerFlag").get_to(p.againTimerFlag);
	if (j.contains("MoveSpeed")) {
		j.at("MoveSpeed").get_to(p.moveSpeed);
	}
	j.at("hitDamage").get_to(p.hitDamage);
}

inline void to_json(nlohmann::json& j, const PlayerAttackStateBase::PlayerAttackData& p) {
	j = {
		{"PlayerState",                StateID::GetID(p.state)},
		{"normalAttackNextID",         StateID::GetID(p.normalAttackNextID)},
		{"specialAttackNextID",        StateID::GetID(p.specialAttackNextID)},
		{"hitDamage",                  p.hitDamage},
		{"attackAgainStartCounterMax", p.attackAgainStartCounterMax},
		{"attackNum",                  p.attackNum},
		{"collsionStartTime",          p.collsionStartTime},
		{"collsionFinishTime",         p.collsionFinishTime},
		{"motionCancelStartTime",      p.motionCancelStartTime},
		{"attackInputStartTime",	   p.attackInputStartTime},
		{"nextAttackRunTimer",         p.nextAttackRunTimer},
		{"noAttackRunTimer",           p.noAttackRunTimer},
		{"AttackMove",                 p.attackMove},
		{"collTrans",                  p.collTrans},
		{"chargeLevels",               p.chargeLevels},
	};
}
inline void from_json(const nlohmann::json& j, PlayerAttackStateBase::PlayerAttackData& p) {
	p.state = StateID::StringToID(j["PlayerState"].get<std::string>());
	p.normalAttackNextID = StateID::StringToID(j["normalAttackNextID"].get<std::string>());
	p.specialAttackNextID = StateID::StringToID(j["specialAttackNextID"].get<std::string>());
	j.at("hitDamage").get_to(p.hitDamage);
	j.at("attackAgainStartCounterMax").get_to(p.attackAgainStartCounterMax);
	j.at("attackNum").get_to(p.attackNum);
	j.at("collsionStartTime").get_to(p.collsionStartTime);
	j.at("collsionFinishTime").get_to(p.collsionFinishTime);
	j.at("motionCancelStartTime").get_to(p.motionCancelStartTime);
	if (j.contains("attackInputStartTime")) {
		j.at("attackInputStartTime").get_to(p.attackInputStartTime);
	}
	if (j.contains("nextAttackRunTimer")) {
		j.at("nextAttackRunTimer").get_to(p.nextAttackRunTimer);
	}
	if (j.contains("noAttackRunTimer")) {
		j.at("noAttackRunTimer").get_to(p.noAttackRunTimer);
	}
	if (j.contains("AttackMove")) {
		j.at("AttackMove").get_to(p.attackMove);
	}
	j.at("collTrans").get_to(p.collTrans);

	// チャージ非対応のStateはキーなしでもOK
	if (j.contains("chargeLevels")) {
		j.at("chargeLevels").get_to(p.chargeLevels);
	}
}

