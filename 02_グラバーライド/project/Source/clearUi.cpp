#include "clearUi.h"
#include "../ImGui/imgui.h"
#include "starData.h"
#include "../Library/sceneManager.h"
#include "commonData.h"
#include "function.h"
#include "LoadManager.h"
#include "inputManager.h"
#include "soundManager.h"
#include "function.h"

namespace 
{
	const int INDEX_NUM = 2;
	const float STAR_EXRATE = 0.8f;
	const int RIGHT_STAR_X = 1350.0f;//右に表示される☆のX座標
	const VECTOR2F HEAD_SCORE_UI_POS = { 590.0f,350.0f };
	const float STAR_Y_ADD = 35.0f;//ScoreUIPosからいくつY座標を足すか
	const float SLASH_SCALE = 1.5f;
	const float SLASH_NUMBER_Y_ADD = 3.0f;//DrawNum関数を呼ぶときの数値にこれを足してSlashDraw関数を呼ぶとちょうどよくなる
	const float TIME_COIN_NUM_Y_ADD = 40.0f;//タイムなどの座標描画の時に左のUI描画用の座標にこれを足す
}
ClearUi::ClearUi(SceneBase* _scene) : UiSelectBase(_scene)
{
	nowStage = SceneManager::CommonScene()->FindGameObject<CommonData>()->GetData().stage;
	loadManager = SceneManager::CommonScene()->FindGameObject<LoadManager>();
	input = SceneManager::CommonScene()->FindGameObject<InputManager>();
	sound = SceneManager::CommonScene()->FindGameObject<SoundManager>();
	numberTextImage = loadManager->GetHandle("NUMBER_TEXT");
	goSelectImage = loadManager->GetHandle("RESULT_5");
	score = GetScene()->FindGameObject<StageScore>();

	backPos = V2Get((float)SCREEN_WIDTH_CENTER, (float)SCREEN_HEIGHT_CENTER);
	backSize = V2Get((float)SCREEN_WIDTH_CENTER * 1.7f, 932.0f);
	titlePos = V2Get(630.0f, 160.0f);
	selectDistance = 125.0f;

	for (int i = 0;i < STAR_NUM;i++)
	{
		scoreUiPos[i] = HEAD_SCORE_UI_POS;
		scoreUiPos[i].y += i * selectDistance;

		starDrawPos[i] = scoreUiPos[i].y + STAR_Y_ADD;
		starDrawPos[i].x = RIGHT_STAR_X;
		firstDrawPos[i] = starDrawPos[i];
		moveTime[i] = 0.0f;
		light[i] = false;
		lightSub[i] = false;
	}
	beforeIndex = 0;

	goSelectTextPos = V2Get(SCREEN_WIDTH / 2, 800);
	selectAlpha = 0;

	backColor = RGB_BLACK;
	/*for (int i = 0; i < 3; i++)
	{
		select[i].color = RGB_WHITE;
	}*/

	for (int i = 0; i < STAR_NUM; i++)
	{
		getStar[i] = false;
	}

	titleSize.x = 1.0f;
	titleSize.y = 1.0f;

	starImage = LoadGraph("data\\texture\\star2.png");

	for (int i = 0; i < RESULT_NUM; i++) {
		std::string name = "RESULT_";
		name += std::to_string(i + 1);
		resultImage[i] = loadManager->GetHandle(name);
		
	}
	sub = false;
	sound = SceneManager::CommonScene()->FindGameObject<SoundManager>();

	numImage = loadManager->GetHandle("TIME_NUM");
	productSkip = false;
}

ClearUi::~ClearUi()
{
}

void ClearUi::Update()
{
	if(gameManager->GetGameState()==GameManager::RESULT_DRAW)
	{
		//スターを入手出来るかのフラグを入手(1回だけ呼べば良いので1回だけ呼ぶ処理に後でする)
		for (int i = 0; i < STAR_NUM; i++)
		{
			getStar[i] = score->GetCanStar((StarType)i);
		}
		if (input->GetIsKeyOrButtonPutNow(KEY_INPUT_Z, XINPUT_BUTTON_A)) {
			AllProductSkip();
		}
		StarMove();
	}
}

