#pragma once
#include "../Component/Component.h"

class Stage : public Component
{
public:
	Stage();
	~Stage();
	void Update() override;
	void Draw() override;
	void Start();

private:

};