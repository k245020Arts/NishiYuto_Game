#include "Wave.h"
#include "../Enemy/TrashEnemy/TrashEnemyManager.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/Boss/BossCreater.h"
#include "../GameControler/GameControler.h"
#include "../Common/Fead/Fead.h"

Wave::Wave()
{
	tEnemyManager = new TrashEnemyManager;
	
	waveNow = 0;
	battleCounter = 0;
	isCooperate = false;
	bossCreate = true;
	first = false;
	bossWave = false;
}

Wave::~Wave()
{
}

void Wave::Update()
{
	if (!first) {
		return;
	}
	
	EnemySpawn();
	
	CooperateAttack();
	RangedCooperateAttack();
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
	if (waveNow >= WAVE_MAX)
		return;

	int _counter = tEnemyManager->GetActiveEnemy();
	if (_counter <= 0)
	{
		waveNow++;

		if(waveNow <= WAVE_MAX - 1)
			tEnemyManager->CreateEnemy(SPWNPOS, spawnData[waveNow - 1].MeleeEnemyCounter, spawnData[waveNow - 1].RangedEnemyCounter);
		
		if (waveNow == WAVE_MAX)
			if (bossCreate)
			{
				FindGameObject<BossCreater>()->CreateBoss();
				FindGameObject<GameControler>()->ChangeState(GameControler::GameState::BOSS_PLAY_BEFORE);

				FindGameObject<Fead>()->FeadIn(1.0f, 0x000000, Easing::EaseIn<int>);
				bossCreate = false;
				bossWave = true;
			}
		isCooperate = false;
	}
}

void Wave::CooperateAttack()
{
	int counter = tEnemyManager->GetMeleeActiveEnemy();

	if (counter > 4 || isCooperate)
		return;

	if (waveNow == 1)
	{
		tEnemyManager->Cooperate();
		isCooperate = true;
	}
}

void Wave::RangedCooperateAttack()
{
	if (waveNow != 2)
		return;
	int counter = tEnemyManager->GetMeleeActiveEnemy();

	if (counter <= 5)
		isCooperate = false;

	if (counter > 10 || isCooperate)
		return;

	tEnemyManager->SetStartRangedAttack(true);
	isCooperate = true;
}
