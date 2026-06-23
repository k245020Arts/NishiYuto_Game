#pragma once
#include "../../EnemyState/EnemyStateBase.h"
#include "../../../Common/ID/EffectID.h"
//#define DataSave
#include "../../../Component/Collider/CollsionInformation.h"

class SphereCollider;
class Boss;

class EnemyAttackBase: public EnemyStateBase
{
public:

	struct RayColliderInfo
	{
		float rayStartPos; //Rayの判定の開始の座標
		float rayFinishPos; //Rayの判定の終了の座標
		RayColliderInfo() {
			rayStartPos = 0.0f;
			rayFinishPos = 0.0f;
		}
	};

	struct DountColliderInfo
	{
		float inRadius; //内側の半径
		float outRadius; //外側の円の半径
		DountColliderInfo() {
			inRadius = 0.0f;
			outRadius = 0.0f;
		}
	};

	struct SoundEffectEvent
	{
		std::string name;
		float soundStartFrame;
		float soundStopFrame;
		bool loop;
		bool soundStopFrameAfetrSoundPlay;
		bool loopAnim;
		SoundEffectEvent() {
			name = "";
			soundStartFrame = 0.0f;
			soundStopFrame = 0.0f;
			loop = false;
			soundStopFrameAfetrSoundPlay = false;
			loopAnim = false;
		}
	};

	struct ThrowObjectAttackData
	{
		// 基本データ
		std::string throwObjectID;						// 投擲オブジェクトID
		VECTOR3 baseGravity;							// 基本重力
		VECTOR3 baseFirction;							// 基本摩擦

		// 押し返し当たり判定
		bool pushCollCan;								// 押し返し判定を有効にするか
		Transform pushCollTransform;					// 押し返し判定用Transform

		// 地面関連
		bool randCan;									// 地面着地判定を行うか
		RayColliderInfo randCollInfo;					// 地面判定用Ray情報
		float randTime;									// 着地後に消えるまでの時間

		// プレイヤーヒット判定
		bool playerHit;									// プレイヤーに当たるか
		bool playerHitNoReaction;						// プレイヤーに当たってもリアクションしないか
		float playerHitCollRadius;						// プレイヤーヒット判定半径
		float playerHitJustAvoidCollRadius;				// ジャスト回避判定半径
		bool capsule;									// カプセルコライダーを使用するか
		VECTOR3 capselEndPos;							// カプセル終点座標
		VECTOR3 capselStartPos;							// カプセル始点座標
		bool capsuleEndPosExtend;						// カプセル終点座標を延長するか
		VECTOR3 capsuleEndPosExtendOffset;				// カプセル終点座標への追加オフセット
		bool capsuleEndPosExtendFromThrowStart;			// 投擲開始時から終点延長を有効化するか
		float capsuleEndPosExtendStartAnimFrame;		// 指定アニメーションフレームから終点延長を開始

		// 多段ヒット
		bool alotCollsionHit;							// 多段ヒットを行うか
		float alotHitColliderCreateCounter;				// 多段ヒット判定生成間隔

		// 地面設置後プレイヤーヒット
		bool playerGroundHit;							// 地面設置後もプレイヤーに当たるか
		bool playerGroundOneHit;						// 地面設置後のヒットを1回だけにするか
		bool playerGroundNoDamageReactionHit;			// 地面設置後ヒット時にリアクション無しにするか
		float playerGroundCollRadius;					// 地面設置後ヒット判定半径

		// ボスヒット判定
		bool bossHit;									// ボスにダメージを与えるか
		float bossHitCollRadius;						// ボスヒット判定半径

		// プレイヤー攻撃による弾き返し
		bool playerAttackFlying;						// プレイヤー攻撃で弾き返せるか
		float playerAttackFlyingCollRadius;				// 弾き返し判定半径
		float flyingSpeed;								// 弾き返し時の飛行速度
		float flyingHeight;								// 弾き返し時の飛行高度

		// ボス突進ヒット
		bool bossRushHit;								// ボス突進攻撃でひるむか
		float bossRushHitCollRadius;					// ボス突進ヒット判定半径

