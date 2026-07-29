#include "AttackSorting.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
#include "BossStatus.h"
#include "../../../Common/Random/Random.h"
#include "../../../Common/FileSystemUtils/FileSystemUtils.h"
#include "../../../Common/ResourceLoader/ResourceLoader.h"
#include "../../../../ImGui/imgui.h"
#include "../../../Stage/StageSelectData.h"
#include "../../../Component/Animator/Animator.h"

namespace {
	constexpr int ATTACK_KIND_MAX		= 6;
	constexpr int COMBO_ATTACK_KIND_MAX = 3;

	//通常攻撃の重み
	const std::vector<std::vector<double>> normalAttackParam{
		{ 1.0,	1.0,	0.5,	0.0,	0.0,	0.0,	1.0,	1.0,	0.5 },
		{ 1.0,	1.0,	1.0,	0.0,	0.0,	0.0,	1.0,	1.0,	1.0  },
		{ 0.1,	0.1,	0.5,	0.7,	1.0,	0.1,	0.5,	0.5,	0.5  },
		{ 0.05,	0.05,	1.0,	0.5,	0.9,	0.1,	0.5,	0.5,	0.5  },
	};
}

AttackSorting::AttackSorting()
{
	string		= Function::GetClassNameC<AttackSorting>();
	hp			= Boss::MAX;
	attackNum	= 0;
	jump		= false;
	kind		= 0;

	bossPriority = 0;
	copyPriority = 0;
	moveCounter = 0;

	copyState = StateID::STATE_MAX;

	pVec = VZero;
	forceAttack = false;
	nextAttack = false;
}

AttackSorting::~AttackSorting()
{

}

void AttackSorting::Update()
{
	Boss* b = GetBase<Boss>();
	if (b->isComboAttack)
	{
		if (attacks[nextState]->isFinishAttack())
		{
			AttackFinish();
			b->enemyBaseComponent.anim->Play(ID::B_IDOL);
			nextAttack = false;
		}
	}
		
	if (nextAttack)
	{
		if (attacks[nextState] == nullptr)
		{
			//次の状態が何も入ってなかったら攻撃ステートを終了
			b->BossAttackStateChange();
			return;
		}
		attacks[nextState]->Update();
	}
	else
	{
		if (nextState == "")
		{
			//次の状態が何も入ってなかったらもう一度抽選
			BuildTable(bossPriority);
			return;
		}
		else if (comboIdSave.size() != 0)
		{
			nextState = comboIdSave[0];
			AttackStart();
			//保存したコンボの先頭を削除して次の技を0に保存する
			comboIdSave.erase(comboIdSave.begin());
			nextAttack = true;
			return;
		}
		else if (b->isComboAttack)//コンボ攻撃が終了したとき
		{
			b->BossAttackStateChange();
			return;
		}

		b->enemyBaseComponent.state->ChangeState(StateID::StringToID(nextState));
	}
}

void AttackSorting::Start()
{
	animId = ID::B_IDOL;
	EnemyStateBase::Start();
	Boss* b = GetBase<Boss>();
	
	hp = b->Hp();
	//コンボ攻撃をするか決める
	float comboAttackRate = 0.0f;

	switch (hp)
	{
	case Boss::MAX:
		comboAttackRate = 0.0f;
		bossPriority = 10;
		break;
	case Boss::EIGHT:
		comboAttackRate = 0.2f;
		bossPriority = 30;
		break;
	case Boss::FIVE:
		comboAttackRate = 0.7f;
		bossPriority = 50;
		break;
	case Boss::THREE:
		comboAttackRate = 0.9f;
		bossPriority = 80;
		break;
	}

	pVec = b->enemyBaseComponent.playerObj->GetTransform()->position - b->GetEnemyObj()->GetTransform()->position;
	BuildTable(bossPriority);
	
	b->comboFirstAttack = true;
}

void AttackSorting::Finish()
{
	Boss* boss = GetBase<Boss>();
	copyState = nextState;
	moveCounter++;
	AttackFinish();
	forceAttack = false;
	boss->isComboAttack = false;
}

void AttackSorting::ForcedAttackStart(const std::string& _attackID)
{
	nextState = _attackID;
	nextAttack = true;
	AllAddWeightZero();
	AttackStart();
	forceAttack = true;
}

