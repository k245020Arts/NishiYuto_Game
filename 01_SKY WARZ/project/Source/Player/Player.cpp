#include "player.h"
#include "../State/StateManager.h"
#include "../Player/PlayerState/PlayerStateManager.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../Component/Animator/Anim2D.h"
#include "../Component/Physics/Physics.h"
#include "../Camera/Camera.h"
#include "../Common/InputManager/InputManager.h"
#include "../Common/InputManager/PadInput.h"
#include "../Common/InputManager/KeyboardInput.h"
#include "../../ImGui/imgui.h"
#include "../Component/Transform/Transform.h"
#include "../Component/Animator/Animator.h"
#include "../Camera/cameraManager.h"
#include "../Player/PlayerState/PlayerStateBase.h"
#include "../Component/ComponentManager.h"
#include "../Player/PlayerState/PlayerAvoid.h"
#include "../Component/Shaker/Shaker.h"
#include "../Common/Sound/SoundManager.h"
#include "../Common/Effect/EffectManager.h"
#include "../Weapon/WeaponManager.h"
#include "../Component/MotionBlur/MotionBlur.h"
#include "../Player/PlayerState/PlayerWait.h"
#include "../Player/PlayerState/playerWalk.h"
#include "../Player/PlayerState/AttackState/playerAttack1.h"
#include "../Player/PlayerState/AttackState/playerAttack2.h"
#include "../Player/PlayerState/playerDamage.h"
#include "../Player/PlayerState/playerjustAvoid.h"
#include "../Player/PlayerState/PlayerBlowAway.h"
#include "../Player/PlayerState/PlayerTurn.h"
#include "../Player/PlayerState/PlayerBossAppear.h"
#include "../Player/PlayerState/AttackState/playerJustAvoidAttack1.h"
#include "../Player/PlayerState/AttackState/playerAttack3.h"
#include "../Player/PlayerState/AttackState/playerAttack4.h"
#include "../Player/PlayerState/AttackState/PlayerAttack5.h"
#include "../Player/PlayerState/AttackState/PlayerSpecialAttack.h"
#include "../Common/ResourceLoader.h"
#include "../Enemy/TrashEnemy/TrashEnemy.h"
#include "../Enemy/EnemyState/EnemyStateManager.h"
//#include "../Enemy/TrashEnemy/EnemyState/AttackState/EnemyAttack1.h"
#include "../Common/Transitor/FadeTransitor.h"
#include "../GameControler/GameControler.h"
#include "../Player/PlayerState/PlayerDie.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/EnemyState/Attack/EnemyAttackBase.h"
#include "../Enemy/Boss/BossState/AttackSorting.h"
#include "../Common/Easing.h"
#include "../Component/UI/ButtonUI.h"
#include "PlayerParamWindow.h"
#include "../Common/Debug/DebugLogText.h"
#include "../Component/EnemyAttackObject/BossRock/BossRockBase.h"
#include "../Component/UI/TextRenderer.h"
#include "../Screen.h"

namespace {

	std::unordered_map<StateID::State_ID, PlayerInformation::PlayerReaction> attackEffects; /*= {
	{ StateID::PLAYER_ATTACK1_S, PlayerInformation::PlayerReaction(StateID::PLAYER_ATTACK1_S,VECTOR3(50,50,50), 0.07f, VECTOR3(40,40,40), 0.1f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE) },
	{ StateID::PLAYER_JUST_AVOID_ATTACK1_S, PlayerInformation::PlayerReaction(StateID::PLAYER_JUST_AVOID_ATTACK1_S,VECTOR3(100,100,100), 0.15f, VECTOR3(100,100,100), 0.3f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE) },
	{ StateID::PLAYER_ATTACK2_S, PlayerInformation::PlayerReaction(StateID::PLAYER_ATTACK2_S,VECTOR3(50,50,50), 0.1f, VECTOR3(40,40,40), 0.1f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE) },
	{ StateID::PLAYER_ATTACK3_S, PlayerInformation::PlayerReaction(StateID::PLAYER_ATTACK3_S,VECTOR3(50,50,50), 0.12f, VECTOR3(40,40,40), 0.1f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE) },
	{ StateID::PLAYER_ATTACK5_S, PlayerInformation::PlayerReaction(StateID::PLAYER_ATTACK5_S,VECTOR3(150,100,100), 0.25f, VECTOR3(100,100,100), 0.3f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE) },
	{ StateID::PLAYER_ATTACK4_S, PlayerInformation::PlayerReaction(StateID::PLAYER_ATTACK4_S,VECTOR3(100,100,100), 0.15f, VECTOR3(100,100,100), 0.3f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE) },
	{ StateID::PLAYER_HEAVY_ATTACK_S, PlayerInformation::PlayerReaction(StateID::PLAYER_HEAVY_ATTACK_S,VECTOR3(150,150,150), 0.4f, VECTOR3(100,100,100), 0.3f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE) },

	};*/

	
	const float SPECIAL_UI_INIT_POS_Y = 1025.0f;
}



