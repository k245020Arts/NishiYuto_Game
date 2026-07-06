#include "BossLose.h"
#include "../../../Camera/Camera.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../Boss.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../GameControler/GameControler.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Component/Collider/ColliderBase.h"
#include "../../../Component/ComponentManager.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Component/Collider/ModelCollider.h"
#include "../../../Component/Collider/SphereCollider.h"
#include "../../../Component/UI/UIManager/UIManager.h"

BossLose::BossLose()
{
	animId = ID::BOSS_DIE;
	//id = ID::BOSS_DIE;
	string = Function::GetClassNameC<BossLose>();
	//slowTime = 0.0f;
}

BossLose::~BossLose()
{
}

void BossLose::Update()
{
	Boss* b = GetBase<Boss>();
	//‰¹º‚ªI‚í‚Á‚½‚çI—¹
	if (!SoundManager::GetInstance()->CheckSe(Sound_ID::WIN)) {
		b->enemyBaseComponent.gameManager->ChangeState(GameControler::SCENE_CHANGE);
	}
}

void BossLose::Draw()
{
}

void BossLose::Start()
{
	Boss* b = GetBase<Boss>();
	EnemyStateBase::Start();
	SoundManager::GetInstance()->StopBGM(Sound_ID::PLAY_BGM);
	SoundManager::GetInstance()->PlaySe(Sound_ID::WIN);
	b->enemyBaseComponent.camera->CutSceneChangeState("PlayerWin", false);
	FindGameObject<UIManager>()->SetUIDraw(false);
}

void BossLose::Finish()
{
}
