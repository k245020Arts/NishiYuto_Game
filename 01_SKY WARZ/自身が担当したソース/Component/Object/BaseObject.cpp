#include "BaseObject.h"
#include "../Transform/Transform.h"
#include "../ComponentManager.h"
#include "../../../ImGui/imgui.h"
#include "../Color/Color.h"
#include "../Hierarchy/Hierarchy.h"
#include "../../Common/Transitor/TransitorManager.h"
#include "../../Common/Memory/MemoryCount.h"
#include "../../Common/Debug/Debug.h"
#include "../../PlayScene.h"
#include "../../GameControler/GameControler.h"

int BaseObject::idPlus = 0;

BaseObject::BaseObject()
{
	objectTimeRate = 1.0f;
	baseTimeSpeed = 1.0f;
	componentManager = nullptr;
	transform = nullptr;
	transitor = nullptr;
	//Debug::DebugLog(std::to_string(GetTrackedMemoryUsage()));
	id = idPlus++;
	parent = nullptr;
	transformParent = true;
	sleep = false;
	draw = true;
	gameControler = FindGameObject<GameControler>();
}

BaseObject::~BaseObject()
{
	/*if (GetTag() == "PLAYER") {
		int a = 0;
	}*/
	delete componentManager;
	// Hierachyが存在する場合のみ呼ぶ（Release中は存在しない）
	Hierachy* h = FindGameObject<Hierachy>();
	if (h != nullptr)
	{
		h->RemoveHierachy(this);
	}
	RemoveParent();
	DeleteAllChildren();
}

void BaseObject::Update()
{
	//フェード中なら動かさない
	/*if (!transitor->IsTransitor()) {
		return;
	}*/

	if (gameControler != nullptr && GameControler::PAUSE_SCENE == gameControler->GetStateNumber()) { //ポーズ画面なら通さない
		return;
	}

	if (sleep) { //sleepモード中ならUpdateを通さない
		return;
	}

	PlayScene* playscene = GetScene<PlayScene>();
	//カメラ以外のオブジェクト
	if (GetTag() != "CAMERA_OBJ") {
		//カメラEditorモードならUpdateを止める
		if (playscene != nullptr && playscene->GetMode() == PlayScene::CAMERA_EDITOR) {
			return;
		}
	}

	

	//コンポーネントで回している
	componentManager->Update();

	
	
	//子供のオブジェクトをまわす
	for (auto itr = children.begin(); itr != children.end(); )
	{
		BaseObject* child = *itr;
		//削除申請がなければUpdateを回す
		if (!child->DestroyRequested())
		{
			child->Update();
			++itr;
		}
		else
		{
			//Transformの親子関係が成立されてるなら子供を切る
			if (transformParent && transform != nullptr) {
				transform->RemoveChild((*itr)->GetTransform());
			}
			delete child;
			itr = children.erase(itr);
		}
	}
}

void BaseObject::Draw()
{
	//描画
	if (draw) {
		componentManager->Draw();
	}
	for (BaseObject* child : children) {
		child->Draw();
	}
}

void BaseObject::Init(std::string name)
{
	componentManager = new ComponentManager();

	componentManager->Init(this);
	SetTag(name);
	//デバックウィンドウはここで登録している
	Hierachy* h = FindGameObject<Hierachy>();
	if (h != nullptr)
	{
		h->Start(name, this);
	}
	else {
		Color* color = Component()->AddComponent<Color>();
	}
	
	transitor = FindGameObject<TransitorManager>();
}

void BaseObject::AddChild(BaseObject* child)
{
	AddChild(child, true);
}

void BaseObject::AddChild(BaseObject* child, bool _transformParent)
{
	transformParent = _transformParent;
	if (transformParent) {
		if (child->parent) {
			child->parent->RemoveChild(child); // 元の親から外す
		}
		transform->AddChild(child->GetTransform());
	}
	child->parent = this;
	ObjectManager::Pop(static_cast<GameObject*>(child)); //子供はObjectの中で管理したいのでObjectManagerのリストから削除
	children.emplace_back(child);
}

void BaseObject::RemoveChild(BaseObject* child) {
	for (auto itr = children.begin(); itr != children.end();) {
		if (*itr == child) {
			BaseObject* obj = *itr;
			//Transformの親子関係が成立されてるなら子供を切る
			if (transformParent && transform != nullptr) {
				transform->RemoveChild(obj->GetTransform());
			}
			(*itr)->parent = nullptr;
			itr = children.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void BaseObject::DeleteChild(BaseObject* child) {
	for (auto itr = children.begin(); itr != children.end();) {
		//一致したら削除
		if (*itr == child) {
			BaseObject* obj = *itr;
			delete obj;
			itr = children.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void BaseObject::DeleteAllChildren() {
	for (auto itr = children.begin(); itr != children.end();) {
		BaseObject* obj = *itr;
		itr = children.erase(itr);
		delete obj;
		
	}
}

void BaseObject::RemoveParent() {

	if (parent) {//親がいるなら親の子供リストから削除
		if (!this->DestroyRequested()) { //削除要請があったらUpdateで削除するので削除しない
			parent->RemoveChild(this);
		}
		parent = nullptr;
	}
}