void AttackSorting::LoodAttackSelect(const std::string& _fileName, const std::string& _atkCombo)
{
	JsonReader json;
	json.Load(_fileName);

	auto& j = json.Data();

	for (const auto& copy : j["BossAttackSelect"])
	{
		selectData.push_back(copy);
	}

	JsonReader json2;
	json2.Load(_atkCombo);

	auto& j2 = json2.Data();
	
	for (int i = 0; i < j2.size(); i++)
	{
		atkComboData.push_back(j2.at("Combo" + std::to_string(i)).get<AttackComboData>());
	}
}

void AttackSorting::NormalAttackSelect()
{
	Boss* b = GetBase<Boss>();
	std::vector<double> rand = normalAttackParam[hp];
	
	VECTOR3 dist = b->obj->GetTransform()->position - b->enemyBaseComponent.playerObj->GetTransform()->position;
	float size = dist.Size();
	if (size > 1400.0f) {
		rand[6] += 0.5f;
		rand[7] += 0.5f;
		rand[8] += 0.5f;
	}
	else {
		rand[0] += 0.5f;
		rand[1] += 0.5f;
		rand[2] += 0.5f;
	}
	if (size > 5000.0f) {
		rand[5] += 3.0f;
	}
	kind = Random::GetWeightedIndex(rand);
	b->maxAttack = -1;
}

void AttackSorting::BuildTable(int _priority)
{
	if (forceAttack) 
		return;

	int c = 0;
	int n = 0;
	for (int i = 0; i < selectData.size(); i++)
	{
		if (bossPriority <= selectData[i].priority)
		{
			c = selectData[i].comboParam;
			n = selectData[i].normalParam;
			break;
		}
	}
	//距離が離れてるときにコンボ攻撃を選択するとその場で攻撃するようになる
	int p = GetRand(c + n);
	if (p - c < 0 && pVec.Size() < ComboDistance && atkComboData.size()!=0)
		SelectNextComboAction(_priority);
	else
		SelectNextAction(_priority);
}

void AttackSorting::AllAddWeightZero()
{
	for (auto& itr : actions)
	{
		if (itr.addWeight == 0)
			continue;

		itr.addWeight = 0;
	}
}

void AttackSorting::Load(const std::string& _bossName,Boss* _boss)
{
	std::string filePath = "data/json/BossAttack/" + _bossName;

	for (const auto& entry : std::filesystem::directory_iterator(filePath)) {
		//フォルダだったらスルー
		if (entry.is_directory()) {
			continue;
		}
		std::string fileName = entry.path().filename().string();
		std::string key = entry.path().stem().string();
		JsonReader jsonReader;
		JSON root;
		//ロードできなかった時
		if (!jsonReader.Load(filePath + "/" + fileName)){
			continue;
		}

		root = jsonReader.Data();

		if (root.contains(key))
		{
			attackParam[key] = root[key].get<EnemyAttackBase::BossAttackParam>();
		}
		else if (!root.empty())
		{
			// フォールバック
			attackParam[key] = root.begin().value().get<EnemyAttackBase::BossAttackParam>();
		}

		// 念のためID補完
		if (attackParam[key].attackID.empty())
		{
			attackParam[key].attackID = key;
		}

		
		ID::IDType bossAttackAnimID = static_cast<ID::IDType>(attackParam[key].animNum);

		ResourceLoad::LoadAnim(attackParam[key].animFileName, bossAttackAnimID);
		//StringからIDに変換
		attackParam[key].animID = ID::StringToID(attackParam[key].animFileName);

		attacks[key] = std::make_shared<BossAttack>();

		attacks[key]->Init(obj,StateID::StringToID(key));

		attacks[key]->SetAttackParam(attackParam[key]);
		
	}
	for (auto& t : attacks) {
		t.second->SetComponent<Boss>(_boss);
	}

	LoadSorting(_bossName);
}

void AttackSorting::AttackStart() 
{
	if (attacks[nextState] == nullptr) {
		Debug::DebugLog("ボスの攻撃がスタートできません");
		return;
	}
	attacks[nextState]->Start();
}

