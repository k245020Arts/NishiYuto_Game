#include "PauseScreen.h"
#include "../Component/UI/UIManager/UIManager.h"
#include "../Common/InputManager/InputManager.h"
#include "../Common/InputManager/PadInput.h"
#include "../Common/ResourceLoader.h"
#include "../Component/UI/TextRenderer.h"
#include "../Component/ComponentManager.h"
#include "../Screen.h"
#include "../Common/Sound/SoundManager.h"

PauseScreen::PauseScreen()
{
	pause = false;
	screen = -1;
	timeRate = 0.0f;
	UiManager = FindGameObject<UIManager>();
	animationTime = 0.0f;

	pauseItem = Back;
	SetDrawOrder(-300000);
	ResourceLoad::LoadFont("MonopinJRegular", ".dft", Font_ID::PAUSE_FONT, 4);

	const int EDGE_NUM_X = 300;
	const int EDGE_NUM_Y = 50;

	

	Object2D* pauseObj = new Object2D();
	pauseObj->Init(VECTOR2F(440.0f + EDGE_NUM_X,150.0f + EDGE_NUM_Y),VECTOR2F(0.0f,0.0f),VECTOR2F(2.0f,2.0f),"PauseText");
	pauseText = pauseObj->Component()->AddComponent<TextRenderer>();
	pauseText->TextSetting("ポーズ画面", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);
	pauseObj->SetDrawOrder(-350000);

	Object2D* back = new Object2D();
	back->Init(VECTOR2F(525.0f + EDGE_NUM_X, 350.0f + EDGE_NUM_Y), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "BackText");
	backText = back->Component()->AddComponent<TextRenderer>();
	backText->TextSetting("ゲームに戻る", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);
	back->SetDrawOrder(-350000);

	Object2D* guide = new Object2D();
	guide->Init(VECTOR2F(505.0f + EDGE_NUM_X, 500.0f + EDGE_NUM_Y), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "guideText");
	guideText = guide->Component()->AddComponent<TextRenderer>();
	guideText->TextSetting("操作説明", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);
	guide->SetDrawOrder(-350000);


	Object2D* title = new Object2D();
	title->Init(VECTOR2F(505.0f + EDGE_NUM_X, 650.0f + EDGE_NUM_Y), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "TitleText");
	titleText = title->Component()->AddComponent<TextRenderer>();
	titleText->TextSetting("タイトルに戻る", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);
	title->SetDrawOrder(-350000);

	
	Object2D* select = new Object2D();
	select->Init(VECTOR2F(355.0f + EDGE_NUM_X, 800.0f + EDGE_NUM_Y), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.5f, 1.5f), "Select");
	selectText = select->Component()->AddComponent<TextRenderer>();
	selectText->TextSetting("Aボタンを押して選択", "MonopinJRegular", ".dft", WHITE, 4, Font_ID::PAUSE_FONT);
	select->SetDrawOrder(-350000);

	Object2D*  cursorObj = new Object2D();
	cursorObj->Init(VECTOR2F(780.0f, 400.0f),VECTOR2F(0, 0),VECTOR2F(1, 1),"Cursor");
	cursorText = cursorObj->Component()->AddComponent<TextRenderer>();
	cursorText->TextSetting("→","MonopinJRegular",".dft",WHITE,4, Font_ID::PAUSE_FONT);
	cursorObj->SetDrawOrder(-350000);
	
	pauseText->SetAlpha(TRANSPARENT_COLOR_F);
	selectText->SetAlpha(TRANSPARENT_COLOR_F);
	cursorText->SetAlpha(TRANSPARENT_COLOR_F);

	selectDelayTime = 0.0f;

	delayCountStart = false;
	baseSinRate = 0.0f;

	menuTexts.push_back(backText);
	menuTexts.push_back(guideText);
	menuTexts.push_back(titleText);

	for (auto* menu : menuTexts) {
		menu->SetAlpha(TRANSPARENT_COLOR_F);
	}

	
	//カーソルの位置の設定
	cursorPos.push_back(VECTOR3(780, 400, 0));
	cursorPos.push_back(VECTOR3(755, 550, 0));
	cursorPos.push_back(VECTOR3(735, 700, 0));
}

