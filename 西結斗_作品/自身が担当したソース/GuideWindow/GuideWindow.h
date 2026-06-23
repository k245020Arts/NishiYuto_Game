#pragma once
#include "../../Library/GameObject.h"

class TextRenderer;

class GuideWindow : public GameObject
{
public:
	GuideWindow();
	~GuideWindow();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// 操作説明書の開始
	/// </summary>
	/// <param name="_menu">ポーズ画面から開いたらtrue、ゲーム開始直後から開いたらfalse</param>
	void GuideWindowDraw(bool _menu);
	/// <summary>
	/// 操作説明書の終了
	/// </summary>
	void GuideWindowFinish();
	/// <summary>
	/// ボタンが押されたかどうかの取得
	/// </summary>
	/// <returns>trueならボタンを押している</returns>
	bool GetPush() { return buttonPut; }
	/// <summary>
	///	ゲーム状態に遷移するかの設定
	/// </summary>
	/// <returns>trueならゲーム状態へ移行</returns>
	bool GetGameBack() { return gameBack; }
	/// <summary>
	/// 操作説明書が開かれているかの説明
	/// </summary>
	/// <returns></returns>
	bool GetActive() { return  active; }

private:

	bool menu;

	bool gameBack;

	TextRenderer* gameBackText;
	TextRenderer* pauseBackText;

	int guideWindowControlerHandle;
	int guideWindowkeyHandle;

	bool active;

	int aButtonImage;
	int bButtonImage;
	bool buttonPut;
	bool controler;
};