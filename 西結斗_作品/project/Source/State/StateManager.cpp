#include "StateManager.h"
#include "../GameControler/GameControler.h"

StateManager::StateManager()
{
	state = nullptr;
	nodeBaseNum = -1;
	nodeName = "";
	nodeWindow = ed::CreateEditor();
	currentId = StateID::STATE_MAX;
	nextId = StateID::STATE_MAX;
	stateNoChange = false;
	debugId = 20;
	tag = Function::GetClassNameC<StateManager>();
	beforeId = StateID::STATE_MAX;
	newLinkIdNum = 0;

}

StateManager::~StateManager()
{
	//for (auto& t : stateInfo) {
	//	//t.second.reset();
	//	//t.second = nullptr;
	//}
	state = nullptr;
	stateInfo.clear();
	ed::DestroyEditor(nodeWindow);
	//Debug::DebugLogPrintf(Debug::printfString("%s : size = %d", nodeName.c_str(), state.use_count()));
}

void StateManager::Update()
{
	if (currentId != nextId) { //ステートの変更の要請があったら変更する
		currentId = nextId;
		Change(currentId);
	}
	if (state != nullptr) {
		state->Update();
	}
}

void StateManager::Draw() 
{
	state->Draw();
}

void StateManager::StartState(StateID::State_ID _id)
{
	std::string id = StateID::GetID(_id);
	//Debug::DebugOutPutPrintf("idNumber : %d : idString : %s", _id, id.c_str());
	state = stateInfo[id];
	state->Start();
}

void StateManager::ChangeState(StateID::State_ID _id)
{
	if (obj->GetTag() == "Boss") {
		int a = 0;
	}
	if (currentId == _id) {
		return;
	}
	nextId = _id;
}

void StateManager::NowChangeState(StateID::State_ID _id)
{
	nextId = _id;
	currentId = _id;
	Change(currentId);
}

void StateManager::NodeDrawReady()
{
	for (auto s : stateInfo) {
		idName.emplace_back(s.second->GetID());
	}
}

void StateManager::LoadState()
{
	CsvReader csvReader;

	csvReader.Load(fileName);

	for (int r = 0; r < csvReader.GetLines(); r++) {
		if (r == csvReader.GetLines() - 1) {
			newLinkIdNum = std::stoi(csvReader.GetString(r, 0));
		}
		else {
			Link links;
			for (int c = 0; c < csvReader.GetColums(r); c++) {
				std::string string;
				switch (c)
				{
				case 0:
					links.input = std::stoi(csvReader.GetString(r, c));
					break;
				case 1:
					links.output = std::stoi(csvReader.GetString(r, c));
					break;
				case 2:
					links.id = std::stoi(csvReader.GetString(r, c));
					break;
				default:
					break;
				}
			}
			link.emplace_back(links);
		}

	}
}

void StateManager::SaveState()
{
	CsvReader csvReader;

	std::vector<std::vector<std::string>> data;

	for (const Link& l : link) {
		std::vector<std::string> row;
		row.emplace_back(std::to_string(l.input.Get()));
		row.emplace_back(std::to_string(l.output.Get()));
		row.emplace_back(std::to_string(l.id.Get()));
		data.emplace_back(row);
	}
	std::vector<std::string> num;
	num.emplace_back(std::to_string(newLinkIdNum));
	data.emplace_back(num);

	csvReader.SaveAll(fileName, data);
}

void StateManager::StateNodeDraw()
{
	SetCurrentEditor(nodeWindow);
	std::string nName = nodeName + "StateNode";
	ed::Begin(nName.c_str());
	for (int i = 0; i < idName.size(); i++) {

		if (state->GetID() == idName[i]) {
			ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(255, 0, 0));
		}
		int baseId = i * nodeBaseNum;
		int nodeId = baseId;
		int pinInputId = baseId + 1;
		int pinOutputId = baseId + 2;
		ed::BeginNode(nodeId);

		std::string string = StateID::GetID(idName[i]);
		std::string start = string + "Start";
		std::string end = string + "End";
		ImGui::Text(string.c_str());
		ed::BeginPin(pinInputId, ed::PinKind::Input);
		ImGui::Text("In");
		ed::EndPin();
		ed::BeginPin(pinOutputId, ed::PinKind::Output);
		ImGui::Text("Out");
		ed::EndPin();
		ed::EndNode();
		if (state->GetID() == idName[i]) {
			ed::PopStyleColor();
		}
	}

	//生成処理
	if (ed::BeginCreate()) {
		ed::PinId startPinId, finishPinId;
		Debug::DebugLog("delete");
		if (ed::QueryNewLink(&startPinId, &finishPinId)) {
			if (ed::AcceptNewItem()) {
				Debug::DebugLog("newLink");
				ed::LinkId newLinkId = ed::LinkId(newLinkIdNum++);
				link.emplace_back(Link(finishPinId, startPinId, newLinkId));
				ed::Link(newLinkId, startPinId, finishPinId);
			}
		}
	}
	ed::EndCreate();

	ed::LinkId popLink = ed::GetHoveredLink();
	std::string popName = nodeName + "LinkPopMenu";
	if (popLink && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
		keepLink = popLink; // 右クリックしたリンクを保存
		Debug::DebugLog(std::to_string(keepLink.Get()));
		ed::Suspend();
		ImGui::OpenPopup(popName.c_str());
		ed::Resume();

	}

	ed::Suspend();
	if (ImGui::BeginPopup(popName.c_str())) {
		if (ImGui::MenuItem("Delete Link")) {
			ed::LinkId deleteId = keepLink.Get();
			Debug::DebugLog("Deleting link ID: " + std::to_string(deleteId.Get()));
			//削除処理
			for (auto e = link.begin(); e != link.end();) {
				if ((e)->id == deleteId) {
					e = link.erase(e);
					break;
				}
				e++;
			}
			keepLink = ed::LinkId();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	ed::Resume();



	// 削除処理の後にリンクを描画する
	for (Link& l : link) {
		ed::Link(l.id, l.output, l.input);
	}

	ed::NodeId doubleClick = ed::GetDoubleClickedNode();
	if (doubleClick) {
		int nodeId = static_cast<int>(doubleClick.Get());
		int index = nodeId / nodeBaseNum;
		ChangeState(idName[index]);
	}
	ed::End();
}

void StateManager::ImguiDraw()
{
	ImGui::Text("%s", state->GetString().c_str());
	if (ImGui::TreeNode("stateChange")) {
		for (auto& t : stateInfo) {
			std::string name = t.second->GetString();
			if (ImGui::Button(name.c_str())) {
				ChangeState(t.second->GetID());
			}
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	for (auto& t : stateInfo) {
		std::string name = t.second->GetString() + "__Window";
		if (ImGui::TreeNode(name.c_str())) {
			t.second->StateImguiDraw();
			ImGui::TreePop();
		}
	}
}

void StateManager::Change(StateID::State_ID _id)
{
	if (stateNoChange) {
		return;
	}
	state->Finish();
	beforeId = _id;
	StartState(_id);
}
