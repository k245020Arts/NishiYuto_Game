#pragma once
#include "Transitor.h"

class PushTransitor : public Transitor
{
public:
	enum Direction
	{
		UP,
		DOWN,
		RIGHT,
		LEFT,
	};
	PushTransitor();
	~PushTransitor();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// PushTransitorの開始
	/// </summary>
	/// <param name="_sceneName">チェンジするシーンの名前</param>
	/// <param name="_time">時間</param>
	/// <param name="_dir">どの方向にpushさせるか</param>
	void StartPush(std::string _sceneName, float _time, Direction _dir);

private:
	Direction dir;
};
