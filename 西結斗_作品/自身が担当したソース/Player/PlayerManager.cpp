#include "playerManager.h"
#include "../Component/Object/Object3D.h"
#include "../Component/ComponentManager.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "../Component/Transform/Transform.h"
#include "../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "player.h"
#include "../Component/Physics/Physics.h"
#include "../Camera/Camera.h"
#include "../Component/Hierarchy/Hierarchy.h"
#include "../State/StateManager.h"
#include "../Enemy/EnemyManager.h"
#include "../Common/ResourceLoader.h"
#include "../Component/Collider/SphereCollider.h"
#include "../Component/Animator/Animator.h"
#include "../Component/Light/ComponentLight.h"
#include "../Component/Shaker/Shaker.h"
#include "../Component/MotionBlur/MotionBlur.h"
#include "../Component/Object/Object2D.h"
#include "../Component/Guage/Guage.h"
#include "../Component/Collider/RayCollider.h"
#include "../Component/Shadow/Shadow.h"
#include "../Player/PlayerState/PlayerWait.h"
#include "../Player/PlayerState/playerWalk.h"
#include "../Player/PlayerState/AttackState/playerAttack1.h"
#include "../Player/PlayerState/AttackState/playerAttack2.h"
#include "../Player/PlayerState/playerDamage.h"
#include "../Player/PlayerState/PlayerLose.h"
#include "../Player/PlayerState/playerjustAvoid.h"
#include "../Player/PlayerState/PlayerBlowAway.h"
#include "../Player/PlayerState/PlayerTurn.h"
#include "../Player/PlayerState/AttackState/playerJustAvoidAttack1.h"
#include "../Player/PlayerState/AttackState/playerAttack3.h"
#include "../Player/PlayerState/AttackState/playerAttack4.h"
#include "../Player/PlayerState/AttackState/playerAttack5.h"
#include "../Player/PlayerState/AttackState/PlayerHeavyCharge.h"
#include "../Player/PlayerState/AttackState/PlayerHeavyAttack.h"
#include "../Player/PlayerState/PlayerAvoid.h"
#include "../Player/PlayerState/PlayerDie.h"
#include "../Player/PlayerState/AttackState/PlayerSpecialAttack.h"
#include "../GameControler/GameControler.h"
#include "PlayerState/PlayerWin.h"
#include "PlayerState/PlayerBefore.h"
#include "../Component/Animator/Anim2D.h"
#include "../Screen.h"
#include "../Title/TitlePlayer.h"
#include "../Player/PlayerState/PlayerBossAppear.h"
#include "../PlayScene.h"

PlayerManager::PlayerManager()
{
	playerPointer = nullptr;
	gameManager = nullptr;
	stateManager = nullptr;
	playerPtr = nullptr;
}

PlayerManager::~PlayerManager()
{

}

void PlayerManager::Update()
{
	if (gameManager == nullptr) {
		gameManager = FindGameObject<GameControler>();
	}
	
	//GameManagerのnullptrに対してのセット
	if (gameManager != nullptr) {
		//bool isChange = ;
		
		if (gameManager->GetChangeStateOneFrame()) {
			
			GameSceneChangeState();
		}
		
	}
}

void PlayerManager::Draw()
{

}

