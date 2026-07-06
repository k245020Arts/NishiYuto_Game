#pragma once
#include "../../EnemyState/EnemyStateBase.h"
#include "../Boss.h"
#include "Attack/BossAttackJsonParse.h"
#include "Attack/BossAttack.h"

struct ActionParam
{
	std::string id;
	bool attackState; // 攻撃のStateか
	int priority;     // プライオリティ
	int weight;       // 重さ
	int maxAction;    // 連続で何回行動できるか

	float distance = 0; // 距離によってその技が出やすいかどうか
	int addWeight = 0;  // 数字変動

	ActionParam() {
		id = "";
		attackState = false;
		priority = 1;
		weight = 0;
		maxAction = 1;
		distance = 0;
		addWeight = 0;
	}
};

// to_json
inline void to_json(JSON& j, const ActionParam& p)
{
	j = JSON{
		{"id", p.id},
		{"attackState", p.attackState},
		{"priority", p.priority},
		{"weight", p.weight},
		{"maxAction", p.maxAction},
		{"distance", p.distance},
		{"addWeight", p.addWeight}
	};
}

// from_json
inline void from_json(const JSON& j, ActionParam& p)
{
	j.at("id").get_to(p.id);
	j.at("attackState").get_to(p.attackState);
	j.at("priority").get_to(p.priority);
	j.at("weight").get_to(p.weight);
	j.at("maxAction").get_to(p.maxAction);

	// optional扱い（デフォルト値あり）
	if (j.contains("distance"))
		j.at("distance").get_to(p.distance);

	if (j.contains("addWeight"))
		j.at("addWeight").get_to(p.addWeight);
}


class AttackSorting :public EnemyStateBase
{
public:

	AttackSorting();
	~AttackSorting();
	void Update()override;

	void Start()override;
	void Finish()override;
	/// <summary>
	/// 強制的に次の攻撃のStateの決定をする
	/// </summary>
	/// <param name="_attackID"></param>
	void ForcedAttackStart(std::string _attackID);

	/// <summary>
	/// 攻撃情報のロードをする
	/// </summary>
	/// <param name="_bossName">ボスのID</param>
	/// <param name="_boss">ボスのポインタ</param>
	void Load(std::string _bossName, Boss* _boss);
	/// <summary>
	/// 攻撃の終了
	/// </summary>
	void AttackFinish();
	/// <summary>
	/// 今の攻撃状態を記録
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<EnemyAttackBase> GetNowAttackState();
	/// <summary>
	/// 攻撃抽選情報のセーブ
	/// </summary>
	/// <param name="_bossName">ボスのID</param>
	void SaveSorthing(std::string _bossName);
	/// <summary>
	/// 攻撃抽選情報のロード
	/// </summary>
	/// <param name="_bossName">ボスのID</param>
	void LoadSorting(std::string _bossName);
	/// <summary>
	/// 攻撃抽選情報の取得
	/// </summary>
	/// <returns></returns>
	std::vector<ActionParam> GetActionParam();
	/// <summary>
	/// 攻撃情報の取得
	/// </summary>
	/// <returns></returns>
	std::unordered_map<std::string, EnemyAttackBase::BossAttackParam> GetAttackParam();
	/// <summary>
	/// 攻撃の追加
	/// </summary>
	/// <param name="_param">攻撃情報</param>
	/// <param name="_boss">ボスのポインタ</param>
	void AddAttack(EnemyAttackBase::BossAttackParam _param, Boss* _boss);
	/// <summary>
	/// 攻撃の追加
	/// </summary>
	/// <param name="_param">攻撃情報</param>
	/// <param name="_boss">ボスのポインタ</param>
	/// <param name="_attackID">攻撃のID</param>
	void AddAttack(EnemyAttackBase::BossAttackParam _param,Boss* _boss,std::string _attackID);
	/// <summary>
	/// 攻撃情報のリロードをする
	/// </summary>
	/// <param name="_param"></param>
	/// <param name="_reLoadID"></param>
	void ReloadParam(EnemyAttackBase::BossAttackParam _param,std::string _reLoadID);

	//void StateImguiDraw()override;

private:
	const float COOLTIME = 0.5f;

	void NormalAttackSelect();
	void AttackStart();

	
	
	//int AttackPriority();
	
	/// <summary>
	/// 行動を決める
	/// </summary>
	/// <param name="_priority">今のボスの状況を入れる</param>
	void BuildTable(int _priority);
	//変動させた値をすべてゼロにする
	void AllAddWeightZero();

	float coolTime;

	Boss::HP_RATE hp;

	bool jump;
	int kind;
	int attackNum;
	bool nextAttack;

	/*struct ActionRange
	{
		int min;
		int max;
		StateID::State_ID id;
		ActionRange();
	};
	std::vector<ActionRange> table;*/

	int bossPriority;//
	std::string nextState;
	std::string copyState;

	int copyPriority;
	int moveCounter;
	bool forceAttack;

	VECTOR3 vec;
	std::unordered_map<std::string, std::shared_ptr<BossAttack>> attacks; //攻撃のポインターの保持
	std::unordered_map<std::string, EnemyAttackBase::BossAttackParam> attackParam; ///攻撃のパラメーターの保持
};