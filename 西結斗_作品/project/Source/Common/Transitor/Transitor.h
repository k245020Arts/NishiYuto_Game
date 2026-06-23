#pragma once
#include "../../../Library/GameObject.h"

class Transitor : public GameObject
{
public:

	enum TransitorType
	{
		STAY,
		PLAY,
		FINISH,
	};

	Transitor();
	virtual ~Transitor();
	/// <summary>
	/// 場面遷移の開始
	/// </summary>
	/// <param name="_sceneName">移行したいシーン名</param>
	/// <param name="_time">場面遷移時間</param>
	virtual void StartTransitor(std::string _sceneName,float _time);
	/// <summary>
	/// 終了したかどうか
	/// </summary>
	/// <returns></returns>
	virtual bool IsEnd() const;

protected:

	int newWindow;
	int oldWindow;

	float timeMax;
	float time;

	VECTOR2I size;

	TransitorType transitorType;

};