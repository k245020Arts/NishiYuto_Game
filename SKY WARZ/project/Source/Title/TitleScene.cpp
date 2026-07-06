#include "TitleScene.h"
#include "../../Source/Component/Object/Object3D.h"
#include "../../Source/Component/ComponentManager.h"
#include "../../Source/Component/MeshRenderer/MeshRenderer.h"
#include "../../Source/Component/Transform/Transform.h"
#include "../../Source/Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../../Source/Player/Player.h"
#include "../../Source/Component/Physics/Physics.h"
#include "../../Source/Camera/Camera.h"
#include "../../Source/Component/Hierarchy/Hierarchy.h"
#include "../../Source/Stage/StageManager.h"
#include "../../Source/Player/PlayerManager.h"
#include "../../Source/Enemy/EnemyManager.h"
#include "../../Source/Component/Collider/CollsionManager.h"
#include "../../Source/Weapon/WeaponManager.h"
#include "../../Source/Light/Light.h"
#include "../../Source/Camera/CameraManager.h"
#include "../../Source/Common/Effect/EffectManager.h"
#include "../../Source/Stage/Sky.h"
#include "TitleControl.h"
#include "../Common/BlurScreen.h"

TitleScene::TitleScene()
{
	//hImage = LoadGraph("data/image/SlashLocus.png");

	BlurScreen* blur = new BlurScreen();

	Hierachy* hierachy = new Hierachy();
	
	StageManager* stageManager = new StageManager();
	
	PlayerManager* player = new PlayerManager();

	WeaponManager* weapon = new WeaponManager();

	CollsionManager* collManager = new CollsionManager();

	Light* light = new Light();

	CameraManager* cameraManager = new CameraManager();
	Sky* sky = new Sky();

	stageManager->CreateStage();
	//enemy->CreateEnemy();
	cameraManager->TitleCameraCreate();
	player->CreateTitlePlayer();
	//enemy->PlayerObjPointer();
	cameraManager->TitleParentObj("PLAYER");
	weapon->CreatePlayerWeapon();
	collManager->SetDrawOrder(-90000);
	hierachy->SetDrawOrder(-100000);
	sky->CreateSky();

	TitleControl* titleControl = new TitleControl();
}

TitleScene::~TitleScene()
{
	//DeleteGraph(hImage);
	//Load::AllDelete();
	//FindGameObject<SoundManager>()->AllDeleteSound();
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	//DrawGraph(0, 0, hImage, true);
	//DrawGraph(750, 100, titleImage, true);
	//DrawRotaGraph(1000, 900, (double)exrate,0.0,keyImage, true);

	/*DrawBox(0, 0, 1920, 1080, 0x111111, true);
	extern const char* Version();*/
	/*DrawString(0, 20, Version(), GetColor(255,255,255));
	DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	DrawFormatString(100, 100, GetColor(255,255,255), "%4.1f", 1.0f / Time::DeltaTime());
	DrawString(100, 400, "Push [P]Key To Play", GetColor(255, 255, 255));*/
	//DrawGraph(100, 100, hImage, true);
}