		// 爆発関連
		bool blastCan;									// 爆発を行うか
		DountColliderInfo blastColliderInfo;			// 爆発判定情報
		DountColliderInfo blastJustAvoidColliderInfo;	// ジャスト回避用爆発判定情報
		float blastBlinkMaxCounter;						// 爆発点滅演出時間
		bool randomBlast;								// ランダム爆発を行うか
		float randomBlastRate;							// ランダム爆発発生率
		float maxRadius;								// 爆発最大半径
		float waveSpeed;								// 爆風拡大速度

		// ランダム高さ
		bool randomHeight;								// ランダム高さ変更を行うか
		float minHeight;								// 最小高さ
		float maxHeight;								// 最大高さ

		// ランダム速度
		bool randomSpeed;								// ランダム速度変更を行うか
		float minSpeed;									// 最小速度
		float maxSpeed;									// 最大速度

		// 予測線関連
		bool predictionCicleCan;						// 予測サークルを表示するか
		RayColliderInfo predictionCicleColliderInfo;	// 予測サークル判定情報

		// 投擲関連
		bool armThrow;									// 腕から投げるか
		bool armSwordHand;								// 剣を持つ腕を使用するか
		int armFrameNum;								// 投擲を行うアニメーションフレーム番号
		float throwStartTime;							// 投擲開始時間
		float throwObjectApperaTime;					// 投擲オブジェクト出現時間
		VECTOR3 armAddPos;								// 腕位置からの追加オフセット
		bool throwToPlayer;								// プレイヤー方向へ投げるか
		bool thorwToFront;								// 前方向へ投げるか
		float throwSpeed;								// 投擲速度
		float upSpeed;									// 上方向速度
		float throwFirstSpeed;							// 初速
		VECTOR3 diffusionAngle;							// 拡散角度

		// 放物線投擲
		bool throwToFall;								// 落下する投擲を行うか
		float throwHeight;								// 投擲最高高度
		float throwFallGravity;							// 落下重力
		bool throwToFallToPlayer;						// プレイヤー位置へ落下させるか

		// 自由方向投擲
		bool freeDir;									// 自由方向ベクトルを使用するか
		VECTOR3 thorwStartPos;							// 投擲開始位置
		VECTOR3 thorwVelocity;							// 投擲速度ベクトル

		// 消滅関連
		bool groundDelete;								// 地面接触後に削除するか
		bool playerAttackObjectDrop;					// プレイヤー攻撃時にオブジェクトを落とすか
		bool attackFinishDelete;						// 攻撃終了時に削除するか
		

		ThrowObjectAttackData()
		{
			throwObjectID = "";

			baseGravity = VECTOR3();
			baseFirction = VECTOR3();

			pushCollCan = false;
			pushCollTransform = Transform();

			randCan = false;
			randCollInfo = RayColliderInfo();
			randTime = 0.0f;

			playerHit = false;
			playerHitCollRadius = 0.0f;
			playerHitJustAvoidCollRadius = 0.0f;
			capsule = false;
			capselEndPos = VZero;
			capselStartPos = VZero;


			playerGroundHit = false;
			playerGroundCollRadius = 0.0f;

			bossHit = false;
			bossHitCollRadius = 0.0f;

			playerAttackFlying = false;
			playerAttackFlyingCollRadius = 0.0f;
			flyingSpeed = 0.0f;
			flyingHeight = 0.0f;
			maxRadius = 0.0f;
			waveSpeed = 0.0f;

			bossRushHit = false;
			bossRushHitCollRadius = 0.0f;

			blastCan = false;
			blastColliderInfo = DountColliderInfo();
			blastJustAvoidColliderInfo = DountColliderInfo();
			blastBlinkMaxCounter = 0.0f;
			randomBlast = false;
			randomBlastRate = 0.0f;
			randomHeight = false;
			minHeight = 0.0f;
			maxHeight = 0.0f;
			randomSpeed = false;
			minSpeed = 0.0f;
			maxSpeed = 0.0f;

			predictionCicleCan = false;
			predictionCicleColliderInfo = RayColliderInfo();

			armThrow = false;
			armFrameNum = 0;
			throwStartTime = 0.0f;
			throwObjectApperaTime = 0.0f;
			armAddPos = VECTOR3();
			throwToPlayer = false;
			thorwToFront = false;
			throwSpeed = 0.0f;
			upSpeed = 0.0f;
			throwFirstSpeed = 0.0f;
			diffusionAngle = VECTOR3();
			throwToFall = false;
			throwHeight = 0.0f;
			throwFallGravity = 0.0f;
			throwToFallToPlayer = false;
			freeDir = false;
			thorwStartPos = VECTOR3();
			thorwVelocity = VECTOR3();

			groundDelete = false;

			playerAttackObjectDrop = false;
			armSwordHand = false;

			capsuleEndPosExtend = false;
			capsuleEndPosExtendOffset = VZero;
			capsuleEndPosExtendFromThrowStart = false;
			capsuleEndPosExtendStartAnimFrame = 0.0f;

			attackFinishDelete = false;
			playerHitNoReaction = false;
			alotCollsionHit = false;
			alotHitColliderCreateCounter = 0.0f;
		}
	};

