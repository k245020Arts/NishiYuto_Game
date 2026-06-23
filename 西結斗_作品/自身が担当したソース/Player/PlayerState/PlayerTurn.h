#pragma once
#include "PlayerStateBase.h"

class PlayerTurn : public PlayerStateBase
{
public:
	PlayerTurn();
	~PlayerTurn();

	void Update() override;
	void Draw() override;
	void Start() override;
	void Finish()override;

private:
	bool event;
};