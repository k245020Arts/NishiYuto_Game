#include "RangedEnemyCamera.h"
#include "../Camera.h"
#include "../../Enemy/TrashEnemy/TrashEnemyManager.h"
#include "../../Player/player.h"
#include "../../Common/Easing.h"

RangedEnemyCamera::RangedEnemyCamera()
{
	tEnemyManager = nullptr;
	player = nullptr;

	enemyPos = VZero;
	pPos = VZero;

	copyCamPos = VZero;

	angle = 0.0f;
	isCameraArrived = false;

	isAngleSet = false;

}

RangedEnemyCamera::~RangedEnemyCamera()
{
}

void RangedEnemyCamera::Update()
{
	Camera* camera = GetBase<Camera>();

	UpdateCamPos(camera);

	camera->target = (pPos + enemyPos) * 0.5f;
	UpdeteCamMove(camera);
	CamRotYMove(camera);
}

void RangedEnemyCamera::Draw()
{
}

void RangedEnemyCamera::Start()
{
	Camera* camera = GetBase<Camera>();
	tEnemyManager = FindGameObject<TrashEnemyManager>();
	player = camera->cameraComponent.player.obj->Component()->GetComponent<Player>();

	enemyPos = tEnemyManager->GetRangedLeaderPos();
	pPos = player->GetPlayerObj()->GetTransform()->position + PlayerPosOffset;

	isAngleSet = false;
}

void RangedEnemyCamera::Finish()
{
}

void RangedEnemyCamera::UpdateCamPos(Camera* _camera)
{
	enemyPos = tEnemyManager->GetRangedLeaderPos();
	pPos = player->GetPlayerObj()->GetTransform()->position + PlayerPosOffset;

	VECTOR3 dir = VNorm(enemyPos - pPos);
	dir.y = 0.0f;

	float dist = VSize(enemyPos - pPos);

	VECTOR3 offset = dir * -1 * CamPos + VECTOR3(0, 400, 0);

	copyCamPos = pPos + offset;
}

void RangedEnemyCamera::UpdeteCamMove(Camera* _camera)
{
	const float CamSpeed = 0.1f;
	//カメラのポジションを格納
	const VECTOR3 CamPos = _camera->cameraComponent.cameraTransform->position;

	//指定したポジションのangleをだす
	VECTOR3 targetoffset = copyCamPos - pPos;
	float targetAngle = atan2f(targetoffset.z, targetoffset.x);

	VECTOR3 offset = CamPos - pPos;

	if (!isAngleSet)
	{
		angle = atan2f(offset.z, offset.x);
		isAngleSet = true;
	}
	
	float diff = targetAngle - angle;

	if (diff > DX_PI_F)  diff -= DX_TWO_PI_F;
	if (diff < -DX_PI_F) diff += DX_TWO_PI_F;	
	
	angle += diff * CamSpeed;

	//半径
	float radius = VSize(targetoffset);
	_camera->cameraComponent.cameraTransform->position.x = pPos.x + cosf(angle) * radius;

	if(CamPos.y >= copyCamPos.y * CamSpeed || CamPos.y <= copyCamPos.y * -CamSpeed)
		_camera->cameraComponent.cameraTransform->position.y = (copyCamPos.y - CamPos.y )* CamSpeed;

	_camera->cameraComponent.cameraTransform->position.z = pPos.z + sinf(angle) * radius;
}

void RangedEnemyCamera::CamRotYMove(Camera* _camera)
{
	VECTOR3 offset = _camera->cameraComponent.cameraTransform->position - pPos;

	VECTOR3 enemyToPlayer = VNorm(pPos - enemyPos);
	VECTOR3 cameraForward = VNorm(_camera->target - _camera->cameraComponent.cameraTransform->position);
	float dot = VDot(enemyToPlayer, cameraForward);
	float pi = 0;

	if (dot <= 0.0f)
		pi = DX_PI_F;

	_camera->cameraComponent.cameraTransform->rotation.y = atan2(offset.x, offset.z) + pi;
}