	enum PlayerDamagePattern
	{
		NONE = -1,
		NO_BACK = 0,
		BACK,
		BLOW_AWAY,
	};

	static std::string ToString(PlayerDamagePattern p)
	{
		switch (p)
		{
		case NO_BACK:    return "NO_BACK";
		case BACK:       return "BACK";
		case BLOW_AWAY:  return "BLOW_AWAY";
		case NONE:       return "NONE";
		}
		return "NONE";
	}

	static PlayerDamagePattern FromString(const std::string& s)
	{
		if (s == "NO_BACK")    return NO_BACK;
		if (s == "BACK")       return BACK;
		if (s == "BLOW_AWAY")  return BLOW_AWAY;
		if (s == "NONE")       return NONE;

		// 不正データ対策
		return NONE;
	}

	struct BossAttackParam
	{
		BossAttackParam()
		{
			bossID = "";
			attackID = "";
			animNum = 0;
			animFileName = "";

			useFlash = false;
			attackFlashStartTime = 0.0f;

			slowTime = 0.0f;
			slowAmout = 0.0f;

			speedUpMotionSpeed = 0.0f;

			attackCollsionStartTime = 0.0f;
			attackCollsionEndTime = 0.0f;

			justAvoidCollsionStartTime = 0.0f;
			justAvoidCollsionEndTime = 0.0f;

			attackPositionFrameNum = 0;

			attackSoundStartTime = 0.0f;

			attackCollTransform = Transform();
			justAvoidCollTransform = Transform();

			animID = ID::IDType();
			attackBeforeAnimID = ID::ID_MAX;

			hitDamage = 0.0f;
			damagePattern = NONE;

			voiceName = "";

			// 移動イベント
			frontMove = false;
			moveSpeed = 0.0f;

			// プレイヤー追従イベント
			playerAloowMove = false;
			baseFirstSpeed = 0.0f;
			baseSpeed = 0.0f;
			playerNearStop = false;
			playerNearAloowStop = false;
			addVelocity = false;
			playerBaseNear = 0.0f;
			maxMoveSpeed = 0.0f;
			minMoveSpeed = 0.0f;
			moveStartTime = 0.0f;
			moveFinishTime = 0.0f;

			// 突進イベント
			rushMove = false;
			rushAfterAnimID = ID::ID_MAX;
			rushAfterSpeed = 0.0f;
			rushSoundRightFoot = 0.0f;
			rushSoundLeftFoot = 0.0f;
			rushTime = 0.0f;
			rushColl = false;
			addRushCollScale = 0.0f;

			// 回転イベント
			rotateMove = false;
			angleMoveAmout = 0.0f;

			// ジャンプイベント
			jump = false;
			addGravity = 0.0f;
			jumpSpeed = 0.0f;
			jumpStartTime = 0.0f;
			groundEffectStartTime = 0.0f;
			jumpGroundEffect = Effect_ID::EFFECT_ID();
			groundShakeCamera = 0.0f;
			groundShakeTime = 0.0f;

			// 衝撃波イベント
			shockWave = false;
			shockMoveEffect = Effect_ID::EFFECT_ID();
			shockWaveSpeed = 0.0f;
			startRange = 0.0f;

			// プレイヤー見るイベント
			lookPlayer = false;
			lookNum = 0;
			lookMaxCounter = 0.0f;

			// 投擲イベント
			throwObject = false;
			throwAttackData.clear();
			armThrow = false;
			

			throwObjectNum = 0;
			objectApperaPosition = VECTOR3();
			intervalTime = 0.0f;
			intervalTimeSub = false;
			maxIntervalTime = 0.0f;
			minIntervalTime = 0.0f;

			attackCameraBossLook = false;
			cameraChangeSpeed = 0.0f;

			useTrail = false;
			trailRightHand = false;
		}

		
		// 基本データ
		std::string bossID;								// ボスID
		std::string attackID;							// 攻撃ID
		std::string animFileName;						// 使用アニメーションファイル名
		int animNum;									// 再生アニメーション番号
		bool useFlash;									// 攻撃時フラッシュ演出を使用するか
		float attackFlashStartTime;						// フラッシュ演出開始時間
		float slowTime;									// スロー演出時間
		float slowAmout;								// スロー倍率
		float speedUpMotionSpeed;						// モーション加速倍率
		float attackCollsionStartTime;					// 攻撃判定開始時間
		float attackCollsionEndTime;					// 攻撃判定終了時間
		float justAvoidCollsionStartTime;				// ジャスト回避判定開始時間
		float justAvoidCollsionEndTime;					// ジャスト回避判定終了時間
		int attackPositionFrameNum;						// 攻撃位置取得フレーム番号
		float attackSoundStartTime;						// 攻撃SE再生開始時間
		Transform attackCollTransform;					// 攻撃判定Transform
		Transform justAvoidCollTransform;				// ジャスト回避判定Transform
		ID::IDType animID;								// 再生アニメーションID
		ID::IDType attackBeforeAnimID;					// 攻撃前アニメーションID
		float hitDamage;								// 攻撃ダメージ量
		PlayerDamagePattern damagePattern;				// プレイヤーダメージパターン
		std::string voiceName;							// ボイス名

