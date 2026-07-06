#include "Physics.h"
#include "../Transform/Transform.h"
#include "../../../ImGui/imgui.h"
#include "../../Common/Transitor/TransitorManager.h"

namespace {
	const VECTOR3 MIN_FRICTION = VECTOR3(5.0f, 5.0f, 5.0f);
	
	const int PHYSICS_SUBSTEPS = 6;  // 1フレームを何分割するか
	const int GROUND_MISS_THRESHOLD = 1;
}

Physics::Physics()
{
	lastTransform = nullptr;
	currentTransform = nullptr;
	gravity = VZero;
	velocity = VZero;
	noGravity = false;
	debugId = 9;
	tag = Function::GetClassNameC<Physics>();
	ground = false;
	transitor = nullptr;
	groundMissCount = 0;
	baseGravity = VZero;
}

Physics::~Physics()
{
	if (lastTransform != nullptr) {
		delete lastTransform;
	}
}

void Physics::Update()
{

	if (!transitor->IsTransitor()) { //場面遷移中なら移動させない
		return;
	}
	if (obj->GetTag() == "PLAYER") {
		int a = 0;
		if (velocity.Size() != 0.0f) {
			int c = 0;
		}
	}
	*lastTransform = *currentTransform;

	float dt = obj->GetObjectTimeRate();

	// 地面にいない場合のみ重力を加算
	if (ground)
	{
		if (velocity.y < 0.0f)
		{
			velocity.y = 0.0f;
		}
		gravity = VZero;
		/*groundMissCount++;
		if (groundMissCount >= GROUND_MISS_THRESHOLD)
		{
			ground = false;
			groundMissCount = 0;
		}*/
	}
	else
	{
		groundMissCount = 0;
		gravity = baseGravity;
		velocity += gravity * dt;
	}
	
	// 摩擦減衰
	float resistance = velocity.Size();
	VECTOR3 velo = velocity;
	//velo.y -= 5.0f * dt;

	float newSpeed = max(resistance - firction.Size() * dt, 0.0f);
	velocity = velocity.Normalize() * newSpeed;
	velocity.y = velo.y;

	// 微小速度をカット（誤差防止）
	if (fabs(velocity.y) < 0.01f)
		velocity.y = 0.0f;

	// 位置更新
	currentTransform->position += velocity * dt;
}

void Physics::Draw()
{

#ifdef _DEBUG
	//進んでる距離をラインに起こす
	VECTOR3 pos = obj->GetTransform()->position;
	DrawLine3D(pos, pos + (velocity * obj->GetObjectTimeRate()), 0xff0000);

#endif //_DEBUG

}

void Physics::Start(const VECTOR3& _gravityAmout, const VECTOR3& _fir)
{
	currentTransform = obj->GetTransform();
	lastTransform = new Transform(VECTOR3(0,0,0),VZero,VZero);

	baseGravity = _gravityAmout;
	firction = _fir;
	transitor = FindGameObject<TransitorManager>();
}

void Physics::AddVelocity(const VECTOR3& _addVelocity, bool _deltaTime)
{
	VECTOR3 add = _addVelocity;
	//毎フレーム加算をしたいならここをTrueにする
	if (_deltaTime) {
		add *= obj->GetObjectTimeRate();
	}
	velocity += add;
}

void Physics::SetFirction(const VECTOR3& _setFirction)
{
	firction = _setFirction;
}

void Physics::AddFirction(const VECTOR3& _addFirction)
{
	firction += _addFirction;
}

void Physics::SetGravity(const VECTOR3& _setGravity)
{
	baseGravity = _setGravity;
}

void Physics::AddGravity(const VECTOR3& _addGraivty)
{
	baseGravity += _addGraivty;
}

void Physics::ImguiDraw()
{
	ImGui::Separator();
	ImGui::InputFloat3("velocity", &velocity.x);
	ImGui::Separator();
	ImGui::DragFloat3("gravity", &baseGravity.x, 0.0f, -200.0f, 200.0f);
	ImGui::DragFloat3("firction", &firction.x, 0.0f, -200.0f, 200.0f);
	ImGui::Separator();
	if (ImGui::Button("velocityReset")) {
		velocity = VZero;
	}
	if (ImGui::Button("gravityModeChange")) {
		noGravity = !noGravity;
	}
}

void Physics::SetGround(bool _g)
{
	ground = _g;
	groundMissCount = 0;
}
