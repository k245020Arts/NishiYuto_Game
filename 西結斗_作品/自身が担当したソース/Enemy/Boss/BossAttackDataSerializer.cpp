#include "BossAttackDataSerializer.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../../ImGui/imgui.h"
#include "BossState/AttackSorting.h"
#include <iostream>
#include "../../../Source/Common/FileSystemUtils/FileSystemUtils.h"
#include "../../Common/ResourceLoader.h"
#include "../../../Source/Component/EnemyAttackObject/BossRock/BossRockBase.h"

#define ANIM_FILE

BossAttackDataSerializer::BossAttackDataSerializer() : BossAttackDataSerializer(nullptr,nullptr,"")
{
}

BossAttackDataSerializer::BossAttackDataSerializer(std::shared_ptr<AttackSorting> _sort, Boss* _boss, std::string _bossName)
{
	windowMode = false;
	BossName = _bossName;
	sorting = _sort;
	attackParam = sorting->GetAttackParam();
	actions = sorting->GetActionParam();
	attackKeys.clear();
	//攻撃の数分文字列の準備
	for (auto& a : attackParam) {
		attackKeys.push_back(a.first);
	}
	bossAnim = nullptr;
	currentSelectAnimInfos = Animator::AnimFileInfo();


	std::string filePath = "data/model/animation";

	for (const auto& entry : std::filesystem::directory_iterator(filePath)) {
		// フォルダはスキップ
		if (entry.is_directory()) {
			continue;
		}
		//アニメーションのファイル名の取得
		std::string name = entry.path().stem().string();
		animFileName.push_back(name);
	}
	//ヌル文字を最後に足す
	for (char& c : newAnimFile) {
		c = '\0';
	}
	for (char& c : newAttackID) {
		c = '\0';
	}
	rockManager = nullptr;
	boss = _boss;
	isEffect = false;

	std::string soundPath = "data/sound";
	//サウンドデータのファイル名一覧取得
	for (const auto& entry : std::filesystem::directory_iterator(soundPath))
	{
		if (entry.is_directory())
		{
			continue;
		}
		//サウンドファイル名の取得
		std::string name = entry.path().stem().string();
		soundFileNames.push_back(name);
	}
}

BossAttackDataSerializer::~BossAttackDataSerializer()
{
}

void BossAttackDataSerializer::SetAnim(Animator* anim)
{
	bossAnim = anim;
}

void BossAttackDataSerializer::SetThrowManager(BossRockManager* _data)
{
	rockManager = _data;
	throwObjectsData = rockManager->GetThrowObjectsData();

	throwObjectKeys.clear();
	for (auto& [key, _] : throwObjectsData)
	{
		throwObjectKeys.push_back(key);
	}
	throwKeyReset = false;
}

