#pragma once
#include "collisionBase.h"

enum class RAY_DIRECTION_KIND
{
	SIDE = 0,//プレイヤーの逆進行方向はレイをとばさない
	UP,
	DOWN,
};

class Player;
class RayCollision : public CollisionBase
{
public:

	RayCollision(Object* obj);
	~RayCollision();

	void Draw() override;

	bool CollisionToSphere(SphereCollision* col) override { return false; }
	/// <summary>
	/// レイと球の当たり判定...OBBなのに球なのは簡単だから
	/// </summary>
	/// <param name="col"></param>
	/// <returns></returns>
	bool CollisionToOBB(OBBCollision* col) override;
	bool CollisionToRay(RayCollision* col) override { return false; }

	VECTOR GetRayDirection() { return reRay; }

	void SetRayDirecition(VECTOR ray) { rayDirection = ray; }
	void SetRayDirecition(RAY_DIRECTION_KIND _kind)
	{
		directionKind = _kind;
		ResetRayDirection();
	}

private:

	void ResetRayDirection();

	RAY_DIRECTION_KIND directionKind;
	VECTOR rayDirection;
	VECTOR reRay;
	Player* player;
};