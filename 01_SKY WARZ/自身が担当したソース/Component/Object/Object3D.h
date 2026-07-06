#pragma once
#include "BaseObject.h"

class Object3D : public BaseObject
{
public:
	Object3D();
	virtual ~Object3D();
	void Update() override;
	void Draw() override;
	/// <summary>
	/// 初期化処理3Dならこっちを使うのを推奨
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_rotate">回転</param>
	/// <param name="_scale">大きさ</param>
	/// <param name="name">オブジェクトの名前</param>
	void Init(const VECTOR3& _pos, const VECTOR3& _rotate, const VECTOR3& _scale, std::string name);
	/// <summary>
	/// 初期化処理3Dならこっちを使うのを推奨
	/// </summary>
	/// <param name="_transform">トランスフォーム</param>
	/// <param name="name">オブジェクトの名前</param>
	void Init(const Transform& _transform,std::string name);

private:

};