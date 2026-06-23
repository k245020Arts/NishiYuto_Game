#include "BootScene.h"
#include "../Source/Common/InputManager/InputManager.h"
#include "../Source/Common/ResourceLoader.h"
#include "../Source/Component/Collider/CollsionManager.h"
#include "../Source/Common/Sound/SoundManager.h"
#include "../Source/Common/Effect/EffectManager.h"
#include "../Source/Common/Debug/Debug.h"
#include "../Source/Common/Transitor/TransitorManager.h"
#include "../Source/Common/Transitor/FadeTransitor.h"
#include "Common/Random.h"
#include "../Source/Common/Fead/Fead.h"

BootScene::BootScene()
{
	InputManager* input			= InputManager::GetInstance();
	//初期化
	ResourceLoad::Init();
	ID::Init();
	Sound_ID::InitID();
	Debug::InitDebug(input);
	Random::Init();
	SoundManager::GetInstance();
	EffectManager::GetInstance();
	TransitorManager* transitor = new TransitorManager();
	new CollsionManager();
	new Fead();
	/*Load::SetAsync(false);*/
	//タイトルとプレイシーンに必要な背景モデルと当たり判定使用モデルをロード
	ResourceLoad::LoadModel("stage_s", ID::S_MODEL, true);
	ResourceLoad::LoadModel("cube", ID::WALL, true);

}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	
	FindGameObject<FadeTransitor>()->StartTransitor("TITLE", 1.0f); // 起動が終わったらTitleを表示
}

void BootScene::Draw()
{
}
