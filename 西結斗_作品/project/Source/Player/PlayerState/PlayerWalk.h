#pragma once
#include "playerStateBase.h"

class PlayerWalk : public PlayerStateBase
{
public:
	PlayerWalk();
	~PlayerWalk();

	void Update() override;
	void Draw() override;
	void Start() override;
	void Finish()override;

private:
	bool avoidStart;
	bool sound;
	bool firstOnes;
	bool secondOnes;
};