#pragma once
#include "Transitor.h"

class WipeTransitor : public Transitor
{
public:
	enum Direction
	{
		UP,
		DOWN,
		RIGHT,
		LEFT,
	};
	WipeTransitor();
	~WipeTransitor();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// 画面がずれていく画面遷移の開始
	/// </summary>
	/// <param name="_sceneName">移行したいシーンの名前</param>
	/// <param name="_time">場面遷移の時間</param>
	/// <param name="_dir">どの方向にずれていくか上下左右の4方向</param>
	void WipeStart(std::string _sceneName, float _time, Direction _dir);
	/// <summary>
	/// グラデーション情報のセット
	/// </summary>
	void SetGradation();

private:
	Direction dir;
	int hGradation;
};
