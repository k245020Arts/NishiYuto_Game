#pragma once
#include "playerStateBase.h"

class PlayerDie : public PlayerStateBase
{
public:
	PlayerDie();
	~PlayerDie();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;


private:
	float slowTime;
	//bool one;
};