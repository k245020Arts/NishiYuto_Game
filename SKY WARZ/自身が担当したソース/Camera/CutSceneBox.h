#pragma once
#include "../../Library/GameObject.h"

class CutSceneBox : public GameObject
{
public:
	CutSceneBox();
	~CutSceneBox();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// カットシーン用のboxを生成する関数
	/// </summary>
	/// <param name="_time">時間</param>
	/// <param name="_color">色</param>
	/// <param name="_func">easingの関数ポインタ</param>
	void StartBox(float _time, int _color, std::function<int(int, int, float)> _func);
	/// <summary>
	/// カットシーン用のboxを終了する関数
	/// </summary>
	/// <param name="_time">時間</param>
	/// <param name="_color">色</param>
	/// <param name="_func">easingの関数ポインタ</param>
	void FinishBox(float _time, int _color, std::function<int(int, int, float)> _func);
	/// <summary>
	/// フェード中かどうか
	/// </summary>
	/// <returns>フェード中ならtrue</returns>
	bool IsFead()const { return feedTime > 0.0f; };

private:

	float feedTime;
	float feedCountMax;

	int alpha;
	std::function<int(int, int, float)> easingFunc; //easingを制御するための関数ポインタ

	bool feadIn;

	const int BOX_SIZE = 150;

	const int UP_BOX_LIMIT = BOX_SIZE;
	const int DOWN_BOX_LIMIT = 1080 - BOX_SIZE;

	int boxPosY;
};
