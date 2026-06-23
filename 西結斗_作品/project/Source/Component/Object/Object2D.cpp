#include "Object2D.h"
#include "../Transform/Transform.h"
#include "../ComponentManager.h"
#include "../../../ImGui/imgui.h"
#include "../Color/Color.h"
#include "../Hierarchy/Hierarchy.h"
#include "../../Common/Transitor/TransitorManager.h"
#include "../../Common/Memory/MemoryCount.h"
#include "../../Common/Debug/Debug.h"

Object2D::Object2D()
{
}

void Object2D::Update()
{
	BaseObject::Update();
}

void Object2D::Draw()
{
	BaseObject::Draw();
}

void Object2D::Init(const VECTOR2F& _pos, const VECTOR2F& _rotate, const VECTOR2F& _scale, std::string name)
{
	//2D‚¾‚Æz‚Í‚È‚¢‚Ì‚Å0‚É‚µ‚Äs‚¤
	BaseObject::Init(name);
	transform = Component()->AddComponent<Transform>();
	transform->position = VECTOR3(_pos.x, _pos.y, 0.0f);
	transform->rotation = VECTOR3(_rotate.x, _rotate.y, 0.0f);
	transform->scale = VECTOR3(_scale.x, _scale.y, 0.0f);
}

void Object2D::Init(const Transform& _transform, std::string name)
{
	Init(VECTOR2F(_transform.position.x,_transform.position.y), VECTOR2F(_transform.rotation.x, _transform.rotation.y), VECTOR2F(_transform.scale.x, _transform.scale.y), name);
}