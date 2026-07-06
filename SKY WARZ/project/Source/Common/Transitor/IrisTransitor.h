#pragma once
#include "Transitor.h"

class IrisTransitor : public Transitor
{
public:
	IrisTransitor();
	~IrisTransitor();

	void Update()override;
	void Draw()override;

	void SetMask();
	/// <summary>
	/// マスク画面遷移の開始
	/// </summary>
	/// <param name="_sceneName">移行したいシーンの名前</param>
	/// <param name="_time">場面遷移にかかる時間</param>
	/// <param name="_out">内から外にマスクしていくか、外から内にマスクしていくか</param>
	void StartIris(std::string _sceneName, float _time, bool _out);

private:
	int maskScreen;
	int maskHandle;
	float sizeCross;
	bool irisOut;
};
