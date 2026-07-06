#pragma once
#include "../Component.h"

//オブジェクトの近くにライトを設定したい時に使うクラス。

class ComponentLight : public Component
{
public:
	ComponentLight();
	~ComponentLight();
	void Update()override;
	void Draw()override;

	void Start()override;
	/// <summary>
	/// ポイントライトを設定したいときに行う関数
	/// </summary>
	
	void PointLightHandleStart(VECTOR position,float range,float attan1,float attan2,float attan3);

	//スポットライトの説明
	//https://dxlib.xsrv.jp/function/dxfunc_3d_light.html#R13N23
	/// <summary>
	/// スポットライトを設定したいときに行う関数
	/// </summary>
	void SpotLightHandleStart(VECTOR position, VECTOR direction, float outAngle, float inAngle, float range, float atten0, float atten1, float atten2);

	void ImguiDraw()override;
	void SpotUpdate();
	void PointUpdate();

private:

	enum LightKind
	{
		SPOT,
		POINT,
	};
	int lightHandle;
	Transform* lightTransform;
	float rangeHalf;
	VECTOR3 dir;
	LightKind kind;
	
};