PauseScreen::~PauseScreen()
{
	if (pauseText != nullptr) {
		pauseText = nullptr;
	}
	if (backText != nullptr) {
		backText = nullptr;
	}
	if (titleText != nullptr) {
		titleText = nullptr;
	}
	if (selectText != nullptr) {
		selectText = nullptr;
	}
	if (screen >= 0) {
		DeleteGraph(screen);
	}
}

void PauseScreen::Update()
{
	if (!pause) { //ポーズ中じゃないならリターン
		return;
	}
	if (inputDelayTime >= 0.0f) {
		inputDelayTime -= Time::DeltaTimeRate();
	}
	else {
		static constexpr float INPUT_DELAY_TIME = 0.2f; //連続で入力情報が入らない時間

		bool stickUp = InputManager::GetInstance()->GetControllerInput()->GetStickInput().leftStick.y >= 0.5f;
		bool buttonUp = InputManager::GetInstance()->KeyInputDown("PauseUp");
		if (stickUp || buttonUp)
		{
			SoundManager::GetInstance()->PlaySe(Sound_ID::PUSH);
			//スティックで入力しているときには待機時間を付ける
			if (stickUp) {
				inputDelayTime = INPUT_DELAY_TIME;
			}
			else {
				inputDelayTime = 0.0f;
			}
			
			currentIndex--;

			if (currentIndex < 0)
			{
				currentIndex = 0;
			}
			pauseItem = (PauseMenuItem)currentIndex;
		}
		bool stickDown = InputManager::GetInstance()->GetControllerInput()->GetStickInput().leftStick.y <= -0.5f;
		bool buttonDown = InputManager::GetInstance()->KeyInputDown("PauseDown");
		if (stickDown || buttonDown)
		{
			currentIndex++;
			SoundManager::GetInstance()->PlaySe(Sound_ID::PUSH);
			//スティックで入力しているときには待機時間を付ける
			if (stickDown) {
				inputDelayTime = INPUT_DELAY_TIME;
			}
			else {
				inputDelayTime = 0.0f;
			}
			if (currentIndex >= PauseMenuItem::Max)
			{
				currentIndex = PauseMenuItem::Max - 1;
			}
			pauseItem = (PauseMenuItem)currentIndex;
			
		}
	}

	if (InputManager::GetInstance()->KeyInputDown("PauseSelect"))
	{
		switch (pauseItem)
		{
		case Back:
			result = PauseResult::Resume;
			selectMenu = true;
			break;
		case Guide:
			result = PauseResult::OpenGuide;
			selectMenu = true;
			break;
		case Title:
			delayCountStart = true;
			break;
		}
	}

	const float OFFSET_PLUS_RATE = 10.0f;

	const float ANIM_SPEED = 0.1f;

	if (delayCountStart) {
		animationTime += ANIM_SPEED;
		baseSinRate = sinf(animationTime);
	}
	else {
		animationTime += ANIM_SPEED;
		baseSinRate = sinf(animationTime);
	}
	

	float cursorOffset = baseSinRate * OFFSET_PLUS_RATE;

	VECTOR3 pos = cursorPos[currentIndex];
	cursorText->SetPos(VECTOR3(pos.x + cursorOffset,pos.y,pos.z));

	if (delayCountStart) {
		selectDelayTime -= Time::DeltaTimeRate();
		if (selectDelayTime <= 0.0f) {
			selectMenu = true;
			result = PauseResult::ToTitle;
		}
		float alpha = 180.0f + sinf(animationTime * 5.0f) * 75.0f;
		selectText->SetAlpha(alpha);
	}
	//コントローラーが接続されている時のUIの変更
#ifdef KEY_GUIDE
	if (InputManager::GetInstance()->GetControllerInput()->GetIsPadInput()) {
		selectText->SetText("Aボタンを押して選択");
		selectText->SetPos(VECTOR3(655.0f, 850.0f, 0.0f));
	}
	else {
		selectText->SetText("スペースキーを押して選択");
		selectText->SetPos(VECTOR3(555.0f, 850.0f, 0.0f));
	}
#endif
}

