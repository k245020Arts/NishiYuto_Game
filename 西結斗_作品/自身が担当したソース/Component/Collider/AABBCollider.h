#pragma once
#include "colliderBase.h"

class AABBCollider : public ColliderBase
{
public:

	struct AABBInfo
	{
		VECTOR3 boxMin; //左上の座標
		VECTOR3 boxMax; //右下の座標
		AABBInfo() {
			boxMin = VZero;
			boxMax = VZero;
		}
		AABBInfo(const VECTOR3& min, const VECTOR3& max) {
			boxMin = min;
			boxMax = max;
		}
	};
	AABBCollider();
	~AABBCollider();

	void Update()override;
	void Draw()override;

	/// <summary>
	/// AABBの当たり判定のセット
	/// </summary>
	/// <param name="_info">当たり判定の情報</param>
	/// <param name="_boxMin">AABBの最小位置</param>
	/// <param name="_boxMax">AABBの最大位置</param>
	/// <param name="_tag">当たり判定のタグ名</param>
	void SetUpAABBCollsion(const CollsionInfo& _info, const VECTOR3& _boxMin, const VECTOR3& _boxMax, std::function<void(const CollsionEventData&)> _funk, std::string _tag);
	void SetUpAABBCollsion(const CollsionInfo& _info,const VECTOR3& _boxMin, const VECTOR3& _boxMax,std::function<void(const CollsionEventData&)> _funk);
	
	/// <summary>
	/// 当たり判定の情報の付与
	/// </summary>
	/// <returns></returns>
	const AABBInfo& GetAABBInfo() { return aabbInfo; }

private:
	AABBInfo aabbInfo;
};