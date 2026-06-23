#include "BossRockManager.h"
#include "BossRockBase.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Component/Shadow/Shadow.h"
#include "../../../Enemy/Boss/Boss.h"
#include "../../../State/StateManager.h"
#include "../../../Common/Easing.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../../../Enemy/Boss/BossState/BossStatus.h"
#include "../../../Camera/Camera.h"
#include "../../../Common/Random.h"
#include "../../MeshRenderer/MeshRenderer.h"
#include "../../../Common/ResourceLoader.h"
#include "../../../Common/Effect/EffectManager.h"
#include "../../../Weapon/CharaWeapon.h"
#include "../../../Common/Debug/DebugNew.h"
#include "../../MeshRenderer2D/MeshRenderer2D.h"

BossRockManager::BossRockManager() :  BossRockManager(nullptr)
{
}

BossRockManager::BossRockManager(Boss* _boss)
{
	if (_boss == nullptr) {
		my_error_assert("ボスのポインターが入っていません");
	}
	
	boss = _boss;
	rocks.clear();

	JsonReader json;
	json.Load("data/json/ThrowObject/ThrowObjects.json");
	auto j = json.Data();

	modelNum = static_cast<int>(ID::THROW_OBJECT_1);
	effectNum = static_cast<int>(Effect_ID::THROW_EFFECT1);
	for (const auto& elem : j["ThrowObjectsData"])
	{
		BossThrowObjectData data = elem.get<BossThrowObjectData>();

		if (data.id.empty()) {
			continue;
		}

		throwObjectsData[data.id] = data;
		if (data.isEffect) {
			LoadEffect(data);
		}
		else {
			LoadModel(data);
		}
	}
}

BossRockManager::~BossRockManager()
{
	rocks.clear();
	throwObjectsData.clear();
	if (boss != nullptr) {
		boss = nullptr;
	}
}

void BossRockManager::Update()
{


}

void BossRockManager::Draw()
{

}

void BossRockManager::CreateThrowEmptyObject(const std::vector<EnemyAttackBase::ThrowObjectAttackData>& _data)
{
	for (auto data : _data) { //ボスの中に入っている投擲物データのすべてを初期化状態にしておく
		Object3D* throwObject = new Object3D();
		throwObject->Init(Transform(), "bossThrowObject");
		BossRockBase* bossRock = throwObject->Component()->AddComponent<BossRockBase>();
		bossRock->SetThrowParam(data);
		bossRock->SetPlayerTransform(boss->enemyBaseComponent.playerObj->GetTransform());
		boss->obj->AddChild(throwObject, false);
	}
}

