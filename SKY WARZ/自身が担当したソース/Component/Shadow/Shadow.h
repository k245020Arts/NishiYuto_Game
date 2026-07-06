#pragma once
#include "../Component.h"

class Color;
class MeshRenderer;

class Shadow : public Component
{
public:
	Shadow();
	~Shadow();

	void Update()override;
	void Draw()override;

	void Start(float _height);

	/// <summary>
	/// 影の大きさをセットする関数
	/// </summary>
	/// <param name="_dist">プレイヤーと地面の距離</param>
	/// <param name="_groundPos">地面の位置</param>
	void ChangeScale(const VECTOR3& _dist, const VECTOR3& _groundPos);

	/// <summary>
	/// 影の大きさがゼロになる距離の値を取得
	/// </summary>
	/// <returns>最大距離</returns>
	const float GetMaxDist() const  { return MAX_DIST; }

private:
	int model;
	VECTOR3 baseScale;
	const float MAX_DIST = 4500.0f;
	VECTOR3 groundPos;
	VECTOR3 dist;
	BaseObject* obj;
	float height;
};
