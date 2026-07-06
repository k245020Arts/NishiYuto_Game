#include "TitleControl.h"
#include "TitlePlayer.h"

#include "../../Source/Component/Object/Object3D.h"
#include "../../Source/Common/Transitor/FadeTransitor.h"
#include "../../Source/Common/Transitor/PushTransitor.h"
#include "../../Source/Common/Transitor/WipeTransitor.h"
#include "../../Source/Common/Transitor/StripTransitor.h"
#include "../../Source/Common/Transitor/TitleTransitor.h"
#include "../../Source/Common/Transitor/IrisTransitor.h"
#include "../../Source/Common/Debug/Debug.h"
#include "../../Source/Common/InputManager/inputManager.h"
#include "../../Source/Common/ResourceLoader.h"
#include "../../Source/Common/Sound/SoundManager.h"
#include "../../Source/Common/Easing.h"
#include "../State/StateManager.h"
#include "../Screen.h"
#include "../Stage/StageSelectData.h"
#include "../Component/Color/Color.h"
#include "../Common/ID/FontID.h"
#include "../Component/UI/TextRenderer.h"

namespace {
	const float ALPHA_TIME = 1.0f;
	
}

TitleControl::TitleControl()
{

	hImage = ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Title", ID::TITLE_BACK);
	titleImage = ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "TitleImage", ID::TITLE);
	keyImage = ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "TitlePush", ID::PUSH_BUTTON);
	pushKeyImage = ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "TitlePushKey", ID::PUSH_KEY);
	//font = LoadFontDataToHandle("data/font/MPlus2C.dft", 4);
	font = ResourceLoad::LoadFont("MPlus2C", ".dft", Font_ID::TITLE_USE_FONT,4);
	SoundManager::GetInstance()->AllDeleteSound();
	SoundManager::GetInstance()->TitleSceneLoad();
	SoundManager::GetInstance()->PlayBGM(Sound_ID::TITLE_BGM, true, true);
	firstCounter = 1.0f;
	pushCounter = 0.0f;
	exrate = 0.0f;
	moveButton = 0.0f;
	moveButton = 1.0f;
	progress = 0.0f;
	controler = true;
	
	Object3D* obj = FindGameObjectWithTag<Object3D>("PLAYER");
	player = obj->Component()->GetComponent<TitlePlayer>();

	SetDrawOrder(-100);
	stageID = StageSelectData::GetInstance()->GetNowStageData().id;
	selectCounter = 0.0f;
	titleState = TITLE;
	alpha = 255;

	const int STAGE_POS = 300;
	Object2D* stageTextObj = new Object2D();
	stageTextObj->Init(VECTOR2F(Screen::WIDTH / 2 + STAGE_POS, 850), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "stageText");
	stageText = stageTextObj->Component()->AddComponent<TextRenderer>();
	
	stageText->TextSetting("", "MPlus2C", ".dft",WHITE,4,Font_ID::TITLE_USE_FONT);

	Object2D* commonTextObj = new Object2D();
	commonTextObj->Init(VECTOR2F(Screen::WIDTH / 2 + STAGE_POS - 100, 650), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "Text");
	text = commonTextObj->Component()->AddComponent<TextRenderer>();
	
	text->TextSetting("Aボタンを押してスタート!!", "MPlus2C", ".dft", WHITE, 4, Font_ID::TITLE_USE_FONT);

	std::vector<StageData> stages = StageSelectData::GetInstance()->GetAllStageData();

	for (const auto& stage : stages) {
		stageHandle.emplace_back(LoadGraph((ResourceLoad::IMAGE_PATH + stage.stageImageHandleName + ".png").c_str()));
	}
}

TitleControl::~TitleControl()
{
	Debug::ClearLogger();
	//DeleteFontToHandle(font);

	for (int& handle : stageHandle) {
		if (handle >= 0) {
			DeleteGraph(handle);
		}
	}
	stageHandle.clear();
}

