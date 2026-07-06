#include "CollsionManager.h"
#include "colliderBase.h"
#include "../../Common/Function.h"
#include "../Transform/Transform.h"
#include "../../Common/Debug/Debug.h"
#include "CollsionEvent.h"
#include "ModelCollider.h"
#include "../ComponentManager.h"
#include "../Physics/Physics.h"
#include "rayCollider.h"
#include "DountCollider.h"
#include "AABBCollider.h"
#include "../Shadow/Shadow.h"
#include "CapsuleCollider.h"
#include "../../GameControler/GameControler.h"

static int plus = 0;

CollsionManager::CollsionManager()
{
	using namespace Function;
	using namespace CollsionInformation;
	//当たり判定でどの形同士ならこの関数にいれるという登録
	collsionKind[EnumTag(SPHERE ,SPHERE,SHAPE_MAX)]			= &CollsionManager::CollsionSphereToSphere;
	collsionKind[EnumTag(SPHERE ,MODEL,SHAPE_MAX)]			= &CollsionManager::CollsionSphereToModel;
	collsionKind[EnumTag(SPHERE, DONUT,SHAPE_MAX)]			= &CollsionManager::CollsionSphereToDount;
	collsionKind[EnumTag(MODEL ,RAY,SHAPE_MAX)]				= &CollsionManager::CollsionModelToRay;
	collsionKind[EnumTag(RAY ,AABB,SHAPE_MAX)]				= &CollsionManager::CollsionAABBToRay;
	collsionKind[EnumTag(SPHERE, CAPSULE,SHAPE_MAX)]		= &CollsionManager::CollsionSphereToCapsule;
	collList.clear();

	event = new CollsionEvent();
	InitSetPair();
	//DontDestroyOnSceneChange(true);
	//SetDrawOrder(0);
	gameControler = nullptr;
}

CollsionManager::~CollsionManager()
{
	delete event;
	event = nullptr;
}

void CollsionManager::Update()
{
	if (gameControler != nullptr && gameControler->GetStateNumber() == GameControler::PAUSE_SCENE) { //ポーズシーン中ならここは通さない
		return;
	}
	for (auto itr1 = collList.begin(); itr1 != collList.end();itr1++) {
		for (auto itr2 = itr1; itr2 != collList.end(); itr2++) {
			if (itr1 == itr2) {
				continue;
			}
			//当たり判定がどちらかが終了していたら無視する
			if ((*itr1)->GetFinish() || (*itr2)->GetFinish()) {
				continue;
			}
			CollsionInformation::Tag tag1 = (*itr1)->GetCollTag();
			CollsionInformation::Tag tag2 = (*itr2)->GetCollTag();
			//タグ同士の当たり判定に許可がでてなければ無視する。
			if (!CollsionInformation::IsCollPair(tag1, tag2)) {
				continue;
			}

			bool hit = false;
			//当たり判定の形を見て関数ポインタを代入
			if ((*itr1)->GetShape() > (*itr2)->GetShape()) {
				Function::swap(*itr1, *itr2);
			}
			CollsionKind kind = collsionKind[Function::EnumTag((*itr1)->GetShape(), (*itr2)->GetShape(),CollsionInformation::SHAPE_MAX)];
			if (kind == nullptr) {
				continue;
			}
			Pushback resolver = Pushback();
			VECTOR3 hitPos = VZero;
			hit = (this->*kind)((*itr1), (*itr2),resolver,hitPos);
			if (!hit) {
				continue;
			}
			if ((*itr1)->GetOneColl()) {
				(*itr1)->CollsionFinish();
			}
			if ((*itr2)->GetOneColl()) {
				(*itr2)->CollsionFinish();
			}
			event->Event((*itr1), (*itr2),resolver, hitPos);
		}
	}
}

void CollsionManager::Draw()
{
	/*for (ColliderBase* coll : collList) {
		coll->Draw();
	}*/
}

void CollsionManager::AddPush(ColliderBase* obj)
{
	collList.emplace_back(obj);
}

void CollsionManager::RemoveCollList(ColliderBase* obj)
{
	for (auto itr = collList.begin(); itr != collList.end();) {
		if (*itr == obj) {
			collList.erase(itr);
			return;
		}
		else {
			itr++;
		}
	}
}