void BossAttackDataSerializer::Update()
{
	if (InputManager::GetInstance()->KeyInputDown("BossParamWindow")) {
		windowMode = !windowMode;
		//パラメーター調整中は敵を止める
		if (windowMode) {
			boss->GetBaseObject()->SetObjectTimeRate(0.0f);
		}
		else {
			boss->GetBaseObject()->SetObjectTimeRate(1.0f);
		}
	}
	if (!windowMode) {
		return;
	}

	ImGui::Begin("BossAttackSerialize");

	//攻撃リスト作成
	static int currentIndex = 0;

	std::vector<const char*> items;
	for (auto& k : attackKeys) {
		items.push_back(k.c_str());
	}
	bool change = false;
	if (!items.empty()) {
		change = ImGui::Combo("AttackSelect", &currentIndex, items.data(), (int)items.size());
	}
	if (change) {
		std::string selectedID = attackKeys[currentIndex];
		auto& param = attackParam[selectedID];
		currentSelectAnimInfos = bossAnim->GetSelectFileInfo(param.animFileName);
	}
	//選択中の攻撃取得
	if (!attackKeys.empty())
	{
		std::string selectedID = attackKeys[currentIndex];
		if (ImGui::BeginTabBar("EditTabs")) {
			//攻撃のソートの調整
			if (ImGui::BeginTabItem("AttackSort")) {

				auto& param = attackParam[selectedID];

				// ActionParamをIDで探す
				ActionParam* actionPtr = nullptr;

				for (auto& a : actions) {
					if (a.id == selectedID) {
						actionPtr = &a;
						break;
					}
				}

				//表示
				ImGui::Separator();
				ImGui::Text("AttackID : %s", selectedID.c_str());

				//ActionParam編集
				if (actionPtr) {
					ImGui::DragInt("Priority", &actionPtr->priority);
					ImGui::DragInt("Weight", &actionPtr->weight);
					ImGui::DragInt("MaxAction", &actionPtr->maxAction);
					ImGui::DragFloat("Distance", &actionPtr->distance);
					ImGui::DragInt("AddWeight", &actionPtr->addWeight);
					ImGui::Checkbox("IsAttack", &actionPtr->attackState);
				}

				// 保存
				if (ImGui::Button("ActionsSave")) {

					ActionsSave();

					sorting->LoadSorting(BossName);
				}
				ImGui::EndTabItem();
			}
			//攻撃のイベントパラメータの追加
			if (ImGui::BeginTabItem("AttackParam"))
			{
				DrawAttackParamEditor(selectedID);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Animation")) {
				// アニメ編集
				ImGui::Separator();
				ImGui::Text("Anim : %s", currentSelectAnimInfos.fileName.c_str());

				ImGui::Text("ModelHandle", &currentSelectAnimInfos.hModel);
				ImGui::Checkbox("Loop", &currentSelectAnimInfos.loop);

				ImGui::InputFloat("MaxFrame", &currentSelectAnimInfos.maxFrame);
				ImGui::InputFloat("PlaySpeed", &currentSelectAnimInfos.playSpeed);

				ImGui::InputFloat("EventStart", &currentSelectAnimInfos.eventStartTime);
				ImGui::InputFloat("EventEnd", &currentSelectAnimInfos.eventFinishTime);

				// fileNameは安全に編集
				static char fileBuffer[256];
				strcpy_s(fileBuffer, currentSelectAnimInfos.fileName.c_str());

				if (ImGui::InputText("FileName", fileBuffer, 256)) {
					currentSelectAnimInfos.fileName = fileBuffer;
					currentSelectAnimInfos.id = fileBuffer;
				}
				if (ImGui::Button("AnimSave")) {

					bossAnim->SetSelectFileInfo(currentSelectAnimInfos);

					bossAnim->AnimDataReSave(currentSelectAnimInfos.id);
					//bossAnim->AnimDataLoad("BossAnimData");

				}
				ImGui::EndTabItem();
			}
			//投擲物の追加
			if (ImGui::BeginTabItem("ThrowObjectAdd"))
			{
				DrawAddThrowObjects(throwObjectsData);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
		

	}

	ImGui::Separator();

	//攻撃の追加
	ImGui::Separator();
	ImGui::Text("Add New Attack");

	// 入力欄
	ImGui::InputText("AttackID (FileName)", newAttackID, 64);
#ifdef ANIM_FILE
	ImGui::InputText("AnimFileName", newAnimFile, 128);

	std::vector<const char*> animItems;
	animItems.clear();

	for (auto& fileName : animFileName) {
		animItems.push_back(fileName.c_str());
	}


	static int animFileIndex = 0;

	if (!animItems.empty())
	{
		if (ImGui::Combo("AnimationFile", &animFileIndex, animItems.data(), (int)animItems.size()))
		{
			// 選択された瞬間だけ反映
			strcpy_s(newAnimFile, sizeof(newAnimFile), animFileName[animFileIndex].c_str());
			newAnimFile[sizeof(newAnimFile) - 1] = '\0';
		}
	}
#else
	static char inputBuf[128] = "";
	static int selectedIndex = -1;

	// 入力欄
	if (ImGui::InputText("AnimSearch", inputBuf, sizeof(inputBuf)))
	{
		selectedIndex = -1; // 入力変わったらリセット
	}

	//候補生成（フィルタ）
	std::vector<std::string> filtered;
	std::vector<const char*> items;

	for (auto& name : animFileName)
	{
		// 部分一致検索
		if (std::string(name).find(inputBuf) != std::string::npos)
		{
			filtered.push_back(name);
			items.push_back(filtered.back().c_str());
		}
	}

	//Combo表示
	if (!items.empty())
	{
		if (ImGui::Combo("Candidates", &selectedIndex, items.data(), (int)items.size()))
		{
			// 選択したら入力欄に反映
			strcpy_s(inputBuf, sizeof(inputBuf), filtered[selectedIndex].c_str());
		}
	}
#endif // ANIM_FILE

	//バリデーション
	bool canAdd = true;
	std::string idStr = newAttackID;

	// 空チェック
	if (idStr.empty()) {
		canAdd = false;
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "ID is empty");
	}

	// 重複チェック
	else if (attackParam.find(idStr) != attackParam.end()) {
		canAdd = false;
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "ID already exists");
	}

	// アニメーション未入力
	else if (strlen(newAnimFile) == 0) {
		canAdd = false;
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "AnimFile is empty");
	}
	//ファイル存在チェック
	else if (!std::filesystem::exists(ResourceLoad::ANIM_PATH + std::string(newAnimFile) + ".mv1")) {
		canAdd = false;
		ImGui::TextColored(ImVec4(1,0,0,1), "Anim file not found");
	}
	

	//AddAttackボタン
	if (!canAdd) {
		ImGui::BeginDisabled();
	}

	if (ImGui::Button("AddAttack"))
	{
		std::string newID = newAttackID;

		// AttackParam追加（ローカル）
		EnemyAttackBase::BossAttackParam newParam;
		newParam.attackID = newID;
		newParam.animFileName = newAnimFile;
		newParam.animNum = FileSystemUtils::GetFileCount("data/json/BossAttack/" + BossName);

		attackParam[newID] = newParam;

		ID::IDType bossAttackAnimID = static_cast<ID::IDType>(attackParam[newID].animNum);

		ResourceLoad::LoadAnim(attackParam[newID].animFileName, bossAttackAnimID);

		attackParam[newID].animID = ID::StringToID(attackParam[newID].animFileName);

		AttackSave(newID);
		sorting->AddAttack(newParam,boss, newID);

		// ActionParam追加（ローカル）
		//空のデータを追加
		ActionParam newAction;
		newAction.id = newID;
		newAction.attackState = true;
		newAction.priority = 1;
		newAction.weight = 0;
		newAction.maxAction = 1;
		newAction.distance = 0.0f;
		newAction.addWeight = 0;

		actions.push_back(newAction);

		ActionsSave();

		sorting->LoadSorting(BossName);
			
		Animator::AnimFileInfo animInfos = Animator::AnimFileInfo();

		//モーションも基本からのデータを取得

		animInfos.fileName = newAnimFile;
		animInfos.id = newAnimFile;
		animInfos.hModel = ResourceLoad::GetHandle(ID::StringToID(animInfos.id));
		animInfos.maxFrame = MV1GetAnimTotalTime(animInfos.hModel, 0);

		bossAnim->SetSelectFileInfo(animInfos);

		bossAnim->AnimDataReSave(animInfos.id);
		//bossAnim->AnimDataLoad("BossAnimData");

		// 入力リセット
		newAttackID[0] = '\0';
		newAnimFile[0] = '\0';

		attackKeys.clear();
		for (auto& a : attackParam) {
			attackKeys.push_back(a.first);
		}
	}

	if (!canAdd) {
		ImGui::EndDisabled();
	}

	ImGui::End();
}

