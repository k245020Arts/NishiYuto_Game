#include "TrashEnemyManager.h"
#include "../../Common/ResourceLoader.h"
#include "../../Component/MeshRenderer/MeshRenderer.h"
#include "../../../ImGui/imgui.h"
#include "../../Component/MeshRenderer/MeshRenderer.h"
#include "../../Component/Transform/Transform.h"
#include "../../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../../Player/Player.h"
#include "../../Component/Physics/Physics.h"
#include "../../Component/Hierarchy/Hierarchy.h"
#include "../../Component/Collider/sphereCollider.h"
#include "../../Component/Animator/Animator.h"
#include "../../Weapon/WeaponManager.h"
#include "../../Component/Shaker/Shaker.h"
#include "../../Component/Object/Object2D.h"
#include "../../Component/Guage/Guage.h"
#include "../Boss/Boss.h"
#include "../../Component/Collider/rayCollider.h"
#include "../../Component/Collider/ModelCollider.h"
#include "../../Common/Random.h"
#include "../../Common/Debug/Debug.h"
#include "../../GameControler/GameControler.h"
#include "TrashEnemyGroup.h"
#include "../../Common/ResourceLoader.h"

TrashEnemyManager::TrashEnemyManager()
{
	player = FindGameObjectWithTag<Object3D>("PLAYER");
	
	enemyGroup = new TrashEnemyGroup;
	trashEnemy = nullptr;

	for (int i = 0; i < 4; i++)
	{
		stage[i] = FindGameObjectWithTag<Object3D>("WALL" + std::to_string(i));
	}
	
	WayPointOffset();

	comboRequest = false;
	
	standbyCounter = 0;

	debugWaypoint = false;

	searchCounter = 1.0f;
	maxAttackCounter = ATK_COUNTER_MAX * (float)Random::GetReal();

	startRangedAtk = false;

	numTypeCounter = 0;
	runPoint = 0;

	setPos = VZero;
	hasLeader = false;
	resources.push_back(EnemyResource("1", "000", ResourceLoad::LoadModel("1000_Model", ID::IDType::E_MODEL)));
	resources.push_back(EnemyResource("1", "100", ResourceLoad::LoadModel("1100_Model", ID::IDType::E_MODEL)));
	//近距離の敵のアニメーション設定
	ResourceLoad::LoadAnim(resources[0].charaTypeID + "_IDOL",		ID::TE_IDOL);
	ResourceLoad::LoadAnim(resources[0].charaTypeID + "_RUN",		ID::TE_RUN);
	ResourceLoad::LoadAnim(resources[0].charaTypeID + "_ATTACK1",	ID::TE_ATTACK);
	ResourceLoad::LoadAnim(resources[0].charaTypeID + "_C_ATTACK",	ID::TE_C_ATTACK);
	ResourceLoad::LoadAnim(resources[0].charaTypeID + "_DAMAGE",	ID::TE_DAMAGE);
	ResourceLoad::LoadAnim(resources[0].charaTypeID + "_DEAD",		ID::TE_DEAD);
	ResourceLoad::LoadAnim(resources[0].charaTypeID + "_Stance",	ID::TE_STANCE);
	//遠距離の敵のアニメーション設定
	ResourceLoad::LoadAnim(resources[1].charaTypeID + "_IDOL",		ID::TE_R_IDOL);
	ResourceLoad::LoadAnim(resources[1].charaTypeID + "_RUN",		ID::TE_R_RUN);
	ResourceLoad::LoadAnim(resources[1].charaTypeID + "_ATTACK1",	ID::TE_R_ATTACK);
	ResourceLoad::LoadAnim(resources[1].charaTypeID + "_C_ATTACK",	ID::TE_R_C_ATTACK);
	ResourceLoad::LoadAnim(resources[1].charaTypeID + "_DAMAGE",	ID::TE_R_DAMAGE);
	ResourceLoad::LoadAnim(resources[1].charaTypeID + "_DEAD",		ID::TE_R_DEAD);
	ResourceLoad::LoadAnim(resources[1].charaTypeID + "_Stance",	ID::TE_R_STANCE);
}