Player::Player()
{
	playerCom.stateManager		= nullptr;
	avoidReady					= false;
	size						= 0.00f;
	avoidStart					= false;
	enemyHit					= false;
	justAvoid					= false;
	justAvoidCanCounter			= 0.0f;
	avoidCounter				= 0;
	noAvoidCounter				= 0.0f;
	playerTransform				= nullptr;
	debugId						= 14;
	tag							= Function::GetClassNameC<Player>();
	hp							= MAX_HP;
	maxHp						= hp;
	avoidReadyCounter			= 0.0f;
	justAvoidBlurImage			= ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "visionEffect", ID::JUST_AVOID_BLUR);
	justFeedInTime				= 0.0f;
	justFeedOutTime				= 0.0f;
	bossThreat					= false;
	largeJustAvoid				= false;
	noDamage					= false;
	redCounter					= 0.0f;
	playerCom					= PlayerInformation::CharaComponent();
	turn						= false;
	specialAttackCenterPos		= VZero;
	specialAttackStartPos		= VZero;
	specialAttackBar			= 0.0f;
	specialAttackBarMax			= 100.0f;
	attackTargetTrans			= Transform();
	charge						= false;
	attackLevel					= -1;
	specialAttackGuageMax		= false;
	objHit						= false;
	bossRockManager				= nullptr;
	paramWindow					= std::make_unique<PlayerParamWindow>(this);
	justAvoidCan				= false;
	justAvoidColHit				= false;
	specialTextObj				= nullptr;
	hpTextObj					= nullptr;
	hpUIMoveCounter				= 0.0f;
	specialTextPhysics			= nullptr;
	specialMoveCounter			= 0.0f;
}

Player::~Player()
{
	//delete playerCom.stateManager;
	/*DeleteGraph(justAvoidBlurImage);*/
	justAvoidBlurImage = -1;
	//paramWindow.reset();
}

void Player::Update()
{
	//playerCom.stateManager->Update();
	//回避状態が始まるときに回転に補正を掛けるための処理
	//DebugLogText::GetInstance()->Log(LogLevel::INFO, Debug::printfString("playerPositionY = %.3f", playerTransform->position.y));
	//playerCom.physics->GetVelocity().y)
	if (justAvoidColHit) {
		justAvoidCan = true;
	}
	else {
		justAvoidCan = false;
	}
	//playerTransform->position = playerCom.anim->BoneMovePositionAdd(playerTransform->position);
	if (avoidReady) {
		AvoidRotationChange();
	}
	//攻撃のあたりはんていが終わったら削除
	if (playerCom.stateManager->GetState<PlayerAttackStateBase>() != nullptr) {
		if (!playerCom.stateManager->GetState<PlayerAttackStateBase>()->IsAttack()) {
			DeleteCollision(&attackColl);
		}
	}

	//------------------------------デバックボタン---------------------------------------------
	
	if (InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_B)){
		playerCom.stateManager->ChangeState(StateID::PLAYER_SPECIAL_ATTACK_S);
	}
	if (InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_4)) {
		SpecialVarAdd(20.0f);
	}
	if (InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_6)) {
		//playerCom.camera->CutSceneChangeState("test");
		hp -= 5.0f;
	}
	if (InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_M)) {
		noDamage = true;
	}
	if (InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_N)) {
		noDamage = true;
	}

	//------------------------------------------------------------------------------------------

	//必殺技が発動できればUIを不透明度を上げる。
	if (CanSpecialAttack()) {
		playerCom.specialAttackButton->ButtonActiveStart();
		playerCom.specialAttackButton->ColorGradeMode();
	}
	else {
		//必殺技が発動できなければUIの不透明度を下げる
		playerCom.specialAttackButton->ButtonActiveFinish();
		playerCom.specialAttackButton->ColorGradeModeFinish();
	}
	
	//死亡条件
	if (hp <= 0.0f && (playerCom.stateManager->GetState<StateBase>()->GetID() == StateID::PLAYER_WAIT_S)) {
		if (!playerCom.camera->IsCutScene()) {
			//playerCom.gameManager->ChangeState(")
			playerCom.stateManager->NowChangeState(StateID::PLAYER_DIE_S);
			playerCom.stateManager->SetNoStateChange(true);
		}
		
	}
	//ダメージを受けた時の赤点滅
	if (redCounter > 0.0f) {
		redCounter -= Time::DeltaTimeRate();
		if (redCounter <= 0.0f) {
			redCounter = 0.0f;
			playerCom.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
		}
	}
	justAvoidColHit = false;
	//playerCom.physics->AddVelocity(VECTOR3(50.0f, 0.0f, 0.0f), false);
	
	paramWindow->PlayerParamWindowView();
	
	HpUIUpdate();
	SpecialUIUpdate();
}

void Player::Draw()
{
	playerCom.stateManager->Draw();
	playerCom.renderer->Draw();

	//DrawLine3D(playerCom.camera->GetCameraTransform()->position, playerCom.camera->GetCameraTransform()->position + VECTOR3(0, 0, 1000) * obj->GetTransform()->GetRotationMatrix(), 0xffffff);

}

