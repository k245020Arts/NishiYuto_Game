#pragma once
#include "../EnemyBase.h"
#include "../../Component/Object/Object3D.h"
#include "BossInformation.h"
#include "../EnemyInformation.h"
#include "../../Player/PlayerState/AttackState/PlayerAttack3.h"
#include "../../State/StateManager.h"

class Player;
class CharaWeapon;
class BossStatus;
class BossRockManager;
//class TrashEnemyManager;
//class PlayerAttack3;

class PlayerSpecialAttack;
class BossAttackDataSerializer;

class Boss : public EnemyBase
{
public:
	friend class AttackSorting;
	friend class BossCoolTime;
	friend class BossIdol;
	friend class BossRun;
	friend class BossWalk;
	friend class BossNormalAttack1;
	friend class BossNormalAttack2;
	friend class BossNormalAttack3;
	friend class BossNormalAttack4;
	friend class BossNormalAttack5;
	friend class BossNormalAttack6;
	friend class BossNormalAttack7;
	friend class BossSpecialAttack1;
	friend class BossSpecialSmallAttack1;
	friend class BossSpecialAttack2;
	friend class BossDie;
	friend class BossRoar;
	friend class BossThreat;
	friend class EnemyAttackBase;
	friend class BossAttack;
	friend class BossDamage;
	friend class BossAppear;
	friend class BossLose;
	friend class BossHalfSpecialAttack;
	friend class BossRockManager;
	friend class BossFear;
	friend class EnemyStateBase;
	friend class BossBackStep;
	
	enum HP_RATE
	{
		MAX,
		EIGHT,
		FIVE,
		THREE,
	};

	struct BossParam
	{
		int bossID;
		std::string bossName;
		std::string	modelName;
		float hp;
		float defense;

		BossParam(){
			bossID = 0;
			bossName = "";
			modelName = "";
			hp = 0.0f;
			defense = 0.0f;
		}
	};
	

	Boss();
	~Boss();
	void Update()override;
	void Draw()override;
	/// <summary>
	/// jsonを読み込み
	/// </summary>
	/// <param name="j">jsonデータ</param>
	/// <returns>jsonDataの中身</returns>
	EnemyInformation::EnemyReaction JsonRead(const JSON& j);
	/// <summary>
	/// ボスの開始
	/// </summary>
	/// <param name="_obj"></param>
	void Start(Object3D* _obj, const BossParam& _param);
	
	void ImguiDraw()override;
	
	/// <summary>
	/// プレイヤーの方に向く関数
	/// </summary>
	void LookPlayer();

	/// <summary>
	/// プレイヤーの方に向く関数
	/// </summary>
	/// <param name="speed">振り向くスピード</param>
	void LookPlayer(float speed);

	void LookPlayer(const VECTOR3& _target,float speed);

	/// <summary>
	/// 攻撃の当たり判定の開始
	/// </summary>
	/// <typeparam name="T">当たり判定のクラス名</typeparam>
	/// <param name="_shape">当たり判定の形</param>
	/// <param name="_trans">基準のTransform</param>
	/// <returns>当たり判定クラスのポインタ</returns>
	template<typename T>
	T* CollsionStart(CollsionSet* _set,const Transform& _trans, std::function<void(const CollsionEventData&)> _funk) {
		if (_set->instance == nullptr) {
			CollsionInfo info = CharaBase::CollsionInstant<T>(_set, _trans);
			info.tag = _set->tag;
			//collName = _set->collName;
			_set->instance->CollsionAdd(info, _trans, _funk,_set->collName);
		};
		return static_cast<T*>(_set->instance);
	}
	/// <summary>
	/// playerにダメージを食らった時の判定
	/// </summary>
	void PlayerHit(const CollsionEventData& _data)override;
	/// <summary>
	/// 攻撃の軌跡を生成
	/// </summary>
	/// <param name="_right"></param>
	void Drail(bool _right);
	/// <summary>
	/// HPの割合を取得
	/// </summary>
	/// <returns>HPの割合の列挙型</returns>
	HP_RATE Hp();
	/// <summary>
	/// ボスが移動をする
	/// </summary>
	/// <param name="_speed">スピード</param>
	/// <param name="_max">最大スピード</param>
	void MoveBoss(float _speed,float _max);
	/// <summary>
	/// 攻撃のクールタイムの取得
	/// </summary>
	/// <returns>攻撃のクールタイム</returns>
	float GetAttackCoolTime();

	/// <summary>
	/// ボスの攻撃をさせたいときに呼ぶ
	/// </summary>
	void BossAttackStateChange();
	/// <summary>
	/// プレイヤーの範囲内なら攻撃
	/// </summary>
	/// <returns>trueなら攻撃可能</returns>
	bool RunChangeAttack();
	/// <summary>
	/// 石にダメージを食らった時の処理
	/// </summary>
	void RockHitDamage(const CollsionEventData& _data);
	/// <summary>
	/// 突進でダメージを食らった時の処理
	/// </summary>
	void RockHitRushDamage(const CollsionEventData& _data);

	std::shared_ptr<PlayerAttack3> GetStateState() {
		return pState->GetState<PlayerAttack3>();
	}

	void BossDamageCollsionEvent(const CollsionEventData& _data);

private:
	void PlayerSpecialAttackHit(const EnemyInformation::EnemyReaction& _e, std::shared_ptr<PlayerSpecialAttack> _ps,VECTOR3 _randomPos,float _randomAngle);
	Boss* boss;
	BossStatus* bs;

	//BossInformation::BossComponent bossComponent;
	Transform* bossTransform;

	const int BOSS_RIGHT_HAND_FRAME = 15;
	const int BOSS_LEFT_HAND_FRAME = 11;

	const int BOSS_RIGHT_HAND_FRAME_1 = 61;
	const int BOSS_LEFT_HAND_FRAME_1 = 37;
	CharaWeapon* chara;

	int maxAttack;
	bool comboFirstAttack;

	HP_RATE hpRate;
	bool roaf;

	//TrashEnemyManager* trashEnemy;
	float coolTime;

	float attackNum;
	Player* player;
	bool threat;

	float noAttackChangeCounter;
	int alotAttack;
	bool oneDie;

	BossRockManager* rockManager;
	std::unique_ptr<BossAttackDataSerializer> bossAttackDataSerializer;

	BossParam bossParam;
	float defense;
	int bossID;
	std::string bossFileName;
	std::list<SphereCollider*> bossHitCollider;
	int rightHandFrame;
	int leftHandFrame;

	std::function<void(const CollsionEventData&)> attackFunk;
	std::function<void(const CollsionEventData&)> justAvoidAttackFunk;

	VECTOR3 GetDamageDrawPos();
};

// to_json
inline void to_json(JSON& j, const Boss::BossParam& p)
{
	j = JSON{
		{"bossID", p.bossID},
		{"bossName", p.bossName},
		{"modelName", p.modelName},
		{"hp", p.hp},
		{"defense", p.defense}
	};
}

// from_json
inline void from_json(const JSON& j, Boss::BossParam& p)
{
	// 必須扱い
	j.at("bossID").get_to(p.bossID);
	j.at("bossName").get_to(p.bossName);
	j.at("modelName").get_to(p.modelName);
	j.at("hp").get_to(p.hp);
	j.at("defense").get_to(p.defense);
}
