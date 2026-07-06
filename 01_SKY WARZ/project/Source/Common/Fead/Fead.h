#pragma once
#include "../../../Library/GameObject.h"
#include "../Easing.h"

class Color;

class Fead : public GameObject
{
public:

	enum FeadState
	{
		NONE = -1,
		FEAD_OUT,
		FEAD_IN,
	};
	Fead();
	~Fead();

	void Update()override;
	void Draw()override;

	/// <summary>
	/// feedInの開始
	/// </summary>
	/// <param name="_time">時間</param>
	/// <param name="_color">色</param>
	/// <param name="_func">Easingの関数ポインタ</param>
	void FeadIn(float _time,int _color,std::function<int(int, int, float)> _func);
	/// <summary>
	/// feedOutの開始
	/// </summary>
		/// <param name="_time">時間</param>
	/// <param name="_color">色</param>
	/// <param name="_func">Easingの関数ポインタ</param>
	void FeadOut(float _time, int _color, std::function<int(int, int, float)> _func);

	/// <summary>
	/// フェード中かどうか
	/// </summary>
	/// <returns>フェード中ならTrue</returns>
	bool IsFead() const { return feedTime > 0.0f; };

	/// <summary>
	/// 現在のフェードの状態を取得
	/// </summary>
	/// <returns>フェードの状態</returns>
	FeadState GetFeadState() const { return feadIn; }


private:

	float feedTime;
	float feedCountMax;

	int color;
	int alpha;
	std::function<int(int, int, float)> easingFunc;
	FeadState feadIn;

};