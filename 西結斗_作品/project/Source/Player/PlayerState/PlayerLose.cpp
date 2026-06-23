#include "PlayerLose.h"
#include "../../Component/Animator/Animator.h"
#include "../../GameControler/GameControler.h"
#include "../../Camera/Camera.h"
#include "../player.h"
#include "../../Common/Sound/SoundManager.h"
#include "../../Component/Collider/SphereCollider.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Wave/Wave.h"
#include "../../../Source/Component/UI/UIManager/UIManager.h"

PlayerLose::PlayerLose()
{
	animId = ID::P_DIE;
	//id = ID::P_DIE;
	string = Function::GetClassNameC<PlayerLose>();
	//one = false;

}

PlayerLose::~PlayerLose()
{
}

void PlayerLose::Update()
{
	Player* p = GetBase<Player>();
	if (!p->playerCom.camera->IsCutScene()) { //負けた時のカットシーンが終わったらシーンが終了
		p->playerCom.gameManager->ChangeState(GameControler::SCENE_CHANGE);
	}
	if (p->playerCom.camera->GetCutNum() == 2) { //カットが2の時に負けのUIを描画
		p->playerCom.gameManager->ResultUiStart(false);
	}
}

void PlayerLose::Draw()
{
}

void PlayerLose::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	SoundManager::GetInstance()->StopBGM(Sound_ID::PLAY_BGM);
	SoundManager::GetInstance()->PlaySe(Sound_ID::LOSE);
	//one = false;
	p->playerCom.enemyManager->CameraRockOnStart(p->playerCom.camera);

	Wave* wave = FindGameObject<Wave>();

	if (wave != nullptr) {
		//ボスが出ているならボス用のカットインを流す
		if (wave->GetBossWave()) {
			p->playerCom.camera->CutSceneChangeState("PlayerDieBoss", false);
		}
		else {
			p->playerCom.camera->CutSceneChangeState("PlayerDie", false);
		}
	}
	
	FindGameObject<UIManager>()->SetUIDraw(false);
}

void PlayerLose::Finish()
{
}