void AttackSorting::SelectNextComboAction(int _priority)
{
	Boss* boss = GetBase<Boss>();
	boss->isComboAttack = true;
	float totalWeight = 0;
	for (auto& itr : atkComboData)
	{
		if (itr.priority > _priority)
			continue;
		else
		{
			totalWeight += itr.weight;
		}
	}
	
	//打てる技の合計からランダムな数字をだす
	int rand = GetRand((int)totalWeight - 1);

	for (auto& itr : atkComboData)
	{
		if (itr.priority == 0)//上で制御した行動を戻す代わりに選出されないようにした
		{
			itr.priority = copyPriority;
			continue;
		}
		else if (itr.priority > _priority)//プライオリティを超えて技をださないようにする
			continue;

		rand -= (int)itr.weight;

		if (rand < 0)
		{
			for (auto& id : itr.id)
			{
				comboIdSave.push_back(id);
				nextAttack = false;
			}
			nextState = comboIdSave[0];
			break;
		}
	}
}

void AttackSorting::SelectNextAction(int _priority)
{
	int totalWeight = 0;
	//距離が離れていて選択されやすい攻撃をするときにどれだけ出やすくするか
	float _addWeight = 0;

	for (auto& itr : actions)
	{
		if (itr.id == copyState)//一個前の行動と同じとき
		{
			if (itr.maxAction < moveCounter)//指定した行動
			{
				//同じ行動を取らないためにプライオリティをコピーしてから0を代入して選ばれないようにしている(後で戻すためにコピーしてる)
				copyPriority = itr.priority;
				itr.priority = 0;
				continue;
			}
		}
		else
			moveCounter = 0;

		if (itr.priority > _priority)
			continue;
		else
		{
			if (itr.distance != 0.0f && pVec.Size() > itr.distance)
				itr.addWeight = 3000;

			totalWeight += itr.weight + itr.addWeight;
		}
	}

	//打てる技の合計からランダムな数字をだす
	int rand = GetRand(totalWeight - 1);

	for (auto& itr : actions)
	{
		if (itr.priority == 0)//上で制御した行動を戻す代わりに選出されないようにした
		{
			itr.priority = copyPriority;
			continue;
		}
		else if (itr.priority > _priority)//プライオリティを超えて技をださないようにする
			continue;

		rand -= itr.weight + itr.addWeight;

		if (rand < 0)
		{
			nextState = itr.id;
			nextAttack = itr.attackState;
			AllAddWeightZero();
			AttackStart();
			break;
		}
	}
}

void AttackSorting::AttackFinish() 
{
	if (attacks[nextState] == nullptr) {
		Debug::DebugLog("ボスの攻撃が終了できません");
		return;
	}
	attacks[nextState]->Finish();
}

std::shared_ptr<EnemyAttackBase> AttackSorting::GetNowAttackState()
{
	return attacks[nextState];
}

void AttackSorting::SaveSorthing(const std::string& _bossName)
{
	std::string filePath = std::string("data/json/BossAttack/" + _bossName + "/Sorting") + "/AttackSort" + ".json";

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

void AttackSorting::LoadSorting(const std::string& _bossName)
{
	JsonReader json;
	std::string filePath = std::string("data/json/BossAttack/" + _bossName + "/Sorting") + "/AttackSort" + ".json";
	json.Load(filePath);

	actions.clear();
	for (auto& j : json.Data()["AttackSort"]) {
		ActionParam action;
		j.get_to(action);
		actions.push_back(action);
	}
}

const std::vector<ActionParam> AttackSorting::GetActionParam()const
{
	return actions;
}

const std::unordered_map<std::string, EnemyAttackBase::BossAttackParam> AttackSorting::GetAttackParam()const
{
	return attackParam;
}

void AttackSorting::AddAttack(const EnemyAttackBase::BossAttackParam& _param, Boss* _boss)
{
	AddAttack(_param,_boss, _param.attackID);
}

void AttackSorting::AddAttack(const EnemyAttackBase::BossAttackParam& _param, Boss* _boss, const std::string& _attackID)
{
	std::string key = _attackID;	
	if (attacks[key] != nullptr) { //攻撃がすでにあったら追加しない
		return;
	}
	attacks[key] = std::make_shared<BossAttack>();

	attacks[key]->Init(obj, StateID::StringToID(key));

	attacks[key]->SetComponent<Boss>(_boss);
	attacks[key]->SetAttackParam(attackParam[key]);
}

void AttackSorting::ReloadParam(const EnemyAttackBase::BossAttackParam& _param, const std::string& _reLoadID)
{
	//設定のし直し
	attackParam[_reLoadID] = _param;
	attacks[_reLoadID]->SetAttackParam(attackParam[_reLoadID]);
}
