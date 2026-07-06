#pragma once
#include "ColliderBase.h"

class DountCollider : public ColliderBase
{
public:
	DountCollider();
	~DountCollider();

	void Update()override;
	void Draw()override;

	/// <summary>
	/// ドーナツ型の当たり判定の開始
	/// </summary>
	/// <param name="_info">当たり判定の情報</param>
	/// <param name="_transform">ドーナツの外側のTransform</param>
	/// <param name="_outRadius">外側の円の半径</param>
	void DountSet(const CollsionInfo& _info, const Transform& _transform,float _outRadius, std::function<void(const CollsionEventData&)> _funk);

	/// <summary>
	/// 外側の半径の取得
	/// </summary>
	/// <returns>外側の半径の取得</returns>
	float GetOutRadius() { return outRadius; }
	/// <summary>
	/// 外側の半径を足す
	/// </summary>
	/// <param name="radius"></param>
	void AddOutRadius(float radius) { outRadius += radius; }

private:

	float outRadius; //ドーナツの外側の半径

	float speed;

};