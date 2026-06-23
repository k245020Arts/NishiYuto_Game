#include "PlayerBefore.h"

PlayerBefore::PlayerBefore()
{
	string = string = Function::GetClassNameC<PlayerBefore>();;
	animId = ID::P_ANIM_IDOL;
}

PlayerBefore::~PlayerBefore()
{
}

void PlayerBefore::Update()
{
}

void PlayerBefore::Draw()
{
}

void PlayerBefore::Start()
{
	PlayerStateBase::Start();
}

void PlayerBefore::Finish()
{
}
