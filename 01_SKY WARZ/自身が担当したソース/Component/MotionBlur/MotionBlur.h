#pragma once
#include "../Component.h"
#include <vector>
#include "../../Common/ID/ID.h"
#include "../Transform/Transform.h"

class Animator;

struct BlurInfo
{

	BlurInfo() {
		baseModel = 1;
		time = 0.0f;
		maxTime = 0.0f;
		alpha = 0.0f;
		animFrame = 0.0f;
		attackID = 0;
		matrix = MGetIdent();
		firstPos = VZero;
		firstMatrix = MGetIdent();
		transform = Transform();
	}
	int baseModel; //モーションブラーに使用するモデルデータ
	float time; //時間
	float maxTime; //最大時間
	float alpha; //半透明度

	float animFrame; //ブラーで使用するアニメーションのフレーム
	int attackID; //アタッチのID
	MATRIX matrix; //アニメーションで使われている行列

	VECTOR3 firstPos; //最初の位置
	MATRIX firstMatrix; //最初のマトリックス
	Transform transform; //トランスフォーム
};

//モーションブラーを行うためのクラス.

class MotionBlur : public Component
{
public:
	MotionBlur();
	~MotionBlur();

	void Update()override;
	void Draw()override;

	//初期化するときに行う関数。ベースとなるモデルをnewした直後に設定する。
	void Start(int _model);

	/// <summary>
	/// モーションブラーの開始
	/// </summary>
	/// <param name="_time">継続時間</param>
	/// <param name="_alpha">半透明度</param>
	/// <param name="_id">アニメーションのID</param>
	/// <param name="_modelFrame">アニメーションの移動をなくすためにやる。フレームの番号を書く</param>
	void MosionStart(float _time,float _alpha, ID::IDType _id,int _modelFrame);

private:

	std::vector<BlurInfo> blur; //ブラー情報
	int model;
	Animator* anim;
};