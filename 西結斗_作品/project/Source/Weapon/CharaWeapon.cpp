#include "CharaWeapon.h"
#include "../Component/Transform/Transform.h"
#include "../Common/ResourceLoader.h"
#include "../../ImGui/imgui.h"
#include "../Component/ComponentManager.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "swordEffect.h"

CharaWeapon::CharaWeapon()
{
	weapon = true;
	anim = nullptr;
	chara = nullptr;
	frame = 0;
	matrix = MGetIdent();
	modelHandle = -1;
	transform = nullptr;
	parent = false;
	timeRate = 0.0f;
	weaponHandle = -1;
	debugId = 17;
	tag = Function::GetClassNameC<CharaWeapon>();
}

CharaWeapon::~CharaWeapon()
{
}

void CharaWeapon::Update()
{
	/*obj->GetTransform()->position = (VECTOR3)MV1GetFramePosition(modelHandle, frame);
	if (weapon) {
		obj->GetTransform()->rotation = chara->GetTransform()->rotation;
	}*/
	/// + VECTOR3(3.08f, 0.0f, 0.0f);
}

void CharaWeapon::Draw()
{
	if (parent) {
		matrix = MV1GetFrameLocalWorldMatrix(modelHandle, frame);//くっつけたいモデルのマトリックスをとる。
		MATRIX scaleMat = MGetScale(transform->scale);
		// 元の行列にスケールを掛ける
		MATRIX result = scaleMat * matrix;
		// 剣に適用
		MV1SetMatrix(weaponHandle, result);
		MV1DrawModel(weaponHandle);
	}
}

void CharaWeapon::ObjectPointer(BaseObject* _obj, int _frame, ID::IDType _baseModelName, int _weaponHandle)
{
	chara = _obj;
	anim = chara->Component()->GetComponent<Animator>();
	if (anim != nullptr) {
		anim->SetBoneFrame(frame);
	}
	
	frame = _frame;
	modelHandle = ResourceLoad::GetHandle(_baseModelName);
	weaponHandle = _weaponHandle;
	parent = true;
	transform = obj->GetTransform();

	SwordEffect* sE = obj->Component()->AddComponent<SwordEffect>();
}

void CharaWeapon::SetParent(bool _parent)
{
	//親を外すかどうか
	parent = _parent;
	if (!parent) {
		MeshRenderer* me = obj->Component()->AddComponent<MeshRenderer>();
		me->ModelHandle(weaponHandle);
	}
}

void CharaWeapon::ImguiDraw()
{
	if (ImGui::TreeNode("Weapon")) {
		ImGui::DragInt("frame", &frame, 1, 0, 53);
		if (ImGui::Button("RotationMove")) {
			weapon = !weapon;
		}
		ImGui::TreePop();
	}
}

void CharaWeapon::CreateSwordEffect(const VECTOR3& _nPos, const VECTOR3& _fPos, float _r, float _g, float _b, float _a, int index, float _time)
{
	
	SwordEffect* sE = obj->Component()->GetComponent<SwordEffect>();
	sE->CreateEffect(_nPos, _fPos,Color::Rgb(_r,_g,_b,_a), static_cast<int>(index), _time);
}

void CharaWeapon::SetImage(int _model)
{
	SwordEffect* sE = obj->Component()->GetComponent<SwordEffect>();
	sE->SetImage(_model);
}
