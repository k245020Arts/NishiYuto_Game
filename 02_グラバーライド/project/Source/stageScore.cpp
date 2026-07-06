#include "stageScore.h"
#include "../Library/time.h"
#include "../ImGui/imgui.h"
#include "../Library/sceneManager.h"
#include "commonFind.h"
#include "gameManager.h"
#include "commonData.h"
#include "stageData.h"

StageScore::StageScore(SceneBase* _scene):GameObject(_scene)
{
	commonData = SceneManager::CommonScene()->FindGameObject<CommonFind>()->CommonFindObject<CommonData>();
	nowStage = commonData->GetData().stage;

	ResetNowState();

	for(int i=0;i<STAR_NUM;i++)
	{
		canGetStar[i] = false;
	}

	//starTarget.targetTime = 0;
	//starTarget.targetCoin = 0;
	//starTarget.targetDamageNum = 0;

	gameManager = nullptr;
	stageData = SceneManager::CommonScene()->FindGameObject<CommonFind>()->CommonFindObject<StageData>();
	starTarget.targetTime = stageData->GetStageConfigData().time;
	doSaveCommonData = false;
}

StageScore::~StageScore()
{
}

void StageScore::Update()
{
	if (gameManager == nullptr)
	{
		gameManager = GetScene()->FindGameObject<GameManager>();
	}

	//タイムの加算
	if(gameManager->GetGameState()==GameManager::PLAYING)
	{
		gameTimer += Time::DeltaTime();
		if (gameTimer > GAMETIME_MAX)
		{
			gameTimer = GAMETIME_MAX;
		}
	}
	//リザルト判定になったらデータをCommonDataに上げる
	if (!doSaveCommonData && gameManager->GetGameState() == GameManager::RESULT_DRAW)
	{
		doSaveCommonData = true;

		//commonData->SetBestCoinNum(nowStage, hasCoinNum);
		//commonData->SetBestClearTime(nowStage,gameTimer);

		std::vector<std::string> saveData;
		saveData.push_back(std::to_string(hasCoinNum));
		saveData.push_back(std::to_string(gameTimer));
		
		for (int i = 0; i < STAR_NUM; i++)
		{
			//commonData->SetGetStar(nowStage, (StarType)i, canGetStar[(StarType)i]);
			saveData.push_back(std::to_string(canGetStar[(StarType)i]));
		}		
		
		stageData->csvReader.Load("data\\score\\score.csv");
		stageData->csvReader.SaveLine("data\\score\\score.csv",nowStage,saveData);
		//stageData->csvReader.ResetAll("data\\score\\score.csv", std::to_string(-1));全スコアのリセット
	}

#if _DEBUG
	ImGui::Begin("StageScore");
	ImGui::DragFloat("Time", &gameTimer);
	ImGui::DragInt("Coin", &hasCoinNum);
	ImGui::DragInt("Damage", &damageNum);
	ImGui::DragFloat("TargetTime", &starTarget.targetTime);
	ImGui::DragInt("TargetCoin", &starTarget.targetCoin);
	ImGui::DragInt("TargetDamage", &starTarget.targetDamageNum);
	//ImGui::Text("Time星=%d,Coin星=%d,Damage星=%d",canGetStar[TIME_STAR],canGetStar[COIN_STAR], canGetStar[DAMAGE_STAR]);
	ImGui::End();

#endif

	UpdateCanGetStar();
}

void StageScore::Draw()
{
}

void StageScore::SetStarTarget(StarTarget _target)
{
	starTarget = _target;
}

void StageScore::SetCoinTarget(int _num)
{
	starTarget.targetCoin = _num;
}

void StageScore::AddGetCoin(int _addNum)
{
	hasCoinNum += _addNum;
}

void StageScore::AddDamageNum(int _addNum)
{
	damageNum += _addNum;
}

void StageScore::ResetNowState()
{
	gameTimer = 0;
	hasCoinNum = 0;
	damageNum = 0;
	score = 0;
}

void StageScore::UpdateCanGetStar()
{
	//クリア条件のStarは常にtrue
	canGetStar[CLEAR_STAR] = true;

	//指定したタイムより速い場合、タイム☆をゲット出来るフラグをTrueにする
	if (starTarget.targetTime >= gameTimer)
	{
		canGetStar[TIME_STAR] = true;
	}
	else
	{
		canGetStar[TIME_STAR] = false;
	}
	//指定したコイン数よりも多くコイン入手している場合、コイン☆をゲット出来るフラグをTrueにする
	if (starTarget.targetCoin <= hasCoinNum)
	{
		canGetStar[COIN_STAR] = true;
	}
	else
	{
		canGetStar[COIN_STAR] = false;
	}
	//指定したダメージ数より被弾数が少ない場合、ダメージ☆をゲット出来るフラグをTrueにする
	/*if (starTarget.targetDamageNum >= damageNum)
	{
		canGetStar[DAMAGE_STAR] = true;
	}
	else
	{
		canGetStar[DAMAGE_STAR] = false;
	}*/
}
