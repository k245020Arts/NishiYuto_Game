#pragma once
#include "../../Library/GameObject.h"
#include "../Common/MethodExec.h"
#include "../Component/Object/Object2D.h"

typedef MethodExec<class GameControler> MEB;
typedef MethodExec<class GameControler> MEBDraw;
class SoundManager;
class Camera;
class ResultUi;
class PauseScreen;
class GuideWindow;

//#define STRING_MODE

class GameControler : public GameObject
{
public:

	enum GameState
	{
		NONE = -1,
		BEFORE,
		PLAY,
		PAUSE_SCENE,
		BOSS_PLAY_BEFORE,
		WIN,
		LOSE,
		SCENE_CHANGE,
	};

	friend class EnemyIdol;
	friend class PlayerWait;
	friend class PlayerDie;
	friend class EnemyDie;
	
	GameControler();
	~GameControler();

	void Update()override;
	void Draw()override;

#ifdef STRING_MODE
	void ChangeState(std::string _name);
	//std::string GetStateName();
	int GetStateNumber();
#else
	/// <summary>
	/// Stateの種類の取得
	/// </summary>
	/// <returns>現在のstate</returns>
	GameState GetStateNumber();
	/// <summary>
	/// ゲームStateの変更
	/// </summary>
	/// <param name="_name">ゲームステート</param>
	void ChangeState(GameState _name);
#endif // STRING_MODE

	/// <summary>
	/// カメラのポインタを取得
	/// </summary>
	void SetPointer();
	/// <summary>
	/// Stateが1フレーム前にチェンジされたかどうかを取得
	/// </summary>
	/// <returns>1フレ前にステートがチェンジされていたらtrue</returns>
	bool GetChangeStateOneFrame() const { return changeState; }
	/// <summary>
	/// 勝ちか負けかの判定を行う
	/// </summary>
	/// <param name="_win"></param>
	void ResultUiStart(bool _win);
	
	
private:

	void BeforeUpdate();
	void BeforeDraw();

	void PlayUpdate();
	void PlayDraw();

	void PauseUpdate();
	void PauseDraw();

	void BossPlayBeforeUpdate();
	void BossPlayBeforeDraw();

	void WinUpdate();
	void WinDraw();

	void LoseUpdate();
	void LoseDraw();

	void SceneChangeUpdate();
	void SceneChangeDraw();

	float startCount;

	float resultCounter;

	std::string nowState;
	Camera* camera;
	
	std::string beforeState;

	GameState gameState;
	GameState beforeGameState;

	GameState keepGameState;

	ResultUi* resultUi;

	bool changeState;
	PauseScreen* pause;

	GuideWindow* guide;

	static int gamePlayNum;

	void PauseResult();
	void GuideWindowUpdate();
};