void BossRockManager::AppearThrowObject(BossRockBase* _rock, int _index, int _total, float _rotateAngle)
{
	//本格的に登場
	Object3D* throwObject = static_cast<Object3D*>(_rock->GetBaseObject());
	EnemyAttackBase::ThrowObjectAttackData _data = _rock->GetThrowObjectsData();
	SetRockComponent(throwObject, _data.baseGravity, _data.baseFirction, _data); //投擲物データに必要な物を生成
	if (_data.throwToFallToPlayer || _data.throwToFall) { //プレイヤーに向かって落ちるか、またはランダムに落ちるか
		VECTOR3 center = VZero;
		VECTOR3 dir = VZero;
		if (_data.throwToFallToPlayer) { //プレイヤーに向かって落ちるならプレイヤーの予測線をもとに計算して位置を算出
			auto playerObj = boss->enemyBaseComponent.playerObj;
			VECTOR3 playerPos = playerObj->GetTransform()->position;

			Physics* playerPhy = playerObj->Component()->GetComponent<Physics>();

			VECTOR3 playerVel = VZero;
			if (playerPhy != nullptr) {
				playerVel = playerPhy->GetVelocity();
			}

			float futureTime = Random::GetFloat(0.3f, 0.8f);
			//予測値	
			VECTOR3 futurePos = playerPos + playerVel * futureTime;

			center = Easing::Lerp(playerPos, futurePos, 0.9f);
		}
		else {
			center = _data.thorwStartPos;
		}
		dir.y = 0;
		dir.Normalize();
		Camera* cam = boss->enemyBaseComponent.camera;
		VECTOR3 camForward = cam->GetCameraTransform()->Forward();
		VECTOR3 camRight = camForward * MGetRotY(90.0f * DegToRad);

		float limitAngle = 180.0f * DegToRad * 0.45f;

		float baseAngle = (2.0f * 180.0f * DegToRad / _total) * _index;
		float ramdom = Random::GetFloat(-0.25f, 0.25f);

		float finalAngle = std::clamp(baseAngle + _rotateAngle + ramdom, -limitAngle, limitAngle);

		dir = camForward * cosf(finalAngle) + camRight * sinf(finalAngle);

		float wave = sinf(_index * 0.8f) * 350.0f;
		float radius = 1000.0f + wave + Random::GetFloat(-200.0f, 200.0f);
		VECTOR3 startPos = center + dir * radius;
		if (_data.randomHeight) {
			//VECTOR3 startPos = center + dir * radius;
			//8000,12000
			float offset = Random::GetFloat(_data.minHeight, _data.maxHeight);
			startPos.y = center.y + offset;
		}


		VECTOR3 rotation = VZero;
		//rotation.z = 45.0f * DegToRad;
		throwObject->GetTransform()->position = startPos;
		throwObject->GetTransform()->rotation = rotation;

		VECTOR3 gravity = VECTOR3(0, _data.throwFallGravity, 0);
		if (_data.randomSpeed) {
			//1500,3000
			gravity.y = Random::GetFloat(_data.minSpeed, _data.maxSpeed);
			gravity.y *= -1.0f;
		}

		Physics* phy = throwObject->Component()->GetComponent<Physics>();
		phy->SetGravity(gravity);
	}
	else if (_data.freeDir) { //自由に飛ぶ設定にしているなら初期値を設定しておく
		throwObject->GetTransform()->position = _data.thorwStartPos;
	}
	else if (_data.armThrow) { //腕で投げる設定にしてあるなら初期化をする
		Physics* phy = throwObject->Component()->GetComponent<Physics>();
		phy->Start(VZero, VZero);
	}
	if (_data.randomBlast) { //ランダムで爆発をする設定があるならランダムで爆発
		bool doBlast = true;
		//0.1
		if (Random::GetFloat(0.0f, 1.0f) < _data.randomBlastRate)
		{
			doBlast = false;
		}

		BossRockBase* rockComp = throwObject->Component()->GetComponent<BossRockBase>();
		rockComp->SetCanBlast(doBlast);
	}
	/*else if (data.blastCan) {
		BossRockBase* rockComp = throwObject->Component()->GetComponent<BossRockBase>();
		rockComp->SetCanBlast(true);
	}*/
	BossRockBase* rock = throwObject->Component()->GetComponent<BossRockBase>();
	CharaWeapon* chara = throwObject->Component()->GetComponent<CharaWeapon>();
	MATRIX* matrix = nullptr;
	if (chara != nullptr) {
		matrix = chara->GetMatrixPtr();
	}
	
	rock->Start(_data, matrix);
	
}

void BossRockManager::CreateThrowObject(const std::vector<EnemyAttackBase::ThrowObjectAttackData>& _data, int _index, int _total, float _rotateAngle)
{
	//空のオブジェクトは関係なく作成
	for (auto data : _data) {
		Object3D* throwObject = new Object3D();
		throwObject->Init(Transform(), "bossThrowObject");
		BossRockBase* bossRock = throwObject->Component()->AddComponent<BossRockBase>();
		bossRock->SetThrowParam(data);
		bossRock->SetPlayerTransform(boss->enemyBaseComponent.playerObj->GetTransform());
		boss->obj->AddChild(throwObject, false);
		AppearThrowObject(bossRock, _index, _total, _rotateAngle);
	}
}

VECTOR3 BossRockManager::GetPushCollSize(const EnemyAttackBase::ThrowObjectAttackData& _data)
{
	return throwObjectsData[_data.throwObjectID].pushTransform.scale;
}

void BossRockManager::PushList(BossRockBase* _obj)
{
	rocks.push_back(_obj);
}

void BossRockManager::RemoveList(BossRockBase* _obj)
{
	for (auto itr = rocks.begin(); itr != rocks.end();) {
		if (*itr == _obj) {
			rocks.erase(itr);
			return;
		}
		else {
			itr++;
		}
	}
}

bool BossRockManager::IsFreePos(const VECTOR3& _pos, float _minDist)
{
	for (auto rock : rocks) {
		VECTOR3 diff = rock->GetBaseObject()->GetTransform()->position - _pos;
		if (diff.Size() < _minDist) {
			return false;
		}
	}
	return true;
}

void BossRockManager::ShakeCamera()
{
	boss->enemyBaseComponent.camera->CameraPerspectiveShakeStart(2.0f, 0.1f);
}

