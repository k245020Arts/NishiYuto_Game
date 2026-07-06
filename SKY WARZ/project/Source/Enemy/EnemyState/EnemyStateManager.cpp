#include "EnemyStateManager.h"


EnemyStateManager::EnemyStateManager()
{
	
	newLinkIdNum = 200000;
	nodeBaseNum = 20000;
	nodeName = "Enemy";
	fileName = "data/csv/enemyStateNodeData.csv";
	LoadState();
	//Com = EnemyInformation::EnemyComponent();
}

EnemyStateManager::~EnemyStateManager()
{
	SaveState();
}