TrashEnemyManager::~TrashEnemyManager()
{
	player = nullptr;
}

void TrashEnemyManager::Update()
{
	if (startRangedAtk)
		enemyGroup->RangedEnemyAttack();
}

void TrashEnemyManager::Draw()
{
	if(Debug::GetDebugMode())
		ImguiDraw();
   
	if (!debugWaypoint)
		return;
	
	for (auto itr : wayPoint)
	{
		if (!itr.active)
			continue;
		DrawSphere3D(itr.position, 40.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), itr.active);
	}
}

void TrashEnemyManager::CreateEnemy(VECTOR3 _pos, int meleeSpawnCounter, int rangedSpawnCounter)
{
	setPos = _pos;
	//同じ種類の敵を何体出すか
	int kindsCounter = meleeSpawnCounter / 3;
	//その種類の敵が何体スポーンしたか
	int spawnCounter = 0;
	//現在の生成数
	int enemyCounter = 0;
	//生成する合計の敵
	int max = meleeSpawnCounter + rangedSpawnCounter;
	
    for (int i = 0; i < max; i++)
    {
		//敵の種類の数ができるだけ均等にするための処理
		if (spawnCounter >= kindsCounter)
		{
			numTypeCounter++;
			spawnCounter = 0;
		}
		//
		runPoint++;
		if (runPoint >= wayPointOffsets.size())
			runPoint = 0;

		spawnCounter++;
		enemyCounter++;

		if (meleeSpawnCounter >= enemyCounter)//近距離の敵の生成
		{
			CreateData(resources[0],  i,EnemyType::MELEE);
			enemyGroup->SetMeleeEnemy(trashEnemy);
			continue;
		}
		else if (max >= enemyCounter)//遠距離の敵の生成
		{
			if(hasLeader)
				CreateData(resources[1], i,EnemyType::RANGED);
			else
			{
				CreateData(resources[1], i,EnemyType::RANGED_LEADER);
				hasLeader = true;
			}
			enemyGroup->SetRangedEnemy(trashEnemy);
			continue;
		}
    }
	hasLeader = false;
}

int TrashEnemyManager::GetActiveEnemy()const
{
	return enemyGroup->GetActiveEnemy();
}

int TrashEnemyManager::GetMeleeActiveEnemy()const
{
	return enemyGroup->GetMeleeActiveEnemy();
}

int TrashEnemyManager::GetRangedActiveEnemy()const
{
	return enemyGroup->GetRangedActiveEnemy();
}

void TrashEnemyManager::ImguiDraw()
{
    ImGui::Begin("TrashEnemyManager");

	ImGui::Text("enemiesSize: %d", enemyGroup->GetActiveEnemy());

	if (ImGui::Button("enemySpwn"))
		CreateEnemy(VZero, 1,0);
	if (ImGui::Button("ack1"))
		Cooperate();
	if (ImGui::Button("ack2"))
		startRangedAtk = true;
	if (ImGui::Button("DeadMeleeEnemy"))
		enemyGroup->DeadMeleeEnemy();
	if (ImGui::Button("DeadRangedReader"))
		enemyGroup->DeadRangedEnemy(true);
	if(ImGui::Button("DeadRanged"))
		enemyGroup->DeadRangedEnemy(false);

	if (ImGui::Button("waypoint"))
	{
		if (debugWaypoint)
			debugWaypoint = false;
		else
			debugWaypoint = true;
	}
	
	for (auto& way : wayPoint)
	{
		/*if (way.active)
			return;*/
	}
    ImGui::End();
}

void TrashEnemyManager::Cooperate()
{
	PlayerWayPoint();

	enemyGroup->CloseWayPoint(wayPoint);
}

VECTOR3 TrashEnemyManager::GetRangedLeaderPos()const
{
	return enemyGroup->RangedLeaderPosition();
}