void Player::Start(Object3D* _obj)
{
	//初期化の値を設定
	//必要なコンポーネントを付けている。
	obj						= _obj;
	playerCom.stateManager	= obj->Component()->GetComponent<StateManager>();
	

	playerCom.player		= this;

	ComponentManager* componentMangager		= obj->Component();
	
	playerCom.renderer		= componentMangager->GetComponent<MeshRenderer>();
	playerCom.meshRenderer2D= componentMangager->GetComponent<MeshRenderer2D>();
	playerCom.physics		= componentMangager->GetComponent<Physics>();

	playerCom.camera		= FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<Camera>();

	//playerCom.InputManager = FindGameObject<ImputManager>();

	playerTransform			= obj->GetTransform();

	playerCom.anim			= obj->Component()->GetComponent<Animator>();
	playerCom.anim2D		= obj->Component()->GetComponent<Anim2D>();
	playerCom.anim->Play(ID::P_ANIM_RUN);
	playerCom.color			= obj->Component()->GetComponent<Color>();
	//playerCom.targetObj = FindGameObjectWithTag<Object3D>("ENEMY");
	playerCom.targetObj		= nullptr;
	playerCom.hitObj		= nullptr;
	playerCom.shaker		= componentMangager->GetComponent<Shaker>();

	playerCom.weapon		= FindGameObject<WeaponManager>();
	playerCom.blur			= obj->Component()->GetComponent<MotionBlur>();

	playerCom.gameManager	= FindGameObject<GameControler>();
	playerCom.enemyManager	= FindGameObject<EnemyManager>();

	playerCom.specialAttackButton = FindGameObjectWithTag<Object2D>("XButton")->Component()->GetComponent<ButtonUI>();
	
	avoidStart				= false;
	justAvoidCanCounter		= 0.0f;
	attackColl = CharaBase::CollsionSet(nullptr, CollsionInformation::SPHERE, "p_attack", false, CollsionInformation::P_ATTACK);
	//playerCom.physics->SetVelocity(VECTOR3(10.0f, 5.0f, 0.0f));

	//physics->SetInterect(VECTOR3(5.0f, -1.0f, 0.0f),0.1);
	//ステートのセット
	

	playerCom.stateManager->NodeDrawReady();
	playerCom.stateManager->SetComponent<Player>(this);

	playerCom.stateManager->StartState(StateID::PLAYER_WAIT_S);
	redCounter = 0.0f;
	//3DSoundのベースはプレイヤーに持たせる。
	SoundManager::GetInstance()->Base3DSoundObject(obj);

	std::string filePath = std::string("data/json/") + "PlayerReaction" + ".json";
	JsonReader json;
	json.Load(filePath);
	
	
	attackEffects.clear();
	for (auto& j : json.Data()["PlayerReactions"]) {
		PlayerInformation::PlayerReaction reaction;
		j.get_to(reaction);
		attackEffects[reaction.state] = reaction;
	}


	std::string filePathAttack = std::string("data/json/") + "PlayerAttackData" + ".json";
	JsonReader json2;
	json2.Load(filePathAttack);

	attackData.clear();
	for (auto& j : json2.Data()["PlayerAttackData"]) {
		PlayerAttackStateBase::PlayerAttackData attackD;
		j.get_to(attackD);
		attackData[attackD.state] = attackD;
	}

	DataLoadPlayerState();

	//HPのテキストのセッティング
	hpTextObj = new Object2D();
	hpTextObj->Init(VECTOR2F(610.0f, 925.0f), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "hp");
	TextRenderer* hpText = hpTextObj->Component()->AddComponent<TextRenderer>();
	hpText->UseDrawUI();
	Shaker* shake = hpTextObj->Component()->AddComponent<Shaker>();
	hpText->TextSetting("HP", "MPlus2C", ".dft", LIGHT_GREEN, 4, Font_ID::UI_FONT);
	hpTextObj->GetTransform()->position.x -= hpText->GetTextWidth();

	//必殺技のテキストのセッティング
	specialTextObj = new Object2D();
	specialTextObj->Init(VECTOR2F(610.0f, SPECIAL_UI_INIT_POS_Y), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 1.0f), "special");
	TextRenderer* specialText = specialTextObj->Component()->AddComponent<TextRenderer>();
	/*specialTextPhysics = specialTextObj->Component()->AddComponent<Physics>();
	specialTextPhysics->Start(VECTOR3(0.0f, 6.0f, 0.0f), VECTOR3(0.0f, 16.0f, 0.0f));*/
	specialText->TextSetting("SPECIAL", "MPlus2C", ".dft", YELLOW, 4, Font_ID::UI_FONT);
	specialTextObj->GetTransform()->position.x -= specialText->GetTextWidth();
	specialText->UseDrawUI();

	//playerCom.stateManager->DataSaveState();

	/*nlohmann::json& root = json.Data();
	if (!root.contains("PlayerReactions")) {
		root["PlayerReactions"] = nlohmann::json::object();
	}
	for (auto& p : attackEffects) {

		std::string key = StateID::GetID(p.second.state);
		root["PlayerReactions"][key] = p.second;

		json.Save(filePath, root);
	}*/
	
}

void Player::Move(float _speed, float _speedMax)
{
	//if (CheckHitKey(KEY_INPUT_RIGHT)) {
	//	//VECTOR3 power = com.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);
	//	//if (power.Size() <= 1.0f) {
	//	playerCom.physics->AddVelocity(VECTOR3(50.0f, 0.0f, 0.0f), false);
	//	//}

	//}
	PlayerStickInput(); //InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_LEFT)
	
	std::shared_ptr<PlayerStateBase> pB = playerCom.stateManager->GetState<PlayerStateBase>();

	StickDirections stick = InputManager::GetInstance()->GetControllerInput()->GetStickKnockingReverce(0.6f, 8).leftStick;
	StickDirections nowStick = InputManager::GetInstance()->GetControllerInput()->GetStickKnocking(0.6f, 1).leftStick;
	
	//スティックの傾きの量が少なかったら移動しない
	const float DEAD_ZONE = 0.3f;
	if ((fabs(walkAngle.x) >= DEAD_ZONE || fabs(walkAngle.z) >= DEAD_ZONE) && hp > 0.0f) {
		//回転処理
		RotationChange(walkAngle,12.0f/*スピード*/);

		VECTOR3 dir = VZero;
		dir.x		= walkAngle.x * 1.0f * _speed;
		dir.z		= walkAngle.z * 1.0f * _speed;
		dir.y		*= 0.0f;
		dir			= dir * MGetRotY(playerCom.camera->GetCameraTransform()->rotation.y);
		playerCom.physics->AddVelocity(dir, false);
		VECTOR3 moveVelo;
		moveVelo	= playerCom.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);
		
		float max	= _speedMax;
		size		= moveVelo.SquareSize();

		//最大速度までいったらスピードマックスに補正
		if (moveVelo.SquareSize() >= max * max) {
			moveVelo = moveVelo.Normalize() * _speedMax;
			//上でYのvelocityを0にしているので元のvelocity.yを代入
			moveVelo.y = playerCom.physics->GetVelocity().y;
			playerCom.physics->SetVelocity(moveVelo);
		}
		playerCom.stateManager->ChangeState(StateID::PLAYER_WALK_S);
		//アニメーションのスピードを傾き方で測定
		if (InputManager::GetInstance()->GetControllerInput()->GetIsPadInput()) {
			playerCom.anim->SetPlaySpeed(walkAngle.Size());
		}
		else {
			//キーボードの時は一定にする
			playerCom.anim->SetPlaySpeed(1.0f);
		}
		
		/*if (nowStick == S_NO_DIRECTION || stick == S_NO_DIRECTION) {
			return;
		}
		if (nowStick == stick) {
			playerCom.stateManager->ChangeState(StateID::PLAYER_TURN_S);
		}*/
	}
	else {
		playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
	}
}

