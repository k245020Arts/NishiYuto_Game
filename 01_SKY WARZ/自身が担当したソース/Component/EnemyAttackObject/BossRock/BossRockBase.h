#pragma once
#include "../EnemyAttackObject.h"
#include <unordered_set>
#include "../../../Enemy/EnemyState/Attack/EnemyAttackBase.h"

class SphereCollider;
class RayCollider;
class ModelCollider;
class BossRockManager;
class DountCollider;
class SoundManager;
class EffectManager;
class CapsuleCollider;
class MeshRenderer2D;

class BossRockBase : public EnemyAttackObject
{
public:
	 BossRockBase();
	~BossRockBase();
	void Awake()override;

	void Update()override;
	void Draw()override;
	/// <summary>
	/// オブジェクトに当たったかどうかの判定
	/// </summary>
	/// <param name="_obj">今当たったオブジェクト</param>
	/// <returns>過去に当たってたかどうか</returns>
	bool HitObjects(BaseObject* _obj) {
		return (hitObjects.count(_obj) > 0);
	}
	/// <summary>
	/// 地面についたときの処理
	/// </summary>
	void Ground(const CollsionEventData& _data);

	/// <summary>
	/// オブジェクトが当たった時にオブジェクト情報の追加
	/// </summary>
	/// <param name="_obj">追加させるオブジェクト</param>
	void AddHitObj(BaseObject* _obj) { hitObjects.insert(_obj); }
	/// <summary>
	/// 爆発をするかどうかの判定
	/// </summary>
	/// <param name="_blast"></param>
	void SetCanBlast(bool _blast) { blastCan = _blast; }
	/// <summary>
	/// プレイヤーの攻撃が投擲物に当たった時の処理
	/// </summary>
	/// <param name="_playerTransform">プレイヤーのトランスフォーム</param>
	void PlayerAttackRockFlyAway(const CollsionEventData& _data);
	/// <summary>
	/// 攻撃がボスに当たった時にするイベント
	/// </summary>
	void RockBossHit(const CollsionEventData& _data);
	/// <summary>
	/// 投擲物の開始
	/// </summary>
	/// <param name="_attack">投擲物が持つデータ</param>
	/// <param name="_handMatrix">手のマトリックスの情報、いらないならnullptrを入れる</param>
	void Start(const EnemyAttackBase::ThrowObjectAttackData& _attack, MATRIX* _handMatrix);
	/// <summary>
	/// 投擲物を落とすときに使う
	/// </summary>
	void DropObject();
	/// <summary>
	/// 投擲物を投げ始めるときに呼ぶ
	/// </summary>
	/// <param name="_player"></param>
	void ThrowRockStart(BaseObject* _player);
	/// <summary>
	/// 投擲物のデータを取得
	/// </summary>
	/// <returns>投擲物のデータ</returns>
	const EnemyAttackBase::ThrowObjectAttackData& GetThrowObjectsData()const { return attackData; }
	/// <summary>
	/// 予測線を出すときの処理
	/// </summary>
	/// <param name="_pos">地面の座標</param>
	void SetPreInfo(const CollsionEventData& _data);
	/// <summary>
	/// 投擲物のデータをセット
	/// </summary>
	/// <param name="_data">投擲物のデータ</param>
	void SetThrowParam(EnemyAttackBase::ThrowObjectAttackData _data) { attackData = _data; }
	/// <summary>
	/// 空のオブジェクトなのかStartの関数を呼んだかどうかの判定
	/// </summary>
	/// <returns>trueならStartが呼ばれている。falseなら空のオブジェクト</returns>
	bool GetThrowObjectStart() { return start; }
	/// <summary>
	/// カプセルコリジョンのポジション移動開始
	/// </summary>
	void CapsuleColliderPosAddStart();

	void BlastCollsionEvent(const CollsionEventData& _data);

	void SetPlayerTransform(Transform* _transform);

protected:

	RayCollider* randColl;
	SphereCollider* playerHitColl;
	CapsuleCollider* playerHitCapsuleColl;
	SphereCollider* bossHitColl;
	SphereCollider* playerAttackHitColl;
	SphereCollider* bossRushHitColl;
	DountCollider* blastColl;
	DountCollider* blastJustAvoidColl;
	RayCollider* uiColl;
	ModelCollider* pushColl;
	SphereCollider* justAvoidCollider;
	CapsuleCollider* justAvoidCapselCollider;

	Physics* physics;
	BossRockManager* rockManager;


	bool blast;
	Color* color;
	float blinkCounter;
	float blinkBaseMax;
	bool ChangeColorMode;

	bool nowBlast;

	std::unordered_set<BaseObject*> hitObjects;
	bool blastCan;
//private:
	bool groundInit;
	bool fly;
	int useHandleNumber;
	EnemyAttackBase::ThrowObjectAttackData attackData;

	void BlastCollsionCreate();
	bool preDraw;
	Transform preTransform;
	int preModel;

	bool throwRock;
	VECTOR3 dir;
	int throwRockBaseModel;
	int boneNum;
	bool throwReady;
	VECTOR3 addPos;
	float groundTime;
	float flyCounter;
	bool playerAttackHit;
	bool velocityAdd;
	MATRIX* handMatrix;
	bool start;
	VECTOR3 capsuleEndPos;
	VECTOR3 capsuleJustAvoidEndPos;
	bool colliderAddStart;
	float alotHitCounter;
	void AlotCollsionSet();

	int playerFlyAwayBButtonUI;
	int playerFlyAwayYButtonUI;
	bool flyingUIDraw;

	std::list<MeshRenderer2D*> meshRenderer2DList;
	Transform* playerTransform;
};