void BossAttackDataSerializer::ActionsSave()
{
	std::string filePath = std::string("data/json/BossAttack/" + BossName + "/Sorting") + "/AttackSort" + ".json";

	JsonReader json;
	json.Load(filePath);

	nlohmann::json& root = json.Data();

	if (!root.contains("AttackSort")) {
		root["AttackSort"] = nlohmann::json::object();
	}
	for (auto& attack : actions) {
		std::string key = attack.id;
		root["AttackSort"][key] = attack;
	}


	json.Save(filePath, root);
}

void BossAttackDataSerializer::AttackSave(std::string _attackID)
{
	JSON root;

	root[_attackID.c_str()] = attackParam[_attackID];

	JsonReader jsonReader;

	jsonReader.Save("data/json/BossAttack/" + BossName + "/" + _attackID + ".json", root);
}

void BossAttackDataSerializer::DrawAttackParamEditor(std::string _selectID)
{
	auto& param = attackParam[_selectID];

	//基本
	if (ImGui::CollapsingHeader("Basic", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("HitDamage", &param.hitDamage, 0.1f);
		ImGui::DragFloat("SlowTime", &param.slowTime, 0.01f);
		ImGui::DragFloat("SlowAmount", &param.slowAmout, 0.01f);
		ImGui::DragFloat("SpeedUp", &param.speedUpMotionSpeed, 0.01f);

		ImGui::DragFloat("AttackStart##Basic", &param.attackCollsionStartTime, 0.01f);
		ImGui::DragFloat("AttackEnd##Basic", &param.attackCollsionEndTime, 0.01f);

		ImGui::DragFloat("JustAvoidStart##Basic", &param.justAvoidCollsionStartTime, 0.01f);
		ImGui::DragFloat("JustAvoidEnd##Basic", &param.justAvoidCollsionEndTime, 0.01f);

		ImGui::DragInt("AttackFrame", &param.attackPositionFrameNum, 1);
		ImGui::DragFloat("SoundTime", &param.attackSoundStartTime, 0.01f);

		DrawTransform("AttackCollTransform", param.attackCollTransform);
		DrawTransform("JustAvoidTransform", param.justAvoidCollTransform);

		ImGui::Checkbox("UseFlash", &param.useFlash);
		ImGui::DragFloat("FlashStartTime", &param.attackFlashStartTime, 0.01f);
	}

	// 移動
	if (ImGui::CollapsingHeader("Move Event"))
	{
		ImGui::Checkbox("FrontMove", &param.frontMove);
		if (param.frontMove)
		{
			ImGui::DragFloat("MoveSpeed##Move", &param.baseSpeed, 0.1f);
			ImGui::DragFloat("BaseFirstSpeed##Follow", &param.baseFirstSpeed, 0.1f);
			ImGui::DragFloat("StartTime##Move", &param.moveStartTime, 0.01f);
			ImGui::DragFloat("EndTime##Move", &param.moveFinishTime, 0.01f);
			ImGui::Checkbox("AddVelocity##Move", &param.addVelocity);
		}
	}

	//プレイヤー追従
	if (ImGui::CollapsingHeader("Player Follow"))
	{
		ImGui::Checkbox("Enable##Follow", &param.playerAloowMove);

		if (param.playerAloowMove)
		{
			ImGui::DragFloat("BaseSpeed##Follow", &param.baseSpeed, 0.1f);
			ImGui::DragFloat("BaseFirstSpeed##Follow", &param.baseFirstSpeed, 0.1f);
			ImGui::Checkbox("NearBossSpeedStop", &param.playerNearStop);
			ImGui::Checkbox("AllowPlayerStop", &param.playerNearAloowStop);
			ImGui::Checkbox("AddVelocity##Follow", &param.addVelocity);

			ImGui::DragFloat("NearDist", &param.playerBaseNear, 0.1f);
			ImGui::DragFloat("MaxSpeed", &param.maxMoveSpeed, 0.1f);
			ImGui::DragFloat("MinSpeed", &param.minMoveSpeed, 0.1f);

			ImGui::DragFloat("StartTime##Follow", &param.moveStartTime, 0.01f);
			ImGui::DragFloat("EndTime##Follow", &param.moveFinishTime, 0.01f);
		}
	}

	//突進
	if (ImGui::CollapsingHeader("Rush Event"))
	{
		ImGui::Checkbox("Enable##Rush", &param.rushMove);

		if (param.rushMove)
		{
			ImGui::DragFloat("Speed##Rush", &param.rushAfterSpeed, 0.1f);
			ImGui::DragFloat("Time##Rush", &param.rushTime, 0.01f);

			ImGui::Checkbox("Collision", &param.rushColl);
			ImGui::DragFloat("CollScale", &param.addRushCollScale, 0.1f);

			ImGui::DragFloat("RightFootSE", &param.rushSoundRightFoot, 0.01f);
			ImGui::DragFloat("LeftFootSE", &param.rushSoundLeftFoot, 0.01f);
		}
	}

	//回転
	if (ImGui::CollapsingHeader("Rotate Event"))
	{
		ImGui::Checkbox("Enable##Rotate", &param.rotateMove);

		if (param.rotateMove)
		{
			ImGui::DragFloat("Angle", &param.angleMoveAmout, 0.1f);
		}
	}

	//ジャンプ
	if (ImGui::CollapsingHeader("Jump Event"))
	{
		ImGui::Checkbox("Enable##Jump", &param.jump);

		if (param.jump)
		{
			ImGui::DragFloat("JumpSpeed", &param.jumpSpeed, 0.1f);
			ImGui::DragFloat("Gravity", &param.addGravity, 0.1f);

			ImGui::DragFloat("StartTime##Jump", &param.jumpStartTime, 0.01f);
			ImGui::DragFloat("EffectTime", &param.groundEffectStartTime, 0.01f);

			ImGui::DragFloat("ShakePower", &param.groundShakeCamera, 0.1f);
			ImGui::DragFloat("ShakeTime", &param.groundShakeTime, 0.01f);
		}
	}

	///プレイヤー見る
	if (ImGui::CollapsingHeader("PlayerLook Event"))
	{
		ImGui::Checkbox("Enable##PlayerLook", &param.lookPlayer);

		if (param.lookPlayer)
		{
			ImGui::DragInt("lookNum", &param.lookNum, 0.1f);
			ImGui::DragFloat("lookMaxCounter", &param.lookMaxCounter, 0.1f);
		}
	}

	//衝撃波
	if (ImGui::CollapsingHeader("ShockWave"))
	{
		ImGui::Checkbox("Enable##Shock", &param.shockWave);

		if (param.shockWave)
		{
			ImGui::DragFloat("Speed", &param.shockWaveSpeed, 0.1f);
			ImGui::DragFloat("StartRange", &param.startRange, 0.1f);
		}
	}

	//投擲
	if (ImGui::CollapsingHeader("Throw Event"))
	{
		ImGui::Checkbox("Enable##Throw", &param.throwObject);

		if (param.throwObject)
		{
			ImGui::Checkbox("ArmThrow", &param.armThrow);
			

			ImGui::DragInt("ThrowNum", &param.throwObjectNum, 1);
			ImGui::DragFloat3("SpawnPos", &param.objectApperaPosition.x, 0.1f);

			ImGui::DragFloat("Interval", &param.intervalTime, 0.01f);
			ImGui::Checkbox("IntervalSub", &param.intervalTimeSub);
			ImGui::DragFloat("MaxInterval", &param.maxIntervalTime, 0.01f);
			ImGui::DragFloat("MinInterval", &param.minIntervalTime, 0.01f);

			ImGui::Separator();

			DrawThrowObjectEditor(param.throwAttackData,throwObjectsData);
		}
	}

	//カメラ
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Checkbox("BossLook", &param.attackCameraBossLook);
		ImGui::DragFloat("ChangeSpeed", &param.cameraChangeSpeed, 0.1f);
	}

	//トレイル
	if (ImGui::CollapsingHeader("Trail"))
	{
		ImGui::Checkbox("UseTrail", &param.useTrail);

		if (param.useTrail)
		{
			ImGui::Checkbox("RightHand", &param.trailRightHand);
		}
	}

	//サウンドイベント
	if (ImGui::CollapsingHeader("Sound Event"))
	{
		DrawSoundEventEditor(param.soundEvent);
	}

	CopyParam(_selectID);

	//保存
	if (ImGui::Button("ParamSave")) {
		AttackSave(_selectID);
		sorting->ReloadParam(param, _selectID);
	}
}

