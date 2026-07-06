#include "App.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Time.h"
#include <DxLib.h>
#include "../Source/Common/ResourceLoader.h"
#include "../Source/Common/Singleton/SingleTonUpdater.h"
#include "../Source/Common/PostEffect/PostEffect.h"

//#define POST_EFFECT

void AppInit()
{
	ObjectManager::Init();
	SceneManager::Init();
	Time::Init();
#ifdef POST_EFFECT
	PostEffect::Init();
#endif

	
}

void AppUpdate()
{
	SceneManager::Update();
	ObjectManager::Update();
	SingleTonUpdater::Update();

#ifdef POST_EFFECT
	PostEffect::BeginScene();
#endif
	
}

void AppDraw()
{
	Time::Refresh();
	SceneManager::Draw();
	ObjectManager::Draw(); 
	SingleTonUpdater::Draw();
#ifdef POST_EFFECT
	PostEffect::EndScene();
#endif
}

void AppRelease()
{
	// DontDestroyフラグを全解除してから終了
	/*for (GameObject* obj : ObjectManager::GetAllObject()) {
		obj->DontDestroyOnSceneChange(false);
	}*/

	Time::Release();
	SceneManager::Release();
	ObjectManager::Release();
	SingleTonUpdater::Release();
	ResourceLoad::AllDelete();
#ifdef POST_EFFECT
	PostEffect::Release();
#endif

}

bool AppIsExit()
{
	return SceneManager::IsExit();
}