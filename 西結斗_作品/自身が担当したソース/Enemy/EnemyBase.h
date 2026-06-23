#pragma once
#include "../CharaBase/CharaBase.h"
#include "enemyInformation.h"
#include "EnemyState/EnemyDamage.h"
#include "../Common/ID/EffectID.h"

class EnemyBase : public CharaBase
{
public:
	friend class EnemyStateBase;
	friend class EnemyAttackBase;
	EnemyBase();
	virtual ~EnemyBase();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// 当たり判定の生成
	/// </summary>
	/// <typeparam name="T">当たり判定のクラス名</typeparam>
	/// <param name="_shape">当たり判定の形</param>
	/// <param name="_trans">基準のTransform</param>
	/// <returns>当たり判定クラスのポインタ</returns>
	template<typename T>
	T* CollsionStart(CollsionInformation::Shape _shape, const Transform& _trans) {
		return nullptr;
	}
	/// <summary>
	/// StateManagerの取得
	/// </summary>
	/// <returns>StateManagerの取得</returns>
	StateManager* GetStateManager() { return enemyBaseComponent.state; }
	/// <summary>
	/// Enemyのベースオブジェクトを取得
	/// </summary>
	/// <returns>ベースオブジェクトのポインタ</returns>
	BaseObject* GetEnemyObj() { return obj; }

	virtual void PlayerHit(const CollsionEventData& _data) { hit = true; }
	/// <summary>
	/// 剣の軌跡
	/// </summary>
	void DrawTrail();
	/// <summary>
	/// 敵の攻撃がダメージを食らった時の移動
	/// </summary>
	/// <param name="_info"></param>
	void EnemyDamageMove(const EnemyDamage::EnemyDamageInfo& _info);
	/// <summary>
	/// プレイヤーの攻撃がヒットしたかどうかの判定
	/// </summary>
	/// <returns>プレイヤーの敵の判定</returns>
	bool GetHit() { return hit; }
	/// <summary>
	/// 半透明度の設定
	/// </summary>
	/// <param name="_alpha">半透明度</param>
	void SetAlpha(int _alpha) { alpha = _alpha; }
	/// <summary>
	/// 必殺技が当たっているかどうかの判定
	/// </summary>
	void SpecialAttackHit() { specialAttackHit = true; }
	
	/// <summary>
	/// ダメージ計算式
	/// </summary>
	/// <param name="_pos">ポジションを入れる</param>
	/// <param name="_damage">食らうダメージを入れる</param>
	/// <param name="_defense">防御数値を入れる</param>
	/// <param name="deviation">ダメージ表記するときの数字の誤差</param>
	/// <returns></returns>
	float DamageCalculation(const VECTOR3& _pos,float _damage, float _defense,float deviation);

protected:

	EnemyInformation::EnemyComponent enemyBaseComponent;
	float damageFlash;
	float hitCounter;
	int loopNum;
	StateManager* pState;
	Object2D* damageNum;
	
	const float MAX_HP = 100;
	bool hit;
	int alpha;

	int cursolImage;

	VECTOR3 upPos;

	bool specialAttackHit;
	float justAvoidCollTime;
	const char* name;
	
};
