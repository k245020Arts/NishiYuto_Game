#pragma once
#include "colliderBase.h"

class SphereCollider : public ColliderBase
{
public:
	SphereCollider();
	~SphereCollider();
	void Update() override;
	void Draw() override;

	void Start()override;

private:

};
