#include "cameraManager.h"
#include "../Component/Object/Object3D.h"
#include "../Component/Transform/Transform.h"
#include "camera.h"
#include "../Enemy/EnemyManager.h"
#include "../Component/Light/ComponentLight.h"
#include "../Component/ComponentManager.h"
#include "../Component/Collider/ModelCollider.h"
#include "../Common/ResourceLoader.h"
#include "../Component/Collider/SphereCollider.h"
#include "../Component/Collider/RayCollider.h"
#include "../../Source/Component/Physics/Physics.h"
#include "../State/StateManager.h"
#include "../Title/TitleCamera.h"

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::Update()
{
}

void CameraManager::Draw()
{
}

void CameraManager::CreateCamera()
{
	mainCamera = new Object3D();
	const VECTOR3 InitPos = VECTOR3(0, 3000, -6000);
	mainCamera->Init(InitPos, VZero,VOne, "CAMERA_OBJ");

	mainCamera->Component()->AddComponent<StateManager>();

	Camera* camera = mainCamera->Component()->AddComponent<Camera>();
	camera->Start(nullptr);

	/*SphereCollider* c = mainCamera->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.oneColl = false;
	info.parentTransfrom = camera->GetCameraTransform();
	info.shape = CollsionInformation::SPHERE;
	info.tag = CollsionInformation::C_FLOOR;
	c->CollsionAdd(info, Transform(VZero, VZero, VOne * VECTOR3(100,0,0)));*/

	

	mainCamera->Component()->AddComponent<Physics>()->Start(VZero, VZero);

	mainCamera->SetDrawOrder(-1);

}

void CameraManager::ParentObj(std::string _str)
{
	Object3D* obj = FindGameObjectWithTag<Object3D>(_str);
	cameraComponentPtr = mainCamera->Component()->GetComponent<Camera>();
	cameraComponentPtr->PlayerSet(obj);

	//当たり判定の情報をセット
	RayCollider* c = mainCamera->Component()->AddComponent<RayCollider>();
	CollsionInfo info;
	info.oneColl			= false;
	info.parentTransfrom	= mainCamera->Component()->GetComponent<Camera>()->GetCameraTransform();
	info.shape				= CollsionInformation::RAY;
	info.tag				= CollsionInformation::C_FLOOR;
	const float RAY_LONG = 100;

	std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data){ cameraComponentPtr->PushCamera(_data);};

	c->RaySet(info, Transform(VECTOR3(0, -RAY_LONG, 0), VZero, VOne), Transform(VECTOR3(0, RAY_LONG, 0), VZero, VOne), func);
}

void CameraManager::TitleCameraCreate()
{
	mainCamera = new Object3D();
	const VECTOR3 InitPos = VECTOR3(0, 3000, -6000);
	mainCamera->Init(InitPos, VZero, VOne, "CAMERA_OBJ");

	//mainCamera->Component()->AddComponent<StateManager>();

	TitleCamera* camera = mainCamera->Component()->AddComponent<TitleCamera>();
	camera->Start(mainCamera);

	/*SphereCollider* c = mainCamera->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.oneColl = false;
	info.parentTransfrom = camera->GetCameraTransform();
	info.shape = CollsionInformation::SPHERE;
	info.tag = CollsionInformation::C_FLOOR;
	c->CollsionAdd(info, Transform(VZero, VZero, VOne * VECTOR3(100,0,0)));*/



	mainCamera->Component()->AddComponent<Physics>()->Start(VZero, VZero);

	mainCamera->SetDrawOrder(-6);


}

void CameraManager::TitleParentObj(std::string _str)
{
	Object3D* obj = FindGameObjectWithTag<Object3D>(_str);
	//プレイヤーのセット
	mainCamera->Component()->GetComponent<TitleCamera>()->PlayerSet(obj);
}
