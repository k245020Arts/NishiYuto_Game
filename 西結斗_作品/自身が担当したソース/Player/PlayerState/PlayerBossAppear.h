#pragma once
#include "PlayerStateBase.h"

class Fead;

class PlayerBossAppear : public PlayerStateBase
{
public:
	PlayerBossAppear();
	~PlayerBossAppear();

	void Update()override;
	void Draw()override;

	void Start() override;
	void Finish()override;

private:

	Fead* fead;
	bool feadOut;

};
