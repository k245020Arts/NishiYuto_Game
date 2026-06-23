#pragma once
#include "PlayerStateBase.h"

class PlayerWin : public PlayerStateBase
{
public:
	PlayerWin();
	~PlayerWin();

	void Update() override;
	void Draw() override;
	void Start() override;
	void Finish()override;


private:

};
