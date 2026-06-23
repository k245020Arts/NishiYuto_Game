#pragma once
#include "Transitor.h"
#include <vector>
#include <random>

class TitleTransitor : public Transitor
{
public:
	TitleTransitor();
	~TitleTransitor();

	void Update()override;
	void Draw()override;

	void SetTile();
	/// <summary>
	/// タイルフェードの開始
	/// </summary>
	/// <param name="_sceneName">移行したいシーン名</param>
	/// <param name="_time">場面遷移の時間</param>
	/// <param name="_useGravity">タイルがはがれた後下に落ちるかどうか</param>
	void StartTile(std::string _sceneName, float _time,bool _useGravity);

	struct TitleInfo
	{
		TitleInfo() { TitleInfo(VECTOR2I(0, 0), 0.0f, 0.0f); }
		TitleInfo(VECTOR2I _p, float _g, float _v) {
			pos = _p;
			gravity = _g;
			velocity = _v;
		}

		VECTOR2I pos;
		float gravity;
		float velocity;
	};

private:
	int tileSize;
	
	std::mt19937 mt;
	std::vector<TitleInfo> tile;
	float erase;
	float gravity;
	bool useGravity;
	float gravityTime;
};