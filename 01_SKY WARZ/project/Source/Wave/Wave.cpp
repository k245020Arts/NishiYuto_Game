#include "Wave.h"
#include "../Enemy/TrashEnemy/TrashEnemyManager.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/Boss/BossCreater.h"
#include "../GameControler/GameControler.h"
#include "../Common/Fead/Fead.h"
#include "../Stage/StageSelectData.h"
#include "../Common/JsonReader/JsonReader.h"

Wave::Wave()
{
	tEnemyManager = new TrashEnemyManager;
	const StageData stageData = StageSelectData::GetInstance()->GetNowStageData();
	stageNum = stageData.id;

	JsonReader json; 
	json.Load("data/json/WaveData.json");
	
	JSON jsonData = json.Data();

	const auto& wave = jsonData["WaveEnemyData"][std::to_string(stageNum)];
	stageMax = (int)wave.size();
	SpawnData data;
	for (int i = 0; i < stageMax; i++)
	{
		const std::string StageName = "Wave" + std::to_string((i + 1));
		data.SetData(wave[StageName]["melee"], wave[StageName]["range"]);
		spawnData.push_back(data);
	}

	waveNow = 0;
	battleCounter = 0;
	isCooperate = false;
	bossCreate = true;
	first = false;
	bossWave = false;

	spawnTimeCounter = 0;

	gameControler = FindGameObject<GameControler>();

}

Wave::~Wave()
{
	tEnemyManager = nullptr;
	spawnData.clear();
}

void Wave::Update()
{
	if (!first)
		return;
	
	EnemySpawn();
}

void Wave::Draw()
{
}

void Wave::FirstRespown()
{
	first = true;
}

bool Wave::GetBossWave()
{
	return bossWave;
}

void Wave::EnemySpawn()
{
	if (waveNow > stageMax)
		return;

	if (gameControler->GetStateNumber() == GameControler::PAUSE_SCENE) 
		return; //操作説明ガイドが表示されているときにはこのクラスは通過しない

	int _counter = tEnemyManager->GetActiveEnemy();
	if (_counter <= 0)
	{
		spawnTimeCounter += Time::DeltaTimeRate();
		if (spawnTimeCounter <= 1.0f)
			return;

		waveNow++;

		if(waveNow <= stageMax)
			tEnemyManager->CreateEnemy(SPWNPOS, spawnData[waveNow - 1].MeleeEnemyCounter, spawnData[waveNow - 1].RangedEnemyCounter);
		else
			if (bossCreate)
			{
				FindGameObject<BossCreater>()->CreateBoss();
				FindGameObject<GameControler>()->ChangeState(GameControler::GameState::BOSS_PLAY_BEFORE);

				FindGameObject<Fead>()->FeadIn(1.0f, 0x000000, Easing::EaseIn<int>);
				bossCreate = false;
				bossWave = true;
			}
		isCooperate = false;

		spawnTimeCounter = 0;
	}
}
