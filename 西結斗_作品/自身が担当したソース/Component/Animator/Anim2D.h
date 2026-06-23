#pragma once
#include "../Component.h"

class Anim2D : public Component
{
public:
	Anim2D();
	~Anim2D();

	void Update()override;
	/// <summary>
	/// 2Dアニメーションの設定
	/// </summary>
	/// <param name="_speed">アニメーションのスピード</param>
	/// <param name="num">写真を何分割するか</param>
	void AnimSetting(float _speed, int num);

	/// <summary>
	/// アニメーションが再生中かどうか
	/// </summary>
	/// <returns></returns>
	bool IsPlaying() { return playing; }

	/// <summary>
	/// 今現在のアニメーションカウンター
	/// </summary>
	/// <returns>今現在のアニメーションカウンタ</returns>
	float GetAnimCounter() { return animCounter; }
	/// <summary>
	/// 今現在写真を何分割しているかどうか
	/// </summary>
	/// <returns> 今現在写真を何分割しているかどうか</returns>
	int GetGraphNum() { return graphNum; }

	/// <summary>
	/// アニメーションを終了
	/// </summary>
	void AnimStop() { playing = false; }
	//アニメーションの開始
	void AnimPlay() { playing = true; }
	//アニメーションカウンタのリセット
	void AnimReset() { animCounter = 0.0f; }

private:

	bool playing; //アニメーションが再生中かをとる
	float animCounter; //アニメーションのカウンタ
	int graphNum; //分割の枚数
	float animSpeed; //アニメーションのスピードの倍率
};
