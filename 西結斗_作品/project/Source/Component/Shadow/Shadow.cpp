#include "Shadow.h"
#include "../../Common/ID/ID.h"
#include "../../Common/ResourceLoader.h"
#include "../MeshRenderer/MeshRenderer.h"
#include "../Color/Color.h"
#include "../ComponentManager.h"
#include "../../Common/Easing.h"
#include "../Transform/Transform.h"

Shadow::Shadow()
{
	model		= -1;
	debugId		= 24;
	tag			= Function::GetClassNameC<Shadow>();
	height = -1;
	obj = nullptr;
	//gameControler = FindGameObject<GameControler>();
}

Shadow::~Shadow()
{
}

void Shadow::Update()
{
	//親のポジション
	obj->GetTransform()->position = obj->GetParent()->GetTransform()->position;
	//地面のポジションを参照
	obj->GetTransform()->position.y = groundPos.y;
}

void Shadow::Draw()
{

}

void Shadow::Start(float _height)  
{  
	obj = GetBaseObject();
    model = ResourceLoad::LoadModel("shadow1", ID::SHADOW);
    MeshRenderer* mesh = GetBaseObject()->Component()->AddComponent<MeshRenderer>();  
    mesh->ModelHandle(model);  
    mesh->DrawLocalPosition();  
    baseScale = obj->GetTransform()->scale;
	height = _height;
}

void Shadow::ChangeScale(const VECTOR3& _dist, const VECTOR3& _groundPos)
{
	float base = baseScale.x;
	float d = _dist.y;
	//最大の距離より遠くなったらこれ以上影を小さくしない
	if (d > MAX_DIST) {
		d = MAX_DIST;
	}
	float s = Easing::EaseOut(base, 0.2f, d / MAX_DIST);
	obj->GetTransform()->scale.x = s;
	obj->GetTransform()->scale.z = s;
	groundPos = _groundPos + VECTOR3(0, height, 0); //影がしっかり描画されるように地面より少し高く設定
	dist = _dist;
}