void Player::RotationChange(VECTOR3 _angle,float _speed)
{
	VECTOR3 forward = VECTOR3(0, 0, 1) * MGetRotY(playerTransform->rotation.y);
	VECTOR3 right	= VECTOR3(1, 0, 0) * MGetRotY(playerTransform->rotation.y);
	VECTOR3 target	= _angle * MGetRotY(playerCom.camera->GetCameraTransform()->rotation.y);
	float dot = VDot(target.Normalize(), forward.Normalize());	//コサインの値が正面ベクトルとカメラの角度を計算
	//内積を使って補正
	if (dot >= cosf(_speed * DegToRad)) {
		float inRot = atan2f(target.x, target.z);
		playerTransform->rotation.y = inRot;
		avoidStart					= true;
	}
	else{
		if (VDot(right, target) > 0) {
			playerTransform->rotation.y += _speed * DegToRad;
		}
		else {
			playerTransform->rotation.y -= _speed * DegToRad;
		}
		avoidStart = false;
	}
}

void Player::RotationChange()
{
	RotationChange(walkAngle,5.0f);
}

void Player::Avoid(float _speed, float _speedMax, float cameraAngle, float _upSpeed)
{
	//回避の共通処理
	VECTOR dir	= VZero;
	dir			= VECTOR3(0.0f, 0.0f, _speed) * MGetRotY(playerTransform->rotation.y);
	//回避の時に空中に浮かせるためにyの値をプラス
	dir.y		+= _upSpeed;
	playerCom.physics->SetVelocity(dir);

}

void Player::ImguiDraw()
{
	ImGui::InputFloat("size", &size);
	ImGui::Text(playerCom.stateManager->GetState<StateBase>()->GetString().c_str());
	//playerCom.stateManager->StateNodeDraw();
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
	/*bool open = ImGui::TreeNodeEx("PlayerState", flags);
	if(open){
		if (ImGui::Button("p_Damage")) {
			playerCom.stateManager->ChangeState(ID::P_DAMAGE);
		}
		if (ImGui::Button("p_attack1")) {
			playerCom.stateManager->ChangeState(ID::P_ANIM_ATTACK1);
		}
		if (ImGui::Button("p_attack2")) {
			playerCom.stateManager->ChangeState(ID::P_ANIM_ATTACK2);
			}
		if (ImGui::Button("p_blowWay")) {
			playerCom.stateManager->ChangeState(ID::P_BLOWAWAY);
		}
		if (ImGui::Button("justAvoid")) {
			playerCom.stateManager->ChangeState(ID::P_ANIM_JUST_AVOID);
		}
		if (ImGui::Button("playerDie")) {
			playerCom.stateManager->NowChangeState(ID::P_DIE);
			playerCom.stateManager->SetNoStateChange(true);
		}
		ImGui::TreePop();
	}*/

	//static int screen = MakeScreen(256, 256, TRUE);
	//static int modelHandle = Load::GetHandle(ID::P_MODEL);

	//// モデル描画先をレンダーターゲットに
	//SetDrawScreen(screen);
	//ClearDrawScreen();
	//MV1DrawModel(modelHandle);

	//SetDrawScreen(DX_SCREEN_BACK); // 元に戻す

	//// D3D11 のテクスチャを取得
	//ID3D11ShaderResourceView* srv = GetUseDirect3D11ShaderResourceView(modelHandle);

	//// ImGui で画像を表示
	//ImGui::Image((ImTextureID)modelHandle, ImVec2(256, 256));

}

void Player::PlayerStickInput()
{
	
	if (InputManager::GetInstance()->GetControllerInput()->GetIsPadInput()) {
		//スティックの角度とり
		walkAngle = VECTOR3(InputManager::GetInstance()->GetControllerInput()->GetStickInput().leftStick.x, 0.0f, InputManager::GetInstance()->GetControllerInput()->GetStickInput().leftStick.y);
	}
	else {
		KeyBoardPlayerRotate();
	}
	
}

void Player::AvoidReady()
{
	/*if (avoidCounter <= 0) {
		return;
	}*/
	//回避のスタート処理
	if (justAvoidCan) {
		playerCom.stateManager->ChangeState(StateID::PLAYER_JUST_AVOID_S);

		playerCom.enemyManager->JustAvoidTargetChange(dynamic_cast<Object3D*>(playerCom.hitObj));
		playerCom.camera->TargetSet(playerCom.hitObj);
		Debug::DebugLog("JustAvoid");
		avoidStart = false;
		avoidReady = false;
		justAvoidCanCounter = 0.0f;
		justAvoid = true;
		justFeedInTime = JUST_FEED_IN_TIME;
		SpecialVarAdd(10.0f);
	}
	else {
		PlayerStickInput();
		avoidReady = true;
		justAvoidCanCounter = 0.0f;
		avoidCounter--;
		noAvoidCounter = 2.0f;
		avoidReadyCounter = 0.00f;
	}

}

