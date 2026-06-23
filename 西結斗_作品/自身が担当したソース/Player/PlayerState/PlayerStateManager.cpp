#include "playerStateManager.h"
#include "../../Common/CsvReader.h"
#include <fstream>
#include <iostream>

PlayerStateManager::PlayerStateManager()
{
	newLinkIdNum = 100000;
	nodeBaseNum = 10000;
	nodeName = "Player";
	fileName = "data/csv/playerStateNodeData.csv";
	LoadState();
	//Com = PlayerInformation::CharaComponent();
}

PlayerStateManager::~PlayerStateManager()
{
	SaveState();
}