void TrashEnemyManager::WayPointOffset()
{
	const int WaypointMax = 8;
	const float Range = 1500.0f;// プレイヤー中心の半径

	for (int i = 0; i < WaypointMax; i++)
	{
		//均等に割って円形に配置
		float angle = (2.0f * DX_PI_F) * (float)i / (float)WaypointMax;

		//回転を反映した方向
		VECTOR3 rotatedDir = VECTOR3(cosf(angle), 0, sinf(angle));
		//プレイヤーからの絶対座標
		VECTOR3 target = rotatedDir * Range;
		target.y = 0;

		wayPointOffsets.emplace_back(target);
	}
}

void TrashEnemyManager::PlayerWayPoint()
{
	searchCounter = 0;
	wayPoint.clear();
	
	VECTOR3 playerPos = player->GetTransform()->position;
	
	for (auto& itr : wayPointOffsets)
	{
		wayPoint.emplace_back(WayPoint(itr + playerPos, true));
	}
}

std::vector<VECTOR3> TrashEnemyManager::GetWayPointPosition()
{
	std::vector<VECTOR3> _pos;
	PlayerWayPoint();

	for (auto itr : wayPoint)
	{
		_pos.push_back(itr.position);
	}

	return _pos;
}

void TrashEnemyManager::CreateData(EnemyResource _resource, int _i, EnemyType _type)
{
	// 個別のenemyを作る
	Object3D* e;
	e = new Object3D();
	e->Init(EnemyInformation::BASE_POS, VZero, VECTOR3(2.5f, 2.5f, 2.5f), "ENEMY" + std::to_string(_i));
	//当たり判定を生成（やられ判定）
	trashEnemy = e->Component()->AddComponent<TrashEnemy>();
	CollsionInfo info;
	info.parentTransfrom = e->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::ENEMY;
	info.size = 1.0f;//

	RayCollider* collider3 = e->Component()->AddComponent<RayCollider>();
	info.shape = CollsionInformation::RAY;
	info.tag = CollsionInformation::E_FLOOR;
	collider3->RaySet(info, Transform(VECTOR3(0, 150, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, 1, 0), VZero, VECTOR3(1.0f, 1, 1)), nullptr);

	Shaker* shaker = e->Component()->AddComponent<Shaker>();

	std::string charaTypeID = _resource.charaTypeID;

	MeshRenderer* me = e->Component()->AddComponent<MeshRenderer>();
	int handle = MV1DuplicateModel(_resource.model);
	me->ModelHandle(handle, true);
	me->RotationMesh(1, DX_PI_F);

	Animator* anim = e->Component()->AddComponent<Animator>();
	anim->BaseModelSet(handle, 1);

	anim->AnimDataLoad(_resource.charaID, _resource.typeID);

	Physics* physics = e->Component()->AddComponent<Physics>();
	physics->Start(VECTOR3(0.0f, -150.0f, 0.0f), VECTOR3(10.0f, 10.0f, 10.0f));

	// 位置を決める
	const int R_MAX = 2000;
	const float PosY = 3000.0f;

	float rangeX = (float)GetRand(R_MAX * 2) - R_MAX;
	float rangeY = (float)GetRand(R_MAX * 2) - R_MAX;
	VECTOR3 pos = VECTOR3(rangeX, PosY, rangeY);

	trashEnemy->CreateTrashEnemy(setPos + pos, numTypeCounter, runPoint);

	//hp表示
	Object2D* guage = new Object2D();
	guage->Init(VECTOR2F(150, 115), VECTOR2F(0.0f, 0.0f), VECTOR2F(0.2f, 0.2f), "TrashEnemyHpGuage");
	e->AddChild(guage);
	Guage* g = guage->Component()->AddComponent<Guage>();
	g->EdgeDrawReady(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "bossHpEdge1", ID::BOSS_HP_EDGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Transform(VECTOR3(915.0f, 120.0f, 0.0f), VZero, VECTOR3(0.2f, 0.2f, 0.2f)));
	g->GuageDrawReady<TrashEnemy>(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "playerHp",
		ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F,
		Guage::BAR_MODE::HP);
	g->WorldToScreenMode(true, VECTOR3(0, 700, 0));

	// 個別のTrashEnemyを追加
	trashEnemy->Start(e, _type, guage);
}
