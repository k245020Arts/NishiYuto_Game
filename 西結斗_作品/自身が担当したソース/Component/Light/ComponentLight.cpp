#include "ComponentLight.h"
#include "../Transform/Transform.h"
#include "../../../ImGui/imgui.h"

ComponentLight::ComponentLight()
{
	lightHandle = -1;
	debugId = 5;
	tag = Function::GetClassNameC<ComponentLight>();
}

ComponentLight::~ComponentLight()
{
	if (lightTransform != nullptr) {
		delete lightTransform;
		lightTransform = nullptr;
	}
	if (lightHandle >= 0) {
		DeleteLightHandle(lightHandle);
		lightHandle = -1;
	}
	
}

void ComponentLight::Update()
{
	switch (kind)
	{
	case ComponentLight::SPOT:
		SpotUpdate();
		break;
	case ComponentLight::POINT:
		PointUpdate();
		break;
	default:
		break;
	}

}

void ComponentLight::Draw()
{
	//DrawSphere3D(lightTransform->WorldTransform().position, rangeHalf, 10, 0x00ff00, 0x00ff00, false);
}

void ComponentLight::Start()
{
}

void ComponentLight::PointLightHandleStart(VECTOR position, float range, float attan1, float attan2, float attan3)
{
	lightTransform = new Transform(position, VZero, VZero);
	lightTransform->SetParent(obj->GetTransform());
	lightHandle = CreatePointLightHandle(lightTransform->position,range,attan1,attan2,attan3);
	rangeHalf = range;
	kind = POINT;
}

void ComponentLight::SpotLightHandleStart(VECTOR position, VECTOR direction, float outAngle, float inAngle, float range, float atten0, float atten1, float atten2)
{
	lightTransform = new Transform(position, VZero, VZero);
	lightTransform->SetParent(obj->GetTransform());
	lightHandle = CreateSpotLightHandle(lightTransform->position,direction,outAngle,inAngle,range,atten0,atten1,atten2);
	kind = SPOT;
}

void ComponentLight::ImguiDraw()
{
	ImGui::DragFloat3("DIR", &dir.x, 1.0f, -180, 180);
	if (ImGui::Button("DIR_CHANGE")) {
		dir *= DegToRad;
		SetLightDirectionHandle(lightHandle, dir);
	}
	ImGui::TreePop();
}

void ComponentLight::SpotUpdate()
{
	//スポットライトの設定
	SetLightPositionHandle(lightHandle, lightTransform->WorldTransform().position);
	SetLightDifColorHandle(lightHandle, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
	SetLightDirectionHandle(lightHandle, obj->GetTransform()->WorldTransform().rotation);
}

void ComponentLight::PointUpdate()
{
	//ポイントライトの設定
	SetLightPositionHandle(lightHandle, lightTransform->WorldTransform().position);
	SetLightDifColorHandle(lightHandle, GetColorF(0.0f, 1.0f, 0.0f, 1.0f));
}
