#pragma once
#include "playerStateBase.h"

class PlayerWait : public PlayerStateBase
{
public:
	PlayerWait();
	~PlayerWait();

	void Update() override;
	void Draw() override;
	void Start() override;
	void Finish()override;

private:

};