void CollsionManager::FindGameControler()
{
	gameControler = FindGameObject<GameControler>();
}

bool CollsionManager::CollsionSphereToSphere(ColliderBase* col1, ColliderBase* col2, Pushback& resolver,VECTOR3& _hitPos)
{
	Transform* trans1 = col1->GetTransform();
	Transform* trans2 = col2->GetTransform();

	float dist = VSize(VSub(trans1->WorldTransform().position, trans2->WorldTransform().position));

	if (dist <= trans1->scale.x + trans2->scale.x) {
		return true;
	}

	return false;
}

bool CollsionManager::CollsionModelToRay(ColliderBase* col1, ColliderBase* col2, Pushback& resolver, VECTOR3& _hitPos)
{
	Transform* modelTransform = col1->GetTransform();
	Transform* rayStartTrans = col2->GetTransform();
	Transform* rayEndTrans = dynamic_cast<RayCollider*>(col2)->GetEndTransform();

	VECTOR3 startPos = rayStartTrans->WorldTransform().position;
	VECTOR3 endPos = rayEndTrans->WorldTransform().position;

	auto result = MV1CollCheck_Line(dynamic_cast<ModelCollider*>(col1)->GetModel(), -1, startPos, endPos);
	Physics* p = col2->GetObj()->Component()->GetComponent<Physics>();

	if (result.HitFlag != 0)
	{
		/*if (col2->GetCollTag() == CollsionInformation::SHADOW)
		{
			int a = 0;
		}*/
		VECTOR3 normal = result.Normal;
		VECTOR3 rayVec = endPos - startPos;
		float rayLength = VSize(rayVec);
		float hitDist = VSize(result.HitPosition - startPos);
		float penetration = rayLength - hitDist;
		if (penetration < 0.0f) penetration = 0.0f;

		resolver.AddPush(normal, penetration, CollsionInformation::Shape::RAY, result.HitPosition);
		resolver.Apply(col2->GetObj()->GetTransform(), p, true, 50.0f * Time::DeltaTimeRate());

		if (p != nullptr)
		{
			bool grounded = resolver.IsGrounded(0.5f);
			p->SetGround(grounded);   // ← ヒット時のみ ground を更新
		}

		if (col2->GetCollTag() == CollsionInformation::SHADOW)
		{
			VECTOR3 push = startPos - result.HitPosition;
			col2->GetBaseObject()->Component()->GetComponent<Shadow>()->ChangeScale(push, result.HitPosition);
		}

		return true;
	}
	else
	{
		if (p != nullptr)
		{
			p->SetGround(false);
		}
		return false;
	}
	return true;
}

bool CollsionManager::CollsionSphereToModel(ColliderBase* col1, ColliderBase* col2, Pushback& resolver,VECTOR3& _hitPos)
{
	Transform* trans1 = col1->GetTransform();
	Transform* trans2 = col2->GetTransform();



	VECTOR3 pos = trans1->WorldTransform().position + VECTOR3(0,100.0f,0);
	float raius = col1->GetRadius();
	Physics* phy = col1->GetObj()->Component()->GetComponent<Physics>();
	//MV1RefreshCollInfo(dynamic_cast<ModelCollider*>(col1)->GetModel(), -1);
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(dynamic_cast<ModelCollider*>(col2)->GetModel(), -1, pos, raius);
	VECTOR3 ret = VZero;

	//Pushback resolver;
	if (result.HitNum <= 0) {
		MV1CollResultPolyDimTerminate(result);
		return false;
	}

	for (int i = 0; i < result.HitNum; i++) {
		auto& pol = result.Dim[i];

		VECTOR3 centerToPoly = pos - pol.Position[i];
		float penetration = VDot(pol.Normal, centerToPoly);

		resolver.AddPush(pol.Normal, penetration,CollsionInformation::SPHERE, pol.HitPosition);
	}

	// 解決
	resolver.Apply(col1->GetObj()->GetTransform(), phy,true,20.0f);


	MV1CollResultPolyDimTerminate(result);

	return true;

}

