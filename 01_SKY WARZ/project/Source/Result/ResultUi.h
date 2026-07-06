#pragma once
#include "../Component/UI/UIBase.h"

class ResultUi : public Component
{
public:
	ResultUi();
	~ResultUi();

	void Update()override;
	void Draw()override;

	void ResultStart(bool _win);

private:

	int hImage;
	bool win;

	float normalExrate;
	float addExrate;
	bool start;
	float addCount;
	int addDrawValue;
	float scaleCount;

	float flashTime; 
};
