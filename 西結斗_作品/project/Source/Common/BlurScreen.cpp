#include "BlurScreen.h"
#include "../../Source/Screen.h"
#include "ResourceLoader.h"

#define NEW_VERSION  (0);
#define OLD_VERSION  (1);

#define BLUR_VERSION  0;

//const int BlurScreen::SCREEN_NUM = 2;

BlurScreen::BlurScreen()
{
	for (int i = 0; i < SCREEN_NUM; i++) {
		blurScreen[i] = MakeScreen(Screen::WIDTH, Screen::HEIGHT, true);
	}

	Reset();

	defalutAlpha	= 150.0f;
	alpha			= defalutAlpha;
	smallScreen		= -1;
	vignetteGraph	= -1;

	
	vignetteGraph	= ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "visionEffect", ID::SCREEN_BLUR_IMAGE);
	//縮小してガウスフィルタを掛けている。(描画負荷を抑えつつ、ぼかす)
	GraphFilter(vignetteGraph, DX_GRAPH_FILTER_DOWN_SCALE, 4);
	GraphFilter(vignetteGraph, DX_GRAPH_FILTER_GAUSS, 256, 128);
	
}

BlurScreen::~BlurScreen()
{
	ScreenFinish();

	if (vignetteGraph != -1) {
		//DeleteGraph(vignetteGraph);
		vignetteGraph = -1;
	}
}

void BlurScreen::Update()
{
	if (!use) {
		return; //スクリーンモードでなければスルー
	}

	CurrentScreenSet();

	time -= Time::DeltaTimeRate();
	if (time <= 0.0f && !fadeOut) {
		FeedOut(); //フェードアウト始まり
	}

	//フェードアウトの処理
	if (fadeOut) {

		fadeTime -= Time::DeltaTimeRate();

		if (fadeTime >= 0.0f) {
			alpha = (fadeTime / defalutFadeTime) * defalutAlpha;
		}
		else {
			Reset();
			ScreenFinish();
		}
	}

}

void BlurScreen::Draw()
{
#if 0
	if (!use) {
		return;
	}

	int drawMode = GetDrawMode(); //今の描画モードを保存

	SetUseSetDrawScreenSettingReset(FALSE);

	SetDrawMode(DX_DRAWMODE_BILINEAR);

	int blendMode, param; //今の描画のブレンドのモードとパラメーターを保存
	GetDrawBlendMode(&blendMode, &param);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));

	screenDraw += Time::DeltaTimeRate();


	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawBlendMode(blendMode, param);

	SetDrawMode(drawMode);

	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0, 0, blurScreen[currentScreen], false);

	ScreenFlip();

	currentScreen = 1 - currentScreen;
#else
	if (!use) {
		return;
	}

	SetDrawScreen(smallScreen);
	DrawExtendGraph(0, 0, smallW, smallH, blurScreen[currentScreen], true);

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha);

	DrawExtendGraph(0, 0, Screen::WIDTH, Screen::HEIGHT, smallScreen, true);

	SetDrawBright(80, 200, 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(alpha * 1.2f));
	if (vignetteGraph != -1) {
		//画像が大きいので少し端っこからおいている。
		const VECTOR2I imagePos = VECTOR2I(-2000, -2000);
		const VECTOR2I scale = VECTOR2I(10, 10);
		DrawExtendGraph(imagePos.x, imagePos.y, Screen::WIDTH * scale.x, Screen::HEIGHT * scale.y,vignetteGraph, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBright(255, 255, 255);


	// スクリーン切り替え
	currentScreen = 1 - currentScreen;

#endif
	

	
}

void BlurScreen::Play(float _time, float _fadeTime)
{
#if 0
	Reset();
	SetUseSetDrawScreenSettingReset(FALSE);//SetDrawScreenをしてもカメラが影響ないようにする
	for (int i = 0; i < SCREEN_NUM; i++) {
		if (blurScreen[i] != -1) {
			DeleteGraph(blurScreen[i]);
		}
		blurScreen[i] = MakeScreen(Screen::WIDTH, Screen::HEIGHT, true); //スクリーンの生成
		//GraphFilter(blurScreen[i], DX_GRAPH_FILTER_GAUSS, 2, 2);
	}

	alpha = defalutAlpha;

	use = true;
	time = _time;
	fadeTime = _fadeTime;
	defalutFadeTime = _fadeTime;
	fadeOut = false;
#else
	Reset();

	//スクリーンの作成
	smallW		= Screen::WIDTH;
	smallH		= Screen::HEIGHT;
	smallScreen = MakeScreen(smallW, smallH, TRUE);

	//vignetteGraph = Load::GetHandle(ID::SCREEN_BLUR_IMAGE);
	

	//GraphFilter(smallScreen, DX_GRAPH_FILTER_GAUSS, 8192);
	//ブラースクリーンのキャプチャを取る
	for (int i = 0; i < SCREEN_NUM; i++) {
		if (blurScreen[i] != -1) {
			DeleteGraph(blurScreen[i]);
		}
		blurScreen[i] = MakeScreen(Screen::WIDTH, Screen::HEIGHT, TRUE);
		
	}

	use				= true;
	fadeOut			= false;

	time			= _time;
	fadeTime		= _fadeTime;
	defalutFadeTime = _fadeTime;

	alpha			= defalutAlpha;


#endif
}


void BlurScreen::ScreenFinish()
{
	for (int i = 0; i < SCREEN_NUM; i++) {
		if (blurScreen[i] != -1) {
			DeleteGraph(blurScreen[i]);
			blurScreen[i] = -1;
		}
	}
	if (smallScreen != -1) {
		DeleteGraph(smallScreen);
		smallScreen = -1;
	}
	
	SetDrawScreen(DX_SCREEN_BACK);
}

void BlurScreen::FeedOut()
{
	fadeOut = true;
}

void BlurScreen::Reset()
{
	use				= false;

	alpha			= 0.0f;

	fadeOut			= false;

	time			= 0.0f;
	fadeTime		= 0.0f;

	currentScreen	= 0;
	screenDraw		= 0.0f;

}

void BlurScreen::CurrentScreenSet()
{
	if (blurScreen[currentScreen] != -1) {
		SetDrawScreen(blurScreen[currentScreen]); //描画先を更新
	}
}
