#pragma once
#include "../../Library/GameObject.h"

class Object3D;
class GameControler;
class StateManager;
class Player;

class PlayerManager : public GameObject
{
public:
	PlayerManager();
	~PlayerManager();
	void Update() override;
	void Draw() override;
	/// <summary>
	/// プレイシーンのプレイヤーの生成
	/// </summary>
	void CreatePlayer();

	
	/// <summary>
	/// タイトルシーンのプレイヤーの生成
	/// </summary>
	void CreateTitlePlayer();

private:

	Object3D* playerPointer;

	GameControler* gameManager;
	StateManager* stateManager;
	void GameSceneChangeState();
	Player* playerPtr;
};