void ClearUi::Draw()
{
	if (gameManager->GetGameState() == GameManager::RESULT_DRAW)
	{
#ifdef _DEBUG

		ImGui::Begin("CLEAR");
		ImGui::SliderFloat2("backPos", &backPos.x, 0.0f, SCREEN_WIDTH);
		ImGui::SliderFloat2("backSize", &backSize.x, 0.0f, SCREEN_WIDTH);
		ImGui::SliderInt3("backColor", &backColor.r, 0, 255);
		ImGui::SliderFloat2("titlePos", &titlePos.x, 0.0f, 10000.0f);
		ImGui::SliderFloat2("titleSize", &titleSize.x, 0.0f, 10.0f);

		ImGui::SliderFloat2("subDrawFontPos", &subDrawFontPos.x, 0.0f, 10000.0f);

		ImGui::SliderFloat2("timePos", &scoreUiPos[0].x, 0.0f, 10000.0f);

		ImGui::SliderFloat2("coinPos", &scoreUiPos[1].x, 0.0f, 10000.0f);

		ImGui::SliderFloat2("damagePos", &scoreUiPos[2].x, 0.0f, 10000.0f);

		ImGui::SliderFloat("selectPosDistance", &selectDistance, 0.0f, 500.0f);

		ImGui::SliderFloat2("goSelectTextPos", &goSelectTextPos.x, 0.0f, 1920.0f);
		ImGui::SliderFloat2("start", &start.x, 0.0f, 1000.0f);
		ImGui::SliderFloat2("size", &size.x, 0.0f, 1000.0f);
		ImGui::End();
#endif // DEBUG


		//背景の黒■の描画
		VECTOR2F drawPos1 = V2Get(backPos.x - backSize.x / 2, backPos.y - backSize.y / 2);//左上の座標
		VECTOR2F drawPos2 = V2Get(backPos.x + backSize.x / 2, backPos.y + backSize.y / 2);//右下の座標
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawFillBox((int)drawPos1.x, (int)drawPos1.y, (int)drawPos2.x, (int)drawPos2.y, backColor);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	char resultDrawText[STAR_NUM][50] = { '\0' };

	switch (gameManager->GetGameState())
	{
	case GameManager::GOAL_FRONT_ANI:

		break;
	case GameManager::RESULT_DRAW:
		
		UIDraw(V2Get(titlePos.x, titlePos.y), titleSize, 0.0f,resultImage[0]);

		UIDraw(scoreUiPos[CLEAR_STAR], titleSize, 0.0f, resultImage[1]);
		UIDraw(scoreUiPos[TIME_STAR], titleSize, 0.0f, resultImage[2]);
		UIDraw(scoreUiPos[COIN_STAR], titleSize, 0.0f, resultImage[3]);
		
		DrawTimeAsImageMS(V2Get(1035.0f, scoreUiPos[TIME_STAR].y+TIME_COIN_NUM_Y_ADD), score->GetGameTime(), 48, numImage, 1.0f, V2Get(60, 56), V2Get(60, 64), V2Get(145, 16), V2Get(15, 31), V2Get(161, 35), V2Get(18, 20));
		DrawTimeAsImageMS(V2Get(1265.0f, scoreUiPos[TIME_STAR].y + TIME_COIN_NUM_Y_ADD), score->GetTargetTime(), 48, numImage, 1.0f, V2Get(60, 56), V2Get(60, 64), V2Get(145, 16), V2Get(15, 31), V2Get(161, 35), V2Get(18, 20));
		SlashDraw(V2Get(1091.0f, scoreUiPos[TIME_STAR].y + TIME_COIN_NUM_Y_ADD+SLASH_NUMBER_Y_ADD), SLASH_SCALE);
		DrawRotaNumStart(970.0f, scoreUiPos[COIN_STAR].y + TIME_COIN_NUM_Y_ADD, score->GetCoinNum(), 50, numImage, 60, 65, 55, 1.1f, 0.0f);
		DrawRotaNumStart(1100.0f, scoreUiPos[COIN_STAR].y + TIME_COIN_NUM_Y_ADD, score->GetTargetCoin(), 50, numImage, 60, 65, 55, 1.1f, 0.0f);
		SlashDraw(V2Get(1020.0f, scoreUiPos[COIN_STAR].y + TIME_COIN_NUM_Y_ADD+SLASH_NUMBER_Y_ADD), SLASH_SCALE);
		for (int i = 0; i < STAR_NUM; i++)
		{
			//入手した☆の描画
			VECTOR2I starSize;
			GetGraphSize(starImage, &starSize.x, &starSize.y);
			
			int rectX = (starSize.x / 2);
			
			DrawRectRotaGraph(firstDrawPos[i].x, firstDrawPos[i].y , 0, 0, starSize.x / 2, starSize.y, 0.83f, 0.0f, starImage, true, false);//横のやつ
			if (getStar[i]) {
				if (light[i]) {
					SetDrawBlendMode(DX_BLENDMODE_ADD, 30);
					for (int j = 0; j < loopNum[i]; j++) {
						DrawRectRotaGraph(starDrawPos[i].x - 2, starDrawPos[i].y, rectX, 0, starSize.x / 2, starSize.y, starExrate[i] + j * 0.03f, starAngle[i], starImage, true, false);//横のやつ
					}
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
				else {
					DrawRectRotaGraph(starDrawPos[i].x - 2, starDrawPos[i].y, rectX, 0, starSize.x / 2, starSize.y, starExrate[i], starAngle[i], starImage, true, false);//横のやつ
				}
			}
		}
		if (state == FINISH) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,selectAlpha);
			//font->FontDraw(goSelectTextPos, select[0].fontSize, 0.0f, kind, RGB_RED, "A:セレクトシーン");
			DrawRotaGraphF(goSelectTextPos.x, goSelectTextPos.y, 1.0f, 0.0f, goSelectImage, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		break;
	case GameManager::PLAY_AFTER:

		break;
	default:
		break;
	}
}

void ClearUi::UIDraw(VECTOR2F _pos, VECTOR2F _size, float _angle, int _himage)
{
	DrawRotaGraph3F(_pos.x, _pos.y, 0.0f, 0.0f, _size.x, _size.y, _angle, _himage, true, false, false);
}

void ClearUi::SlashDraw(VECTOR2F _pos, float _rate)
{
	DrawRectRotaGraphF(_pos.x, _pos.y, 190, start.y, 42, 54, _rate, 0.0f, numImage, true, false, false);
}

void ClearUi::StarMove()
{
	if (allLight) {
		for (int i = 0; i < STAR_NUM; i++) {
			if (lightSub[i]) {
				loopNum[i] -= speed;
				if (loopNum[i] <= 0) {
					loopNum[i] = 0;
					lightSub[i] = false;
					light[i] = false;
					allLight = false;
				}
			}
			else {
				loopNum[i] += speed;
				if (loopNum[i] >= 20) {
					loopNum[i] = 20;
					lightSub[i] = true;
					sound->PlaySe("星が光る");
				}
			}
		}
	}

	else if (light[beforeIndex]) {
		if (lightSub[beforeIndex]) {
			loopNum[beforeIndex] -= speed;
			if (loopNum[beforeIndex] <= 0) {
				loopNum[beforeIndex] = 0;
				lightSub[beforeIndex] = false;
				light[beforeIndex] = false;
			}
		}
		else {
			loopNum[beforeIndex] += speed;
			if (loopNum[beforeIndex] >= 30) {
				loopNum[beforeIndex] = 30;
				lightSub[beforeIndex] = true;
				sound->PlaySe("星が光る");
			}
		}
	}

	switch (state)
	{
	case ClearUi::RATE:
		if (starAlpha[starIndex] < 255) {
			starAlpha[starIndex] += 6;
			float rate = starAlpha[starIndex] / 255.0f;
			starAngle[starIndex] = EaseOut(0.0f, DegToRad * 360.0f * 5.0f, rate);
			starExrate[starIndex] = EaseOut(0.0f, STAR_EXRATE, rate);
		}
		else {
			starAlpha[starIndex] = 255;
			starAngle[starIndex] = 0.0f;
			starAngle[starIndex] = EaseOut(0.0f, DegToRad * 360.0f * 5.0f, 1.0f);
			starExrate[starIndex] = EaseOut(0.0f, STAR_EXRATE, 1.0f);
			if (starIndex < STAR_NUM - 1) {
				speed = 2;
				light[starIndex] = true;
				if (getStar[starIndex]) {
					beforeIndex = starIndex;
				}
				starIndex++;
				if (!getStar[starIndex]) {
					starAlpha[starIndex] = 255;
				}
			}
			else {
				speed = 2;
				if (getStar[starIndex]) {
					beforeIndex = starIndex;
				}
				light[starIndex] = true;
				state = FINISH;
				starIndex = 0;
			}
		}
		break;
	case ClearUi::MOVE:
		/*元々あった☆を下に移動する処理の名残
		if (moveTime[starIndex] <= 30.0f) {
			moveTime[starIndex]++;
			float rate = moveTime[starIndex] / 30.0f;
			starDrawPos[starIndex] = EaseInOut(firstDrawPos[starIndex], downStarPos[starIndex], rate);
		}
		else {
			if (starIndex < STAR_NUM - 1) {
				light[starIndex] = true;
				speed = 2;
				if (getStar[starIndex]) {
					beforeIndex = starIndex;
				}
				starIndex++;

				if (!getStar[starIndex]) {
					moveTime[starIndex] = 30;
				}
			}
			else {
				light[starIndex] = true;
				speed = 2;
				if (getStar[starIndex]) {
					beforeIndex = starIndex;

				}
				state = FINISH;
				starIndex = 0;
			}
		}*/
		break;
	case ClearUi::FINISH:
		if (counter <= 60) {
			counter++;
			if (counter == 60) {
				if (!productSkip) {
					allLight = true;
					for (int i = 0; i < STAR_NUM; i++) {
						light[i] = true;
					}
				}
				gameManager->GameFinish();
			}
		}
		else {
			if (sub) {
				selectAlpha -= 10.0f;
				if (selectAlpha <= 50.0f) {
					selectAlpha = 50.0f;
					sub = false;
				}
			}
			else {
				selectAlpha += 10.0f;
				if (selectAlpha >= 255.0f) {
					selectAlpha = 255.0f;
					sub = true;
				}
			}
		}

		break;
	}
}

void ClearUi::AllProductSkip()
{
	if (!productSkip) {
		state = FINISH;
		//selectAlpha = 255;
		//gameManager->GameFinish();
		for (int i = 0; i < STAR_NUM; i++) {
			//starDrawPos[i] = downStarPos[i];
			starAngle[i] = 360.0f * 5.0f * DegToRad;
			starExrate[i] = STAR_EXRATE;
			light[i] = true;
			lightSub[i] = false;
			loopNum[i] = 0;
		}
		speed = 2;
		allLight = true;
		productSkip = true;
		counter = 60;
		gameManager->GameFinish();
	}
}
