#pragma once
#include "../Component.h"
#include "../Color/Color.h"

class Shaker;

//3Dのモデルを描画したいときに行う関数。

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	~MeshRenderer();
	void Update()override;
	void Draw()override;
	/// <summary>
	/// モデルハンドルのセット
	/// </summary>
	/// <param name="_hHandle">モデルハンドル</param>
	/// <param name="_depricate">複製するかどうか</param>
	void ModelHandle(int _hHandle,bool _depricate);
	/// <summary>
	/// モデルハンドルのセット
	/// </summary>
	/// <param name="_hHandle">モデルハンドル</param>
	void ModelHandle(int _hHandle);
	void ImguiDraw()override;

	//プレイヤーのモデルとかが反対になってしまっているのを強制的に直す関数
	void RotationMesh(int _meshNum,float _angle);

	/// <summary>
	/// ローカルの座標を参照して描画をさせたいときに使う
	/// </summary>
	void DrawLocalPosition() { worldTransform = false; }
	/// <summary>
	/// ワールドの座標から描画させる
	/// </summary>
	void DrawWorldPosition() { worldTransform = true; }
	

private:
	int hModel;
	Color* color;
	int meshNum;
	float angle;

	bool draw;

	Shaker* shaker;
	bool worldTransform;
	bool depricate; //デプリケートモデルにしているかどうか
};