		// 移動イベント
		bool frontMove;									// 前方向へ移動するか
		float moveSpeed;								// 移動速度


		// プレイヤー追従イベント
		bool playerAloowMove;							// プレイヤー追従移動を行うか
		float baseSpeed;								// 基本移動速度
		float baseFirstSpeed;							// 初速
		bool playerNearStop;							// プレイヤー接近時に停止するか
		bool playerNearAloowStop;						// 指定距離内で停止可能にするか
		bool addVelocity;								// 速度加算を行うか
		float playerBaseNear;							// プレイヤーとの停止距離
		float maxMoveSpeed;								// 最大移動速度
		float minMoveSpeed;								// 最小移動速度
		float moveStartTime;							// 移動開始時間
		float moveFinishTime;							// 移動終了時間


		// 突進イベント
		bool rushMove;									// 突進移動を行うか
		ID::IDType rushAfterAnimID;						// 突進後アニメーションID
		float rushAfterSpeed;							// 突進後移動速度
		float rushSoundRightFoot;						// 右足SE再生タイミング
		float rushSoundLeftFoot;						// 左足SE再生タイミング
		float rushTime;									// 突進時間
		bool rushColl;									// 突進判定を有効にするか
		float addRushCollScale;							// 突進判定サイズ追加倍率


		// 回転イベント
		bool rotateMove;								// 回転移動を行うか
		float angleMoveAmout;							// 回転角度量


		// ジャンプイベント
		bool jump;										// ジャンプを行うか
		float addGravity;								// 追加重力
		float jumpSpeed;								// ジャンプ速度
		float jumpStartTime;							// ジャンプ開始時間
		float groundEffectStartTime;					// 着地エフェクト開始時間
		Effect_ID::EFFECT_ID jumpGroundEffect;			// 着地エフェクトID
		float groundShakeCamera;						// 着地時カメラ揺れ強度
		float groundShakeTime;							// 着地時カメラ揺れ時間