void TitleControl::Update()
{
	//プレイヤーの回避アニメーションが終わったら、シーンを遷移
	if (progress >= 0.99f)
	{
		Time::ChangeDeltaRate(1);
		FindGameObject<FadeTransitor>()->StartTransitor("PLAY", 1.0f);
	}

	switch (titleState)
	{
	case TITLE:
		TitleUpdate();
		break;
	case STAGE_SELECT:
		StageUpdate();
		break;
	default:
		break;
	}

	if (alphaTime > 0.0f) {
		alphaTime -= Time::DeltaTimeRate();
		if (alphaTime <= 0.0f) {
			alphaTime = 0.0f;
		}
		float rate = alphaTime / ALPHA_TIME;
		switch (titleState)
		{
		case TITLE:
			alpha = Easing::Lerp(OPAQUE_COLOR, TRANSPARENT_COLOR, rate);
			break;
		case STAGE_SELECT:
			alpha = Easing::Lerp(TRANSPARENT_COLOR, OPAQUE_COLOR, rate);
			break;
		default:
			break;
		}
	}
	KeyControlerChangeText();
}

float TitleControl::GetNowProgress()
{
	return progress;
}

void TitleControl::SetNowProgress(float nowProgress)
{
	progress = nowProgress;
}

void TitleControl::StageSelect()
{
	selectMoveCounter += Time::DeltaTimeRate();
	if (selectCounter > 0.0f) {
		return;
	}
	const float SELECT_COUNTER = 0.3f;
	bool rightStick = InputManager::GetInstance()->GetControllerInput()->GetStickInput().leftStick.x >= 0.5f;
	bool rightButton = InputManager::GetInstance()->KeyInputDown("StageSelectRight");
	if (rightStick || rightButton) {
		stageID++;
		int stageMax = StageSelectData::GetInstance()->GetStageMax() - 1;
		if (stageID >= stageMax) {
			stageID = stageMax;
		}
		StageSelectData::GetInstance()->SetStageID(stageID);
		//スティックで入力しているときには待機時間を付ける
		if (rightStick) {
			selectCounter = SELECT_COUNTER;
		}
		else {
			selectCounter = 0.0f;
		}
		
		selectMoveCounter = 0.0f;
		SoundManager::GetInstance()->PlaySe(Sound_ID::PUSH);
	}

	bool leftStick = InputManager::GetInstance()->GetControllerInput()->GetStickInput().leftStick.x <= -0.5f;
	bool leftButton = InputManager::GetInstance()->KeyInputDown("StageSelectLeft");
	if (leftStick || leftButton) {
		stageID--;
		if (stageID <= 0) {
			stageID = 0;
		}
		StageSelectData::GetInstance()->SetStageID(stageID);
		//スティックで入力しているときには待機時間を付ける
		if (leftStick) {
			selectCounter = SELECT_COUNTER;
		}
		else {
			selectCounter = 0.0f;
		}
		selectMoveCounter = 0.0f;
		SoundManager::GetInstance()->PlaySe(Sound_ID::PUSH);
	}
}

void TitleControl::KeyControlerChangeText()
{
#ifdef KEY_GUIDE
	//コントローラーが接続されている時のUIの変更
	if (InputManager::GetInstance()->GetControllerInput()->GetIsPadInput()) {
		text->SetText("Aボタンを押してスタート!!");
		controler = true;
	}
	else {
		text->SetText("  Pキーを押してスタート!!");
		controler = false;
	}
#endif
}

#define BLACK_TEXTURE SetDrawBright(0, 0, 0)
#define DEFAULT_TEXTURE SetDrawBright(255, 255, 255)

