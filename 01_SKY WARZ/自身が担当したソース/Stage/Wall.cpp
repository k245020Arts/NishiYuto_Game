#include "Wall.h"
#include "../Component/Transform/Transform.h"
#include "../Component/Object/Object3D.h"
#include "../Common/Effect/EffectManager.h"
#include "../Common/Easing.h"
#include "../../ImGui/imgui.h"

Wall::Wall()
{
	debugId = 16;
	tag = Function::GetClassNameC<Wall>();
	count = 0;
	model = -1;
	pTransform = nullptr;
	transform = nullptr;
}

Wall::~Wall()
{
	int a = 0;
	a += 0;
}

void Wall::Update()
{
	if (pTransform == nullptr) {
		Object3D* obj = FindGameObjectWithTag<Object3D>("PLAYER");
		if (obj != nullptr) {
			pTransform = obj->GetTransform();
		}
		else {
			return;
		}
		
	}
	float distance;
	//X方向に壁壁のpositionがある場合上のif文の中
	if (transform->position.x >= 10000 || transform->position.x <= -10000) {
		distance = transform->position.x - pTransform->position.x;
	}
	else {
		//z方向に壁のpositionがある場合にはこちら
		distance = transform->position.z - pTransform->position.z;
	}
	if (fabs(distance) < 4000) {
		
		if (!EffectManager::GetInstance()->IsPlayIng(Effect_ID::EFFECT_WALL)) {
			//エフェクトが再生されていないときに再生
			EffectManager::GetInstance()->CreateEffekseer(Transform(VZero, VZero, VOne * 41.0f), nullptr, Effect_ID::EFFECT_WALL, 1.0f);
		}
		else {
			//距離が離れれば離れるほど半透明度をあげる。
			Color::Rgb rgb;
			float disRate;
			disRate = distance / 4000.0f;
			rgb = Color::Rgb(255.0f, 255.0f, 255.0f, Easing::Lerp(255.0f, 0.0f, disRate));
			EffectManager::GetInstance()->SetColor(Effect_ID::EFFECT_WALL, rgb);
		}
		
	}
	count++;
}

void Wall::Draw()
{
}

void Wall::ModelSet(int _modelHandle)
{
	model = _modelHandle;
	transform = obj->GetTransform();
}

void Wall::ImguiDraw()
{
	if (count == 0) {
		ImGui::Text("no");
	}
	else if (count == 1) {
		ImGui::Text("right");
		
	}
	else {
		ImGui::Text("a lot");
	}
	count = 0;
}
