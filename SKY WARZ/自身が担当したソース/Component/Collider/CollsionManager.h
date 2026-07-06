#pragma once
#include "../../../Library/GameObject.h"
#include <array>
#include "CollsionInformation.h"

class Object3D;
class ColliderBase;
class CollsionEvent;
class GameControler;

//当たり判定の総まとめをするクラス。

class CollsionManager : public GameObject
{
public:
	CollsionManager();
	~CollsionManager();
	void Update()override;
	void Draw()override;
	/// <summary>
	/// 当たり判定リストに追加
	/// </summary>
	/// <param name="obj"></param>
	void AddPush(ColliderBase* obj);
	/// <summary>
	/// 当たり判定リストを削除
	/// </summary>
	/// <param name="obj"></param>
	void RemoveCollList(ColliderBase* obj);
	/// <summary>
	/// ゲームコントローラークラスのFindを行う;
	/// </summary>
	void FindGameControler();

private:
	std::list<ColliderBase*> collList;

	
	typedef bool (CollsionManager::*CollsionKind)(ColliderBase* col1, ColliderBase* col2, Pushback& resolver, VECTOR3& _hitPos);
	std::array<CollsionKind, CollsionInformation::COLLSION_KIND_MAX> collsionKind;

	bool CollsionSphereToSphere(ColliderBase* col1, ColliderBase* col2, Pushback& resolver, VECTOR3& _hitPos);
	bool CollsionModelToRay(ColliderBase* col1, ColliderBase* col2, Pushback& resolver, VECTOR3& _hitPos);
	bool CollsionSphereToModel(ColliderBase* col1, ColliderBase* col2, Pushback& resolver, VECTOR3& _hitPos);
	bool CollsionSphereToDount(ColliderBase* col1, ColliderBase* col2, Pushback& resolver, VECTOR3& _hitPos);
	bool CollsionAABBToRay(ColliderBase* col1, ColliderBase* col2, Pushback& resolver, VECTOR3& _hitPos);
	bool CollsionSphereToCapsule(ColliderBase* col1, ColliderBase* col2, Pushback& resolver, VECTOR3& _hitPos);
	CollsionEvent* event;

	GameControler* gameControler;
};