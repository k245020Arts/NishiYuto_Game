#pragma once
#include "../Library/SceneBase.h"
#include <vector>

class InputManager;

/// <summary>
/// ゲームプレイのシーンを制御する
/// </summary>

class PlayScene : public SceneBase
{
public:
	enum Mode
	{
		NORMAL,
		CAMERA_EDITOR,
	};

	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
	/// <summary>
	/// カメラのエディターモードかどうかを取得
	/// </summary>
	/// <returns>カメラのモードかどうか</returns>
	Mode GetMode() { return mode; }
	/// <summary>
	/// カメラのモードの変更
	/// </summary>
	/// <param name="_mode"></param>
	void SetMode(Mode _mode) { mode = _mode; }
	//static int count;
private:

	
	Mode mode;
};

