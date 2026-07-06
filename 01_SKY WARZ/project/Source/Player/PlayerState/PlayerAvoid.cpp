#include "playerAvoid.h"
#include "playerStateManager.h"
#include "../player.h"
#include "../../Camera/Camera.h"
#include "../../Component/Animator/Animator.h"
#include "../../Component/Physics/Physics.h"
#include "../../Common/Easing.h"
#include "../../Common/InputManager/PadInput.h"
#include "../../Component/Color/Color.h"
#include "../../Common/ResourceLoader.h"
#include "../../Common/Function.h"
#include "../../Common/Sound/SoundManager.h"
#include "../../Component/MotionBlur/MotionBlur.h"
#include "../../Enemy/EnemyManager.h"

PlayerAvoid::PlayerAvoid()
{
	time		= 0.0f;
	string		= string = Function::GetClassNameC<PlayerAvoid>();;
	//id = ID::P_ANIM_AVOID;

	animId		= ID::P_ANIM_AVOID;
	cameraAngle = 0.0f;
	maxFrame	= 0.0f;
	animTime	= 0.0f;
	enemyTransform = Transform();
}

PlayerAvoid::~PlayerAvoid()
{
	
}

void PlayerAvoid::Update()
{
	Player* p = GetBase<Player>();
	if (time >= 0.5f) {
		if (p->playerCom.anim->IsFinish())
		{
			p->playerCom.player->AvoidFinishState();
		}
		//スティックを倒していたときは早くステートを切り替える。
		if (maxFrame - 4.0f <= p->playerCom.anim->GetCurrentFrame()) {
			p->playerCom.player->PlayerStickInput();
			VECTOR3 angle = p->playerCom.player->GetWalkAngle();
			if (fabs(angle.x) >= 0.6f || fabs(angle.z) >= 0.6f) {
				p->playerCom.player->AvoidFinishState();
			}
		}
		
	}
	else {
		time += p->playerCom.player->GetPlayerObj()->GetObjectTimeRate();
		if (time >= 0.5f) {
			time = 0.5f;
			/*p->playerCom.physics->SetFirction(VECTOR3(250.0f, 250.0f, 250.0f));*/
		}
		//スピードの調整
		float easeIng;
		if (animTime <= p->playerCom.anim->GetCurrentFrame()) {
			easeIng = 1500.0f;
		}
		else {
			easeIng = Easing::Sin90Cube(7000.0f, 200.0f, time);
		}
		float animEasing	= Easing::Sin90Cube(1.0f, 0.8f,time);
		
		float ySpeed		= Easing::SinCube(800.0f, 0.0f, time);
		p->playerCom.player->Avoid(easeIng, 6000.0f,cameraAngle,ySpeed);
		p->playerCom.anim->SetPlaySpeed(animEasing);
	}
	if (p->playerCom.anim->AnimEventCan()) {
		//モーションブラーの再生
		p->playerCom.blur->MosionStart(0.025f, 0.05f, animId, 1);
	}
}

void PlayerAvoid::Draw()
{
	
}

void PlayerAvoid::Start()
{
	Player* p = GetBase<Player>();
	cameraAngle = p->playerCom.camera->GetCameraTransform()->rotation.y;
	PlayerStateBase::Start();
	p->playerCom.physics->SetVelocity(VZero);
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA  - VECTOR3(9000,0,9000));
	p->playerCom.physics->SetGravity(VECTOR3(0.0f, -6000.0f, 0.0f));
	p->playerCom.camera->CameraLeapSet(0.13f);
	SoundManager::GetInstance()->PlaySe(Sound_ID::V_P_AVOID);

	maxFrame = p->playerCom.anim->GetMaxFrame();
	maxFrame -= 10.0f;
	animTime = 18.0f;
}

void PlayerAvoid::Finish()
{
	Player* p = GetBase<Player>();
	StateChangeBase();
	p->playerCom.player->PlayerStickInput();
	//p->playerCom.color->setRGB(Color::Rgb(255, 255, 255, 255));
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	p->playerCom.physics->SetGravity(VECTOR3(0.0f, -6000.0f, 0.0f));
	p->playerCom.camera->CameraLeapSet(0.2f);
	if (!p->playerCom.enemyManager->CameraInEnemy()) {
		Transform nearEnemyPos = p->playerCom.enemyManager->NearEnemyPos(p->playerTransform->position);
		enemyTransform = nearEnemyPos;
		p->playerCom.camera->AttackEnemyFovChange(&enemyTransform, 300.0f);
	}
}

void PlayerAvoid::StateChangeBase()
{
	Player* p = GetBase<Player>();
	time = 0.0f;
	Time::ChangeDeltaRate(1.0f);
	p->playerCom.anim->SetPlaySpeed(1.0f);
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	//p->playerCom.player->GetPlayerObj()->SetObjectTimeRate(1.0f);
	
}
