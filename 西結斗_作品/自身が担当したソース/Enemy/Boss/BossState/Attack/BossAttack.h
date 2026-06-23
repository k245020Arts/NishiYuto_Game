#pragma once
#include "../../../EnemyState/Attack/EnemyAttackBase.h"

class BossAttack : public EnemyAttackBase
{
public:

	BossAttack();
	~BossAttack();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

	/// <summary>
	/// 回転イベント
	/// </summary>
	void RotateEvent();
	/// <summary>
	/// プレイヤーを見るイベント
	/// </summary>
	void LookEvent();
	/// <summary>
	/// 移動イベント
	/// </summary>
	void MoveEvent();
	/// <summary>
	/// ジャンプイベント
	/// </summary>
	void JumpEvent();
	/// <summary>
	/// 衝撃波イベント
	/// </summary>
	void ShackWaveEvent();
	/// <summary>
	/// 衝撃波の生成
	/// </summary>
	void CreateWave();
	/// <summary>
	/// 投擲物イベント
	/// </summary>
	void ThrowObjectsEvent();
	/// <summary>
	/// 突進イベント
	/// </summary>
	void RushEvent();
	
	/// <summary>
	/// 攻撃後の終了判定
	/// </summary>
	void AttackFinishFrame();

	/// <summary>
	/// サウンドイベントを実行
	/// </summary>
	void SoundEvent();

	void AttackStart();
	void BossDushSound();

	/// <summary>
	/// ボスの攻撃の当たり判定の生成
	/// </summary>
	void BossAttackCollsion();

	/// <summary>
	/// 軌跡の表示
	/// </summary>
	/// <param name="_right">右手から出るか左手から出るか</param>
	void BossTrail(bool _right);
	/// <summary>
	/// ジャスト回避当たり判定の生成
	/// </summary>
	void BossJustAvoidCollsion();

private:
	float averageSpeed;
	bool aloowStop;
	VECTOR3 normal;
	bool firstJump;
	float gravitySpeed;
	bool groundEffect;
	bool throwRock;
	bool rockGet;
	float throwObjectAppearTime;
	int throwObjectNumNow;
	float rushAttackCount;
	bool rushSound;
	bool firstOnes;
	bool secondOnes;
	bool firstMove;

	SphereCollider* rockColl;


	Boss* bossPtr;
	std::unordered_map<std::string, bool> soundPlayed;
	std::unordered_map<std::string, bool> soundLoopPlaying;
};
