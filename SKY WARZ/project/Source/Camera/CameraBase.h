#pragma once
#include "CameraInformation.h"
#include "../Component/Component.h"
#include "../Component/Shaker/Shaker.h"

class CameraBase : public Component
{
public:
	CameraBase();
	~CameraBase();

	void Update()override;
	void Draw()override;

	virtual void Start(BaseObject* _eObj) { ; }

	virtual void PlayerSet(BaseObject* _pObj) { ; }

	virtual void ImguiDraw()override { ; }

	//ターゲットと今のカメラの位置を見てカメラのRotationを決めてる関数
	//カメラの角度を自由に指定したい場合はこの関数を使わないほうがいい
	void CameraRotationSet();
	/// <summary>
	/// カメラがプレイヤーに追従する関数
	/// </summary>
	void Follow();
	/// <summary>
	/// カメラを振動させたいときにこのクラスを呼ぶ
	/// </summary>
	/// <param name="_power">振動の強さ</param>
	/// <param name="_pattern">shakeの種類</param>
	/// <param name="_stop">完全にストップするか</param>
	/// <param name="_second">秒数(-1を指定すると下のshakeStop関数が呼ばれるまでずっと振動)</param>
	void CameraShake(VECTOR3 _power, Shaker::ShakePattern _pattern, bool _stop, float _second);
	/// <summary>
	/// カメラ振動の解除
	/// </summary>
	void CameraShakeStop();
	/// <summary>
	/// カメラトランスフォームの取得
	/// </summary>
	/// <returns> カメラトランスフォーム</returns>
	Transform* GetCameraTransform() const { return cameraComponent.cameraTransform; }

protected:
	CameraInformation::CameraComponent cameraComponent; //コンポーネントを毎回GetComponentしたくないので一括で保管

	float reap; //追従処理をするときにどれくらいの補完を掛けるかを指定する値
	VECTOR3 defalutDistance; //基準となる距離
	VECTOR3 currentDistance; //今の距離

	float fov;
	float nearFog;

	float farFog;

	VECTOR3 target;

	VECTOR3 diffTarget;
	int debugButton;
};