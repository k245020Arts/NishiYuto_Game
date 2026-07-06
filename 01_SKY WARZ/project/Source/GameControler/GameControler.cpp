#include "GameControler.h"
#include "../Common/Transitor/FadeTransitor.h"
#include "../Component/Object/Object2D.h"
#include "../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../Component/ComponentManager.h"
#include "../Common/ResourceLoader.h"
#include "../Common/Sound/SoundManager.h"
#include "../Camera/Camera.h"
#include "../Wave/Wave.h"
#include "../../ImGui/imgui.h"
#include "../Common/Debug/Debug.h"
#include "../Result/ResultUi.h"
#include "../Common/InputManager/InputManager.h"
#include "../Pause/PauseScreen.h"
#include "../GuideWindow/GuideWindow.h"

namespace {
	const int GAME_STATE_MAX = 5;

	const std::string stateName[GAME_STATE_MAX]{
		{"BEFORE"},
		{"PLAY"},
		{"BOSS_PLAY_BEFORE"},
		{"WIN"},
		{"LOSE"},
	};
}

int GameControler::gamePlayNum = 0;

GameControler::GameControler()
{
	startCount = 0.0f;
	gamePlayNum++;
	SetDrawOrder(-500);
	pause = FindGameObject<PauseScreen>();
	//winImage = Load::LoadImageGraph(Load::IMAGE_PATH + "Win", ID::WIN);
	//loseImage = Load::LoadImageGraph(Load::IMAGE_PATH + "Lose", ID::LOSE);

	//結果のUIのセッティング
	Object2D* result = new Object2D();
	result->Init(Transform(VECTOR3(980.0f, 149.0f, 0.00), VZero, VOne), "resultUi");
	result->SetDraw(false);
	resultUi  = result->Component()->AddComponent<ResultUi>();
	result->SetDrawOrder(-1000);
	
	resultCounter = 3.0f;

#ifdef STRING_MODE
	beforeState = "";
	nowState = "BEFORE";
#else
	beforeGameState = GameState::NONE;
	gameState = GameState::BEFORE;
#endif // STRING_MODE
	
	changeState = true;

	guide = new GuideWindow();
	//guide->SetDrawOrder(-400000);
}

GameControler::~GameControler()
{

}

void GameControler::Update()
{
	/*if (gameState != GameState::BEFORE) {
		assert(false);
	}*/
	if (changeState) {
		changeState = false;
		
	}
	else {

#ifdef STRING_MODE
		if (nowState != beforeState) {
			beforeState = nowState;
			changeState = true;
		}
#else
		if (gameState != beforeGameState) {
			beforeGameState = gameState;
			changeState = true;
			
		}
#endif

	}
	switch (gameState)
	{
	case GameControler::NONE:
		break;
	case GameControler::BEFORE:
		BeforeUpdate();
		
		break;
	case GameControler::PLAY:
		PlayUpdate();
		
		break;
	case GameControler::BOSS_PLAY_BEFORE:
		PauseUpdate();
		break;
	case GameControler::WIN:
		WinUpdate();
		break;
	case GameControler::LOSE:
		LoseUpdate();
		break;
	case GameControler::SCENE_CHANGE:
		SceneChangeUpdate();
		break;
	default:
		break;
	}

#ifdef STRING_MODE
	if (Debug::GetDebugMode()) {
		ImGui::Begin("gameState");
		if (ImGui::Button("before")) {
			ChangeState("BEFORE");
		}
		if (ImGui::Button("play")) {
			ChangeState("PLAY");
		}
		if (ImGui::Button("win")) {
			ChangeState("WIN");
		}
		if (ImGui::Button("lose")) {
			ChangeState("LOSE");
		}
		ImGui::End();
	}
#else
	if (Debug::GetDebugMode()) {
		ImGui::Begin("gameState");
		if (ImGui::Button("before")) {
			ChangeState(GameState::BEFORE);
		}
		if (ImGui::Button("play")) {
			ChangeState(GameState::PLAY);
		}
		if (ImGui::Button("win")) {
			ChangeState(GameState::WIN);
		}
		if (ImGui::Button("lose")) {
			ChangeState(GameState::LOSE);
		}
		ImGui::End();
	}
#endif // STRING_MODE

	//操作説明ガイドを表示しているときはポーズの機能をシャットダウン
	if (guide != nullptr) {
		if (!guide->GetActive()) {
			if (InputManager::GetInstance()->KeyInputDown("PauseScreen")) {
				if (gameState == PAUSE_SCENE) {
					gameState = keepGameState;
					pause->PauseButtonGameBack();
				}
				else {
					keepGameState = gameState;
					gameState = PAUSE_SCENE;
					pause->PauseStart();
				}
			}
		}
	}

	PauseResult();
	GuideWindowUpdate();
}

