#include "TitlePlayerIdol.h"
#include "../Component/Animator/Animator.h"

TitlePlayerIdol::TitlePlayerIdol()
{
	animId = ID::PLAYER_WIN_IDOL;
	string = Function::GetClassNameC<TitlePlayerIdol>();;
}

TitlePlayerIdol::~TitlePlayerIdol()
{
}

void TitlePlayerIdol::Update()
{
	obj->GetTransform()->position = firstPos;
}

void TitlePlayerIdol::Draw()
{
}

void TitlePlayerIdol::Start()
{
	TitlePlayer* player = GetBase<TitlePlayer>();
	player->playerCom.anim->Play(animId, blendSpeed);
	noStateChange = false;

	firstPos = obj->Component()->GetComponent<Transform>()->position;
}

void TitlePlayerIdol::Finish()
{
}

