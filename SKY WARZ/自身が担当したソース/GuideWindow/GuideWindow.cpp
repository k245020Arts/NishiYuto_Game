#include "GuideWindow.h"
#include "../Component/UI/UIManager/UIManager.h"
#include "../Common/InputManager/InputManager.h"
#include "../Common/InputManager/PadInput.h"
#include "../Common/ResourceLoader.h"
#include "../Component/UI/TextRenderer.h"
#include "../Component/ComponentManager.h"
#include "../Screen.h"
#include "../Common/Sound/SoundManager.h"

namespace {
	const int LABEL_POS_Y = 950;
	const int GAMEBACK_LABEL_POS_X = 600;
	const int PAUSEBACK_LABEL_POS_X = 1050;
}

GuideWindow::GuideWindow()
{
	aButtonImage = ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "AButtonImage",ID::A_BUTTONIMAGE);
	bButtonImage = ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "BButtonImage", ID::B_BUTTONIMAGE);
	guideWindowControlerHandle = ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "GameGuide", ID::GAME_GUIDE_CONTROLER);
	guideWindowkeyHandle = ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "GameGuideKeyboard", ID::GAME_GUIDE_KEY);

	//---------------------------------------ガイドで使うテキストの初期化---------------------------------------

	Object2D* gameBackObject = new Object2D();
	gameBackObject->Init(VECTOR2F((float)GAMEBACK_LABEL_POS_X, (float)LABEL_POS_Y), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "GameBack");
	gameBackText = gameBackObject->Component()->AddComponent<TextRenderer>();
	gameBackText->TextSetting("ゲームに戻る", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);
	gameBackObject->SetDrawOrder(-350000);

	Object2D* pauseBackObject = new Object2D();
	pauseBackObject->Init(VECTOR2F((float)PAUSEBACK_LABEL_POS_X, (float)LABEL_POS_Y), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "PauseBack");
	pauseBackText = pauseBackObject->Component()->AddComponent<TextRenderer>();
	pauseBackText->TextSetting("ポーズ画面に戻る", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);
	pauseBackObject->SetDrawOrder(-350000);

	pauseBackText->SetAlpha(TRANSPARENT_COLOR_F);
	gameBackText->SetAlpha(TRANSPARENT_COLOR_F);

	//----------------------------------------------------------------------------------------------------------

	SetDrawOrder(-300000);
	menu = false;

	gameBack = false;
	active = false;
	buttonPut = false;;

#ifdef KEY_GUIDE
	if (InputManager::GetInstance()->GetControllerInput()->GetIsPadInput()) {
		controler = true;
	}
	else {
		controler = false;
	}
#else
	controler = true;
#endif
}

GuideWindow::~GuideWindow()
{
}

void GuideWindow::Update()
{
	if (!active)
		return; //メニューが開かれていないときはスルー

	if (InputManager::GetInstance()->KeyInputDown("GameBack")) {
		buttonPut = true;
		gameBack = true;
	}
	//ポーズ画面から開いたときにはポーズ画面の遷移も入れる
	if (menu) {
		if (InputManager::GetInstance()->KeyInputDown("PauseBack")) {
			buttonPut = true;
			gameBack = false;
		}
	}

	//コントローラーが接続されている時のUIの変更
#ifdef KEY_GUIDE
	if (InputManager::GetInstance()->GetControllerInput()->GetIsPadInput()) {
		controler = true;
		gameBackText->SetText("ゲームに戻る");
		gameBackText->SetPos(VECTOR3((float)GAMEBACK_LABEL_POS_X, (float)LABEL_POS_Y, 0.0f));
		pauseBackText->SetText("ポーズ画面に戻る");
	}
	else {
		controler = false;
		gameBackText->SetText("スペースキー : ゲームに戻る");
		gameBackText->SetPos(VECTOR3((float)GAMEBACK_LABEL_POS_X - 400.0f, (float)LABEL_POS_Y, 0.0f));
		pauseBackText->SetText("F3キー : ポーズ画面に戻る");
	}
#endif
}

void GuideWindow::Draw()
{
	if (!active)
		return; //メニューが開かれていないときはスルー

	const int ALPHA = 180;

	//背景の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);
	DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	if (controler) {
		//操作説明書の描画
		const int LABEL_SUB_POS_X = 40;
		const int LABEL_SUB_POS_Y = 20;
		DrawRotaGraph(GAMEBACK_LABEL_POS_X - LABEL_SUB_POS_X, LABEL_POS_Y + LABEL_SUB_POS_Y, 0.2f, 0.0f, aButtonImage, true);
		if (menu) {
			DrawRotaGraph(PAUSEBACK_LABEL_POS_X - LABEL_SUB_POS_X, LABEL_POS_Y + LABEL_SUB_POS_Y, 0.2f, 0.0f, bButtonImage, true);
		}
		DrawRotaGraph(Screen::WIDTH / 2, Screen::HEIGHT / 2, 1.0f, 0.0f, guideWindowControlerHandle, true);
	}
	else {
		DrawRotaGraph(Screen::WIDTH / 2, Screen::HEIGHT / 2, 1.0f, 0.0f, guideWindowkeyHandle, true);
	}
	
	
	
}

void GuideWindow::GuideWindowDraw(bool _menu)
{
	if (active)
		return; //メニューが開かれていたらスルー
	active = true;
	menu = _menu;
	if (menu) {
		pauseBackText->SetAlpha((float)OPAQUE_COLOR);
	}
	gameBackText->SetAlpha((float)OPAQUE_COLOR);
}

void GuideWindow::GuideWindowFinish()
{
	active = false;
	buttonPut = false;
	pauseBackText->SetAlpha(TRANSPARENT_COLOR_F);
	gameBackText->SetAlpha(TRANSPARENT_COLOR_F);
}
