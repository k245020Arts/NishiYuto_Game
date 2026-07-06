#pragma once
#include "../Component/Component.h"
#include "../Component/Collider/ColliderBase.h"
#include "../Component/ComponentManager.h"
#include "../Component/Transform/Transform.h"
#include "../Common/JsonReader.h"

class SphereCollider;

class CharaBase : public Component

{
public:

	struct CollsionSet
	{
		ColliderBase* instance;

		CollsionInformation::Shape shape;
		std::string collName;
		bool oneHit;
		CollsionInformation::Tag tag;

		CollsionSet() {
			instance = nullptr;
			shape = CollsionInformation::Shape::SHAPE_MAX;
			collName = "";
			oneHit = false;
			tag = CollsionInformation::Tag::TAG_MAX;
		}
		CollsionSet(ColliderBase* _inst, CollsionInformation::Shape _shape, std::string _name, bool _one,CollsionInformation::Tag _tag) {
			instance = _inst;
			collName = _name;
			shape = _shape;
			oneHit = _one;
			tag = _tag;
		}
	};
	CharaBase();
	virtual ~CharaBase();

	void Update()override;
	void Draw()override;

	//当たり判定の生成をしている(攻撃の)
	//プレイヤーと敵の攻撃の当たり判定の生成を一括で管理したかったのでこういう作りにしています。
	template<typename T>
	CollsionInfo CollsionInstant(CollsionSet* _set, const Transform& _trans) {
		_set->instance = obj->Component()->AddComponent<T>();
		CollsionInfo info;
		info.parentTransfrom = obj->GetTransform();
		info.oneColl = _set->oneHit;
		info.shape = _set->shape;
		return info;
	}
	//当たり判定の形の情報を保持
	void SetShape(CollsionInformation::Shape _shape, CollsionSet* _set) { _set->shape = _shape; }
	//当たり判定の削除
	void DeleteColliderComponent(CollsionSet* _set);

	/// <summary>
	/// hpの取得、Guageと結びつかせたいのでポインタ渡しにして毎フレームゲットしなくてもいいようにしている
	/// </summary>
	/// <returns></returns>
	float* GetHp() { return &hp; }

	/// <summary>
	/// 最大hpの取得
	/// </summary>
	/// <returns></returns>
	const float GetMaxHp(){return maxHp;}

	/// <summary>
	/// 最終ターゲットかどうかを取得する関数
	/// </summary>
	void LastTargetIn() { lastTarget = true; }
	/// <summary>
	/// 最終ターゲットではない
	/// </summary>
	void LastTargetOut() { lastTarget = false; }
	/// <summary>
	/// 現在カメラロックオンされているかどうかを取得する関数
	/// </summary>
	/// <returns>現在ロックオンされていたらtrue</returns>
	bool GetLastTarget()const  { return lastTarget; }

	/// <summary>
	/// 当たり判定の削除をする、Charaによって違うがある程度は対応できるようにしておく
	/// </summary>
	virtual void DeleteCollision(CollsionSet* _set);

	/// <summary>
	/// hpの取得、Guageと結びつかせたいのでポインタ渡しにして毎フレームゲットしなくてもいいようにしている
	/// </summary>
	/// <returns></returns>
	float* GetSpecialAttackGuage() { return &specialAttackBar; }

	/// <summary>
	/// 最大hpの取得
	/// </summary>
	/// <returns></returns>
	const float GetMaxSpecialAttackGuage() { return specialAttackBarMax; }
	/// <summary>
	/// スペシャル攻撃をだせるかどうかの判定
	/// </summary>
	/// <returns></returns>
	bool CanSpecialAttack()const { return (specialAttackBar >= specialAttackBarMax); }

protected:
	CollsionSet attackColl; //攻撃時の当たり判定の情報
	CollsionSet justAvoidColl; //ジャスト回避判定の情報

	float hp;
	float maxHp;

	bool lastTarget;

	float specialAttackBar;
	float specialAttackBarMax;
};