void TitleControl::Draw()
{
	/*DrawGraph(0, 0, hImage, true);*/
	DrawGraph(750, 100, titleImage, true);
	if (progress > 0) { //画面遷移が始まったら文字の描画を止める
		stageText->SetAlpha(0.0f);
		text->SetAlpha(0.0f);
		return;
	}
		

	StageData stageData = StageSelectData::GetInstance()->GetNowStageData();
	int move = Easing::SinCube(0, 10, selectMoveCounter);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	int titlePushImage = 1;
	//コントロールに接続してるか否かで変更
	if (controler) {
		titlePushImage = keyImage;
	}
	else {
		titlePushImage = pushKeyImage;
	}
	DrawRotaGraph(Screen::WIDTH / 2, 850, (double)exrate * 2, 0.0, titlePushImage, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);

	stageText->GetBaseObject()->GetTransform()->position.y = 850.0f + move;
	stageText->SetText(stageData.name.c_str());
	stageText->SetAlpha(255.0f- alpha);

	text->GetBaseObject()->GetTransform()->position.y = 650.0f + move;
	text->SetAlpha(255.0f - alpha);
	
	int width = stageText->GetTextWidth();

	int stageMax = StageSelectData::GetInstance()->GetStageMax() - 1;
	if (stageID == stageMax) {
		BLACK_TEXTURE;
	}

	const int STAGE_POS = 300;
	
	DrawExtendFormatStringToHandle(Screen::WIDTH / 2 + width + STAGE_POS + 50, 850 + move, 1.0, 1.0, 0xffffff, font,"→");
	DEFAULT_TEXTURE;

	if (stageID == 0) {
		BLACK_TEXTURE;
	}
	DrawExtendFormatStringToHandle(Screen::WIDTH / 2 + STAGE_POS - 100, 850 + move, 1.0, 1.0, 0xffffff, font,"←");
	DEFAULT_TEXTURE;

	DrawRotaGraph(1480, 400 + move, 1.0f, 0.0f, stageHandle[stageID], true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleControl::TitleUpdate()
{
	if (InputManager::GetInstance()->KeyInputDown("SceneChange") && progress == 0) // 押したら
	{
		SoundManager::GetInstance()->PlaySe(Sound_ID::PUSH);
		titleState = STAGE_SELECT;
		alphaTime = ALPHA_TIME;
	}
	if (firstCounter > 0.0f)
	{
		firstCounter -= Time::DeltaTimeRate();
		if (firstCounter <= 0.0f)
		{
			firstCounter = 0.0f;
		}
		float rate = firstCounter / 1.0f;
		exrate = Easing::EaseIn(0.0f, 0.8f, 0.8f - rate);
	}
	else
	{
		if (pushCounter > 0.0f) // 押したあと
		{
			float rate = pushCounter / 0.5f;
			exrate = Easing::Sin90Cube(0.5f, 0.8f, 0.8f - rate);
		}
		else
		{
			float rate = 0.0f;
			if (moveButton > 0.0f)
			{
				moveButton -= Time::DeltaTimeRate();
				rate = moveButton / 1.0f;
				if (moveButton <= 0.0f)
					moveButton = -1.0f;

			}
			else if (moveButton < 0.0f)
			{
				moveButton += Time::DeltaTimeRate();
				rate = moveButton / -1.0f;
				rate = 1 - rate;
				if (moveButton >= 0.0f)
					moveButton = 1.0f;
			}
			exrate = Easing::EaseInOut(0.4f, 0.5f, rate);
		}
	}
}

void TitleControl::StageUpdate()
{
	if (InputManager::GetInstance()->KeyInputDown("SceneChange") && progress == 0) // 押したら
	{
		SoundManager::GetInstance()->AllDeleteSound();
		SoundManager::GetInstance()->TitleSceneLoad();

		SoundManager::GetInstance()->PlaySe(Sound_ID::PUSH);
		SoundManager::GetInstance()->PlaySe(Sound_ID::JUST_AVOID_SOUND);
		//sound->BaseVolumeChange(Sound_ID::JUST_AVOID_SUCCESS);
		SoundManager::GetInstance()->PlaySe(Sound_ID::JUST_AVOID_SUCCESS);
		SoundManager::GetInstance()->PlaySe(Sound_ID::V_P_JUST_AVOID);

		player->playerCom.stateManager->ChangeState(StateID::PLAYER_AVOID_S);
		StageSelectData::GetInstance()->SetStageID(stageID);

		stageText->SetAlpha(0.0f);
		text->SetAlpha(0.0f);
	}

	StageSelect();

	if (selectCounter >= 0.0f) {
		selectCounter -= Time::DeltaTimeRate();
	}
}

