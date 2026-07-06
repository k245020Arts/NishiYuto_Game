#pragma once
#include "../EnemyAttackObject.h"
#include "../../../Common/ID/EffectID.h"
#include "../../Collider/CollsionInformation.h"

class EffectManager;
class Transform;
class DountCollider;

class ShockWave : public EnemyAttackObject
{
public:
	ShockWave();
	~ShockWave();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// 衝撃波を生成する
	/// </summary>
	/// <param name="_tag">当たり判定のタグ</param>
	/// <param name="_transform">今現在のトランスフォーム</param>
	/// <param name="_startOutRange">外側の半径</param>
	/// <param name="_speed"速度></param>
	void CreateWave(CollsionInformation::Tag _tag, const Transform& _transform, float _startOutRange, float _speed);

private:

	EffectManager* effect;
	DountCollider* collider;
	DountCollider* justAvoidCollider;
	float waveSpeed;
};