void Player::AvoidRotationChange()
{
	//回避するときにどれくらいの速度で回転させるかを示す処理
	float speed = 20.0f;
	RotationChange(walkAngle,speed);
	if (avoidReadyCounter >= 0.0f) {
		avoidReadyCounter	-= Time::DeltaTimeRate();
	}
	else {
		justAvoidCanCounter -= Time::DeltaTimeRate();
	}
	
	if (avoidStart && justAvoidCanCounter <= 0.0f && avoidReadyCounter <= 0.0f) {
		playerCom.stateManager->ChangeState(StateID::PLAYER_AVOID_S);
		avoidStart	= false;
		avoidReady	= false;
		justAvoid	= false;
		
	}
}

bool Player::EnemyHit(ID::IDType _attackId,BaseObject* _obj)
{
	//敵の攻撃が当たった時の処理
	std::shared_ptr<StateBase> pB	= playerCom.stateManager->GetState<StateBase>();
	Animator* enemyAnim				= _obj->Component()->GetComponent<Animator>();
	std::shared_ptr<AttackSorting> sorthing = _obj->Component()->GetComponent<StateManager>()->GetState<AttackSorting>();
	std::shared_ptr<EnemyAttackBase> attack;
	//AttackSorthingがあるならAttackSorthingの中から攻撃を取得
	if (sorthing != nullptr) {
		attack = sorthing->GetNowAttackState();
	}
	else {
		attack = _obj->Component()->GetComponent<StateManager>()->GetState<EnemyAttackBase>();
	}
	
	float startTime					= enemyAnim->EventStartTime(_attackId);
	bool damage						= false;
	StateID::State_ID state = pB->GetID();
	if (state == StateID::PLAYER_JUST_AVOID_ATTACK1_S && playerCom.hitObj == _obj) {
		return true;
	}
	//ジャスト回避が出来るならスルーして敵の攻撃が当たっていない返り値で返す
	if (justAvoid)
		return true;
	//敵が攻撃をしてなかったらスルーして敵の攻撃が当たっていない返り値で返す
	if (attack == nullptr)
		return true;

	EnemyAttackBase::BossAttackParam param = attack->GetDamageParam();
	//ジャスト回避が出来る処理
	//if (justAvoidCanCounter > 0.0f && avoidReadyCounter <= 0.0f) {
	//	if (enemyAnim->GetCurrentFrame() <= startTime + 8.0f || startTime >= 0.0f) {
	//		/*if (!LargeJustAvoid(attack)) {
	//			return true;
	//		}*/
	//		
	//	}
	//}
	//else {
	//	//ジャスト回避が出来るようになったらスルー
	//	if (enemyAnim->GetCurrentFrame() <= startTime + 8.0f && attack != nullptr) {
	//		return false;
	//	}
	//	else {
	//		//出来なかったらダメージを食らう
	//		damage = true;
	//	}
	//	
	//}
	if (justAvoidCan) {
		damage = false;
	}
	else {
		damage = true;
	}
	
	//ダメージが入ったらパラメーターのセット
	if (pB->GetID() != StateID::PLAYER_AVOID_S) {
		if (damage) {
			if (attack == nullptr) {
				if (!noDamage) { //無敵モードじゃないなら
					hp -= param.hitDamage;
				}
			}
			else {
				if (!noDamage) { //無敵モードじゃないなら
					hp -= param.hitDamage;
				}

			}
			if (pB->GetID() != StateID::PLAYER_HEAVY_CHARGE_S && pB->GetID() != StateID::PLAYER_SPECIAL_ATTACK_S) {
				InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(80, 30);
				//playerCom.stateManager->ChangeState(StateID::PLAYER_DAMAGE_S);
				switch (param.damagePattern)
				{
				case EnemyAttackBase::NO_BACK:
					playerCom.stateManager->ChangeState(StateID::PLAYER_DAMAGE_S);
					break;
				case EnemyAttackBase::BACK:
					playerCom.stateManager->ChangeState(StateID::PLAYER_DAMAGE_S);
					break;
				case EnemyAttackBase::BLOW_AWAY:
					playerCom.stateManager->ChangeState(StateID::PLAYER_BLOW_AWAY_S);
					break;
				default:
					my_error_assert("ダメージの状態が入っていません");
					break;
				}
			}
			const float HP_UI_MOVE_TIME = 0.5f;
			hpUIMoveCounter = HP_UI_MOVE_TIME;
			Shaker* shaker = hpTextObj->Component()->GetComponent<Shaker>();
			shaker->ShakeStart(VECTOR3(10.0f, 10.0f, 10.0f), Shaker::ShakePattern::HEIGHT_SHAKE, false, HP_UI_MOVE_TIME);
			TextRenderer* hpText = hpTextObj->Component()->GetComponent<TextRenderer>();
			hpText->SetColor(RED);
			playerCom.color->setRGB(RED);
			redCounter = 0.5f;
			playerCom.hitObj = _obj;
			//hp -= playerCom.hitObj->Component()->GetComponent<Enemy>()->GetStateManager()->GetState<EnemyAttack1>()->GetHitDamage();
			SoundManager::GetInstance()->RandamSe("EnemyAttackHit", 4);
			SoundManager::GetInstance()->RandamSe("P_DamageV", 2);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0, 50, 0), VZero, VOne * 6.0f), obj, Effect_ID::PLAYER_HIT, 1.0f);

		}
	}
	return damage;
}

