#include "sphereCollider.h"
#include "../../Common/Debug/Debug.h"

SphereCollider::SphereCollider()
{
	debugId = 3;
	tag = Function::GetClassNameC<SphereCollider>();
	SetTag(tag);
}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::Update()
{

}

void SphereCollider::Draw()
{
	/*if (collTag != CollsionInformation::P_ATTACK) {
		return;
	}*/
	if (Debug::SphereColliderDraw()) { //デバック表示モードがオフなら何もしない
		if (finish) { //当たり判定がストップしているなら黄色の円にする
			DrawSphere3D(collTransform->WorldTransform().position, collTransform->scale.x, 10, 0xffff00, 0xff0000, false);
		}
		else {
			DrawSphere3D(collTransform->WorldTransform().position, collTransform->scale.x, 10, 0xff0000, 0xff0000, false);
		}
	}
}

void SphereCollider::Start()
{
	
}
