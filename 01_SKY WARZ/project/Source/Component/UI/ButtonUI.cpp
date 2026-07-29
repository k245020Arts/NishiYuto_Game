#include "ButtonUI.h"
#include "../../Common/InputManager/PadInput.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../Component/Transform/Transform.h"
#include "../ComponentManager.h"
#include "../Color/Color.h"

namespace {
	int buttonNum[ButtonUI::ButtonType::BUTTON_MAX] = {
		XINPUT_BUTTON_A,XINPUT_BUTTON_B,
		XINPUT_BUTTON_X,XINPUT_BUTTON_Y 
	};

	int keyNum[ButtonUI::ButtonType::BUTTON_MAX] = {
		KEY_INPUT_X,KEY_INPUT_E,
		KEY_INPUT_Q,KEY_INPUT_C
	};
}

ButtonUI::ButtonUI()
{
	type = NONE;
	buttonImage = -1;
	debugId = 31;
	tag = Function::GetClassNameC<ButtonUI>();
	gradeMode = false;
	buttonActive = false;
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	backImage = -1;
	scale = 0;

}

ButtonUI::~ButtonUI()
{

}

void ButtonUI::Update()
{
	if (type == NONE) {
		my_error_assert("ボタンの種類がセットされていません。");
		return;
	}

	//コントローラーが繋がっているならコントローラー用UIにする
	if (InputManager::GetInstance()->GetControllerInput()->GetIsPadInput()) {
		if (InputManager::GetInstance()->GetControllerInput()->GetIsButtonPushingNow(buttonNum[type])) {
			push = true;
		}
		else {
			push = false;
		}
	}
	else {
		//キーボード用UI
		if (InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPushing(keyNum[type])) {
			push = true;
		}
		else {
			push = false;
		}
	}
	
	if (gradeMode) {
		r += Time::DeltaTimeRate() * (float)OPAQUE_COLOR;
		g += Time::DeltaTimeRate() * (float)OPAQUE_COLOR;
		b += Time::DeltaTimeRate() * (float)OPAQUE_COLOR;
		if (r >= (float)OPAQUE_COLOR) {
			r = 0.0f;
		}
		if (g >= (float)OPAQUE_COLOR) {
			g = 0.0f;
		}
		if (b >= (float)OPAQUE_COLOR) {
			b = 0.0f;
		}
	}
}

void ButtonUI::Draw()
{
	const Transform* transform = obj->GetTransform();
	//分かりやすいように押しているときや、ボタンがアクティブじゃなかったら灰色にする
	if (push || !buttonActive) {
		SetDrawBright(50, 50, 50);
	}
	else {
		SetDrawBright((int)rgbColor.r, (int)rgbColor.g, (int)rgbColor.b);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)rgbColor.a);
	}
	//加算合成モードなら加算合成を掛ける
	if (gradeMode) {
		SetDrawBright((int)r, (int)g, (int)b);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 60);
		float add = transform->scale.x / 8;
		for (int i = 0; i < 8; i++) {
			DrawRotaGraph((int)transform->position.x, (int)transform->position.y, (double)add * i, 0.0, backImage, true);	
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawRotaGraph((int)transform->position.x, (int)transform->position.y, transform->scale.x, 0.0, buttonImage, true);
	}
	else {
		DrawRotaGraph((int)transform->position.x, (int)transform->position.y, (double)transform->scale.x, 0.0, backImage, true);
		//UIの部分はアクティブの時以外は戻さないで薄暗くしたい
		if (buttonActive) {
			SetDrawBright(255, 255, 255);
		}
		DrawRotaGraph((int)transform->position.x, (int)transform->position.y, (double)transform->scale.x, 0.0, buttonImage, true);
	}

	//元に戻す
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ButtonUI::Start(ButtonType _buttonType, int _handle, Color::Rgb _rgb)
{
	buttonImage = _handle;
	type = _buttonType;
	obj->GetTransform()->scale = 0.8f;
	buttonActive = true;
	backImage = ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "UICicle",ID::UI_BACK_GRAPH);
	rgbColor = _rgb;
}