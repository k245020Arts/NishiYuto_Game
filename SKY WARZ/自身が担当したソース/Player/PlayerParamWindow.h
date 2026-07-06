#pragma once
#include "Player.h"

class PlayerParamWindow
{
public:
	PlayerParamWindow();
	PlayerParamWindow(Player* _player);
	~PlayerParamWindow();

	/// <summary>
	/// パラメータのWindowを表示する関数
	/// </summary>
	void PlayerParamWindowView();
	


private:
	bool windowMode;
	Player* player;

	void SaveReaction();
	void SaveAttackData();

	void LoadAttackData();
	void LoadReaction();

};