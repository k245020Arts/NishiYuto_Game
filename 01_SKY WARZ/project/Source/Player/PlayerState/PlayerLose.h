#pragma once
#include "PlayerStateBase.h"

class PlayerLose : public PlayerStateBase
{
public:
	PlayerLose();
	~PlayerLose();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

};