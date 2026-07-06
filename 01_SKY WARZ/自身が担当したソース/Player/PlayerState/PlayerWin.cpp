#include "PlayerWin.h"
#include "../Player.h"
#include "../../Component/Animator/Animator.h"
#include "../../Camera/Camera.h"
#include "../../GameControler/GameControler.h"

PlayerWin::PlayerWin()
{
	string = string = Function::GetClassNameC<PlayerWin>();;
	animId = ID::P_WIN;;
	//id = ID::P_ANIM_RUN;
}

PlayerWin::~PlayerWin()
{
}

void PlayerWin::Update()
{
	Player* player = GetBase<Player>();
	if (player->playerCom.anim->IsFinish()) {
		player->playerCom.anim->Play(ID::PLAYER_WIN_IDOL);
		player->playerCom.anim->SetPlaySpeed(1.0f);
	}
	if (player->playerCom.anim->AnimEventCan()) {
		player->playerCom.anim->SetPlaySpeed(0.7f);
	}
	else {
		player->playerCom.anim->SetPlaySpeed(1.0f);
	}
	if (player->playerCom.camera->GetCutNum() == 7) { //7ƒJƒbƒg–Ú‚ÅWin‚Ì•¶Žš‚Ì•`‰æ
		player->playerCom.gameManager->ResultUiStart(true);
	}
}

void PlayerWin::Draw()
{

}

void PlayerWin::Start()
{
	PlayerStateBase::Start();
}

void PlayerWin::Finish()
{
}