void PauseScreen::Draw()
{
	if (!pause) { //ポーズ中じゃないならリターン
		return;
	}

	//DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
	SetDrawScreen(screen);
	DrawGraph(0,0,screen,FALSE);
	SetDrawScreen(DX_SCREEN_BACK);

	const int EDGE_NUM_X = 0;
	const int EDGE_NUM_Y = 0;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(EDGE_NUM_X, EDGE_NUM_Y, Screen::WIDTH, Screen::HEIGHT, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	/*DrawString(550, 250, "PAUSE", GetColor(255, 255, 255));*/

	UpdateButtonState();

	/*DrawString(560, 350, "Continue", resumeColor);
	DrawString(560, 420, "Title", titleColor);*/
}

void PauseScreen::PauseStart()
{
	pause = true;
	screen = MakeScreen(Screen::WIDTH, Screen::HEIGHT, TRUE);
	delayCountStart = false;

	SetDrawScreen(screen);
	GraphFilter(screen, DX_GRAPH_FILTER_GAUSS, 1024);
	DrawExtendGraph(0,0,Screen::WIDTH,Screen::HEIGHT,screen,FALSE);

	SetDrawScreen(DX_SCREEN_BACK);
	const float DELAY_TIME = 1.0f;
	selectDelayTime = DELAY_TIME;
	/*timeRate = Time::GetTimeRate();
	Time::ChangeDeltaRate(0.0f);*/
	//UiManager->SetUIDraw(false);
	selectMenu = false; 

	//すべてのポーズ画面で使うテキストを表示する
	pauseText->SetAlpha((float)OPAQUE_COLOR);
	for (auto* menu : menuTexts) {
		menu->SetAlpha((float)OPAQUE_COLOR);
	}
	selectText->SetAlpha((float)OPAQUE_COLOR);
	cursorText->SetAlpha((float)OPAQUE_COLOR);

	//---------------------------------------------------------------------

	SoundManager::GetInstance()->ChangeVolumeSound(Sound_ID::PLAY_BGM,150);

	result = None;
}

bool PauseScreen::PauseFinish()
{
	pause = false;
	delayCountStart = false;
	if (screen >= 0) {
		DeleteGraph(screen);
		screen = -1;
	}
	/*Time::ChangeDeltaRate(timeRate);*/
	//UiManager->SetUIDraw(true);

	//すべてのポーズ画面で使うテキストの表示を止める

	pauseText->SetAlpha(TRANSPARENT_COLOR_F);
	for (auto* menu : menuTexts) {
		menu->SetAlpha(TRANSPARENT_COLOR_F);
	}
	selectText->SetAlpha(TRANSPARENT_COLOR_F);
	cursorText->SetAlpha(TRANSPARENT_COLOR_F);

	selectMenu = false;
	selectDelayTime = 0.0f;

	//---------------------------------------------------------------------

	SoundManager::GetInstance()->ChangeVolumeSound(Sound_ID::PLAY_BGM, 250);

	if (pauseItem == Back) {
		return true;
	}
	else {
		return false;
	}

	
}

void PauseScreen::PauseButtonGameBack()
{
	result = PauseResult::Resume;
	selectMenu = true;
}

void PauseScreen::UpdateButtonState()
{
	for (int i = 0; i < menuTexts.size(); i++)
	{
		//選択状態ならUIを動かす
		if (i == currentIndex)
		{
			float scale = 1.35f + baseSinRate * 0.08f;

			menuTexts[i]->SetColor(ORANGE);
			menuTexts[i]->SetScale(VOne * scale);
		}
		else
		{
			menuTexts[i]->SetColor(WHITE);
			menuTexts[i]->SetScale(VOne);
		}
	}
}