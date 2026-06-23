#pragma once
#include "TitlePlayer.h"
#include "../Player/PlayerState/PlayerStateBase.h"

class TitlePlayerIdol : public PlayerStateBase
{
public:
	TitlePlayerIdol();
	~TitlePlayerIdol();

	void Update() override;
	void Draw() override;
	void Start() override;
	void Finish()override;
private:
	VECTOR3 firstPos;
};
