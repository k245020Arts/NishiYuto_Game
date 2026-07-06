#pragma once
#include "../Component.h"
#include "CollsionInformation.h"
#include "../Transform/Transform.h"

class ColliderBase : public Component
{
public:
	ColliderBase();
	virtual ~ColliderBase();
	void Update();
	void Draw();

	/// <summary>
	/// 当たり判定の追加をするときに呼び出す
	/// タグを付けたいときは下のを使いタグを作る
	/// </summary>
	/// <param name="_info">当たり判定の情報</param>
	/// <param name="transform">当たり判定のTransform</param>
	void CollsionAdd(const CollsionInfo& _info, const Transform& transform, std::function<void(const CollsionEventData&)> _funk);
	/// <summary>
	/// 当たり判定の追加をするときに呼び出す
	/// タグを付けたいときは下のを使いタグを作る
	/// </summary>
	/// <param name="_info">当たり判定の情報</param>
	/// <param name="transform">当たり判定のTransform</param>
	/// <param name="tag">当たり判定のタグ</param>
	void CollsionAdd(const CollsionInfo& _info, const Transform& transform, std::function<void(const CollsionEventData&)> _funk,std::string _tag);
	
	/// <summary>
	/// 当たり判定の形を取得
	/// </summary>
	/// <returns>当たり判定の形</returns>
	CollsionInformation::Shape GetShape() { return shape; }
	/// <summary>
	/// 当たり判定のタグを取得
	/// </summary>
	/// <returns>当たり判定のタグ</returns>
	CollsionInformation::Tag GetCollTag() { return collTag; }

	/// <summary>
	/// 当たり判定のトランスフォーム
	/// </summary>
	/// <returns>当たり判定の取得</returns>
	Transform* GetTransform() { return collTransform; }
	/// <summary>
	/// 半径の取得
	/// </summary>
	/// <returns>当たり判定の半径</returns>
	float GetRadius() { return radius * collTransform->scale.x; }
	/// <summary>
	/// 一回だけ当てるかどうかの取得
	/// </summary>
	/// <returns>一回だけ当てるかどうか</returns>
	bool GetOneColl() { return oneColl; }
	/// <summary>
	/// 当たり判定が終了しているかどうか
	/// </summary>
	/// <returns>当たり判定が終了ならtrue</returns>
	bool GetFinish() { return finish; }

	/// <summary>
	/// 当たり判定をもう無視したい場合は個の関数を呼ぶ
	/// </summary>
	void CollsionFinish() { finish = true; }
	/// <summary>
	/// 当たり判定を復活させたいならこの処理を呼ぶ
	/// </summary>
	void CollsionRespown() { finish = false; }
	BaseObject* GetObj() { return obj; }

	std::function<void(const CollsionEventData&)> GetEventFunc() { return  collsionEventFunction; }

protected:
	CollsionInformation::Shape shape; //当たり判定の形
	CollsionInformation::Tag collTag; //当たり判定のタグ
	Transform* collTransform;
	float radius;
	bool oneColl;
	bool finish;
	std::function<void(const CollsionEventData&)> collsionEventFunction; //当たり判定イベントの関数ポインタ

};