		// 衝撃波イベント
		bool shockWave;									// 衝撃波を発生させるか
		Effect_ID::EFFECT_ID shockMoveEffect;			// 衝撃波エフェクトID
		float shockWaveSpeed;							// 衝撃波拡大速度
		float startRange;								// 衝撃波開始範囲


		// プレイヤー注視イベント
		bool lookPlayer;								// プレイヤー方向を見るか
		int lookNum;									// 注視回数
		float lookMaxCounter;							// 注視最大時間


		// 投擲イベント
		bool throwObject;								// 投擲攻撃を行うか
		std::vector<ThrowObjectAttackData> throwAttackData;	// 投擲攻撃データ
		bool armThrow;									// 腕から投擲するか
		int throwObjectNum;								// 投擲オブジェクト数
		VECTOR3 objectApperaPosition;					// オブジェクト出現位置
		float intervalTime;								// 投擲間隔時間
		bool intervalTimeSub;							// 投擲間隔を減少させるか
		float maxIntervalTime;							// 最大出現間隔
		float minIntervalTime;							// 最小出現間隔


		// カメライベント
		bool attackCameraBossLook;						// 攻撃開始時カメラをボス方向へ向けるか
		float cameraChangeSpeed;						// カメラ切り替え速度


		// トレイルイベント
		bool useTrail;									// トレイルを使用するか
		bool trailRightHand;							// 右手トレイルを使用するか

		//サウンドイベント
		std::vector<SoundEffectEvent> soundEvent;			//アニメーションフレーム
	};
	
	EnemyAttackBase();
	~EnemyAttackBase();

	void Update()override;

	void BossStart();
	void BossFinish();

	/// <summary>
	/// ダメージを受けているダメージ
	/// </summary>
	/// <returns></returns>
	float GetHitDamage()const  { return attackParam.hitDamage; }
	/// <summary>
	/// ダメージのパラメーターを取得
	/// </summary>
	/// <returns>ダメージのパラメーター</returns>
	const BossAttackParam& GetDamageParam() { return attackParam; }
	/// <summary>
	/// データのセーブをする関数
	/// </summary>
	void DataSaveAll();
	/// <summary>
	/// 攻撃の当たり判定の生成
	/// </summary>
	void AttackCollsion();
	/// <summary>
	/// 攻撃時の音の生成
	/// </summary>
	void AttackSound();
	/// <summary>
	/// 攻撃をするときに手を光らせる前段階の処理
	/// </summary>
	/// <param name="_modelId">モデルのEnum</param>
	/// <param name="_modelFrame">光らせる場所のフレーム</param>
	/// <param name="_voice">ボイスの名前</param>
	void AttackFlash(ID::IDType _modelId, int _modelFrame, std::string _voice);
	/// <summary>
	/// 攻撃をするときに手を光らせる処理
	/// </summary>
	/// <param name="_modelId">モデルのEnum</param>
	/// <param name="_modelFrame">光らせる場所のフレーム</param>
	/// <param name="_voice">ボイスの名前</param>
	void AttackBeforeFrash(ID::IDType _modelId, int _modelFrame, std::string _voice);
	/// <summary>
	/// 軌跡の表示
	/// </summary>
	void Trail();
	/// <summary>
	/// ジャスト回避当たり判定の生成
	/// </summary>
	void EnemyJustAvoidCollsion();
	/// <summary>
	/// 攻撃のデータのロード
	/// </summary>
	void LoadAttackParam();
	/// <summary>
	/// 攻撃のデータの受け渡し
	/// </summary>
	/// <param name="_param"></param>
	void SetAttackParam(BossAttackParam _param);
	/// <summary>
	/// 今再生されてるアニメーションが攻撃アニメーションかどうかを判定する
	/// </summary>
	/// <returns></returns>
	bool CurrentAttackAnim();
	

protected:
	BossAttackParam attackParam;
private:
	
};
