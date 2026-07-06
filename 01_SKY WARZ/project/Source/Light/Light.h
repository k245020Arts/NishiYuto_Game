#pragma once
#include "../../Library/GameObject.h"

class Light : public GameObject
{
public:
	Light();
	~Light();

	void Update()override;
	void Draw() override;

private:
	int hLight;
	VECTOR3 dir;
};