void BossAttackDataSerializer::DrawTransform(const char* label, Transform& t)
{
	if (ImGui::TreeNode(label))
	{
		ImGui::DragFloat3("Position", &t.position.x, 0.1f);
		ImGui::DragFloat3("Rotation", &t.rotation.x, 0.1f);
		ImGui::DragFloat3("Scale", &t.scale.x, 0.1f);
		ImGui::TreePop();
	}
}

const char* BossAttackDataSerializer::MakeLabel(const char* label, const char* category, int index)
{
	static char buf[256];
	sprintf_s(buf, "%s##%s_%d", label, category, index);
	return buf;
}

void BossAttackDataSerializer::DrawSoundEventEditor(std::vector<EnemyAttackBase::SoundEffectEvent>& _eventData)
{
	static int selectIndex = -1;

	ImGui::Separator();
	ImGui::Text("Sound Event List");

	//リスト表示
	for (int i = 0; i < _eventData.size(); i++)
	{
		std::string label;

		if (_eventData[i].name.empty())
		{
			label = std::to_string(i) + " : <Empty>";
		}
		else
		{
			label = std::to_string(i) + " : " + _eventData[i].name;
		}

		if (ImGui::Selectable(label.c_str(), selectIndex == i))
		{
			selectIndex = i;
		}
	}

	//追加・複製・削除
	if (ImGui::Button("Add Sound Event"))
	{
		_eventData.push_back(EnemyAttackBase::SoundEffectEvent());
		selectIndex = (int)_eventData.size() - 1;
	}

	ImGui::SameLine();

	if (ImGui::Button("Copy Sound Event") && selectIndex >= 0)
	{
		_eventData.push_back(_eventData[selectIndex]);
		selectIndex = (int)_eventData.size() - 1;
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete Sound Event") && selectIndex >= 0)
	{
		_eventData.erase(_eventData.begin() + selectIndex);

		if (selectIndex >= _eventData.size())
		{
			selectIndex = (int)_eventData.size() - 1;
		}
	}

	//範囲外なら終了
	if (selectIndex < 0 || selectIndex >= _eventData.size())
	{
		return;
	}

	auto& s = _eventData[selectIndex];

	ImGui::Separator();
	ImGui::Text("Edit Sound Event");

	
	//SoundName Combo

	if (!soundFileNames.empty())
	{
		std::vector<const char*> items;

		for (auto& name : soundFileNames)
		{
			items.push_back(name.c_str());
		}

		int currentIndex = 0;

		for (int i = 0; i < soundFileNames.size(); i++)
		{
			if (soundFileNames[i] == s.name)
			{
				currentIndex = i;
				break;
			}
		}

		if (ImGui::Combo("SoundName", &currentIndex, items.data(), (int)items.size()))
		{
			s.name = soundFileNames[currentIndex];
		}
	}
	else
	{
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "No Sound Files");
	}

	//frame
	ImGui::DragFloat("StartFrame", &s.soundStartFrame, 0.1f, 0.0f);
	ImGui::DragFloat("StopFrame", &s.soundStopFrame, 0.1f, 0.0f);

	//bool
	ImGui::Checkbox("Loop", &s.loop);
	ImGui::Checkbox("StopAfterPlay", &s.soundStopFrameAfetrSoundPlay);
	ImGui::Checkbox("loopAnim", &s.loopAnim);

	//補正
	if (s.soundStopFrame < s.soundStartFrame)
	{
		s.soundStopFrame = s.soundStartFrame;
	}
}

