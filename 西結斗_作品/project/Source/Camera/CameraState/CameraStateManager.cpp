#include "cameraStateManager.h"

CameraStateManager::CameraStateManager()
{
	newLinkIdNum	= 300000;
	nodeBaseNum		= 30000;
	nodeName		= "Camera";
	fileName		= "data/csv/cameraStateNodeData.csv";
	LoadState();
	//Com = CameraInformation::CameraComponent();
}

CameraStateManager::~CameraStateManager()
{
	SaveState();
}