void PlayerManager::CreatePlayer()
{
	playerPointer = new Object3D();
	playerPointer->SetDrawOrder(-5);
	//playerPointer->Init(VECTOR3(300, 100, 1500), VZero, VECTOR3(3.0f, 3.0f,3.0f), "PLAYER");
	playerPointer->Init(VECTOR3(300,0, -2000), VZero, VECTOR3(3.0f ,3.0f,3.0f), "PLAYER");

	playerPtr = playerPointer->Component()->AddComponent<Player>();

	//やられ判定の追加
	ColliderBase* collider = playerPointer->Component()->AddComponent<SphereCollider>();
	CollsionInfo info;
	info.parentTransfrom	= playerPointer->GetTransform();
	info.shape				= CollsionInformation::SPHERE;
	info.oneColl			= false;
	info.tag				= CollsionInformation::Tag::PLAYER;
	info.size				= 1.0f;
	std::function<void(const CollsionEventData&)> func = [this](const CollsionEventData& _data) { playerPtr->CollsionEvent(_data); };
	collider->CollsionAdd(info, Transform(VECTOR3(0, 0, 0), VZero, VECTOR3(150.0f, 0, 0)),func,"playerColl");

	//やられ判定の追加
	ColliderBase* collider2 = playerPointer->Component()->AddComponent<SphereCollider>();
	collider2->CollsionAdd(info, Transform(VECTOR3(0, 100, 0), VZero, VECTOR3(200.0f, 0, 0)), func,"playerColl");
	RayCollider* collider3	= playerPointer->Component()->AddComponent<RayCollider>();
	info.shape				= CollsionInformation::RAY;
	info.tag				= CollsionInformation::P_FLOOR;
	collider3->RaySet(info, Transform(VECTOR3(0, 100, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, -10, 0), VZero, VECTOR3(1.0f, 1, 1)),nullptr);

	Shaker* shaker		= playerPointer->Component()->AddComponent<Shaker>();
	
	MeshRenderer* me2	= playerPointer->Component()->AddComponent<MeshRenderer>();

	MeshRenderer2D* me2D = playerPointer->Component()->AddComponent<MeshRenderer2D>();
	me2D->SetDraw(false);
	me2D->SetTransform(Transform(VECTOR3(Screen::WIDTH / 2.0f, Screen::HEIGHT / 2.0f, 0.0f), VZero, VECTOR3(192.0f, 1.0f,0)));
	//me2D->SetTransform(Transform(VECTOR3(Screen::WIDTH / 2.0f, Screen::HEIGHT / 2.0f, 0.0f), VZero, VECTOR3(0.1f,1,0) * 10.0f));

	me2D->TextureHandle(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "speicialAttackBackSide4", ID::PLAYER_SPECIAL_ATTACK_BACK), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F);

	Anim2D* anim2D = playerPointer->Component()->AddComponent<Anim2D>();
	anim2D->AnimSetting(400.0f, 192);

	me2D->AnimStart(1.0f,10,true);
	
	Physics* physics	= playerPointer->Component()->AddComponent<Physics>();
	
	
	physics->Start(PlayerInformation::BASE_GRAVITY, PlayerInformation::BASE_INTERIA);
	/*Camera* camera = playerPointer->Component()->AddComponent<Camera>();
	camera->Start(FindGameObject<EnemyManager>()->GetEnemy());*/
	//me2->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "exo_red", ID::P_MODEL));
	me2->ModelHandle(ResourceLoad::LoadModel("Player4", ID::P_MODEL));
	me2->RotationMesh(0, 180.0f * DegToRad);

	stateManager = playerPointer->Component()->AddComponent<StateManager>();

	//Stateの追加--------------------------------------------------------------
	stateManager->CreateState<PlayerWait>("PlayerWait", StateID::PLAYER_WAIT_S);
	stateManager->CreateState<PlayerWalk>("PlayerWalk", StateID::PLAYER_WALK_S);
	stateManager->CreateState<PlayerAvoid>("PlayerAvoid", StateID::PLAYER_AVOID_S);
	stateManager->CreateState<PlayerJustAvoid>("PlayerJustAvoid", StateID::PLAYER_JUST_AVOID_S);
	stateManager->CreateState<PlayerAttack1>("PlayerAttack1", StateID::PLAYER_ATTACK1_S);
	stateManager->CreateState<PlayerJustAvoidAttack1>("PlayerJustAvoidAttack1", StateID::PLAYER_JUST_AVOID_ATTACK1_S);
	stateManager->CreateState<PlayerAttack2>("PlayerAttack2", StateID::PLAYER_ATTACK2_S);
	stateManager->CreateState<PlayerAttack3>("PlayerAttack3", StateID::PLAYER_ATTACK3_S);
	stateManager->CreateState<PlayerAttack5>("PlayerAttack5", StateID::PLAYER_ATTACK5_S);
	stateManager->CreateState<PlayerAttack4>("PlayerAttack4", StateID::PLAYER_ATTACK4_S);
	stateManager->CreateState<PlayerDamage>("PlayerDamage", StateID::PLAYER_DAMAGE_S);
	stateManager->CreateState<PlayerBlowAway>("PlayerBlowAway", StateID::PLAYER_BLOW_AWAY_S);
	stateManager->CreateState<PlayerDie>("PlayerDie", StateID::PLAYER_DIE_S);
	stateManager->CreateState<PlayerTurn>("PlayerTurn", StateID::PLAYER_TURN_S);
	stateManager->CreateState<PlayerSpecialAttack>("PlayerSpecialAttack", StateID::PLAYER_SPECIAL_ATTACK_S);
	stateManager->CreateState<PlayerWin>("PlayerWin", StateID::PLAYER_WIN_STATE_S);
	stateManager->CreateState<PlayerBefore>("PlayerBefore", StateID::PLAYER_BEFORE_S);
	stateManager->CreateState<PlayerBossAppear>("PlayerBossAppear", StateID::PLAYER_BOSS_APPEAR_S);
	stateManager->CreateState<PlayerHeavyCharge>("PlayerHeavyCharge", StateID::PLAYER_HEAVY_CHARGE_S);
	stateManager->CreateState<PlayerHeavyAttack>("PlayerHeavyAttack", StateID::PLAYER_HEAVY_ATTACK_S);
	stateManager->CreateState<PlayerLose>("PlayerLose", StateID::PLAYER_LOSE_S);
	//--------------------------------------------------------------------------

	Animator* anim = playerPointer->Component()->AddComponent<Animator>();
	anim->BaseModelSet(ResourceLoad::GetHandle(ID::P_MODEL),		"mixamorig:Hips");

	std::string charaID = "3";
	std::string typeID = "000";
	//----------------------キャラIDとプレイヤーの種類IDをセットしてロード
	ResourceLoad::LoadAnim(charaID + typeID + "_IDOL1", ID::IDType::P_ANIM_IDOL);
	ResourceLoad::LoadAnim(charaID + typeID + "_RUN_M_1", ID::IDType::P_ANIM_RUN);
	ResourceLoad::LoadAnim(charaID + typeID + "_AVOID_M_1", ID::IDType::P_ANIM_AVOID);
	ResourceLoad::LoadAnim(charaID + typeID + "_JUST_AVOID_M_1", ID::IDType::P_ANIM_JUST_AVOID);
	ResourceLoad::LoadAnim(charaID + typeID + "_DAMAGE_M_1", ID::IDType::P_DAMAGE);
	ResourceLoad::LoadAnim(charaID + typeID + "_FALL_M_1", ID::IDType::P_FALL);
	ResourceLoad::LoadAnim(charaID + typeID + "_GETUP_M_1", ID::IDType::P_GETUP);
	ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK01_M_1", ID::IDType::P_ANIM_ATTACK1);
	ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK02_M_1", ID::IDType::P_ANIM_ATTACK2);
	ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK11_M_1", ID::IDType::P_ANIM_JUST_AVOID_ATTACK1);
	ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK03_M_1", ID::IDType::P_ANIM_ATTACK3);
	ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK05_M_1", ID::IDType::P_ANIM_ATTACK_5);
	ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK04_M_1", ID::IDType::P_ANIM_ATTACK4);
	ResourceLoad::LoadAnim(charaID + typeID + "_DIE_M_1", ID::IDType::P_DIE);
	ResourceLoad::LoadAnim(charaID + typeID + "_TURN1", ID::IDType::P_TURN_ANIM);
	ResourceLoad::LoadAnim(charaID + typeID + "_SPECIAL_ATTACK_2", ID::IDType::P_SPECIAL_ATTACK_ANIM);
	ResourceLoad::LoadAnim(charaID + typeID + "_SPECIAL_ATTACK_BEFORE", ID::IDType::P_SPECIAL_ATTACK_BEFORE_ANIM);
	ResourceLoad::LoadAnim(charaID + typeID + "_WIN", ID::IDType::P_WIN);
	ResourceLoad::LoadAnim(charaID + typeID + "_WINIDOL", ID::IDType::PLAYER_WIN_IDOL);
	ResourceLoad::LoadAnim(charaID + typeID + "_LEG_UP", ID::IDType::P_LEG_UP);
	ResourceLoad::LoadAnim(charaID + typeID + "_HEAVY_ATTACK", ID::IDType::P_HEAVY_ATTACK);
	ResourceLoad::LoadAnim(charaID + typeID + "_HEAVY_CHARGE", ID::IDType::P_HEAVY_CHARGE);

	anim->AnimDataLoad(charaID, typeID);;

	anim->SetMaxFrame(ID::IDType::P_SPECIAL_ATTACK_BEFORE_ANIM, 35.4f);
	anim->SetMaxFrame(ID::P_GETUP, 53.0f);

	//----------------------------------------------------------------------------------------------------------------------
	
	MotionBlur* blur = playerPointer->Component()->AddComponent<MotionBlur>();

	blur->Start(ResourceLoad::GetHandle(ID::P_MODEL));

	/*ComponentLight* componentLoght = playerPointer->Component()->AddComponent<ComponentLight>();
	componentLoght->SpotLightHandleStart(VECTOR3(0, 0, 0), VECTOR3(0.0f, 0.0f, 0.0f), DX_PI_F, DX_PI_F / 2, 4000, 0.0f, 0.002f, 0.0f);*/

	playerPtr->Start(playerPointer);

	//影の作成
	Object3D* shadow = new Object3D();
	shadow->Init(Transform(VECTOR3(0.0f, -100.0f, 0.0f), VZero, VECTOR3(playerPointer->GetTransform()->scale.x - 1.0f, 0.15f, playerPointer->GetTransform()->scale.z - 1.0f)), "PlayerShadow");
	Shadow* s = shadow->Component()->AddComponent<Shadow>();
	s->Start(57.0f);

	RayCollider* collider4 = shadow->Component()->AddComponent<RayCollider>();
	info.shape = CollsionInformation::RAY;
	info.tag = CollsionInformation::SHADOW;
	collider4->RaySet(info, Transform(VECTOR3(0, 50, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, -s->GetMaxDist(), 0), VZero, VECTOR3(1.0f, 1, 1)), nullptr);

	playerPointer->AddChild(shadow);

	//HPゲージの作成
	Object2D* guage = new Object2D();
	VECTOR2F GUAGEPOS = VECTOR2F(915.0f, 950.0f);
	guage->Init(GUAGEPOS, VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "playerHp");
	playerPointer->AddChild(guage);

	Guage* g = guage->Component()->AddComponent<Guage>();
	g->GuageDrawReady<Player>(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Player_HpBar_GreenBack", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F,Guage::BAR_MODE::HP);
	g->EdgeDrawReady(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Player_HpBar_Frame", ID::HP_EDGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F,Transform(VECTOR3(915.0f,950.0f,0.0f),VZero,VECTOR3(1.0f,1.0f,0.0f)));

	//スペシャルゲージの作成
	Object2D* SpecialGuage = new Object2D();
	
	VECTOR2F SPECIALGUAGEPOS = VECTOR2F(915.0f, 1050.0f);
	SpecialGuage->Init(SPECIALGUAGEPOS, VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "playerSpecialGuage");
	playerPointer->AddChild(SpecialGuage);

	Guage* specialG = SpecialGuage->Component()->AddComponent<Guage>();
	specialG->GuageDrawReady<Player>(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Player_HpBar_YellowBack", ID::P_SPECIAL_ATTACK_BAR), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Guage::BAR_MODE::SPECIAL_ATTACK);
	specialG->EdgeDrawReady(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Player_HpBar_Frame", ID::HP_EDGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Transform(VECTOR3(915.0f, 1050.0f, 0.0f), VZero, VECTOR3(1.0f, 1.0f, 0.0f)));

}

void PlayerManager::GameSceneChangeState()
{
	
	if (stateManager == nullptr) {
		return; //ステートマネージャーがセットされてなかったらリターン
	}
	//現在のゲームステートを見る
	switch (gameManager->GetStateNumber())
	{
	case GameControler::GameState::BEFORE:
		stateManager->ChangeState(StateID::PLAYER_BEFORE_S);
		break;

	case GameControler::GameState::PLAY:
		stateManager->ChangeState(StateID::PLAYER_WAIT_S);
		
		break;

	case GameControler::GameState::BOSS_PLAY_BEFORE:
		stateManager->ChangeState(StateID::PLAYER_BOSS_APPEAR_S);
		break;

	case GameControler::GameState::WIN:
		stateManager->NowChangeState(StateID::PLAYER_WIN_STATE_S);
		stateManager->SetNoStateChange(true);
		break;

	case GameControler::GameState::LOSE:
		stateManager->NowChangeState(StateID::PLAYER_LOSE_S);
		stateManager->SetNoStateChange(true);
		break;

	default:
		break;
	}
}

void PlayerManager::CreateTitlePlayer()
{
	playerPointer = new Object3D();
	playerPointer->SetDrawOrder(-5);

	//タイトルシーン用のプレイヤーの作成
	
	//初期位置
	playerPointer->Init(VECTOR3(300,800, -2000), VZero, VECTOR3(3.0f, 3.0f, 3.0f), "PLAYER");
	//やられ判定の追加
	/*ColliderBase* collider = playerPointer->Component()->AddComponent<SphereCollider>();*/
	CollsionInfo info;
	info.parentTransfrom = playerPointer->GetTransform();
	info.shape = CollsionInformation::SPHERE;
	info.oneColl = false;
	info.tag = CollsionInformation::Tag::PLAYER;
	info.size = 1.0f;
	/*collider->CollsionAdd(info, Transform(VECTOR3(0, 0, 0), VZero, VECTOR3(150.0f, 0, 0)),"playerColl");*/

	//やられ判定の追加
	ColliderBase* collider2 = playerPointer->Component()->AddComponent<SphereCollider>();
	collider2->CollsionAdd(info, Transform(VECTOR3(0, 100, 0), VZero, VECTOR3(200.0f, 0, 0)), nullptr, "playerColl");
	RayCollider* collider3 = playerPointer->Component()->AddComponent<RayCollider>();
	info.shape = CollsionInformation::RAY;
	info.tag = CollsionInformation::P_FLOOR;
	collider3->RaySet(info, Transform(VECTOR3(0, 200, 0), VZero, VECTOR3(1.0f, 1.0f, 1.0f)), Transform(VECTOR3(0, -20, 0), VZero, VECTOR3(1.0f, 1.0f, 1.0f)), nullptr);

	Shaker* shaker = playerPointer->Component()->AddComponent<Shaker>();

	MeshRenderer* me2 = playerPointer->Component()->AddComponent<MeshRenderer>();

	MeshRenderer2D* me2D = playerPointer->Component()->AddComponent<MeshRenderer2D>();
	me2D->SetDraw(false);
	me2D->SetTransform(Transform(VECTOR3(Screen::WIDTH / 2.0f, Screen::HEIGHT / 2.0f, 0.0f), VZero, VECTOR3(192.0f, 1.0f, 0)));
	//me2D->SetTransform(Transform(VECTOR3(Screen::WIDTH / 2.0f, Screen::HEIGHT / 2.0f, 0.0f), VZero, VECTOR3(0.1f,1,0) * 10.0f));

	me2D->TextureHandle(ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "speicialAttackBackSide4", ID::PLAYER_SPECIAL_ATTACK_BACK), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F);

	Anim2D* anim2D = playerPointer->Component()->AddComponent<Anim2D>();
	anim2D->AnimSetting(400.0f, 192);

	me2D->AnimStart(1.0f, 10, true);

	Physics* physics = playerPointer->Component()->AddComponent<Physics>();
	TitlePlayer* player = playerPointer->Component()->AddComponent<TitlePlayer>();

	physics->Start(PlayerInformation::BASE_GRAVITY, PlayerInformation::BASE_INTERIA);
	/*Camera* camera = playerPointer->Component()->AddComponent<Camera>();
	camera->Start(FindGameObject<EnemyManager>()->GetEnemy());*/
	//me2->ModelHandle(Load::LoadModel(Load::MODEL_PATH + "exo_red", ID::P_MODEL));
	me2->ModelHandle(ResourceLoad::LoadModel("Player4", ID::P_MODEL));
	me2->RotationMesh(0, 180.0f * DegToRad);

	stateManager = playerPointer->Component()->AddComponent<StateManager>();
	//stateの設定の方法
	/*stateManager->CreateState<PlayerWait>("PlayerWait", StateID::PLAYER_WAIT_S);
	stateManager->CreateState<PlayerWalk>("PlayerWalk", StateID::PLAYER_WALK_S);
	stateManager->CreateState<PlayerAvoid>("PlayerAvoid", StateID::PLAYER_AVOID_S);
	stateManager->CreateState<PlayerJustAvoid>("PlayerJustAvoid", StateID::PLAYER_JUST_AVOID_S);
	stateManager->CreateState<PlayerAttack1>("PlayerAttack1", StateID::PLAYER_ATTACK1_S);
	stateManager->CreateState<PlayerJustAvoidAttack1>("PlayerJustAvoidAttack1", StateID::PLAYER_JUST_AVOID_ATTACK1_S);
	stateManager->CreateState<PlayerAttack2>("PlayerAttack2", StateID::PLAYER_ATTACK2_S);
	stateManager->CreateState<PlayerJustAvoidAttack1>("PlayerJustAvoidAttack1", StateID::PLAYER_JUST_AVOID_ATTACK1_S);
	stateManager->CreateState<PlayerAttack3>("PlayerAttack3", StateID::PLAYER_ATTACK3_S);
	stateManager->CreateState<PlayerAttack5>("PlayerAttack5", StateID::PLAYER_ATTACK5_S);
	stateManager->CreateState<PlayerAttack4>("PlayerAttack4", StateID::PLAYER_ATTACK4_S);
	stateManager->CreateState<PlayerJustAvoidAttack4>("PlayerJustAvoidAttack4", StateID::PLAYER_JUST_AVOID_ATTACK4_S);
	stateManager->CreateState<PlayerJustAvoidAttack5>("PlayerJustAvoidAttack5", StateID::PLAYER_JUST_AVOID_ATTACK5_S);
	stateManager->CreateState<PlayerDamage>("PlayerDamage", StateID::PLAYER_DAMAGE_S);
	stateManager->CreateState<PlayerBlowAway>("PlayerBlowAway", StateID::PLAYER_BLOW_AWAY_S);
	stateManager->CreateState<PlayerDie>("PlayerDie", StateID::PLAYER_DIE_S);
	stateManager->CreateState<PlayerTurn>("PlayerTurn", StateID::PLAYER_TURN_S);
	stateManager->CreateState<PlayerSpecialAttack>("PlayerSpecialAttack", StateID::PLAYER_SPECIAL_ATTACK_S);
	stateManager->CreateState<PlayerWin>("_PlayerWin", StateID::PLAYER_WIN_STATE_S);
	stateManager->CreateState<PlayerBefore>("_PlayerBefore", StateID::PLAYER_BEFORE_S);*/

	Animator* anim = playerPointer->Component()->AddComponent<Animator>();
	anim->BaseModelSet(ResourceLoad::GetHandle(ID::P_MODEL), "mixamorig:Hips");
	std::string charaID = "3";
	std::string typeID = "000";

	ResourceLoad::LoadAnim(charaID + typeID + "_IDOL1", ID::IDType::P_ANIM_IDOL);
	ResourceLoad::LoadAnim(charaID + typeID + "_RUN_M_1", ID::IDType::P_ANIM_RUN);
	ResourceLoad::LoadAnim(charaID + typeID + "_AVOID_M_1", ID::IDType::P_ANIM_AVOID);
	ResourceLoad::LoadAnim(charaID + typeID + "_JUST_AVOID_M_1", ID::IDType::P_ANIM_JUST_AVOID);
	ResourceLoad::LoadAnim(charaID + typeID + "_DAMAGE_M_1", ID::IDType::P_DAMAGE);
	ResourceLoad::LoadAnim(charaID + typeID + "_FALL_M_1", ID::IDType::P_FALL);
	ResourceLoad::LoadAnim(charaID + typeID + "_GETUP_M_1", ID::IDType::P_GETUP);
	ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK01_M_1", ID::IDType::P_ANIM_ATTACK1);
	ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK02_M_1", ID::IDType::P_ANIM_ATTACK2);
	ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK11_M_1", ID::IDType::P_ANIM_JUST_AVOID_ATTACK1);
	ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK03_M_1", ID::IDType::P_ANIM_ATTACK3);
	ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK05_M_1", ID::IDType::P_ANIM_ATTACK_5);
	ResourceLoad::LoadAnim(charaID + typeID + "_ATTACK04_M_1", ID::IDType::P_ANIM_ATTACK4);
	ResourceLoad::LoadAnim(charaID + typeID + "_DIE_M_1", ID::IDType::P_DIE);
	ResourceLoad::LoadAnim(charaID + typeID + "_TURN1", ID::IDType::P_TURN_ANIM);
	ResourceLoad::LoadAnim(charaID + typeID + "_SPECIAL_ATTACK_2", ID::IDType::P_SPECIAL_ATTACK_ANIM);
	ResourceLoad::LoadAnim(charaID + typeID + "_SPECIAL_ATTACK_BEFORE", ID::IDType::P_SPECIAL_ATTACK_BEFORE_ANIM);
	ResourceLoad::LoadAnim(charaID + typeID + "_WIN", ID::IDType::P_WIN);
	ResourceLoad::LoadAnim(charaID + typeID + "_WINIDOL", ID::IDType::PLAYER_WIN_IDOL);
	ResourceLoad::LoadAnim(charaID + typeID + "_LEG_UP", ID::IDType::P_LEG_UP);
	ResourceLoad::LoadAnim(charaID + typeID + "_HEAVY_ATTACK", ID::IDType::P_HEAVY_ATTACK);
	ResourceLoad::LoadAnim(charaID + typeID + "_HEAVY_CHARGE", ID::IDType::P_HEAVY_CHARGE);

	anim->SetMaxFrame(ID::IDType::P_SPECIAL_ATTACK_BEFORE_ANIM, 35.4f);
	anim->SetMaxFrame(ID::P_GETUP, 53.0f);

	anim->AnimDataLoad(charaID, typeID);;
	//anim->AnimDataLoad("PlayerAnimData");;


	MotionBlur* blur = playerPointer->Component()->AddComponent<MotionBlur>();

	blur->Start(ResourceLoad::GetHandle(ID::P_MODEL));

	/*ComponentLight* componentLoght = playerPointer->Component()->AddComponent<ComponentLight>();
	componentLoght->SpotLightHandleStart(VECTOR3(0, 0, 0), VECTOR3(0.0f, 0.0f, 0.0f), DX_PI_F, DX_PI_F / 2, 4000, 0.0f, 0.002f, 0.0f);*/

	player->Start(playerPointer);

	//影の作成

	Object3D* shadow = new Object3D();
	shadow->Init(Transform(VECTOR3(0.0f, -100.0f, 0.0f), VZero, VECTOR3(playerPointer->GetTransform()->scale.x - 1.0f, 0.1f, playerPointer->GetTransform()->scale.z - 1.0f)), "PlayerShadow");
	Shadow* s = shadow->Component()->AddComponent<Shadow>();
	s->Start(55.0f);

	RayCollider* collider4 = shadow->Component()->AddComponent<RayCollider>();
	info.shape = CollsionInformation::RAY;
	info.tag = CollsionInformation::SHADOW;
	collider4->RaySet(info, Transform(VECTOR3(0, 50, 0), VZero, VECTOR3(1.0f, 1.0, 1.0)), Transform(VECTOR3(0, -s->GetMaxDist(), 0), VZero, VECTOR3(1.0f, 1, 1)), nullptr);

	playerPointer->AddChild(shadow);
}