void BossRockManager::DropRockStart()
{
	for (auto rock : rocks) {
		if (!rock->GetThrowObjectsData().playerAttackObjectDrop) {
			continue;
		}
		rock->DropObject();
	}
}

VECTOR3 BossRockManager::GetRockPos(const VECTOR3& _pos)
{
	VECTOR3 pos = VECTOR3(INFINITY, INFINITY, INFINITY);

	for (auto rock : rocks)
	{
		VECTOR3 vec = rock->GetBaseObject()->GetTransform()->position - _pos;
		if (pos.Size() > vec.Size())
			pos = rock->GetBaseObject()->GetTransform()->position;
	}

	return pos;
}

std::list<VECTOR3> BossRockManager::GetAllRockPos()
{
	std::list<VECTOR3> _pos;

	for (auto rock : rocks) {
		_pos.emplace_back(rock->GetBaseObject()->GetTransform()->position);
	}
		
	return _pos;
}

void BossRockManager::AddJsonData(BossThrowObjectData& _data)
{
	throwObjectsData[_data.id] = _data;

	JsonReader json;
	std::string fileName = "data/json/ThrowObject/ThrowObjects.json";

	nlohmann::json& root = json.Data();

	if (!root.contains("ThrowObjectsData")) {
		root["ThrowObjectsData"] = nlohmann::json::object();
	}
	for (auto& throwObject : throwObjectsData) {
		std::string key = throwObject.second.id;
		root["ThrowObjectsData"][key] = throwObject.second;
	}

	if (_data.isEffect) {
		LoadEffect(_data);
	}
	else {
		LoadModel(_data);
	}

	json.Save(fileName, root);

}

void BossRockManager::ChangeJsonData(const BossThrowObjectData& _data,const std::string& _oldKey,const std::string& _newKey)
{
	JsonReader json;
	std::string fileName = "data/json/ThrowObject/ThrowObjects.json";

	auto& table = throwObjectsData;

	if (table[_oldKey].isEffect) {
		ResourceLoad::DeleteData(ID::StringToID(table[_oldKey].modelName), false);
	}
	else {
		ResourceLoad::DeleteEffectData(Effect_ID::StringToID(table[_oldKey].modelName), false);
	}

	//キー変更
	if (_oldKey != _newKey)
	{
		table.erase(_oldKey);
	}


	//更新
	table[_newKey] = _data;

	if (table[_newKey].isEffect) {
		LoadEffect(table[_newKey]);
	}
	else {
		LoadModel(table[_newKey]);
	}

	nlohmann::json& root = json.Data();

	if (!root.contains("ThrowObjectsData")) {
		root["ThrowObjectsData"] = nlohmann::json::object();
	}
	for (auto& throwObject : throwObjectsData) {
		std::string key = throwObject.second.id;
		root["ThrowObjectsData"][key] = throwObject.second;
	}
	json.Save(fileName, root);
}

void BossRockManager::RockContorler(const EnemyAttackBase::BossAttackParam& _data, float _animFrame)
{
	for (auto rock : rocks) {
		EnemyAttackBase::ThrowObjectAttackData data = rock->GetThrowObjectsData();
		//投擲物が登場するタイミング
		if (_animFrame >= data.throwObjectApperaTime) {
			if (!rock->GetThrowObjectStart()) {
				boss->rockManager->AppearThrowObject(rock, 0, 0, 0.0f);
			}
		}
		//投げるの開始
		if (data.throwStartTime <= _animFrame) {
			rock->ThrowRockStart(boss->enemyBaseComponent.playerObj);
		}
		
		if (_animFrame <= data.capsuleEndPosExtendStartAnimFrame) {
			if (!data.capsuleEndPosExtendFromThrowStart) {
				rock->CapsuleColliderPosAddStart();
			}
		}
	}
}

void BossRockManager::AttackFinishDelete()
{
	for (auto itr = rocks.begin(); itr != rocks.end();) {
		EnemyAttackBase::ThrowObjectAttackData data = (*itr)->GetThrowObjectsData();
		if (data.attackFinishDelete) { //攻撃後に投擲物が消える設定にしていたら削除
			(*itr)->GetBaseObject()->DestroyMe();
		}
		itr++;
	}
}

void BossRockManager::DeleteEmptyObject()
{
	for (auto itr = rocks.begin(); itr != rocks.end();) {
		bool start = (*itr)->GetThrowObjectStart();
		if (!start) { //初期化状態のまま敵の攻撃が終わったらそのまま削除
			(*itr)->GetBaseObject()->DestroyMe();
		}
		itr++;
	}
}