bool CollsionManager::CollsionSphereToDount(ColliderBase* col1, ColliderBase* col2, Pushback& resolver,VECTOR3& _hitPos)
{
	Transform* trans1 = col1->GetTransform();
	Transform* trans2 = col2->GetTransform();
	float outRadius = dynamic_cast<DountCollider*>(col2)->GetOutRadius();

	float dist = VSize(VSub(trans1->WorldTransform().position, trans2->WorldTransform().position));
	//内側の円の当たり判定に当たっていないかつ外側の円の当たり判定に当たっているとき
	if (dist <= trans1->scale.x + outRadius) {
		if (dist >= trans1->scale.x + trans2->scale.x) {
			return true;
		}
	}

	return false;
}

bool CollsionManager::CollsionAABBToRay(ColliderBase* col1, ColliderBase* col2, Pushback& resolver,VECTOR3& _hitPos)
{
	Transform* rayStartTrans = col1->GetTransform();  // レイの始点
	Transform* rayEndTrans = dynamic_cast<RayCollider*>(col1)->GetEndTransform(); // レイの終点

	// レイの開始点と終了点(ワールド座標)
	VECTOR3 startPos = rayStartTrans->WorldTransform().position;
	VECTOR3 endPos = rayEndTrans->WorldTransform().position;

	VECTOR3 dir = endPos - startPos; // 方向ベクトル
	dir = dir.Normalize();

	AABBCollider::AABBInfo box = dynamic_cast<AABBCollider*>(col2)->GetAABBInfo();

	float tMin = 0.0f;
	float tMax = 20000.0f;

	const int VECTOR_MAX = 3; //X,Y,Zの3つ

	for (int i = 0; i < VECTOR_MAX; ++i)
	{
		float o = ((float*)&startPos)[i];
		float d = ((float*)&dir)[i];
		float min = ((float*)&box.boxMin)[i];
		float max = ((float*)&box.boxMax)[i];

		if (fabs(d) < 0.000001f)
		{
			// レイがこの軸に平行
			if (o < min || o > max)
				return false;
		}
		else
		{
			float inv = 1.0f / d;
			float t1 = (min - o) * inv;
			float t2 = (max - o) * inv;

			if (t1 > t2)
				std::swap(t1, t2);

			tMin = max(tMin, t1);
			tMax = min(tMax, t2);

			if (tMin > tMax)
				return false;
		}
	}

	if (tMax < 0.0f)
		return false;

	float hitT = tMin;

	//もしレイの始点が箱の中にある場合
	if (hitT < 0.0f)
		hitT = tMax;

	_hitPos = startPos + dir * hitT;
	//Debug::DebugLog("AABBHit");

	return true;
}

VECTOR3 ClosestPointOnSegment(VECTOR3& p, VECTOR3& a, VECTOR3& b)
{
	VECTOR3 ab = b - a;
	float t = VDot(p - a, ab) / ab.SquareSize();

	// 0～1にクランプ（線分内に収める）
	t = max(0.0f, min(1.0f, t));

	VECTOR3 dist = ab * t;

	return a + dist;
}

bool CollsionManager::CollsionSphereToCapsule(ColliderBase* col1, ColliderBase* col2, Pushback& resolver, VECTOR3& _hitPos)
{
	CapsuleCollider* capsule = dynamic_cast<CapsuleCollider*>(col2);
	Transform* sphereTransform = col1->GetTransform();
	Transform* capsuleStartTransform = col2->GetTransform();
	Transform* capsuleTransformEnd = capsule->CapselEndTransform();

	VECTOR3 startPos = VZero;
	VECTOR3 endPos = VZero;
	if (capsule->GetMultMatrix()) {
		startPos = capsuleStartTransform->position;
		endPos = capsuleTransformEnd->position;
	}
	else {
		startPos = capsuleStartTransform->WorldTransform().position;
		endPos = capsuleTransformEnd->WorldTransform().position;
	}
	
	
	float capselRadous = capsule->GetRadius();
	VECTOR3 spherePosition = sphereTransform->WorldTransform().position;

	//円の中心点からカプセルの中の一番近い点の算出
	VECTOR3 closest = ClosestPointOnSegment(spherePosition, startPos, endPos);

	float distSq = VSquareSize(sphereTransform->WorldTransform().position - closest);
	float r = sphereTransform->WorldTransform().scale.x + capselRadous;

	bool result = distSq <= r * r;

	return result;
}