void BossAttackDataSerializer::DrawThrowObjectEditor(std::vector<EnemyAttackBase::ThrowObjectAttackData>& list,const std::map<std::string, BossRockManager::BossThrowObjectData>& throwObjectsData)
{
	static int selectIndex = -1;

	ImGui::Separator();
	ImGui::Text("Throw Object List");

	//リスト
	for (int i = 0; i < list.size(); i++)
	{
		std::string label = std::to_string(i) + " : " + list[i].throwObjectID;

		if (ImGui::Selectable(label.c_str(), selectIndex == i))
		{
			selectIndex = i;
		}
	}

	//追加・削除
	if (ImGui::Button("Add Empty"))
	{
		list.push_back(EnemyAttackBase::ThrowObjectAttackData());
		selectIndex = (int)list.size() - 1;
	}

	ImGui::SameLine();

	if (ImGui::Button("Add Copy") && selectIndex >= 0)
	{
		list.push_back(list[selectIndex]);
		selectIndex = (int)list.size() - 1;
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete") && selectIndex >= 0)
	{
		list.erase(list.begin() + selectIndex);
		selectIndex = -1;
	}

	if (selectIndex < 0 || selectIndex >= list.size()) return;

	auto& t = list[selectIndex];

	ImGui::Separator();
	ImGui::Text("Edit Throw Data");

	//ThrowObjectID（Combo）
	if (!throwObjectsData.empty())
	{
		static std::vector<const char*> throwObjectitems;

		throwObjectitems.clear();

		for (auto& k : throwObjectKeys)
		{
			throwObjectitems.push_back(k.c_str());
		}

		int currentIndex = 0;
		for (int i = 0; i < throwObjectKeys.size(); i++)
		{
			if (throwObjectKeys[i] == t.throwObjectID)
			{
				currentIndex = i;
				break;
			}
		}

		if (ImGui::Combo(MakeLabel("ThrowObjectID", "Combo", selectIndex),&currentIndex,throwObjectitems.data(),(int)throwObjectitems.size()))
		{
			t.throwObjectID = throwObjectKeys[currentIndex];
		}
	}

	//ベースパラメーター
	if (ImGui::CollapsingHeader("Base"))
	{
		ImGui::DragFloat3("Gravity##Base_", &t.baseGravity.x, 0.1f);
		ImGui::DragFloat3("Friction##Base_", &t.baseFirction.x, 0.1f);
	}

	//押し出し判定
	if (ImGui::CollapsingHeader("PushCollision"))
	{
		ImGui::Checkbox("Enable##Push_", &t.pushCollCan);

		if (t.pushCollCan)
		{
			DrawTransform("PushTransform##Push_", t.pushCollTransform);
		}
	}

	//接地判定
	if (ImGui::CollapsingHeader("Ground"))
	{
		ImGui::Checkbox("Enable##Ground_", &t.randCan);

		if (t.randCan)
		{
			DrawRayColliderInfo("Collider##Ground_", t.randCollInfo);

			if (t.randCollInfo.rayStartPos > t.randCollInfo.rayFinishPos)
				std::swap(t.randCollInfo.rayStartPos, t.randCollInfo.rayFinishPos);
		}

		ImGui::DragFloat("LifeTime##Ground_", &t.randTime, 0.1f);
	}

	//プレイヤー攻撃判定
	if (ImGui::CollapsingHeader("Player Hit"))
	{
		ImGui::Checkbox("Enable##PlayerHit_", &t.playerHit);

		if (t.playerHit)
		{
			ImGui::Checkbox("PlayerNoHitNoReaction##PlayerHit_", &t.playerHitNoReaction);
			ImGui::DragFloat("Radius##PlayerHit_", &t.playerHitCollRadius, 0.1f);
			ImGui::DragFloat("JustAvoidRadius##PlayerHit_", &t.playerHitJustAvoidCollRadius, 0.1f);
			ImGui::Checkbox("Capsule##PlayerHit_", &t.capsule);
			ImGui::DragFloat3("CapsuleStartPos##PlayerHit_", &t.capselStartPos.x, 0.1f);
			ImGui::DragFloat3("CapsuleEndPos##PlayerHit_", &t.capselEndPos.x, 0.1f);
			ImGui::Checkbox("capsuleEndPosExtend##PlayerHit_", &t.capsuleEndPosExtend);
			ImGui::DragFloat3("capsuleEndPosExtendOffset##PlayerHit_", &t.capsuleEndPosExtendOffset.x, 0.1f);
			ImGui::Checkbox("capsuleEndPosExtendFromThrowStart##PlayerHit_", &t.capsuleEndPosExtendFromThrowStart);
			ImGui::DragFloat("capsuleEndPosExtendStartAnimFrame##PlayerHit_", &t.capsuleEndPosExtendStartAnimFrame, 0.1f);
		}
	}

	//当たり判定たくさん
	if (ImGui::CollapsingHeader("AlotCollsion Hit"))
	{
		ImGui::Checkbox("Enable##alotCollsionHit", &t.alotCollsionHit);
		if (t.alotCollsionHit) {
			ImGui::DragFloat("alotHitColliderCreateCounter##alotCollsionHit", &t.alotHitColliderCreateCounter, 0.1f);
		}
	}


	//地面についてから当たり判定をつけるか
	if (ImGui::CollapsingHeader("Ground Hit"))
	{
		ImGui::Checkbox("Enable##GroundHit_", &t.playerGroundHit);

		if (t.playerGroundHit)
		{
			ImGui::DragFloat("Radius##GroundHit_", &t.playerGroundCollRadius, 0.1f);
			ImGui::Checkbox("OneHit##GroundHit_", &t.playerGroundOneHit);
			ImGui::Checkbox("NoReaction##GroundHit_", &t.playerGroundNoDamageReactionHit);
		}
	}

	//ボスヒット
	if (ImGui::CollapsingHeader("Boss Hit"))
	{
		ImGui::Checkbox("Enable##BossHit_", &t.bossHit);

		if (t.bossHit)
		{
			ImGui::DragFloat("Radius##BossHit_", &t.bossHitCollRadius, 0.1f);
		}
	}

	//プレイヤーの攻撃で飛ばす
	if (ImGui::CollapsingHeader("Player Attack Flying"))
	{
		ImGui::Checkbox("Enable##Flying_", &t.playerAttackFlying);

		if (t.playerAttackFlying)
		{
			ImGui::DragFloat("Radius##Flying_", &t.playerAttackFlyingCollRadius, 0.1f);
			ImGui::DragFloat("Speed##Flying_", &t.flyingSpeed, 0.1f);
			ImGui::DragFloat("Height##Flying_", &t.flyingHeight, 0.1f);
		}
	}

	//突進でひるむ判定
	if (ImGui::CollapsingHeader("Boss Rush Hit"))
	{
		ImGui::Checkbox("Enable##RushHit_", &t.bossRushHit);

		if (t.bossRushHit)
		{
			ImGui::DragFloat("Radius##RushHit_", &t.bossRushHitCollRadius, 0.1f);
		}
	}

	//爆発するか
	if (ImGui::CollapsingHeader("Blast"))
	{
		ImGui::Checkbox("Enable##Blast_", &t.blastCan);

		if (t.blastCan)
		{
			DrawDountColliderInfo("Blast##Blast_", t.blastColliderInfo);
			DrawDountColliderInfo("JustAvoid##Blast_", t.blastJustAvoidColliderInfo);

			t.blastColliderInfo.outRadius = max(t.blastColliderInfo.outRadius, t.blastColliderInfo.inRadius);

			ImGui::DragFloat("Blink##Blast_", &t.blastBlinkMaxCounter, 0.1f);
			ImGui::Checkbox("Random##Blast_", &t.randomBlast);
			ImGui::DragFloat("Rate##Blast_", &t.randomBlastRate, 0.1f);

			ImGui::DragFloat("MaxRadius##Blast_", &t.maxRadius, 0.1f);
			ImGui::DragFloat("WaveSpeed##Blast_", &t.waveSpeed, 0.1f);
		}
	}

	//ランダムで着地するか
	if (ImGui::CollapsingHeader("Random"))
	{
		ImGui::Checkbox("Height##Random_", &t.randomHeight);
		if (t.randomHeight)
		{
			ImGui::DragFloat("MinH##Random_", &t.minHeight, 0.1f);
			ImGui::DragFloat("MaxH##Random_", &t.maxHeight, 0.1f);
		}

		ImGui::Checkbox("Speed##Random_", &t.randomSpeed);
		if (t.randomSpeed)
		{
			ImGui::DragFloat("MinS##Random_", &t.minSpeed, 0.1f);
			ImGui::DragFloat("MaxS##Random_", &t.maxSpeed, 0.1f);
		}
	}

	//予測円
	if (ImGui::CollapsingHeader("Prediction"))
	{
		ImGui::Checkbox("Enable##Prediction_", &t.predictionCicleCan);

		if (t.predictionCicleCan)
		{
			DrawRayColliderInfo("Collider##Prediction_", t.predictionCicleColliderInfo);
		}
	}

	//腕投げ
	if (ImGui::CollapsingHeader("Throw"))
	{
		ImGui::Checkbox("ArmThrow##Throw_", &t.armThrow);
		ImGui::Checkbox("armSwordHand##Throw_", &t.armSwordHand);
		ImGui::DragInt("ArmFrame##Throw_", &t.armFrameNum, 1);

		ImGui::DragFloat("StartTime##Throw_", &t.throwStartTime, 0.01f);
		ImGui::DragFloat("AppearTime##Throw_", &t.throwObjectApperaTime, 0.01f);
		ImGui::DragFloat3("ArmOffset##Throw_", &t.armAddPos.x, 0.1f);

		ImGui::Checkbox("ToPlayer##Throw_", &t.throwToPlayer);
		ImGui::Checkbox("ToFront##Throw_", &t.thorwToFront);

		ImGui::DragFloat("Speed##Throw_", &t.throwSpeed, 0.1f);
		ImGui::DragFloat("UpSpeed##Throw_", &t.upSpeed, 0.1f);
		ImGui::DragFloat("FirstSpeed##Throw_", &t.throwFirstSpeed, 0.1f);

		ImGui::DragFloat3("Diffusion##Throw_", &t.diffusionAngle.x, 0.1f);

		ImGui::Checkbox("FreeDir##Throw_", &t.freeDir);
		ImGui::DragFloat3("Velocity##Throw_", &t.thorwVelocity.x, 0.1f);
	}

	//落ちる
	if (ImGui::CollapsingHeader("Fall"))
	{
		ImGui::Checkbox("Enable##Fall_", &t.throwToFall);

		if (t.throwToFall)
		{
			ImGui::DragFloat("Height##Fall_", &t.throwHeight, 0.1f);
			ImGui::DragFloat("Gravity##Fall_", &t.throwFallGravity, 0.1f);

			ImGui::Checkbox("ToPlayer##Fall_", &t.throwToFallToPlayer);

			ImGui::DragFloat3("StartPos##Fall_", &t.thorwStartPos.x, 0.1f);
		}
	}
	//その他項目
	if (ImGui::CollapsingHeader("Other"))
	{
		ImGui::Checkbox("GroundDelete##Other_", &t.groundDelete);
		ImGui::Checkbox("Drop##Other_", &t.playerAttackObjectDrop);
		ImGui::Checkbox("attackFinishDelete##Other_", &t.attackFinishDelete);
	}
}

void BossAttackDataSerializer::CopyParam(std::string _selectID)
{
	// コピー機能
	auto& param = attackParam[_selectID];

	ImGui::Separator();
	ImGui::Text("Copy");

	static int copyIndex = 0;
	std::vector<std::string> keys;

	for (auto& a : attackParam) {
		keys.push_back(a.first);
	}

	std::sort(keys.begin(), keys.end());

	std::vector<const char*> items;
	for (auto& k : keys) {
		items.push_back(k.c_str());
	}

	ImGui::Combo("Source", &copyIndex, items.data(), (int)items.size());

	auto& src = attackParam[keys[copyIndex]];

	if (ImGui::Button("Copy ALL"))
	{
		//変えてはいけないユニークなデータは変えない
		src.attackID = param.attackID;
		src.animID = param.animID;
		src.animNum = param.animNum;
		src.animFileName = param.animFileName;

		param = src;
	}

	ImGui::SameLine();

	if (ImGui::Button("Copy Basic"))
	{
		param.hitDamage = src.hitDamage;
		param.slowTime = src.slowTime;
		param.slowAmout = src.slowAmout;
		param.speedUpMotionSpeed = src.speedUpMotionSpeed;

		param.attackCollsionStartTime = src.attackCollsionStartTime;
		param.attackCollsionEndTime = src.attackCollsionEndTime;

		param.justAvoidCollsionStartTime = src.justAvoidCollsionStartTime;
		param.justAvoidCollsionEndTime = src.justAvoidCollsionEndTime;

		param.attackPositionFrameNum = src.attackPositionFrameNum;
		param.attackSoundStartTime = src.attackSoundStartTime;

		param.attackCollTransform = src.attackCollTransform;
		param.justAvoidCollTransform = src.justAvoidCollTransform;

		param.useFlash = src.useFlash;
		param.attackFlashStartTime = src.attackFlashStartTime;
	}
	//イベント単位コピー
	ImGui::Separator();
	ImGui::Text("Event Copy");

	// チェック状態
	static bool copyMove = false;
	static bool copyFollow = false;
	static bool copyRush = false;
	static bool copyRotate = false;
	static bool copyJump = false;
	static bool copyShock = false;
	static bool copyThrow = false;
	static bool copyCamera = false;
	static bool copyTrail = false;
	static bool copySound = false;


	// チェックボックス
	ImGui::Checkbox("Move", &copyMove);
	ImGui::SameLine();
	ImGui::Checkbox("Follow", &copyFollow);
	ImGui::SameLine();
	ImGui::Checkbox("Rush", &copyRush);

	ImGui::Checkbox("Rotate", &copyRotate);
	ImGui::SameLine();
	ImGui::Checkbox("Jump", &copyJump);
	ImGui::SameLine();
	ImGui::Checkbox("Shock", &copyShock);

	ImGui::Checkbox("Throw", &copyThrow);
	ImGui::SameLine();
	ImGui::Checkbox("Camera", &copyCamera);
	ImGui::SameLine();
	ImGui::Checkbox("Trail", &copyTrail);
	ImGui::SameLine();
	ImGui::Checkbox("Sound", &copySound);

	// 一括ON/OFF
	if (ImGui::Button("All ON"))
	{
		copyMove = copyFollow = copyRush = copyRotate =
			copyJump = copyShock = copyThrow = copyCamera = copyTrail = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("All OFF"))
	{
		copyMove = copyFollow = copyRush = copyRotate =
			copyJump = copyShock = copyThrow = copyCamera = copyTrail = false;
	}

	//========================
	// ■ コピー実行
	//========================
	if (ImGui::Button("Copy Selected Events"))
	{
		//移動
		if (copyMove)
		{
			param.frontMove = src.frontMove;
			param.baseSpeed = src.baseSpeed;
			param.moveStartTime = src.moveStartTime;
			param.moveFinishTime = src.moveFinishTime;
			param.addVelocity = src.addVelocity;
		}

		//追いかけ
		if (copyFollow)
		{
			param.playerAloowMove = src.playerAloowMove;
			param.baseSpeed = src.baseSpeed;
			param.playerNearStop = src.playerNearStop;
			param.playerNearAloowStop = src.playerNearAloowStop;
			param.addVelocity = src.addVelocity;
			param.playerBaseNear = src.playerBaseNear;
			param.maxMoveSpeed = src.maxMoveSpeed;
			param.minMoveSpeed = src.minMoveSpeed;
			param.moveStartTime = src.moveStartTime;
			param.moveFinishTime = src.moveFinishTime;
		}

		//突進
		if (copyRush)
		{
			param.rushMove = src.rushMove;
			param.rushAfterSpeed = src.rushAfterSpeed;
			param.rushTime = src.rushTime;
			param.rushColl = src.rushColl;
			param.addRushCollScale = src.addRushCollScale;
			param.rushSoundRightFoot = src.rushSoundRightFoot;
			param.rushSoundLeftFoot = src.rushSoundLeftFoot;
		}

		//回転
		if (copyRotate)
		{
			param.rotateMove = src.rotateMove;
			param.angleMoveAmout = src.angleMoveAmout;
		}

		//ジャンプ
		if (copyJump)
		{
			param.jump = src.jump;
			param.jumpSpeed = src.jumpSpeed;
			param.addGravity = src.addGravity;
			param.jumpStartTime = src.jumpStartTime;
			param.groundEffectStartTime = src.groundEffectStartTime;
			param.groundShakeCamera = src.groundShakeCamera;
			param.groundShakeTime = src.groundShakeTime;
		}

		//衝撃波
		if (copyShock)
		{
			param.shockWave = src.shockWave;
			param.shockWaveSpeed = src.shockWaveSpeed;
			param.startRange = src.startRange;
		}

		//投擲
		if (copyThrow)
		{
			param.throwObject = src.throwObject;
			param.throwAttackData = src.throwAttackData;
			param.armThrow = src.armThrow;
			param.throwObjectNum = src.throwObjectNum;
			param.objectApperaPosition = src.objectApperaPosition;
			param.intervalTime = src.intervalTime;
			param.intervalTimeSub = src.intervalTimeSub;
			param.maxIntervalTime = src.maxIntervalTime;
			param.minIntervalTime = src.minIntervalTime;
		}

		//カメラ
		if (copyCamera)
		{
			param.attackCameraBossLook = src.attackCameraBossLook;
			param.cameraChangeSpeed = src.cameraChangeSpeed;
		}

		//トレイル
		if (copyTrail)
		{
			param.useTrail = src.useTrail;
			param.trailRightHand = src.trailRightHand;
		}
		if (copySound) {
			param.soundEvent = src.soundEvent;
		}
	}
}

void BossAttackDataSerializer::DrawRayColliderInfo(const char* label, EnemyAttackBase::RayColliderInfo& r)
{
	if (ImGui::TreeNode(label))
	{
		ImGui::DragFloat("StartPos", &r.rayStartPos, 0.1f);
		ImGui::DragFloat("FinishPos", &r.rayFinishPos, 0.1f);

		ImGui::TreePop();
	}
}

void BossAttackDataSerializer::DrawDountColliderInfo(const char* label, EnemyAttackBase::DountColliderInfo& d)
{
	if (ImGui::TreeNode(label))
	{
		ImGui::DragFloat("InnerRadius", &d.inRadius, 0.1f);
		ImGui::DragFloat("OuterRadius", &d.outRadius, 0.1f);

		ImGui::TreePop();
	}
}
void BossAttackDataSerializer::DrawAddThrowObjects(std::map<std::string, BossRockManager::BossThrowObjectData>& throwObjectsData)
{
	//状態保持
	static int selectIndex = -1;

	static BossRockManager::BossThrowObjectData editData;
	static std::string editKey = "";
	static bool isEditing = false;

	//keys更新（必要な時だけ）
	if (throwKeyReset)
	{
		throwObjectKeys.clear();
		for (auto& [key, _] : throwObjectsData)
		{
			throwObjectKeys.push_back(key);
		}
		throwKeyReset = false;
	}

	//Combo
	if (!throwObjectKeys.empty())
	{
		std::vector<const char*> items;
		for (auto& k : throwObjectKeys) items.push_back(k.c_str());

		if (ImGui::Combo("ThrowObject Select", &selectIndex, items.data(), (int)items.size()))
		{
			editKey = throwObjectKeys[selectIndex];
			editData = throwObjectsData[editKey];
			isEditing = true;
		}
	}
	else
	{
		ImGui::Text("No Data");
	}

	//追加
	static char newID[128] = "";
	ImGui::InputText("New ID", newID, sizeof(newID));
	ImGui::Checkbox("Effect", &isEffect);

	if (ImGui::Button("Add ThrowObject"))
	{
		std::string id = newID;

		if (!id.empty() && throwObjectsData.count(id) == 0)
		{
			BossRockManager::BossThrowObjectData data;
			data.id = id;
			data.modelName = id;
			data.isEffect = isEffect;

			throwObjectsData[id] = data;
			rockManager->AddJsonData(data);

			//再構築
			throwKeyReset = true;

			newID[0] = '\0';
		}
	}

	//編集
	if (!isEditing) return;

	auto& t = editData;

	ImGui::Separator();
	ImGui::Text("Edit : %s", editKey.c_str());

	//ID変更
	char idBuf[128];
	strcpy_s(idBuf, t.id.c_str());

	if (ImGui::InputText("ID##Edit", idBuf, sizeof(idBuf)))
	{
		std::string newKey = idBuf;

		if (newKey != editKey && throwObjectsData.count(newKey) == 0)
		{
			t.id = newKey;
			t.modelName = newKey;
		}
	}

	//モデル
	ImGui::Text("ModelName : %s", t.modelName.c_str());

	ImGui::DragInt("ModelData", &t.modelData, 1);

	//Transform
	DrawTransform("ModelTransform", t.modelTransform);
	DrawTransform("PushTransform", t.pushTransform);

	//その他
	ImGui::Checkbox("IsEffect", &t.isEffect);

	//セーブ
	if (ImGui::Button("Save"))
	{
		// キー変更対応
		if (editKey != t.id && throwObjectsData.count(t.id) == 0)
		{
			throwObjectsData.erase(editKey);
			throwObjectsData[t.id] = t;
		}
		else
		{
			throwObjectsData[editKey] = t;
		}

		// JSON反映
		rockManager->ChangeJsonData(editData, editKey, editData.id);

		//再構築
		throwKeyReset = true;
		isEditing = false;
		selectIndex = -1;
	}

	ImGui::SameLine();

	//Cancel
	if (ImGui::Button("Cancel"))
	{
		isEditing = false;
		selectIndex = -1;
	}
}

