#pragma once
#include "../CharaBase/CharaBase.h"
#include "playerInformation.h"
#include "../Component/Transform/Transform.h"
#include "../Common/ID/ID.h"
#include  "../Common/ID/EffectID.h"
#include "../Common/ID/SoundID.h"
#include "PlayerState/AttackState/PlayerAttackStateBase.h"
#include <unordered_set>

class EnemyAttackBase;
class BossRockManager;
struct PlayerAttackData;
class PlayerParamWindow;

class Player : public CharaBase
{
public:
	friend class PlayerAvoid;
	friend class PlayerBlowAway;
	friend class PlayerDamage;
	friend class PlayerDie;
	friend class PlayerJustAvoid;
	friend class PlayerWait;
	friend class PlayerWalk;
	friend class PlayerAttack1;
	friend class PlayerJustAvoidAttack1;
	friend class PlayerAttack2;
	friend class PlayerJustAvoidAttack1;
	friend class PlayerAttack3;
	friend class PlayerAttack5;
	friend class PlayerAttack4;
	friend class PlayerJustAvoidAttack4;
	friend class PlayerAttack5;
	friend class PlayerJustAvoidAttack5;
	friend class PlayerAttackStateBase;
	friend class PlayerStateBase;
	friend class PlayerTurn;
	friend class PlayerWin;
	friend class PlayerSpecialAttack;
	friend class PlayerBossAppear;
	friend class PlayerHeavyCharge;
	friend class PlayerHeavyAttack;
	friend class PlayerLose;

	Player();
	~Player();

	void Update()override;
	void Draw()override;
	void Start(Object3D* _obj);
	/// <summary>
	/// 移動の関数
	/// </summary>
	/// <param name="_speed">移動</param>
	/// <param name="_speedMax">最大移動速度</param>
	void Move(float _speed,float _speedMax);
	/// <summary>
	/// プレイヤーの回転を滑らかに行うために行っている関数、自由指定ver
	/// </summary>
	/// <param name="_angle">強制する角度</param>
	/// <param name="_speed">速度</param>
	void RotationChange(VECTOR3 _angle,float _speed);
	/// <summary>
	/// プレイヤーの回転を滑らかに行うために行っている関数,、デフォルト値ver
	/// </summary>
	void RotationChange();
	/// <summary>
	/// 回避の向き指定とか移動とかの処理を行っている関数
	/// </summary>
	/// <param name="_speed">速度</param>
	/// <param name="_speedMax">最大速度</param>
	/// <param name="cameraAngle">カメラの角度</param>
	/// <param name="_upSpeed">プレイヤーの上昇速度</param>
	void Avoid(float _speed, float _speedMax,float cameraAngle,float _upSpeed);
	void ImguiDraw()override;
	/// <summary>
	/// プレイヤーの移動のスティックの値を得られる関数
	/// </summary>
	void PlayerStickInput();

	/// <summary>
	/// 歩きのスティックの向きを取得
	/// </summary>
	/// <returns>左スティックの向き</returns>
	VECTOR3 GetWalkAngle()const { return walkAngle; }
	/// <summary>
	/// 回避がスタートできるかを取得
	/// </summary>
	/// <returns>trueなら回避の開始</returns>
	bool GetAvoidStart()const { return avoidStart; }
	/// <summary>
	/// 回避の開始を設定する関数
	/// </summary>
	/// <param name="_set">trueなら回避の開始</param>
	void SetAvoidStart(bool _set) { avoidStart = _set; }
	/// <summary>
	/// 回避が始まるときのちょっとした隙をつくるかんすう
	/// </summary>
	void AvoidReady();
	/// <summary>
	/// 回避した時の回転のチェンジをしている関数
	/// </summary>
	void AvoidRotationChange();
	
