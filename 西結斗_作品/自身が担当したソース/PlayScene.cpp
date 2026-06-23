#include "PlayScene.h"
#include "../Source/Component/Object/Object3D.h"
#include "../Source/Component/ComponentManager.h"
#include "../Source/Component/MeshRenderer/MeshRenderer.h"
#include "../Source/Component/Transform/Transform.h"
#include "../Source/Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../Source/Player/Player.h"
#include "../Source/Component/Physics/Physics.h"
#include "../Source/Camera/Camera.h"
#include "../Source/Component/Hierarchy/Hierarchy.h"
#include "../Source/Stage/StageManager.h"
#include "../Source/Player/PlayerManager.h"
#include "../Source/Enemy/EnemyManager.h"
#include "../Source/Common/ResourceLoader.h"
#include "../Source/Component/Collider/CollsionManager.h"
#include "../Source/Common/Debug/Debug.h"
#include "../Source/Weapon/WeaponManager.h"
#include "../Source/Light/Light.h"
#include "../Source/Camera/CameraManager.h"
#include "../Source/Common/Effect/EffectManager.h"
#include "../Source/Common/Sound/SoundManager.h"
#include "../Source/Stage/Sky.h"
#include "../Source/Common/BlurScreen.h"
#include "../Source/Common/Transitor/FadeTransitor.h"
#include "../Source/Common/Transitor/PushTransitor.h"
#include "../Source/Common/Transitor/WipeTransitor.h"
#include "../Source/Common/Transitor/StripTransitor.h"
#include "../Source/Common/Transitor/TitleTransitor.h"
#include "../Source/Common/Transitor/IrisTransitor.h"
#include "../Source/GameControler/GameControler.h"
#include "../Source/Common/InputManager/inputManager.h"
#include "../Source/Wave/Wave.h"
#include "Component/UI/UIManager/UIManager.h"
#include "../ImGui/imgui.h"
#include "Pause/PauseScreen.h"

//int PlayScene::count = 0;

//YAML_CONVERT_STRUCT(P, name, level, items)

PlayScene::PlayScene()
{
	SoundManager::GetInstance()->AllDeleteSound();

	Hierachy* hierachy = new Hierachy();
	
	BlurScreen* blur =  new BlurScreen();

	StageManager* stageManager = new StageManager();

	EnemyManager* enemy =  new EnemyManager();

	PlayerManager* player = new PlayerManager();

	WeaponManager* weapon = new WeaponManager();

	CollsionManager* collManager =  new CollsionManager();

	Light* light = new Light();

	CameraManager* cameraManager = new CameraManager();

	Sky* sky = new Sky();

	UIManager* uiManager = new UIManager();
	PauseScreen* pauseScreen = new PauseScreen();
	GameControler* gameManager = new GameControler();
	
	collManager->FindGameControler();

	
	uiManager->ButtonUISet();
	
	stageManager->CreateStage();
	//enemy->CreateEnemy();

	cameraManager->CreateCamera();
	player->CreatePlayer();
	//enemy->PlayerObjPointer();
	
	cameraManager->ParentObj("PLAYER");
	enemy->FindPlayer();
	weapon->CreatePlayerWeapon();
	//weapon->CreateEnemyWeapon();
	collManager->SetDrawOrder(-90000);
	hierachy->SetDrawOrder(-100000);
	sky->CreateSky();
	//gameManager->CreateNum();
	gameManager->SetPointer();

	//YamlReader ya;
	//std::vector<P> pl;
	//ya.SaveVector<P>("ala", pl);
	
	blur->SetDrawOrder(-500);
	mode = NORMAL;
	//m->TextureHandle(LoadGraph("data/texture/101backGround.png"),MeshRenderer2D::DRAW_RECT_ROTA_GRAPH,VECTOR3(500,500,500),VZero,VZero);

	Wave* wave = new Wave;
	
	SoundManager::GetInstance()->PlaySceneLoad();
	SoundManager::GetInstance()->PlayBGM(Sound_ID::PLAY_BGM, true, true);
}

PlayScene::~PlayScene()
{
	//Load::AllDelete();
	//int i = Load::GetCounter();
	Debug::ClearLogger();
	//EffectManager::GetInstance()-> DeleteAllEffect();
}

void PlayScene::Update()
{
	//if (input->KeyInputDown("SceneChange"))
	//{
		//FindGameObject<FadeTransitor>()->StartTransitor("TITLE", 1.0f);
	//}
	
#ifdef _DEBUG
	if (InputManager::GetInstance()->KeyInputDown("changeMode")) {
		if (mode == NORMAL) {
			mode = CAMERA_EDITOR;
		}
		else {
			mode = NORMAL;
		}
	}

	if (mode == CAMERA_EDITOR) {
		return;
	}

	if (InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_T)) {
		FindGameObject<FadeTransitor>()->StartTransitor("TITLE", 1.0f);
	}

#endif // _DEBUG
	
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}

}

void PlayScene::Draw()
{
	/*DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));*/
}
