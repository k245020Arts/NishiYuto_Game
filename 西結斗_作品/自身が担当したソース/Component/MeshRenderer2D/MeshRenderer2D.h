#pragma once
#include "../Component.h"

//2Dの画像を描画するときに使うクラス。

class Anim2D;
class Transform;

class MeshRenderer2D : public Component
{
public:
	//drawに使う関数の指定を行う列挙型
	enum GraphMode
	{
		DRAW_GRAPH,
		DRAW_RECT_GRAPH,
		DRAW_RECT_ROTA_GRAPH,
		DRAW_NUM,
		DRAW_RECT_ROTA_GRAPH_FAST_3F,
		DRAW_BILLBOARD,
	};

	MeshRenderer2D();
	~MeshRenderer2D();

	void Update()override;
	void Draw()override;

	/// <summary>
	/// TextureHandleの設定
	/// </summary>
	/// <param name="_image">画像ハンドル</param>
	/// <param name="_mode">描画モード</param>
	void TextureHandle(int _image,GraphMode _mode);
	/// <summary>
	/// 複数個セットするときにobjのTransformが使えないので画像専用のTransformをセットする関数
	/// </summary>
	/// <param name="_pos">ポジション</param>
	/// <param name="_rotate">回転</param>
	/// <param name="_scale">大きさ</param>
	void SetTransform(const VECTOR3& _pos, const VECTOR3& _rotate, const VECTOR3& _scale);
	/// <summary>
	/// 複数個セットするときにobjのTransformが使えないので画像専用のTransformをセットする関数
	/// </summary>
	/// <param name="transform">Transformを直に入れる</param>
	void SetTransform(const Transform& transform);
	void ImguiDraw()override;

	/// <summary>
	/// DrawRectGraphなどの始まる位置をセットする関数
	/// </summary>
	/// <param name="_pos">始まる位置</param>
	void SetStartPos(const VECTOR2I& _pos);

	/// <summary>
	/// アニメーションをスタートさせる関数
	/// </summary>
	/// <param name="_speed">速さ</param>
	/// <param name="_num">画像を何分割しているか</param>
	/// <param name="_animX">アニメーションの移動をX軸基準に行うか</param>
	void AnimStart(float _speed,int _num,bool _animX);
	/// <summary>
	///  アニメーションをスタートさせる関数、アニメーションの移動をX軸基準にする
	/// </summary>
	/// <param name="_speed">速さ</param>
	/// <param name="_num">画像を何分割しているか</param>
	void AnimStart(float _speed,int _num);
	/// <summary>
	/// 画像の大きさを取得
	/// </summary>
	/// <returns></returns>
	VECTOR2I GetImageSize() { return imageSize; }
	/// <summary>
	/// 画像の大きさをセットしている関数
	/// </summary>
	/// <param name="_pos">大きさ</param>
	void SetDrawImageSize(VECTOR2I _size);
	/// <summary>
	/// 文字の描画をしたいときに文字をセットした分の文字が数字で描画されるようにする
	/// </summary>
	/// <param name="_num"></param>
	void SetNum(int _num) { num = _num; }
	
	/// <summary>
	/// 画像の描画をするかしないか
	/// </summary>
	/// <param name="_draw">描画するならtrue</param>
	void SetDraw(bool _draw) { draw = _draw; }

	/// <summary>
	/// 画像の位置をセットする関数
	/// </summary>
	/// <param name="_position">画像の位置</param>
	void SetPosition(const VECTOR3& _position);

	/// <summary>
	/// 加算合成モードにするかどうか
	/// </summary>
	/// <param name="_mode">trueなら加算合成モード</param>
	void SetAddMode(bool _mode) { addMode = _mode;}

	/// <summary>
	/// 画像のフェードインの開始
	/// </summary>
	/// <param name="_timer">時間</param>
	void FeedInDraw(float _timer);
	/// <summary>
	/// 画像のフェードアウトの開始
	/// </summary>
	/// <param name="_timer">時間 </param>
	void FeedOutDraw(float _timer);

	/// <summary>
	/// 画像の半透明度を取得
	/// </summary>
	/// <returns></returns>
	int GetAlpha() const { return alpha; } 

	void SetScale(const VECTOR3& _scale);

private:
	int hImage;
	GraphMode mode; //描画モードの種類
	VECTOR2I imageSize;
	VECTOR2I drawImageSize;
	Transform* transform2D;

	Anim2D* anim2D;
	VECTOR2I startPos;
	int num;

	bool draw;

	void DrawNum();
	void NormalDraw(const Transform& transform);
	void AddDraw(const Transform& transform);

	bool addMode; //加算合成モードかどうか
	bool xAnim; //アニメーションのTextureが横に伸びているかどうか

	int alpha;
	bool feedIn;
	bool feedOut;

	float feedInOutTimer;
	float feedInOutTimerBase;
};