#include "BossCreater.h"
#include "../EnemyManager.h"
#include "../../Component/MeshRenderer/MeshRenderer.h"
#include "../../Component/Transform/Transform.h"
#include "../../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../../Player/Player.h"
#include "../../Component/Physics/Physics.h"
#include "../../Camera/Camera.h"
#include "../../Component/Hierarchy/Hierarchy.h"
#include "../../Common/ResourceLoader.h"
#include "../../Component/Collider/sphereCollider.h"
#include "../../Component/Animator/Animator.h"
#include "../../Weapon/WeaponManager.h"
#include "../../Component/Shaker/Shaker.h"
#include "../../Component/Object/Object2D.h"
#include "../../Component/Guage/Guage.h"
#include "Boss.h"
#include "../../Component/Collider/rayCollider.h"
#include "../../Component/Collider/ModelCollider.h"
#include "../../Component/Shadow/Shadow.h"
#include "../../Common/Easing.h"
#include "../../GameControler/GameControler.h"
#include "../../State/StateManager.h"
#include "../../Stage/StageSelectData.h"
#include "../../Common/FileSystemUtils/FileSystemUtils.h"

namespace {
	std::vector<Boss::BossParam> bossParams;

	//アニメーションの末尾のファイル名と、enumをリンクさせる
	std::unordered_map<std::string, ID::IDType> animMap = {
	{ "IDOL", ID::B_IDOL },
	{ "IDOL2", ID::B_COOLTIME },
	{ "WALK", ID::B_RUN },
	{ "WAIT_SEE", ID::B_WAIT_SEE },
	{ "SATTACK2_STOP", ID::B_S_ATTACK2_STOP },
	{ "SATTACK2_BEFORE_2", ID::B_S_ATTACK2_BEFORE },
	{ "DIE", ID::BOSS_DIE },
	{ "ROAR_ANIM", ID::B_ROAR_ANIM },
	{ "ROAR3", ID::B_ROAR_2 },
	{ "RUN", ID::B_DUSH_ANIM },
	{ "THREAT", ID::B_THREAT },
	{ "DAMAGE", ID::BOSS_DAMAGE },
	{ "APPEAR_FALL", ID::B_APPEAR_FALL },
	{ "APPEAR_LAND", ID::B_APPEAR_LAND },
	{ "FEAR", ID::BOSS_FEAR },
	{ "BACKSTEP", ID::B_BACKSTEP },
	{ "WIN", ID::B_WIN }
	};
}

BossCreater::BossCreater()
{
	bossComponentPtr = nullptr;
	LoadBossParam("data/json/BossAttack/BossParam.json");

	const StageData stageData = StageSelectData::GetInstance()->GetNowStageData();

	std::string charaID = "2";

	int num = stageData.bossID;
	std::ostringstream oss;
	// 3桁、空きは'0'で埋める
	oss << std::setfill('0') << std::setw(3) << num;
	std::string str = charaID + oss.str();
	

	//ボスIDからそのボスのアニメーションデータを取得する
	bossAnimFileName = FileSystemUtils::GetFilesStartsWith("data/model/animation", str);

	//対応する文字列にenumをリンクさせてLoadをする
	for (auto& fileInfo : animMap) {
		//_から後ろの文字列を取得
		std::string name = FileSystemUtils::FindAfterAndMatchFileName(bossAnimFileName, fileInfo.first, "_");
		if (name == "") { //_の後の文字列にに対応する文字列がなかったら
			continue;
		}
		ResourceLoad::LoadAnim(name, fileInfo.second);
	}

	
}

BossCreater::~BossCreater()
{
	/*animMap.clear();
	bossParams.clear();*/
}


