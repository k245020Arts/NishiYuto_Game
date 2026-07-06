#include "Sky.h"
#include "../Component/Object/Object3D.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "../Component/ComponentManager.h"
#include "../Common/ResourceLoader.h"
#include "../Component/Transform/Transform.h"

Sky::Sky()
{
	sky = nullptr;
}

Sky::~Sky()
{
}

void Sky::Update()
{
	//‹ó‚ð‰ñ“]
	sky->GetTransform()->rotation.y += 0.01f * DegToRad;
}

void Sky::Draw()
{
}

void Sky::CreateSky()
{
	sky =  new Object3D();

	const VECTOR3 SCALE = VECTOR3(2000.0f, 2000.0f, 2000.0f);
	const VECTOR3 POSITION = VECTOR3(0, -4000, 0);

	sky->Init(POSITION, VZero, SCALE, "sky");
	sky->SetDrawOrder(10);

	MeshRenderer* mesh = sky->Component()->AddComponent<MeshRenderer>();
	mesh->ModelHandle(ResourceLoad::LoadModel("sky/sky_2", ID::SKY));

}