	/// <summary>
	/// 敵の攻撃を食らった時の処理
	/// </summary>
	/// <param name="_attackId">敵の攻撃のID</param>
	/// <param name="_obj">敵のオブジェクト</param>
	/// <returns>ダメージを受けたかどうか</returns>
	bool EnemyHit(ID::IDType _attackId, BaseObject* _obj);
	/// <summary>
	/// playerのtransformを取得
	/// </summary>
	/// <returns>playerのtransform</returns>
	Transform* GetPlayerTransform()const{ return playerTransform; }
	/// <summary>
	/// プレイヤーのオブジェクトを取得
	/// </summary>
	/// <returns>プレイヤーのオブジェクトを取得</returns>
	BaseObject* GetPlayerObj() { return obj; }
	/// <summary>
	/// カメラのターゲットをセットする関数
	/// </summary>
	/// <param name="_base"></param>
	void TargetObjSet(BaseObject* _base);
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	//BaseObject* GetTargetObj()const { return playerCom.targetObj; }
	/// <summary>
	/// 攻撃を食らったりしたときとか、ジャスト回避した敵の種類をエル関数
	/// </summary>
	/// <param name="_base"></param>
	void HitObjectSet(BaseObject* _base);
	/// <summary>
	/// 攻撃の当たり判定の開始
	/// </summary>
	/// <typeparam name="T">当たり判定のクラス名</typeparam>
	/// <param name="_shape">当たり判定の形</param>
	/// <param name="_trans">基準のTransform</param>
	/// <returns>当たり判定クラスのポインタ</returns>
	template<typename T>
	T* CollsionStart(CollsionSet* _set, const Transform& _trans) {
		if (_set->instance == nullptr) {
			CollsionInfo info = CharaBase::CollsionInstant<T>(_set, _trans);
			info.tag = _set->tag;
			//collName = _set->collName;
			std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) {this->CollsionAttackEvent(_data); };
			_set->instance->CollsionAdd(info, _trans, func,_set->collName);
		};
		return static_cast<T*>(_set->instance);
	}
	/// <summary>
	/// プレイヤーの攻撃が当たった時のリアクション
	/// </summary>
	void PlayerAttackHit();
	/// <summary>
	/// プレイヤーが振動しているかどうか
	/// </summary>
	/// <returns>振動していたらtrue</returns>
	bool IsShake()const ;
	/// <summary>
	/// ジャスト回避中か取得する関数
	/// </summary>
	/// <returns></returns>
	bool GetJustAvoid() const{ return justAvoid; }
	/// <summary>
	/// PlayerのStateManagerを取得する関数
	/// </summary>
	/// <returns></returns>
	StateManager* GetPlayerStateManager()const;

	/// <summary>
	/// ジャスト回避が終わった時に通る関数
	/// </summary>
	void AvoidFinishState();

	/// <summary>
	/// 軌跡表示の関数で引数なしはdefalutのものが呼び出される。
	/// 特別仕様にカスタマイズしたいなら下の関数を使用する
	/// </summary>
	void DrawTrail();
	/// <summary>
	/// パラメーターセット軌跡
	/// </summary>
	/// <param name="_nPos">剣の末端</param>
	/// <param name="_fPos">剣先</param>
	/// <param name="_r">赤成分</param>
	/// <param name="_g">緑成分</param>
	/// <param name="_b">青成分</param>
	/// <param name="_a">半透明成分</param>
	/// <param name="index">剣を持っている手のindex</param>
	/// <param name="_time">何秒表示するか</param>
	void DrawTrail(const VECTOR3& _nPos, const VECTOR3& _fPos, float _r, float _g, float _b, float _a, int index, float _time);
	/// <summary>
	/// 当たり判定の削除
	/// </summary>
	void DeleteCollision(CollsionSet* _set) override;
	/// <summary>
	/// 敵が出す妨害オブジェクトに当たった時に通る関数
	/// </summary>
	/// <param name="_obj">妨害オブジェクトのオブジェクト</param>
	/// <returns>当たっているかどうか</returns>
	bool EnemyAttackObjectHitIsPlayer(BaseObject* _obj,CollsionInformation::Tag _tag);
	/// <summary>
	/// ボスが威嚇しているかどうか
	/// </summary>
	/// <returns>威嚇していたらtrue</returns>
	bool GetBossThreat()const  { return bossThreat; }
	/// <summary>
	/// ボスの威嚇状態を終了させる
	/// </summary>
	void BossThreatFinish() { bossThreat = false; }

	/// <summary>
	/// 大きいジャスト回避をするかどうか
	/// </summary>
	/// <param name="_attack">アタックID</param>
	/// <returns>大きいジャスト回避ならtrued</returns>
	bool LargeJustAvoid(std::shared_ptr<EnemyAttackBase> _attack);

	/// <summary>
	/// 必殺技の円の中心点を取得する関数
	/// </summary>
	/// <returns>必殺技の円の中心点</returns>
	const VECTOR3& GetSpecialAttackCenterPos()const { return specialAttackCenterPos; }
	/// <summary>
	/// 必殺技の円の中心点を取得する関数
	/// </summary>
	/// <returns>必殺技の円の中心点</returns>
	const VECTOR3& GetSpecialAttackStartPos()const { return specialAttackStartPos; }

	/// <summary>
	/// 必殺技ゲージの増殖
	/// </summary>
	/// <param name="_add"></param>
	void SpecialVarAdd(float _add);
	/// <summary>
	/// 現在攻撃の溜めが行われているかどうか
	/// </summary>
	/// <returns></returns>
	bool GetCharge() { return charge; }

	/// <summary>
	///　攻撃が岩に当たった時に処理する
	/// </summary>
	void AttackRockHit();

	/// <summary>
	/// ジャスト回避当たり判定が当たった処理
	/// </summary>
	/// <param name="_obj"></param>
	/// <param name="_tag"></param>
	void JustAvoidCollsionHit(BaseObject* _obj,CollsionInformation::Tag _tag);

	/// <summary>
	/// リアクションの取得
	/// </summary>
	/// <returns></returns>
	std::unordered_map<StateID::State_ID, PlayerInformation::PlayerReaction>& GetReactionMap();
	/// <summary>
	/// プレイヤーの攻撃のデータを取得
	/// </summary>
	/// <returns></returns>
	std::unordered_map<StateID::State_ID, PlayerAttackStateBase::PlayerAttackData>& GetAllAttackData();

	/// <summary>
	/// 攻撃のパラメータのリロード
	/// </summary>
	void ParamReLoad();
	/// <summary>
	/// 攻撃のリアクションのパラメータのリロード
	/// </summary>
	void ReactionReLoad();
	/// <summary>
	/// 当たり判定のイベントを呼び出す用の関数
	/// </summary>
	/// <param name="_data"></param>
	void CollsionEvent(const CollsionEventData & _data);

	void CollsionAttackEvent(const CollsionEventData& _data);

	void HitObjectInsert(BaseObject* _base) { hitObjects.insert(_base); }
	bool IsHitObject(BaseObject* _base) { return hitObjects.count(_base) > 0; }

