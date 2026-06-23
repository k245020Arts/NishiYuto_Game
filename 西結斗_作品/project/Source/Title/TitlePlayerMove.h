#pragma once
#include "TitlePlayer.h"
#include "../Player/PlayerState/PlayerStateBase.h"

class TitleControl;
class TitlePlayer;
class BlurScreen;
class Fead;

class TitlePlayerMove : public PlayerStateBase
{
public:
	TitlePlayerMove();
	~TitlePlayerMove();
	void Update() override;
	void Draw() override;
	void Start() override;
	void Finish()override;
private:
	TitleControl* titleCtrl;
	TitlePlayer* player;
	Fead* fead;
	VECTOR3 cameraRotation;

	float progress;

	//BlurScreen* blurScreen;
	void Avoid(float speed);
};