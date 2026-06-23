#pragma once
#include <vector>
#include "../../../Library/myDxLib.h"

class Transform;
class ColliderBase;
class BaseObject;

namespace CollsionInformation {
	//当たり判定の形
	enum Shape
	{
		SPHERE = 0,
		MODEL,
		RAY,
		AABB,
		DONUT,
		CAPSULE,
		SHAPE_MAX,
	};
	//当たり判定の種類
	enum Tag
	{
		ENEMY,
		PLAYER,
		E_ATTACK,
		P_ATTACK,
		CAMERA,
		WALL,
		BOSS,
		B_ATTACK,
		FLOOR,
		E_FLOOR,
		P_FLOOR,
		B_FLOOR,
		C_FLOOR,
		SHADOW,
		B_E_ATTACK,
		P_SPECIAL_ATTACK,
		BOSS_PUSH,
		BOSS_ROCK,
		BOSS_ROCK_F,
		BOSS_ROCK_ATTACK,
		BOSS_ROCK_DAMAGE,
		BOSS_ROCK_UI,
		BOSS_ROCK_PLAYER_ATTACK,
		FLOOR_AABB,
		BOSS_ROCK_RUSH,
		BOSS_RUSH,
		ROCK_BLAST_DAMAGE,
		JUST_AVOID,
		JUST_AVOID_ENEMY,
		JUST_AVOID_BOSS,
		THROW_OBJECT_GROUND,
		THROW_OBJECT_GROUND_ONE_HIT,
		THROW_OBJECT_GROUND_NO_DAMAGE_REACTION,

		TAG_MAX,
	};

	constexpr int COLLSION_KIND_MAX = SHAPE_MAX * SHAPE_MAX;
	constexpr int COLLSION_TAG_MAX = TAG_MAX * TAG_MAX;

	/// <summary>
	/// 当たり判定をどのオブジェクト同士で行うかを設定する関数。初めに一回呼び出している。
	/// </summary>
	void InitSetPair();
	//そのペアが当たり判定を行ってよいかを判定
	bool IsCollPair(Tag tag1, Tag tag2);

}

struct CollsionInfo
{
	CollsionInfo()
	{
		shape = CollsionInformation::SHAPE_MAX;
		tag = CollsionInformation::TAG_MAX;
		size = 0.0f;
		parentTransfrom = nullptr;
		oneColl = false;
	}
	CollsionInformation::Shape shape;
	CollsionInformation::Tag tag;
	float size;
	Transform* parentTransfrom;
	bool oneColl;
};

class Transform;
class Physics;

struct PushInfo {
	VECTOR3 normal;      // 法線ベクトル
	float penetration; // めり込み量
	CollsionInformation::Shape shape;
	VECTOR3 targetPos;

	PushInfo() {

		normal = VZero;
		penetration = 0.0f;
		shape = CollsionInformation::SHAPE_MAX;
		targetPos = VZero;
	}

	PushInfo(const VECTOR3& n, float p, CollsionInformation::Shape _shape,VECTOR3 _targetPos)
		: normal(n), penetration(p) ,shape(_shape) , targetPos(_targetPos) {
	}
};

class Pushback {
public:
	Pushback();
	~Pushback();
	/// <summary>
	/// 当たり判定情報を削除
	/// </summary>
	void Clear();

	/// <summary>
	/// 押し返しデータのセット
	/// </summary>
	/// <param name="normal">法線</param>
	/// <param name="penetration">押し返しの量</param>
	/// <param name="_shape">形</param>
	/// <param name="_targetPos">押し返しに使う相手の当たり判定</param>
	void AddPush(const VECTOR3& _normal, float _penetration, CollsionInformation::Shape _shape, const VECTOR3& _targetPos);

	// 押し返しベクトルを計算して返す
	VECTOR3 ResultPushback(float  _maxLength = 5.0f,VECTOR3 _pos = VZero);

	/// <summary>
	/// 押し返しを適用
	/// </summary>
	/// <param name="transform">当たり判定に使用するTransform</param>
	/// <param name="physics">フィジックスのポインタ</param>
	/// <param name="affectVelocity">velocityを適用するか</param>
	/// <param name="maxLength">最大の長さ</param>
	void Apply(Transform* _transform, Physics* _physics, bool _affectVelocity = true, float _maxLength = 5.0f);
	bool IsGrounded(float _minYNormal = 0.6f);
	std::vector<PushInfo>& GetPushInfo() { return pushes; }
private:
	std::vector<PushInfo> pushes;
};

struct CollsionEventData
{
	Transform* targetTransform;				//相手のトランスフォーム
	CollsionInformation::Tag targetTag;		//相手の当たり判定のタグ
	CollsionInformation::Shape targetShape; //相手の当たり判定の形
	BaseObject* targetObject;				//相手のオブジェクト
	ColliderBase* targetColliderBase;			//相手のオブジェクト

	std::vector<PushInfo> pushes;			//当たり判定の押し返し情報
	CollsionInformation::Tag myTag;			//自分の当たり判定のタグ
	CollsionInformation::Shape myShape;		//自分の当たり判定の形
	Transform* myTransform;					//自分の当たったトランスフォーム
	BaseObject* myObject;					//自分のオブジェクト
	ColliderBase* myColliderBase;				//自分のオブジェクト

	VECTOR3 hitPos;							//当たった座標



	
	CollsionEventData() {
		targetTransform = nullptr;
		targetTag = CollsionInformation::TAG_MAX;
		targetShape = CollsionInformation::SHAPE_MAX;
		targetColliderBase = nullptr;
		pushes.clear();
		myTag = CollsionInformation::TAG_MAX;
		myShape = CollsionInformation::SHAPE_MAX;
		myTransform = nullptr;
		myColliderBase = nullptr;
		hitPos = VZero;
		targetObject = nullptr;
		myObject = nullptr;
	}
};