void Player::TargetObjSet(BaseObject* _base)
{
	if (_base == nullptr) {
		playerCom.camera->TargetSet(nullptr);
	}
	playerCom.targetObj = _base;
}

void Player::HitObjectSet(BaseObject* _base)
{
	playerCom.hitObj = _base;
}

void Player::PlayerAttackHit()
{
	//プレイヤーの攻撃を当てた時のカメラのシェイクヒットストップの処理
	StateID::State_ID id = playerCom.stateManager->GetState<StateBase>()->GetID();

	auto it = attackEffects.find(id);
	if (it != attackEffects.end()) {
		const auto& e = it->second;
		playerCom.shaker->ShakeStart(e.shakePower, e.shakePattern, e.shakerLoop, e.shakeTime);
		playerCom.camera->CameraShake(e.cameraShakePower, e.shakePattern, false, e.cameraShakeTime);
		SoundManager::GetInstance()->RandamSe(e.soundName, e.soundKind);
		SpecialVarAdd(1.0f);
	}
}

bool Player::IsShake() const
{
	return playerCom.shaker->IsShakeing();
}

StateManager* Player::GetPlayerStateManager()const
{
	return playerCom.stateManager;
}

void Player::AvoidFinishState()
{
	PlayerStickInput();
	//スティックをある程度傾けていたら移動の処理、傾けていなかったら止まる処理
	if (fabs(walkAngle.x) >= 0.3f || fabs(walkAngle.z) >= 0.3f) {
		playerCom.stateManager->ChangeState(StateID::PLAYER_WALK_S);
	}
	else {
		playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
	}
}

void Player::DrawTrail()
{
	//--------------------定数----------------------

	const VECTOR3 NEAR_POS = VECTOR3(-23, -4, -200);
	const VECTOR3 FAR_POS = VECTOR3(23, 4, 16);
	const float R = 0.0f;
	const float G = 0.0f;
	const float B = 255.0f;
	const float A = 100.0f;
	const int INDEX = 28;
	const float TIME = 0.25f;

	//-----------------------------------------------

	DrawTrail(NEAR_POS, FAR_POS, R, G, B, A, INDEX, TIME);
}

void Player::DrawTrail(const VECTOR3& _nPos, const VECTOR3& _fPos, float _r, float _g, float _b, float _a, int index, float _time)
{
	//剣の軌跡の処理
	playerCom.weapon->CreateTrailPlayer(_nPos,_fPos,_r,_g,_b,_a,index,_time);
}

void Player::DeleteCollision(CollsionSet* _set)
{
	CharaBase::DeleteCollision(_set);
}

bool Player::EnemyAttackObjectHitIsPlayer(BaseObject* _obj, CollsionInformation::Tag _tag)
{
	//敵の攻撃が当たった時の処理
	if (noDamage) {
		return true;
	}
	if (justAvoid) {
		return true;
	}
	
	objHit = true;
	struct PlayerDamage
	{
		PlayerDamage() {
			damage = 0.0f;
			changeID = StateID::STATE_MAX;
			moveAdd = VZero;
		}
		PlayerDamage(float _damage, StateID::State_ID _id,VECTOR3 _move) {
			damage = _damage;
			changeID = _id;
			moveAdd = _move;
		}
		float damage; //ダメージ
		StateID::State_ID changeID; //ダメージを食らった時になるステート
		VECTOR3 moveAdd; //仰け反り量
	};
	//当たり判定の情報を記述
	std::unordered_map< CollsionInformation::Tag, PlayerDamage> damageParam
	{
		{CollsionInformation::ROCK_BLAST_DAMAGE, { PlayerDamage(100.0f,StateID::PLAYER_BLOW_AWAY_S,VECTOR3(0,0,-1000)) } },
		{ CollsionInformation::B_E_ATTACK,{PlayerDamage(50.0f,StateID::PLAYER_DAMAGE_S,VECTOR3(0,0,-1000))} },
		{ CollsionInformation::BOSS_ROCK_ATTACK,{PlayerDamage(20.0f,StateID::PLAYER_DAMAGE_S,VECTOR3(0,0,-1000))} },
		{ CollsionInformation::THROW_OBJECT_GROUND,{PlayerDamage(20.0f,StateID::PLAYER_DAMAGE_S,VECTOR3(0,0,-1000))} },
		{ CollsionInformation::THROW_OBJECT_GROUND_ONE_HIT,{PlayerDamage(20.0f,StateID::PLAYER_DAMAGE_S,VECTOR3(0,0,-1000))} },
		{ CollsionInformation::THROW_OBJECT_GROUND_NO_DAMAGE_REACTION,{PlayerDamage(2.0f,StateID::STATE_MAX,VECTOR3(0,0,0))} },

	};

	PlayerDamage& param = damageParam[_tag];
	std::shared_ptr<StateBase> pB = playerCom.stateManager->GetState<StateBase>();
	bool damage = false;
	BaseObject* parent = _obj->GetParent();
	StateID::State_ID state = pB->GetID();
	if (state == StateID::PLAYER_JUST_AVOID_ATTACK1_S && playerCom.hitObj == _obj) {
		return true;
	}
	if (parent != nullptr) {
		playerCom.hitObj = parent;
	}
	else {
		playerCom.hitObj = _obj;
	}
	
	//ジャスト回避が出来る処理
	//if (justAvoidCanCounter > 0.0f && avoidReadyCounter <= 0.0f) {

	//	playerCom.stateManager->ChangeState(StateID::PLAYER_JUST_AVOID_S);
	//	Debug::DebugLog("JustAvoid");
	//	avoidStart = false;
	//	avoidReady = false;
	//	justAvoidCanCounter = 0.0f;
	//	justAvoid = true;
	//	justFeedInTime = JUST_FEED_IN_TIME;
	//	largeJustAvoid = true;
	//}
	//else {
	//	//出来なかったらダメージを食らう
	//	damage = true;
	//}
	/*if (justAvoidCan) {
		damage = false;
	}
	else {
		damage = true;
	}*/
	//ダメージが入ったらパラメーターのセット
	damage = true;
	if (damage) {
		if (pB->GetID() != StateID::PLAYER_AVOID_S) {
			InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(80, 30);
			if (param.changeID != StateID::STATE_MAX) {
				playerCom.stateManager->ChangeState(param.changeID);
			}
			const float HP_UI_MOVE_TIME = 0.5f;
			hpUIMoveCounter = HP_UI_MOVE_TIME;
			Shaker* shaker = hpTextObj->Component()->GetComponent<Shaker>();
			shaker->ShakeStart(VECTOR3(10.0f, 10.0f, 10.0f), Shaker::ShakePattern::HEIGHT_SHAKE, false, HP_UI_MOVE_TIME);
			TextRenderer* hpText = hpTextObj->Component()->GetComponent<TextRenderer>();
			hpText->SetColor(RED);
			playerCom.physics->AddVelocity(param.moveAdd,false);
			hp -= param.damage;
			//hp -= playerCom.hitObj->Component()->GetComponent<Enemy>()->GetStateManager()->GetState<EnemyAttack1>()->GetHitDamage();
			SoundManager::GetInstance()->RandamSe("EnemyAttackHit", 4);
			SoundManager::GetInstance()->RandamSe("P_DamageV", 2);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0, 50, 0), VZero, VOne * 6.0f), obj, Effect_ID::PLAYER_HIT, 1.0f);
		}
	}
	return true;
}

