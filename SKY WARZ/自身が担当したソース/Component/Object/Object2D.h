#pragma once
#include "BaseObject.h"

class Object2D : public BaseObject
{
public:
	Object2D();
	virtual ~Object2D() {}
	void Update()override;
	void Draw()  override;

	/// <summary>
	/// 初期化処理2Dならこっちを使うのを推奨
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_rotate">回転</param>
	/// <param name="_scale">大きさ</param>
	/// <param name="name">オブジェクトの名前</param>
	void Init(const VECTOR2F& _pos, const VECTOR2F& _rotate, const VECTOR2F& _scale, std::string name);
	/// <summary>
	/// 初期化処理2Dならこっちを使うのを推奨
	/// </summary>
	/// <param name="_transform">トランスフォーム</param>
	/// <param name="name">オブジェクトの名前</param>
	void Init(const Transform& _transform, std::string name);

	//void AnimSetting(float _speed, int num);

private:

	//Vector2 imageSize;	// 画像サイズ
	//int hImage;	// 画像ハンドラ

	//float animCounter; //アニメーションのカウンタ
	//int graphNum; //分割の枚数
	//float animSpeed; //アニメーションのスピードの倍率
};