void BossCreater::CreateBoss()
{
	Object3D* obj = FindGameObjectWithTag<Object3D>("PLAYER");
	Object3D* player = nullptr;
	player = obj;
	Boss::BossParam bossParam;
	int currentBossID = StageSelectData::GetInstance()->GetNowStageData().bossID;
	//ステージに登場するボスのパラメーターをセット
	for (auto& param : bossParams) {
		if (currentBossID == param.bossID) {
			bossParam = param;
			break; //IDと一致したら終了
		}
	}
	Object3D* boss;
	boss = new Object3D();
	float bSize = 8.0f;
	const VECTOR3 InitPos = VECTOR3(0, 450, 2000);
	boss->Init(Transform(InitPos, VZero, VECTOR3(bSize, bSize - 1, bSize)), "Boss");

	bossComponentPtr = boss->Component()->AddComponent<Boss>();

	SphereCollider* collider = boss->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom = boss->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::BOSS;
	info.size = 1.0f;
	std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) { bossComponentPtr->BossDamageCollsionEvent(_data); };
	collider->CollsionAdd(info, Transform(VECTOR3(0, 50, 0), VZero, VECTOR3(350.0f, 1.0f, 1.0f)), func);
	collider->SetTag("BossHit");
	SphereCollider* hitUpCollider = boss->Component()->AddComponent<SphereCollider>();
	info.parentTransfrom = boss->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::BOSS;
	info.size = 1.0f;
	hitUpCollider->CollsionAdd(info, Transform(VECTOR3(0, 110, 0), VZero, VECTOR3(350.0f, 1.0f, 1.0f)), func);
	hitUpCollider->SetTag("BossHit");
	//当たり判定を生成（やられ判定）
	/*SphereCollider* collider2 = boss->Component()->AddComponent<SphereCollider>();
	CollsionInfo info2;
	info2.parentTransfrom = boss->GetTransform();
	info2.shape = CollsionInformation::SPHERE;
	info2.oneColl = false;
	info2.tag = CollsionInformation::Tag::BOSS;
	info.size = 1.0f;
	collider2->CollsionAdd(info2, Transform(VECTOR3(0, 150, 0), VZero, VECTOR3(250.0f, 1.0f, 1.0f)));*/
	RayCollider* collider3 = boss->Component()->AddComponent<RayCollider>();
	info.shape = CollsionInformation::RAY;
	info.tag = CollsionInformation::B_FLOOR;
	collider3->RaySet(info, Transform(VECTOR3(0, 200, 0), VZero, VECTOR3(1.0f, 10.0, 1.0)), Transform(VECTOR3(0, -0, 0), VZero, VECTOR3(1.0f, 1, 1)),nullptr);

	ModelCollider* collider4 = boss->Component()->AddComponent<ModelCollider>();
	info.shape = CollsionInformation::MODEL;
	info.tag = CollsionInformation::BOSS_PUSH;
	collider4->ModelColliderSet(info, Transform(VECTOR3(0, 50, 0), VZero, VECTOR3(0.35f, 1.0f, 0.35f)), ResourceLoad::LoadModel("wall", ID::BOSS_PUSH),nullptr);


	Shaker* shaker = boss->Component()->AddComponent<Shaker>();

	MeshRenderer* m = boss->Component()->AddComponent<MeshRenderer>();
	m->ModelHandle(ResourceLoad::LoadModel(bossParam.modelName, ID::B_MODEL));
	m->RotationMesh(0, 180.0f * DegToRad);

	

	Physics* physics = boss->Component()->AddComponent<Physics>();
	physics->Start(BossInformation::BASE_GRAVITY, BossInformation::BASE_FIRCTION);

	Animator* anim = boss->Component()->AddComponent<Animator>();
	anim->BaseModelSet(ResourceLoad::GetHandle(ID::B_MODEL), 1);

	//anim->AddFile(ID::B_IDOL,				"B_IDLE", true, 1.0f);
	//anim->AddFile(ID::B_COOLTIME,			"B_IDLE2", true, 1.0f);
	//anim->AddFile(ID::B_RUN,				"B_WALK", true, 1.2f, 30.0f, 45.0f);
	//anim->AddFile(ID::B_WAIT_SEE,			"B_WAIT_SEE", true, 0.8f, 30.0f, 45.0f);
	//anim->AddFile(ID::B_N_ATTACK1,			"B_ATTACK1", false, 1.2f, 25.0f, 60.0f);
	//anim->AddFile(ID::B_N_ATTACK2,			"B_ATTACK2", false, 1.2f, 25.0f, 60.0f);
	//anim->AddFile(ID::B_N_ATTACK3,			"B_ATTACK3", false, 1.2f, 40.0f, 55.0f);
	//anim->AddFile(ID::B_N_ATTACK4,			"B_ATTACK4", false, 1.2f, 20.0f, 30.0f);
	//anim->AddFile(ID::B_N_ATTACK5,			"B_ATTACK5", false, 1.2f, 20.0f, 30.0f);
	//anim->AddFile(ID::B_N_ATTACK6,			"B_ATTACK6", false, 1.2f, 50.0f, 65.0f);
	//anim->AddFile(ID::B_N_ATTACK7,			"B_ATTACK7", false, 1.2f, 72.41592f, 85.0f); //31.0f, 75.0f
	//anim->AddFile(ID::B_S_ATTACK1,			"B_SATTACK1", false, 1.0f, 50.0f, 55.0f);
	//anim->AddFile(ID::B_S_ATTACK2,			"B_SATTACK2", true, 1.0f, 0.0f, 100.0f);
	//anim->AddFile(ID::B_S_ATTACK2_STOP,		"B_SATTACK2_STOP", false, 1.0f);
	//anim->AddFile(ID::B_S_ATTACK2_BEFORE,	"B_SATTACK2_BEFORE_2", false, 1.0f, 0.0f, 100.0f);
	//anim->AddFile(ID::B_S_ATTACK1_SMALL,	"B_SATTACK1_SAMLL", false, 1.3f, 40.0f, 55.0f);
	//anim->AddFile(ID::BOSS_DIE,				"B_DIE", false, 1.2f);
	//anim->AddFile(ID::B_ROAR_ANIM,			"B_ROAR_ANIM", false, 1.0f, 30.0f, 40.0f);
	//anim->AddFile(ID::B_ROAR_2,				"B_ROAR3", false, 0.6f, 30.0f, 120.0f);
	//anim->AddFile(ID::B_DUSH,				"B_RUN", true, 1.2f, 30.0f, 40.0f);
	//anim->AddFile(ID::B_THREAT,				"B_THREAT", false, 1.2f, 10.0f, 70.0f);
	//anim->AddFile(ID::BOSS_DAMAGE,			"B_DAMAGE", false, 1.8f, 10.0f, 70.0f);
	//anim->AddFile(ID::B_APPEAR_FALL,		"B_APPEAR_FALL", true, 1.0f, 10.0f, 70.0f);
	//anim->AddFile(ID::B_APPEAR_LAND,		"B_APPEAR_LAND", false, 1.5f, 10.0f, 70.0f);
	//anim->AddFile(ID::BOSS_FEAR,			"B_FEAR", true, 1.0f, 10.0f, 70.0f);
	//anim->AddFile(ID::B_BACKSTEP,			"B_BACKSTEP", true, 1.0f);
	//anim->AddFile(ID::B_WIN,				"B_WIN", true, 1.0f, 10.0f, 70.0f);
	/*ResourceLoad::LoadAnim("B_IDLE", ID::B_IDOL);
	ResourceLoad::LoadAnim("B_IDLE2", ID::B_COOLTIME);
	ResourceLoad::LoadAnim("B_WALK", ID::B_RUN);
	ResourceLoad::LoadAnim("B_WAIT_SEE", ID::B_WAIT_SEE);

	ResourceLoad::LoadAnim("B_SATTACK2_STOP", ID::B_S_ATTACK2_STOP);
	ResourceLoad::LoadAnim("B_SATTACK2_BEFORE_2", ID::B_S_ATTACK2_BEFORE);*/
	/*ResourceLoad::LoadAnim("B_ATTACK1", ID::B_N_ATTACK1);
	ResourceLoad::LoadAnim("B_ATTACK2", ID::B_N_ATTACK2);
	ResourceLoad::LoadAnim("B_ATTACK3", ID::B_N_ATTACK3);
	ResourceLoad::LoadAnim("B_ATTACK4", ID::B_N_ATTACK4);
	ResourceLoad::LoadAnim("B_ATTACK5", ID::B_N_ATTACK5);
	ResourceLoad::LoadAnim("B_ATTACK6", ID::B_N_ATTACK6);
	ResourceLoad::LoadAnim("B_ATTACK7", ID::B_N_ATTACK7);
	ResourceLoad::LoadAnim("B_SATTACK1", ID::B_N_ATTACK8);
	ResourceLoad::LoadAnim("B_SATTACK2", ID::B_N_ATTACK9);
	ResourceLoad::LoadAnim("B_SATTACK1_SAMLL", ID::B_N_ATTACK10);
	ResourceLoad::LoadAnim("B_ATTACK_IDOL", ID::B_N_ATTACK11);*/
	/*ResourceLoad::LoadAnim("B_DIE", ID::BOSS_DIE);
	ResourceLoad::LoadAnim("B_ROAR_ANIM", ID::B_ROAR_ANIM);
	ResourceLoad::LoadAnim("B_ROAR3", ID::B_ROAR_2);
	ResourceLoad::LoadAnim("B_RUN", ID::B_DUSH);
	ResourceLoad::LoadAnim("B_THREAT", ID::B_THREAT);
	ResourceLoad::LoadAnim("B_DAMAGE", ID::BOSS_DAMAGE);
	ResourceLoad::LoadAnim("B_APPEAR_FALL", ID::B_APPEAR_FALL);
	ResourceLoad::LoadAnim("B_APPEAR_LAND", ID::B_APPEAR_LAND);
	ResourceLoad::LoadAnim("B_FEAR", ID::BOSS_FEAR);
	ResourceLoad::LoadAnim("B_BACKSTEP", ID::B_BACKSTEP);
	ResourceLoad::LoadAnim("B_WIN", ID::B_WIN);*/

	bossComponentPtr->Start(boss, bossParam);

	//const StageData stageData = StageSelectData::GetInstance()->GetNowStageData();

	//std::string charaID = "2";

	//int num = stageData.bossID;
	//std::ostringstream oss;
	//// 3桁、空きは'0'で埋める
	//oss << std::setfill('0') << std::setw(3) << num;
	//std::string typeID = oss.str();

	//anim->AnimDataLoad(charaID,typeID);
	anim->SetMaxFrame(ID::B_N_ATTACK1, 50.0f);

	Object2D* guage = new Object2D();

	guage->Init(VECTOR2F(915.0f, 70.0f), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "Boss_HpBar_RedBack");

	boss->AddChild(guage);

	Guage* g = guage->Component()->AddComponent<Guage>();
	g->DamageGuageDrawReady(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Boss_HpBar_YellowBack", ID::BOSS_HP_DAMAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Transform(VECTOR3(915.0f, 70.0f, 0.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(1.0f, 1.0f, 1.0f)));
	g->GuageDrawReady<Boss>(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Boss_HpBar_RedBack", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Guage::BAR_MODE::HP);
	g->EdgeDrawReady(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Boss_HpBar_Frame", ID::BOSS_HP_EDGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Transform(VECTOR3(915.0f, 70.0f, 0.0f), VZero, VECTOR3(1.0f, 1.0f, 0.0f)));



	//bossList.emplace_back(b);

	Object3D* shadow = new Object3D();
	shadow->Init(Transform(VECTOR3(0.0f, -20.0f, 0.0f), VZero, VECTOR3(boss->GetTransform()->scale.x - 2.0f, 0.1f, boss->GetTransform()->scale.z - 2.0f)), "BossShadow");
	Shadow* s = shadow->Component()->AddComponent<Shadow>();
	s->Start(50.0f);

	RayCollider* collider5 = shadow->Component()->AddComponent<RayCollider>();
	info.shape = CollsionInformation::RAY;
	info.tag = CollsionInformation::SHADOW;
	collider5->RaySet(info, Transform(VECTOR3(0, 50, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, -s->GetMaxDist(), 0), VZero, VECTOR3(1.0f, 1, 1)),nullptr);
	boss->AddChild(shadow);

	player->Component()->GetComponent<Player>()->TargetObjSet(nullptr);
	player->Component()->GetComponent<Player>()->HitObjectSet(nullptr);
	//FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>()->TargetSet(*enemy.begin());
}

bool BossCreater::LoadBossParam(std::string _fileName)
{
	JsonReader jsonReader;
	if (!jsonReader.Load(_fileName)) {
		return false;
	}

	auto& json = jsonReader.Data();

	for (const auto& elem : json["Boss"])
	{
		Boss::BossParam boss;

		boss = elem;
		bossParams.push_back(boss);
	}
	return true;
}