bool Player::LargeJustAvoid(std::shared_ptr<EnemyAttackBase> _attack)
{
	if (_attack->GetDamageParam().useFlash) {
		largeJustAvoid = true;
	}
	else {
		largeJustAvoid = false;
	}
	return largeJustAvoid;
}

void Player::SpecialVarAdd(float _add)
{
	float add = specialAttackBar + _add;
	//必殺技ゲージがMaxより小さいと進む
	if (add >= specialAttackBarMax) {
		if (!specialAttackGuageMax) {
			specialAttackGuageMax = true;
			SoundManager::GetInstance()->PlaySe(Sound_ID::PLAYER_SPECIAL_ATTACK_CAN);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0.0f, 150.0f, 0.0f), VZero, VOne), obj, Effect_ID::PLAYER_SPECIAL_ATTACK_CAN, 1.0f);
		}
	}
	specialAttackBar = std::clamp(add, 0.0f, specialAttackBarMax);
}

void Player::AttackRockHit()
{
	const float shakeTime = 0.35f;
	playerCom.shaker->ShakeStart(VOne * 50.0f/*power*/, Shaker::MIX_SHAKE, true, shakeTime/*time*/);
	playerCom.camera->CameraPerspectiveShakeStart(5.0f/*power*/, shakeTime/*time*/);
	InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(80/*power*/, 30/*time*/);
}

void Player::JustAvoidCollsionHit(BaseObject* _obj, CollsionInformation::Tag _tag)
{
	std::shared_ptr<StateBase> pB = playerCom.stateManager->GetState<StateBase>();
	StateID::State_ID state = pB->GetID();
	//ジャスト回避状態ならジャスト回避判定をスルー
	if (state == StateID::PLAYER_JUST_AVOID_S) {
		return;
	}
	if (_tag == CollsionInformation::JUST_AVOID) {
		playerCom.hitObj = _obj;
	}
	else {
		playerCom.hitObj = _obj;
	}
	
	justAvoidColHit = true;
}

std::unordered_map<StateID::State_ID, PlayerInformation::PlayerReaction>& Player::GetReactionMap()
{
	return attackEffects;
}

std::unordered_map<StateID::State_ID, PlayerAttackStateBase::PlayerAttackData>& Player::GetAllAttackData()
{
	return attackData;
}

void Player::ParamReLoad()
{
	DataLoadPlayerState();
}

void Player::ReactionReLoad()
{
	std::string filePath = std::string("data/json/") + "PlayerReaction" + ".json";
	JsonReader json;
	json.Load(filePath);


	attackEffects.clear();
	for (auto& j : json.Data()["PlayerReactions"]) {
		PlayerInformation::PlayerReaction reaction;
		j.get_to(reaction);
		attackEffects[reaction.state] = reaction;
	}
}