void BossRockManager::BossDieDeleteObject(float _hp)
{
	if (_hp > 0.0f) {
		return; //ボスが死んでなかったら削除しない
	}
	//死んだら全削除
	for (auto itr = rocks.begin(); itr != rocks.end();) {
		(*itr)->GetBaseObject()->DestroyMe();
		itr++;
	}
}

void BossRockManager::SetRockComponent(Object3D* _base, const VECTOR3& _gravity, const VECTOR3& _fir, const EnemyAttackBase::ThrowObjectAttackData& _data)
{
	//_base->Init(Transform(),"bossThrowObject");
	//BossRockBase* bossRock = _base->Component()->AddComponent<BossRockBase>();
	Physics* phy = _base->Component()->AddComponent<Physics>();
	phy->Start(_gravity, _fir);
	//bossRock->StartCollAdd(CollsionInformation::BOSS_ROCK_ATTACK, Transform());;
	Shaker* shaker = _base->Component()->AddComponent<Shaker>();
	//bossRock->SetRockModel();
	if (throwObjectsData[_data.throwObjectID].isEffect) {
		
		EffectManager::GetInstance()->CreateEffekseer(Transform(), _base, Effect_ID::StringToID(throwObjectsData[_data.throwObjectID].modelName), 100.0f);
		//ResourceLoad::GetHandle(ID::StringToID(throwObjectsData[_data.throwObjectID].modelName)
		_base->GetTransform()->scale = throwObjectsData[_data.throwObjectID].modelTransform.scale;
	}
	else {
		if (_data.armSwordHand) {
			CharaWeapon* weapon = _base->Component()->AddComponent<CharaWeapon>();
			weapon->ObjectPointer(_base, _data.armFrameNum, ID::B_MODEL, ResourceLoad::GetHandle(ID::StringToID(throwObjectsData[_data.throwObjectID].modelName)));
			_base->GetTransform()->scale = throwObjectsData[_data.throwObjectID].modelTransform.scale;
		}
		else {
			MeshRenderer* mesh = _base->Component()->AddComponent<MeshRenderer>();
			mesh->ModelHandle(ResourceLoad::GetHandle(ID::StringToID(throwObjectsData[_data.throwObjectID].modelName)));
			_base->GetTransform()->scale = throwObjectsData[_data.throwObjectID].modelTransform.scale;
		}
		
	}
	MeshRenderer2D* yButton = _base->Component()->AddComponent<MeshRenderer2D>();
	yButton->SetTransform(Transform(VZero,VZero,VOne * 0.5f));
	yButton->TextureHandle(ResourceLoad::GetHandle(ID::Y_BUTTON), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F);
	yButton->SetDraw(false);
	MeshRenderer2D* bButton = _base->Component()->AddComponent<MeshRenderer2D>();
	bButton->SetTransform(Transform(VZero, VZero, VOne * 0.5f));
	bButton->TextureHandle(ResourceLoad::GetHandle(ID::B_BUTTON), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F);
	bButton->SetDraw(false);
	//Shadow* shadow = _base->Component()->AddComponent<Shadow>();
	/*Object3D* shadow = new Object3D();
	shadow->Init(Transform(VECTOR3(0.0f, -20.0f, 0.0f), VZero, VECTOR3(_base->GetTransform()->scale.x + 4.0f, 0.1f, _base->GetTransform()->scale.z + 4.0f)), "BossShadow");
	Shadow* s = shadow->Component()->AddComponent<Shadow>();
	s->Start();*/
	//boss->obj->AddChild(_base, false);
}

void BossRockManager::LoadEffect(BossThrowObjectData& _data)
{
	_data.modelData = ResourceLoad::LoadEffect(_data.modelName, ".efkefc", static_cast<Effect_ID::EFFECT_ID>(effectNum), 50.0f);
	//エフェクトのEnumの調整のために足す
	effectNum++;
}

void BossRockManager::LoadModel(BossThrowObjectData& _data)
{
	_data.modelData = ResourceLoad::LoadModel(_data.modelName, static_cast<ID::IDType>(modelNum));
	//モデルのEnumの調整のために足す
	modelNum++;
}

//void BossRockManager::SetRockComponent(BaseObject* _base, const VECTOR3& _gravity, const VECTOR3& _fir)
//{
//	
//}