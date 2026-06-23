#include "Hierarchy.h"
#include "../../../ImGui/imgui.h"
#include "../Transform/Transform.h"
#include <algorithm>
#include <iostream>
#include "../Color/Color.h"
#include "../../Common/Debug/Debug.h"
#include "../ComponentManager.h"
#include "../../Common/InputManager/InputManager.h"

#include "../../../ImGui/imgui_node_editor.h"
#include "../../Common/Transitor/FadeTransitor.h"
#include "../../PlayScene.h"
#include "../../Camera/Camera.h"

//namespace ed = ax::NodeEditor;
//static ed::EditorContext* g_Context = nullptr;
//void ShowNodeEditor()
//{
//	if (!g_Context)
//		g_Context = ed::CreateEditor();
//	ed::SetCurrentEditor(g_Context);
//	ed::Begin("My Node Editor");
//	ed::BeginNode(1);
//	ImGui::Text("Node A");
//	ed::BeginPin(2, ed::PinKind::Output);
//	ImGui::Text("Out");
//	ed::EndPin();
//	ed::EndNode();
//	ed::BeginNode(3);
//	ImGui::Text("Node B");
//	ed::BeginPin(4, ed::PinKind::Input);
//	ImGui::Text("In");
//	ed::EndPin();
//	ed::EndNode();
//	ed::Link(5, 2, 4); // ID:5, Pin:2 -> Pin:4
//	ed::End();
//	ed::SetCurrentEditor(nullptr);
//}

Hierachy::Hierachy()
{
	//transform = nullptr;
#ifdef _DEBUG
	windowMode = true;
#else
	windowMode = false;

#endif // _DEBUG
	fade = FindGameObject<FadeTransitor>();
	color = nullptr;
	selectId = -1;
	selectObj = nullptr;
}

Hierachy::~Hierachy()
{
	selectObj = nullptr;
}

void Hierachy::Update()
{
	/*if (!fade->IsEnd()) {
		return;
	}*/
#ifdef _DEBUG

	if (InputManager::GetInstance()->KeyInputDown("debugChange")) {
		windowMode = !windowMode; //デバックウィンドウの出し入れ
	}
	//デバックモードのみで作成
	if (Debug::GetDebugMode()) {
		bool open = ImGui::Begin("Hierachy",&open, ImGuiWindowFlags_UnsavedDocument);
		float t = static_cast<float>(ImGui::GetTime());
		float alpha = 0.8f + 0.2f * sin(t * 4.0f); // 点滅エフェクト
		ImVec4 flashingColor = ImVec4(1.0f, 0.0f, 0.2f, alpha); //光のエフェクト
		ImGui::PushStyleColor(ImGuiCol_Button, flashingColor);
		ImGui::PushStyleColor(ImGuiCol_Header, flashingColor);
		//Debug::DebugUpdate();
		//ImGui::Separator();
		//オブジェクトのデバックウィンドウ
		for (BaseObject* obj : debugOutPutObjectList) {
			if (obj->GetParent() == nullptr) {
				DrawHierarchy(obj); //親がいないオブジェクトのみ描画
			}
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::End();
		//二つ目のウィンドウを表示
		InspectorDraw();
	}
	PlayScene* playscene = GetScene<PlayScene>();
	if (playscene != nullptr && playscene->GetMode() == PlayScene::CAMERA_EDITOR) {
		camera->CameraEditor();
	}
#endif // _DEBUG
	
}
	

void Hierachy::Draw()
{
}

void Hierachy::Start(std::string _name, BaseObject* _obj)
{
	//transform = _obj->GetTransform();
	color = _obj->Component()->AddComponent<Color>();
	debugOutPutObjectList.emplace_back(_obj);
	//name = _obj->GetTag();
}

void Hierachy::DrawHierarchy(BaseObject* _obj)
{
	//登録した名前を木の名前にして表示
	ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (selectId == _obj->GetId())
		baseFlags |= ImGuiTreeNodeFlags_Selected;

	bool open = ImGui::TreeNodeEx((void*)(intptr_t)_obj->GetId(), baseFlags, "%s", _obj->GetTag().c_str());
	//クリックをしたらインスペクターに表示
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
		selectId = _obj->GetId();
		selectObj = _obj;
		Debug::DebugLog(_obj->GetTag());
	}
	//開いたら子供のオブジェクトを表示
	if (open) {
		for (BaseObject* child : _obj->GetChildren()) {
			DrawHierarchy(child);
		}
		ImGui::TreePop();  // ここを忘れずに！
	}
}

void Hierachy::InspectorDraw()
{
	ImGui::Begin("inspector");
	//なにも選ばれてなかったら描画をしない。
	if (selectId == -1) {
		ImGui::Text("No object selected.");
	}
	else {
		BaseObject* obj = selectObj;

		if (obj) {
			ImGui::Text("Name: %s", obj->GetTag().c_str());
			ImGui::Separator();
			// 持っている全コンポーネントのUIを表示
			for (auto com : obj->Component()->GetAllComponent()) {
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
				bool open = ImGui::TreeNodeEx(com->GetTag().c_str(), flags);
				if (open) {
					com->ImguiDraw();// 子ノードとしてコンポーネントUIを表示
					ImGui::TreePop();
				}
			}
			ImGui::Separator();
		}
	}
	ImGui::End();
}

//void Hierachy::TransformDraw(BaseObject* _obj)
//{
//	//トランスフォームを描画
//	std::string smallName = _obj->GetTag();
//	std::transform(smallName.begin(), smallName.end(), smallName.begin(), [](char c) { return std::tolower(c); });
//	std::string pName = smallName + "_position";
//	std::string rName = smallName + "_rotation";
//	std::string sName = smallName + "_scale";
//	
//	
//
//}

void Hierachy::RemoveHierachy(BaseObject* _obj)
{
	for (auto itr = debugOutPutObjectList.begin(); itr != debugOutPutObjectList.end(); itr++) {
		if (*itr == _obj) {
			debugOutPutObjectList.erase(itr);
			return;
		}
	}
}

void Hierachy::AllDeleteList()
{
	debugOutPutObjectList.clear();
}

void Hierachy::SetCameraEditor(Camera* _camera)
{
	camera = _camera;
}