void Player::CollsionEvent(const CollsionEventData& _data)
{
	if (_data.targetTag == CollsionInformation::B_ATTACK) {
		Boss* b = _data.targetObject->Component()->GetComponent<Boss>();
		bool damage = EnemyHit(b->GetStateManager()->GetState<EnemyStateBase>()->GetAnimId(), b->GetEnemyObj());
		if (!damage) {
			_data.targetColliderBase->CollsionRespown();
		}
	}
	if (_data.targetTag == CollsionInformation::E_ATTACK) {
		TrashEnemy* e = _data.targetObject->Component()->GetComponent<TrashEnemy>();
		bool damage = EnemyHit(e->GetStateManager()->GetState<EnemyStateBase>()->GetAnimId(), e->GetEnemyObj());
		if (!damage) {
			_data.targetColliderBase->CollsionRespown();
		}
	}
	if (_data.targetTag == CollsionInformation::B_E_ATTACK) {
		bool damage = EnemyAttackObjectHitIsPlayer(_data.targetObject, _data.targetTag);
		if (!damage) {
			_data.targetColliderBase->CollsionRespown();
		}
	}

	if (_data.targetTag == CollsionInformation::BOSS_ROCK_ATTACK || 
	_data.targetTag == CollsionInformation::THROW_OBJECT_GROUND ||
	_data.targetTag == CollsionInformation::THROW_OBJECT_GROUND_ONE_HIT ||
	_data.targetTag == CollsionInformation::THROW_OBJECT_GROUND_NO_DAMAGE_REACTION){
		bool damage = EnemyAttackObjectHitIsPlayer(_data.targetObject, _data.targetTag);
		if (!damage) {
			_data.targetColliderBase->CollsionRespown();
		}
	}

	if (_data.targetTag == CollsionInformation::ROCK_BLAST_DAMAGE) {
		BossRockBase* r = _data.targetObject->Component()->GetComponent<BossRockBase>();
		if (r->HitObjects(_data.targetObject)) {
			return;
		}
		bool damage = EnemyAttackObjectHitIsPlayer(_data.targetObject, _data.targetTag);
		if (!damage) {
			_data.targetColliderBase->CollsionRespown();
		}
	}

	if (_data.targetTag == CollsionInformation::JUST_AVOID_BOSS ||
		_data.targetTag == CollsionInformation::JUST_AVOID_ENEMY) {
		JustAvoidCollsionHit(_data.targetObject, _data.targetTag);
	}

	if (_data.targetTag == CollsionInformation::JUST_AVOID) {
		BaseObject* _obj = _data.targetObject;

		if (_obj->GetParent() != nullptr) {
			_obj = _obj->GetParent();
		}

		JustAvoidCollsionHit(_obj, _data.targetTag);
	}
		
}

void Player::CollsionAttackEvent(const CollsionEventData& _data)
{
	if (IsHitObject(_data.targetObject)) {
		return;
	}
	HitObjectInsert(_data.targetObject);
	hitObjects;
	if (_data.targetTag == CollsionInformation::BOSS_ROCK_ATTACK) {
		AttackRockHit();
	}
	else {
		PlayerAttackHit();
	}
}

void Player::HeavyAttackChangeParam(int _level)
{
	// ---- リアクション ----
	auto& param = attackEffects[StateID::PLAYER_HEAVY_ATTACK_S];
	attackLevel = _level;

	const auto* reactionParam = param.GetChargeLevel(attackLevel);
	//my_error_assert(reactionParam, "チャージ攻撃のレベルがセットされていません");

	param.shakePower = reactionParam->shakePower;
	param.shakeTime = reactionParam->shakeTime;
	param.cameraShakePower = reactionParam->cameraShakePower;
	param.cameraShakeTime = reactionParam->cameraShakeTime;
}

void Player::DataSavePlayerState()
{
	std::string filePath = std::string("data/json/") + "PlayerAttackData" + ".json";
	JsonReader json;
	nlohmann::json& root = json.Data();
	if (!root.contains("PlayerAttackData")) {
		root["PlayerAttackData"] = nlohmann::json::object();
	}
	auto states = playerCom.stateManager->GetStateInfo();
	for (auto& t : states) {
		std::shared_ptr<PlayerAttackStateBase>p = std::dynamic_pointer_cast<PlayerAttackStateBase>(t.second);

		if (p != nullptr) {
			PlayerAttackStateBase::PlayerAttackData data = p->GetAttackData();
			std::string key = StateID::GetID(data.state);
			root["PlayerAttackData"][key] = data;

			json.Save(filePath, root);
		}
	}
}

void Player::DataLoadPlayerState()
{
	auto states = playerCom.stateManager->GetStateInfo();
	for (auto& t : states) {
		std::shared_ptr<PlayerAttackStateBase>p = std::dynamic_pointer_cast<PlayerAttackStateBase>(t.second);

		if (p != nullptr) {
			p->SetAttackData();
		}
	}
}

void Player::HpUIUpdate()
{
	if (hpUIMoveCounter <= 0.0f) {
		return;
	}
	hpUIMoveCounter -= Time::DeltaTimeRate();
	if (hpUIMoveCounter <= 0.0f) {
		TextRenderer* hpText = hpTextObj->Component()->GetComponent<TextRenderer>();
		hpText->SetColor(LIGHT_GREEN);
	}

}

void Player::SpecialUIUpdate()
{
	if (CanSpecialAttack()) {
		specialMoveCounter += Time::DeltaTimeRate() * 0.8f;
	}
	else {
		specialMoveCounter = 0.0f;
		specialTextObj->GetTransform()->scale = VOne;
		return; //スペシャルがたまってない状態なら何もしないのでそのまま返す
	}
	VECTOR3 move = Easing::SinCube(VOne * 0.95f, VOne * 1.05f, specialMoveCounter);
	specialTextObj->GetTransform()->scale = move;
}

void Player::KeyBoardPlayerRotate()
{
	const float LEFT_MOVE_ROTATE = -1.0f;
	const float RIGHT_MOVE_ROTATE = 1.0f;
	const float FRONT_MOVE_ROTATE = 1.0f;
	const float BACK_MOVE_ROTATE = -1.0f;
	bool isFrontMove = false;
	bool isSideMove = false;
	if (InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_A)) {
		walkAngle.x = LEFT_MOVE_ROTATE;
		isSideMove = true;
	}
	if (InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_W)) {
		walkAngle.z = FRONT_MOVE_ROTATE;
		isFrontMove = true;
	}
	if (InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_S)) {
		walkAngle.z = BACK_MOVE_ROTATE;
		isFrontMove = true;
	}
	if (InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_D)) {
		walkAngle.x = RIGHT_MOVE_ROTATE;
		isSideMove = true;
	}
	if(!isFrontMove) {
		walkAngle.z = 0.0f;
	}
	if (!isSideMove) {
		walkAngle.x = 0.0f;
	}
}