#pragma once
#include "../../Library/GameObject.h"

class TrashEnemyManager;
class GameControler;

class Wave :public GameObject
{
public:
	Wave();
	~Wave();
	void Update();
	void Draw();
	/// <summary>
	/// 初めて生成されるときに使用
	/// </summary>
	void FirstRespown();

	bool GetBossWave();

private:
	void EnemySpawn();
	/*void CooperateAttack();
	void RangedCooperateAttack();*/
	TrashEnemyManager* tEnemyManager;
	GameControler* gameControler;
	//最大ウェーブ数
	//const int WAVE_MAX = 3;
	const VECTOR3 SPWNPOS = VECTOR3(0, 0, 2500);
	int waveNow;

	float battleCounter;

	bool isCooperate;

	bool bossCreate;
	bool first;
	bool bossWave;


	int stageNum;
	int stageMax;

	struct SpawnData
	{
		int MeleeEnemyCounter;
		int RangedEnemyCounter;

		SpawnData()
		{
			MeleeEnemyCounter = 0;
			RangedEnemyCounter = 0;
		}

		void SetData(int _meleeCounter, int _rangedCounter)
		{
			MeleeEnemyCounter = _meleeCounter;
			RangedEnemyCounter = _rangedCounter;
		}
	};
	std::vector<SpawnData> spawnData;
	float spawnTimeCounter;
};