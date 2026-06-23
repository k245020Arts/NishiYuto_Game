#include "playerDamage.h"
#include "../../Component/Animator/Animator.h"
#include "playerStateManager.h"
#include "../../Component/Color/color.h"
#include "../../Component/Physics/Physics.h"
#include "../../Component/Shaker/Shaker.h"
#include "../player.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Camera/Camera.h"
#include "../../Component/Transform/Transform.h"

PlayerDamage::PlayerDamage()
{

	string		= string = Function::GetClassNameC<PlayerDamage>();;
	animId		= ID::P_DAMAGE;
	eRotation	= VZero;
	//id = ID::P_DAMAGE;
}

PlayerDamage::~PlayerDamage()
{
}

void PlayerDamage::Update()
{
	Player* p = GetBase<Player>();
	//モーションの再生が終わったら移動ステートに戻る
	if (p->playerCom.anim->IsFinish()) {
		p->playerCom.stateManager->ChangeState(StateID::PLAYER_WALK_S);
	}
	
}

void PlayerDamage::Draw()
{
}

void PlayerDamage::Start()
{
	Player* p = GetBase<Player>();
	PlayerStateBase::Start();
	
	eRotation = p->playerCom.hitObj->GetTransform()->rotation;
	//p->playerCom.physics->SetVelocity(VECTOR3(0, 0, 3000) * MGetRotY(eRotation.y));
	p->playerCom.physics->SetVelocity(VECTOR3(0, 0, 5000) * MGetRotY(-eRotation.y));
	p->playerCom.shaker->ShakeStart(VECTOR3(10, 10, 10), Shaker::HORIZONAL_SHAKE, false, 0.2f);
	blendSpeed = 0.3f;
	VECTOR3 cameraPos = p->playerCom.camera->GetCameraTransform()->position;
	VECTOR3 cameraFowardPos = cameraPos + p->playerCom.camera->GetCameraTransform()->Forward() * 1200.0f;
	VECTOR3 toEnemy = p->playerCom.hitObj->GetTransform()->position - cameraFowardPos;
	float dot = VDot(p->playerCom.camera->GetCameraTransform()->Forward(), toEnemy.Normalize());

	//プレイヤーの真後ろではなく少し斜め前の敵にもカメラの角度を合わせる。
	if (dot < 0.2f) {
		p->playerCom.camera->AttackEnemyFovChange(p->playerCom.hitObj->GetTransform(), 500.0f);
	}
	if (!p->playerCom.enemyManager->CameraInEnemy()) {
		
	}
	p->noDamage = true;
}

void PlayerDamage::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.color->setRGB(Color::Rgb(255.0f, 255.0f, 255.0f, 255.0f));
	p->playerCom.shaker->ShakeFinish();
	//p->playerCom.physics->SetGravity(VECTOR3(0, -150, 0));
	p->noDamage = false;
}