void GameControler::Draw()
{
	switch (gameState)
	{
	case GameControler::NONE:
		break;
	case GameControler::BEFORE:
		BeforeDraw();
		break;
	case GameControler::PLAY:
		PlayDraw();
		break;
	case GameControler::PAUSE_SCENE:
		PauseDraw();
		break;
	case GameControler::BOSS_PLAY_BEFORE:
		break;
	case GameControler::WIN:
		WinDraw();
		break;
	case GameControler::LOSE:
		LoseDraw();
		break;
	case GameControler::SCENE_CHANGE:
		SceneChangeDraw();
		break;
	default:
		break;
	}
}

#ifdef STRING_MODE

void GameManager::ChangeState(std::string _name)
{
	for (int i = 0; i < GAME_STATE_MAX; i++) {
		if (stateName[i] == _name) {
			switch (i)
			{
			case 0:
				state = MEB(&GameManager::BeforeUpdate);
				stateDraw = MEBDraw(&GameManager::BeforeDraw);
				break;
			case 1:
				state = MEB(&GameManager::PlayUpdate);
				stateDraw = MEBDraw(&GameManager::PlayDraw);
				break;
			case 2:
				state = MEB(&GameManager::BossPlayBeforeUpdate);
				stateDraw = MEBDraw(&GameManager::BossPlayBeforeDraw);
				break;
			case 3:
				state = MEB(&GameManager::WinUpdate);
				stateDraw = MEBDraw(&GameManager::WinDraw);

				break;
			case 4:
				state = MEB(&GameManager::LoseUpdate);
				stateDraw = MEBDraw(&GameManager::LoseDraw);
				break;
			}

			nowState = stateName[i];
			//changeState = true;
		}
	}
}


int GameManager::GetStateNumber()
{
	int num = 0;
	if (nowState == "BEFORE") {
		num = 0;
	}
	else if (nowState == "PLAY") {
		num = 1;
	}
	else if (nowState == "BOSS_PLAY_BEFORE") {

		num = 2;
	}
	else if (nowState == "WIN") {
		num = 3;
	}
	else {
		num = 4;
	}
	return num;
}


std::string GameManager::GetStateName()
{
	return nowState;
}

#else

void GameControler::ChangeState(GameState _name)
{
	gameState = _name;

	switch (_name)
	{
	case GameState::BEFORE:
		/*state = MEB(&GameControler::BeforeUpdate);
		stateDraw = MEBDraw(&GameControler::BeforeDraw);*/
		break;

	case GameState::PLAY:
		/*state = MEB(&GameControler::PlayUpdate);
		stateDraw = MEBDraw(&GameControler::PlayDraw);*/
		break;

	case GameState::BOSS_PLAY_BEFORE:
		/*state = MEB(&GameControler::BossPlayBeforeUpdate);
		stateDraw = MEBDraw(&GameControler::BossPlayBeforeDraw);*/
		break;

	case GameState::WIN:
		/*state = MEB(&GameControler::WinUpdate);
		stateDraw = MEBDraw(&GameControler::WinDraw);*/
		
		break;

	case GameState::LOSE:
		/*state = MEB(&GameControler::LoseUpdate);
		stateDraw = MEBDraw(&GameControler::LoseDraw);*/
		/*resultUi->GetBaseObject()->SetDraw(true);
		resultUi->ResultStart(false);*/
		break;
	case GameState::SCENE_CHANGE:
		/*state = MEB(&GameControler::SceneChangeUpdate);
		stateDraw = MEBDraw(&GameControler::SceneChangeDraw);*/
		break;
	default:
		break;
	}
}

