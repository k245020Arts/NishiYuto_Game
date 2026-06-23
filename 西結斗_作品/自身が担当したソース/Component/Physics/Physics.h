#pragma once
#include "../Component.h"
#include "../Transform/Transform.h"

//物理演算関係関数
//基本的にオブジェクトの移動はこのクラスを通して行っている。

class TransitorManager;

class Physics : public Component
{
public:
	Physics();
	~Physics();
	void Update()override;
	void Draw()override;
	void Start(const VECTOR3& _gravityAmout, const VECTOR3& _fir);

	/// <summary>
	/// velocityのセット
	/// </summary>
	/// <param name="_addVelocity">追加するベロシティ</param>
	/// <param name="_deltaTime">毎フレームAddVelocityをするならこれをtrueにする</param>
	void AddVelocity(const VECTOR3& _addVelocity, bool _deltaTime);
	/// <summary>
	/// velocityのセット
	/// </summary>
	/// <param name="_velocity">セットするvelocity</param>
	void SetVelocity(const VECTOR3& _velocity) { velocity = _velocity; }

	
	/*void AddInterect(VECTOR3 _addInterect, float  _firctionCoeffocoent);
	void SetInterect(VECTOR3 _setInterect, float _firctionCoeffocoent);*/
	/// <summary>
	/// 空気抵抗をセットする
	/// </summary>
	/// <param name="_setFirction">セットする空気抵抗</param>
	void SetFirction(const VECTOR3& _setFirction);
	/// <summary>
	/// 空気抵抗をセットする
	/// </summary>
	/// <param name="_setFirction">加算する空気抵抗</param>
	void AddFirction(const VECTOR3& _addFirction);
	
	/// <summary>
	/// 重力の取得
	/// </summary>
	/// <returns>重力</returns>
	VECTOR3 GetGravity()const { return gravity; }
	/// <summary>
	/// 重力をセットする
	/// </summary>
	/// <param name="_setGravity">セットする重力</param>
	void SetGravity(const VECTOR3& _setGravity);
	/// <summary>
	/// 重力をセットする
	/// </summary>
	/// <param name="_setGravity">加算する重力</param>
	void AddGravity(const VECTOR3& _addGraivty);
	/// <summary>
	/// 空気抵抗の取得
	/// </summary>
	/// <returns>空気抵抗</returns>
	VECTOR3 GetFirction()const { return firction; }
	/// <summary>
	/// ベロシティの取得
	/// </summary>
	/// <returns>ベロシティ</returns>
	VECTOR3 GetVelocity()const { return velocity; }

	void ImguiDraw()override;
	/// <summary>
	/// 1フレ前のtransformを取得
	/// </summary>
	/// <returns></returns>
	Transform* GetLastTransform()const { return lastTransform; }
	/// <summary>
	/// ポジションを足す
	/// </summary>
	/// <param name="_pos">プラスするポジション</param>
	void AddPosition(const VECTOR3& _pos) { currentTransform->position += _pos; }

	/// <summary>
	/// 地面についていたらtrue
	/// </summary>
	/// <param name="_g"> 地面についていたらtrue</param>
	void SetGround(bool _g);
	/// <summary>
	/// 地面についているかどうかの判定
	/// </summary>
	/// <returns>地面についているかどうか</returns>
	bool GetGround() { return ground; }

private:

	VECTOR3 velocity;//移動する値

	Transform* currentTransform;//今のトランスフォーム

	Transform* lastTransform;//最後のトランスフォーム

	VECTOR3 gravity; //重力
	VECTOR3 firction; //抵抗、摩擦係数

	bool noGravity;
	bool ground;

	TransitorManager* transitor; //フェード中に移動をするとなぜかすり抜けてしまうことが多々あるのでそれの防止用

	int groundMissCount;
	VECTOR3 baseGravity;

};