#pragma once
#include "../../../Library/GameObject.h"
#include <list>
#include "TrashEnemyManager.h"

class TrashEnemyManager;
class TrashEnemy;
class Camera;

class TrashEnemyGroup : public GameObject
{
public:
	TrashEnemyGroup();
	~TrashEnemyGroup();

	void Update()override;
	void Draw()override;

	//近距離の敵だけを作る
	void SetMeleeEnemy(TrashEnemy* _enemy);
	//遠距離の敵だけを作る
	void SetRangedEnemy(TrashEnemy* _enemy);

	//生きてるすべての敵を数える関数
	int GetActiveEnemy()const;
	//生きてる近距離の敵を数える関数
	int GetMeleeActiveEnemy()const;
	//生きてる遠距離の敵を数える関数
	int GetRangedActiveEnemy()const;

	//一番近いウェイポイントを計算する
	void CloseWayPoint(std::vector<WayPoint>& wayPoint);
	//遠距離の敵の連携攻撃
	void RangedEnemyAttack();

	//近距離の敵を強制的に倒す処理
	void DeadMeleeEnemy();
	/// <summary>
	/// 遠距離の敵を強制的に倒す処理
	/// </summary>
	/// <param name="_readerDead">リーダーだけ倒すか</param>
	void DeadRangedEnemy(bool _readerDead);

	//遠距離のリーダーのポジション
	VECTOR3 RangedLeaderPosition()const { return leaderPos; }
	VECTOR3 HitEnemyPosition()const {return hitEnemyPos; }
private:
	Camera* camera;
	TrashEnemyManager* trashEnemyManager;

	//近距離の敵
	std::vector<TrashEnemy*> meleeEnemies;
	//遠距離の敵
	std::vector<TrashEnemy*> rangedEnemies;
	//全部の敵の情報
	std::list<TrashEnemy*> allEnemy;

	const int ATK_COUNTER_MIN = 2;
	const float ATK_COUNTER_MAX = 3.0f;
	/// <summary>
	/// 指定した敵のリストの中身を見て敵がやられてたらリストから削除する
	/// </summary>
	/// <param name="enemies">敵のリスト</param>
	void EnemyDead(std::vector<TrashEnemy*>& enemies);
	//雑魚的どうしであたった時の押し返し
	void Separation();

	void InCameraWayPoint(WayPoint& _wayPoint);

	//近距離の敵で使う----------
	
	//敵の攻撃
	void MeleeEnemyAttack(TrashEnemy* _enemy);
	//近距離の敵の走るポイントを決める
	void EnemiesRun(TrashEnemy* _enemy);
	//近距離の敵の連携攻撃
	void CooperateAttackMove(TrashEnemy* _enemy);
	//近距離の敵のステートを指定したステートに全員変える処理
	void AllChangeMeleeState(StateID::State_ID _id);
	//連携時に連携する敵を線でつなげる
	void CooperateAttackLine();
	//遠距離の敵の攻撃時に敵が捌けるようにする
	void MeleeEvadeMove(TrashEnemy* _enemy);

	float attackCounter;
	float maxAttackCounter;
	float standbyCounter;
	//連携攻撃でこのカウントが一定の値を超えると強制的に攻撃し始める
	float cooperateCounter;

	int enemiesRunCounter;

	//bool cooperateAtk;

	//--------------------------

	//遠距離の敵----------------

	//敵の待機の挙動
	void RangedEnemySetWaypoint(TrashEnemy* _enemy);
	//連携攻撃を失敗したときの倒される処理
	void RangedDamageMove();
	//遠距離の敵のステートを指定したステートに全員変える処理
	void AllChangeRangedState(StateID::State_ID _id);
	//遠距離の敵のリーダーがやられたとき次のリーダーを決める関数
	void NextLeader();
	//連携攻撃を打ち返された後の処理
	void DeadRangedEnemy(TrashEnemy* _enemy);
	//攻撃時のリーダーの動き
	void AttackLeaderMove(TrashEnemy* _enemy);
	//攻撃時の敵の動き
	void AttackRangedMove(TrashEnemy* _enemy);
	//攻撃で使う変数をリセット
	void EndRangedAttack(TrashEnemy* _enemy);

	VECTOR3 leaderPos;
	VECTOR3 hitEnemyPos;

	float leaderRotY;
	float rangedAtkTime;
	bool rangedDamageMove;
	int rangedAtkCounter;
	//連携開始時のリーダー以外の敵が何体いるか
	int rangedJoinCounter;
	//遠距離の敵の攻撃
	bool startRangedAtk;

	bool startButtonImage;
	//打ち返せの画像
	int yButtonImage;
	//打ち返せ画像のポジション
	const VECTOR3 YButtonPos = VECTOR3(1100.0f, 200.0f, 0.0f);

	const float MaxAttackCounter = 2.5f;
	bool leaderActiveEnd;
	//打ち返されたかどうか
	bool hitBack;
	//連続で連携攻撃を始めないようにする
	float rangedAtkCoolTime;
	//最低何秒間攻撃しないか
	const float MaxCoolTime = 10.0f;

	//--------------------------
};