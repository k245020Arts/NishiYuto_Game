#pragma once
#include "../../Library/GameObject.h"
#include "../GameControler/GameControler.h"


class UIManager;
class TextRenderer;

class PauseScreen : public GameObject
{
public:
	PauseScreen();
	~PauseScreen();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// ポーズ画面に入る
	/// </summary>
	void PauseStart();
	/// <summary>
	/// ポーズ画面を終了する
	/// </summary>
	/// <returns></returns>
	bool PauseFinish();
	/// <summary>
	/// 選択されているかの取得
	/// </summary>
	/// <returns>選択されていたらtrue/returns>
	bool IsSelect()const { return selectMenu; }
	

	enum PauseMenuItem
	{
		Back = 0,
		Guide,
		Title,

		Max
	};

	enum PauseResult
	{
		None = -1,
		Resume,
		OpenGuide,
		ToTitle
	};
	/// <summary>
	/// 選択した状態の取得
	/// </summary>
	/// <returns>状態が帰ってくる</returns>
	PauseResult GetResult() const { return result; }
	/// <summary>
	/// ポーズボタンを押してポーズ画面からゲーム画面に遷移する
	/// </summary>
	void PauseButtonGameBack();

private:
	bool pause;
	float timeRate;
	bool selectMenu;

	int currentIndex;

	PauseResult result;

	UIManager* UiManager;

	PauseMenuItem pauseItem;

	TextRenderer* pauseText;
	TextRenderer* backText;
	TextRenderer* titleText;
	TextRenderer* guideText;
	TextRenderer* selectText;
	TextRenderer* cursorText;

	std::vector<TextRenderer*> menuTexts;

	void UpdateButtonState();

	float animationTime;

	int screen;
	float selectDelayTime;
	bool delayCountStart;
	float baseSinRate;
	float inputDelayTime;

	std::vector<VECTOR3> cursorPos;
};