GameControler::GameState GameControler::GetStateNumber()
{
	return gameState;
}

#endif // STRING_MODE



void GameControler::SetPointer()
{
	camera = FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>();
}

//bool GameControler::GetChangeState()
//{
//	return changeState; 
//}

void GameControler::ResultUiStart(bool _win)
{
	resultUi->GetBaseObject()->SetDraw(true);
	resultUi->ResultStart(_win);
}

void GameControler::BeforeUpdate()
{
	//startCount -= Time::DeltaTime();
	//obj->Component()->GetComponent<MeshRenderer2D>()->SetNum((int)startCount);
	bool isCutScene = (camera->IsCutScene());
	if (isCutScene) {

	}
	else {
		FindGameObject<Wave>()->FirstRespown();
#ifdef STRING_MODE
		ChangeState("PLAY");
#else
		if (gamePlayNum == 1) { //初めてプレイをするときのみ操作説明を表示
			guide->GuideWindowDraw(false);
			keepGameState = PLAY;
			gameState = PAUSE_SCENE;
		}
		else {
			ChangeState(GameState::PLAY);
		}
		
		//ChangeState(GameState::PLAY);
#endif // STRING_MODE


	}
}

void GameControler::BeforeDraw()
{
	//DrawFormatString(1000, 500, 0xffffff, "%d", (int)startCount);

}

void GameControler::PlayUpdate()
{
	//obj->Component()->GetComponent<MeshRenderer2D>()->SetDraw(false);

}

void GameControler::PlayDraw()
{
	//DrawFormatString(1000, 500, 0xffffff, "%d", (int)startCount);

}

void GameControler::PauseUpdate()
{
}

void GameControler::PauseDraw()
{
}

void GameControler::BossPlayBeforeUpdate()
{

}

void GameControler::BossPlayBeforeDraw()
{

}

void GameControler::WinUpdate()
{

	//FindGameObject<FadeTransitor>()->StartTransitor("TITLE", 1.0f);
}

void GameControler::WinDraw()
{
	

}

void GameControler::LoseUpdate()
{

}

void GameControler::LoseDraw()
{
	
	//DrawGraph(750, 100, loseImage, true);

}

void GameControler::SceneChangeUpdate()
{
	FindGameObject<FadeTransitor>()->StartTransitor("TITLE", 1.0f);
	
}

void GameControler::SceneChangeDraw()
{
	
}

void GameControler::PauseResult()
{
	if (pause->IsSelect()) {
		switch (pause->GetResult())
		{
		case PauseScreen::Resume:

			pause->PauseFinish();
			gameState = keepGameState;
			break;

		case PauseScreen::OpenGuide:

			pause->PauseFinish();
			guide->GuideWindowDraw(true);
			break;

		case PauseScreen::ToTitle:

			pause->PauseFinish();
			gameState = SCENE_CHANGE;
			break;
		default:
			my_error_assert("ポーズの状態が入っていません");
			break;
		}

	}
}

void GameControler::GuideWindowUpdate()
{
	bool push = guide->GetPush();
	if (push) {
		bool gameBack = guide->GetGameBack();
		if (gameBack) {
			guide->GuideWindowFinish();
			gameState = keepGameState;
		}
		else {
			guide->GuideWindowFinish();
			pause->PauseStart();
		}
	}
}
