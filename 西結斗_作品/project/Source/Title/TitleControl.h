#pragma once
#include "../../Library/GameObject.h"

class InputManager;
class SoundManager;

class TitlePlayer;
class TextRenderer;

enum TitleState
{
	TITLE,
	STAGE_SELECT,
};

class TitleControl : public GameObject
{
public:
	TitleControl();
	~TitleControl();

	void Update()override;
	void Draw()override;

	void TitleUpdate();
	void StageUpdate();


	float GetNowProgress();
	void SetNowProgress(float nowProgress);
	float progress;

	void StageSelect();

private:
	int hImage;
	int keyImage;
	int pushKeyImage;
	int titleImage;
	float firstCounter;
	float exrate;
	float pushCounter;
	float moveButton;
	int font;
	int alpha;
	float alphaTime;
	float selectMoveCounter;

	TitlePlayer* player;
	int stageID;
	float selectCounter;
	TitleState titleState;

	TextRenderer* stageText;
	TextRenderer* text;
	std::vector<int> stageHandle;

	void KeyControlerChangeText();
	bool controler;
};