private:
	PlayerInformation::CharaComponent playerCom;
	float size;
	Transform* playerTransform;
	VECTOR3 walkAngle;
	bool avoidStart;
	bool avoidReady;
	bool enemyHit;
	float justAvoidCanCounter;
	bool justAvoid;
	int avoidCounter;
	float noAvoidCounter;
	float avoidReadyCounter;

	const float MAX_HP = 2000;
	float redCounter;
	int justAvoidBlurImage;
	float justFeedInTime;
	float justFeedOutTime;
	const float JUST_FEED_IN_TIME = 1.0f;
	const float JUST_FEED_OUT_TIME = 1.0f;

	bool largeJustAvoid;
	bool bossThreat;
	bool noDamage;
	bool turn;
	VECTOR3 specialAttackCenterPos;
	VECTOR3 specialAttackStartPos;

	Transform attackTargetTrans;
	bool charge;

	void HeavyAttackChangeParam(int _level);

	int attackLevel;
	bool specialAttackGuageMax;
	bool objHit;

	BossRockManager* bossRockManager;

	bool justAvoidCan;
	bool justAvoidColHit;

	std::unordered_map<StateID::State_ID, PlayerAttackStateBase::PlayerAttackData> attackData;

	PlayerAttackStateBase::PlayerAttackData GetAttackDataMap(StateID::State_ID _data) { return attackData[_data]; }

	std::unordered_set<BaseObject*> hitObjects;

	void DataSavePlayerState();
	void DataLoadPlayerState();
	Object2D* specialTextObj;
	Object2D* hpTextObj;
	float hpUIMoveCounter;
	Physics* specialTextPhysics;
	float specialMoveCounter;
	void HpUIUpdate();
	void SpecialUIUpdate();

	std::unique_ptr<PlayerParamWindow> paramWindow;

	void KeyBoardPlayerRotate();
};

