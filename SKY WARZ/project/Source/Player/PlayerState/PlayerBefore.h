#pragma once
#include "PlayerStateBase.h"

class PlayerBefore : public PlayerStateBase
{
public:
	PlayerBefore();
	~PlayerBefore();

	void Update()override;
	void Draw()override;

	void Start() override;
	void Finish()override;

private:

};
