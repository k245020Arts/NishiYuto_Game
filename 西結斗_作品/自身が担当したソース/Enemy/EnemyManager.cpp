#include "EnemyManager.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "../Component/Transform/Transform.h"
#include "../Component/MeshRenderer2D/MeshRenderer2D.h"
#include "../Player/Player.h"
#include "../Component/Physics/Physics.h"
#include "../Camera/Camera.h"
#include "../Component/Hierarchy/Hierarchy.h"
#include "../Common/ResourceLoader.h"
#include "../Component/Collider/sphereCollider.h"
#include "../Component/Animator/Animator.h"
//#include "TrashEnemy/enemy.h"
#include "../Weapon/WeaponManager.h"
#include "../Component/Shaker/Shaker.h"
#include "../Component/Object/Object2D.h"
#include "../Component/Guage/Guage.h"
#include "Boss/Boss.h"
#include "../Component/Collider/rayCollider.h"
#include "../Component/Collider/ModelCollider.h"
#include "../Component/Shadow/Shadow.h"
#include "../Common/Easing.h"
#include "../GameControler/GameControler.h"
#include "../State/StateManager.h"
#include "../Stage/StageSelectData.h"
#include "Boss/BossCreater.h"
//#define VERSION2D
//#define DOT_MODE

EnemyManager::EnemyManager()
{
	//CreateEnemy();
	enemy.clear();
	player = nullptr;
	SetDrawOrder(-10);
	cameraTargetObj = nullptr;
	gameContorler = nullptr;
	new BossCreater();
}

EnemyManager::~EnemyManager()
{
	enemy.clear();
	chara.clear();
	cameraTargetObj = nullptr;
	gameContorler = nullptr;
}

void EnemyManager::Update()
{
	if (gameContorler == nullptr) {
		gameContorler = FindGameObject<GameControler>();
	}
	
	if (gameContorler != nullptr && gameContorler->GetChangeStateOneFrame()) {
		GameSceneChangeState();
	}
}

void EnemyManager::Draw()
{
}

void EnemyManager::DebugDrawCamera(Camera* camera)
{
	
}

void EnemyManager::CreateEnemy()
{
	
	//e->Start(enemy);
	/*chara.emplace_back(en);
	
	enemy.emplace_back(e);*/
}

void EnemyManager::PlayerObjPointer()
{
	
	for (auto itr = enemy.begin(); itr != enemy.end(); itr++) {
		//Enemy* e = (*itr)->Component()->GetComponent<Enemy>();
		//e->PlayerPointerSet(obj);
		//e->Start((*itr));
		(*itr)->SetDrawOrder(-5);

		Object2D* guage = new Object2D();

		guage->Init(VECTOR2F(970, 0), VECTOR2F(0.0f, 0.0f), VECTOR2F(1.0f, 0.5f), "enemyHpGuage");

		(*itr)->AddChild(guage);

		Guage* g = guage->Component()->AddComponent<Guage>();
		//g->GuageDrawReady<Enemy>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHpGuage", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Guage::BAR_MODE::HP);
	}
	
	player->Component()->GetComponent<Player>()->TargetObjSet(nullptr);
	player->Component()->GetComponent<Player>()->HitObjectSet(nullptr);
	FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>()->TargetSet(*enemy.begin());
}


void EnemyManager::FindPlayer()
{
	Object3D* obj = FindGameObjectWithTag<Object3D>("PLAYER");
	player = obj;
}

void EnemyManager::AddList(EnemyBase* _enemy, BaseObject* _obj)
{
	chara.emplace_back(_enemy);
	enemy.emplace_back(_obj);
}

void EnemyManager::RemoveList(EnemyBase* _enemy, BaseObject* _obj)
{

	//死んでた時にカメラのロックオンされた敵がいたのならロックオンを解除
	if (cameraTargetObj != nullptr) {
		if (_obj == cameraTargetObj) {
			TargetCancel(nullptr);
		}
	}

	for (auto itr = chara.begin(); itr != chara.end();) {
		if (*itr == _enemy) {
			itr = chara.erase(itr);
		}
		else {
			itr++;
		}
	}

	
	for (auto itrObj = enemy.begin(); itrObj != enemy.end();) {
		if (*itrObj == _obj) {
			itrObj = enemy.erase(itrObj);
		}
		else {
			itrObj++;
		}
	}
	
}

bool EnemyManager::ChangeCameraRockOn(Camera* camera,bool _right)
{
	return ChangeCameraRockOn(camera, _right, true,false);
}

bool EnemyManager::ChangeCameraRockOn(Camera* camera, bool _right,bool _min)
{
	return ChangeCameraRockOn(camera, _right, _min, false);
}

bool EnemyManager::ChangeCameraRockOn(Camera* camera, bool _right, bool _min, bool _debugMode)
{
	if (chara.size() <= 1) {
		return false;
	}
	const float CAM_LONG_DISTANCE = 100000.0f;
	float distance = 1000000;
	VECTOR3 dir = camera->GetTarget() - camera->GetCameraTransform()->position;
	VECTOR3 camPos = camera->GetCameraTransform()->position;
	VECTOR3 camFront = camera->GetCameraTransform()->Forward();
	dir = dir.Normalize();
	auto keepItr = chara.end();
	auto lastItr = chara.end();
	float invertMinMax = 0.0f;
	if (_min) {
		invertMinMax = 1000.0f;;
	}
	else {
		invertMinMax = -1000.0f;;
	}
	float dist2DMax = 10000.0f;
	auto itrs = chara.begin();
	
	float dotMax = 0.0f;
	for (; itrs != chara.end(); itrs++) {
		if ((*itrs)->GetLastTarget()) {
			lastItr = itrs; //最後にロックオンされていた敵のイテレーター
			break;
		}
	}
	VECTOR3 tarPos;
	VECTOR3 fowardPos;
	VECTOR3 lastPos2D;
	//これまでターゲットに入ってなかったらプレイヤーの位置を参照
	if (itrs == chara.end()) {
		tarPos = player->GetTransform()->position;
		fowardPos = player->GetTransform()->Forward();

	}
	else {
		tarPos = (*lastItr)->GetBaseObject()->GetTransform()->position;
		fowardPos = (*lastItr)->GetBaseObject()->GetTransform()->Forward();

		/*tarPos = player->GetTransform()->position;
		fowardPos = player->GetTransform()->Forward();*/
	}
	VECTOR3 targetEnemyPos = tarPos;

	VECTOR3 targetFrontPos = camFront;
	lastPos2D = ConvWorldPosToScreenPos(targetEnemyPos);
	VECTOR3 camDir;
	if (_right) {
		camDir = targetFrontPos * MGetRotY(90.0f * DegToRad);
	}
	else {
		camDir = targetFrontPos * MGetRotY(-90.0f * DegToRad);
	}

	int num = 0;
	if (!_debugMode) {
		int b = 0;
	}
	//一番近い敵を検出
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		if ((*itr)->GetLastTarget()) {
			lastItr = itr; //前にロックオンされていた敵のイテレーター
			num++;
			continue;
		}
		VECTOR3 enemyPos = (*itr)->GetBaseObject()->GetTransform()->position;

		VECTOR3 enemyFrontPos = enemyPos * (*itr)->GetBaseObject()->GetTransform()->Forward();

		camPos.y = 0.0f;
		enemyPos.y = 0.0f;

		VECTOR3 enemyTarget = enemyPos - camPos;

		//DrawLine3D(camPos, enemyPos, 0xffffff);

		//Enemy側から見た右はプレイヤーに対してだと左側になるのでEnemyの左側を判定する
		//VECTOR3 enemyRight = enemyPos * VECTOR3(1, 0, 0);

		camDir.y = 0;
		enemyTarget.y = 0;


		//targetDir.z = 0.0f;

		float camDot = VDot(camDir, enemyTarget);
		//右側か左側の指定した方向に対して0以上なら通るようにする
		if (camDot < 0.0f * DegToRad) {
			num++;
			continue;
		}
#ifdef VERSION2D
		//VECTOR3 target = camPos - enemyPos;
		////より右側に近い敵を判定するために内積を取る。
		//target.y = 0.0f;
		//camFront.y = 0.0f;

		//DOTO 右ベクトル的に一番短い敵のポジションを判定
		/*VECTOR3 target = enemyPos - targetEnemyPos;
		target.y = 0;*/

		VECTOR3 pos2D = ConvWorldPosToScreenPos(enemyPos);

		float dist2D = lastPos2D.x - pos2D.x;

		DrawLine3D(targetEnemyPos, enemyPos, 0xff000f);

		/*VECTOR3 sideDir = targetDir.Normalize();*/

		/*float sideDist = VDot(target, sideDir);*/

		float dist = VECTOR3(camPos - targetEnemyPos).Size();
		//距離が一定以上あったら無視
		if (dist >= 100000) {
			continue;
		}
		distance = dist;
		if (fabs(dist2D) < dist2DMax) {
			dist2DMax = fabs(dist2D);
			keepItr = itr;
		}

		#elseifdef DOT_MODE

			VECTOR3 dotEnemyPos = enemyPos - camPos;
		dotEnemyPos.y = 0.0f;
		camFront.y = 0;
		DrawLine3D(camPos, enemyPos, 0xff000f);
		float dist = VECTOR3(camPos - targetEnemyPos).Size();

		float dot = VDot(camFront.Normalize(), dotEnemyPos.Normalize());
		if (dist >= 100000) {
			continue;
		}
		distance = dist;

		if (dot > dotMax) {
			dotMax = dot;
			keepItr = itr;
		}
#else

		float score = 0.0f;

		VECTOR3 dotEnemyPos = enemyPos - camPos;
		dotEnemyPos.y = 0.0f;
		camFront.y = 0;
		//DrawLine3D(camPos, enemyPos, 0xff000f);

		//正面角度の反映
		float frontDot = VDot(camFront.Normalize(), dotEnemyPos.Normalize());
		//正面角度のスコアの計算
		float frontScore = (1.0f - frontDot) * 1.0f;

		//どれだけ横側にいるかどうかの判定
		//右に傾けると右側により右側にいるかどうか
		//左に傾けるとより左側にいるかどうか
		
		//左右角度の反映
		float sideDot = VDot(camDir.Normalize(), dotEnemyPos.Normalize());
		//左右方向のスコアの計算
		float sideScore = sideDot * 0.5f;

		//距離の遠さの反映
		float dist = VECTOR3(camPos - targetEnemyPos).Size();
		//距離のスコアの計算
		float distScore = (dist / CAM_LONG_DISTANCE) * 0.3f;

		score = frontScore + sideScore + distScore;

		//評価値が低い場合と高い場合の2パターンがある。
		//基本は評価値が低い場合で行う
		if (_min) {
			if (score < invertMinMax) {
				invertMinMax = score;
				keepItr = itr;
			}
		}
		else {
			if (score > invertMinMax) {
				invertMinMax = score;
				keepItr = itr;
			}
		}
		
		//VECTOR3 pos2D = ConvWorldPosToScreenPos(enemyPos);
		//DrawFormatString(pos2D.x, pos2D.y,0xffffff,"front: %.2f  side: %.2f  dist: %.2f  total: %.2f",frontScore, sideScore, distScore, score);
#endif 
		num++;
	}
	if (_debugMode) {
		return false;
	}
	//一定距離離れているとターゲットに入れない
	if (keepItr != chara.end()) {
		if (lastItr != chara.end()) {
			(*lastItr)->LastTargetOut();
		}
		SetCameraRockOnObject((*keepItr), camera);
		return true;
	}
	//右にいる敵がいなかったら、左にいる一番評価値が低い敵にロックオンを起動させる。
	bool next = (_right) ? false : true;
	bool rockOn = ChangeCameraRockOn(camera, next, false, false);
	my_assert(rockOn, "ロックオンの変更が失敗しました。");
	return true;
}

bool EnemyManager::TargetCancel(Camera* camera)
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		if ((*itr)->GetBaseObject() == cameraTargetObj) { //ロックオンしてたら
			(*itr)->LastTargetOut();
			player->Component()->GetComponent<Player>()->TargetObjSet(nullptr);
			cameraTargetObj = nullptr;
			return true;
		}
	}
	return false;
}

void EnemyManager::JustAvoidTargetChange(BaseObject* _obj)
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		if ((*itr)->GetBaseObject() == _obj) { //オブジェクトが一致したら
			(*itr)->LastTargetIn();
			cameraTargetObj = _obj;
			player->Component()->GetComponent<Player>()->TargetObjSet((*itr)->GetBaseObject());
			continue;
		}
		if ((*itr)->GetLastTarget()) {
			(*itr)->LastTargetOut();
			continue;
		}
	}
}

EnemyBase* EnemyManager::PlayerNearEnemy()
{
	if (chara.empty()) {
		return nullptr;
	}
	float minDist = 100000.0f;
	EnemyBase* nearEnemy = nullptr;
	for (auto itrs = chara.begin(); itrs != chara.end(); itrs++) {
		VECTOR3 dir = (*itrs)->GetBaseObject()->GetTransform()->position - player->GetTransform()->position;
		float dist = dir.Size();
		if (dist <= minDist) { //一番距離が近い敵を判定
			minDist = dist;
			nearEnemy = (*itrs);
		}
	}
	return nearEnemy;
}

void EnemyManager::NearEnemyAlpha(const VECTOR3& camPos)
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		VECTOR3 dist = (*itr)->GetBaseObject()->GetTransform()->position - camPos;
		if (dist.Size() <= 1000.0f) {
			float rate = dist.Size() / 1000.0f;
			int alpha = Easing::Lerp(-700, 255, rate);//0距離にするとほんとの意味で0距離にしないと透明にならないので余裕を持たしている
			if (alpha < 0) { //余裕を持たした分0に丸める
				alpha = 0;
			}
			(*itr)->SetAlpha((int)alpha);
		}
		else {
			(*itr)->SetAlpha(255);
		}
	}
}

Transform EnemyManager::NearEnemyPos(const VECTOR3& _pos)
{

	if (chara.size() == 0) { //リストに何も入ってなかったら空のTransformを付ける
		return Transform();
	}
	float nearDist = 10000.0f;
	Transform nearTransform = *(*chara.begin())->GetBaseObject()->GetTransform();
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		VECTOR3 dist = (*itr)->GetBaseObject()->GetTransform()->position - _pos;
		if (dist.Size() <= nearDist) { //一番近い敵
			nearTransform = *(*itr)->GetBaseObject()->GetTransform();
			nearDist = dist.Size();
		}
	}
	return nearTransform;
}

Transform EnemyManager::NearFovEnemyPos(Transform& _transform, float _angle)
{
	if (chara.size() == 0) {//リストに何も入ってなかったら空のTransformを付ける
		return Transform();
	}
	float nearDist = 10000.0f;
	Transform nearTransform = *(*chara.begin())->GetBaseObject()->GetTransform();
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		VECTOR3 dir = _transform.Forward();
		dir = dir.Normalize();

		VECTOR3 target = _transform.position - (*itr)->GetBaseObject()->GetTransform()->position ;
		float dist = target.Size();

		if (VDot(dir, target.Normalize()) < cosf(_angle * DegToRad)) {
			continue;
		}
		if (dist <= nearDist) {
			nearTransform = *(*itr)->GetBaseObject()->GetTransform();
			nearDist = dist;
		}
	}
	return nearTransform;
}

void EnemyManager::SetCameraRockOnObject(EnemyBase* _enemy, Camera* _camera)
{
	if (_enemy == nullptr) { //指定したオブジェクトがnullptrならリターン
		return;
	}
	_enemy->LastTargetIn();
	player->Component()->GetComponent<Player>()->TargetObjSet(_enemy->GetBaseObject());
	_camera->TargetSet(_enemy->GetBaseObject());
	cameraTargetObj = _enemy->GetBaseObject();
}

bool EnemyManager::CameraRockOnStart(Camera* _camera)
{
	EnemyBase* chara = PlayerNearEnemy();
	SetCameraRockOnObject(chara, _camera);
	return (chara != nullptr); //ロックオンが成功したらtrue
}

EnemyAttackChangeCameraDirection EnemyManager::BossAttackCamera(Camera* camera, const Transform& _targetTransform)
{
	//ボスが攻撃を始めたらこの関数を呼ぶ
	//カメラの視野が90°より敵側に向いていなかったら敵の方向に向けて左側に分けるか右側に分けるかを決定する。
	VECTOR3 dir = camera->GetTarget() - camera->GetCameraTransform()->position;
	dir = dir.Normalize();

	VECTOR3 target = _targetTransform.position - player->GetTransform()->position;
	float dist = target.Size();

	if (VDot(dir, target.Normalize()) > cosf(90.0f * DegToRad)) {
		return EnemyAttackChangeCameraDirection::NONE;
	}

	VECTOR3 right = target.Normalize() * MGetRotY(90.0f * DegToRad);
	VECTOR3 tar = player->GetTransform()->position - _targetTransform.position;
	tar.y = 0;
	//右ベクトルに対して正面なら右向き
	if (VDot(right, tar) > 0) {
		
		return EnemyAttackChangeCameraDirection::RIGHT;
	}
	else {
		
		return EnemyAttackChangeCameraDirection::LEFT;
	}
}

bool EnemyManager::CameraInEnemy()
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		VECTOR3 pos = (*itr)->GetBaseObject()->GetTransform()->position;
		if (!CheckCameraViewClip(pos)) {
			return true;
		}
	}
	return false;
}

void EnemyManager::CanPlayerSpecialHit()
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		(*itr)->SpecialAttackHit();
	}
}

void EnemyManager::SleepAllEnemy(bool _sleep)
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		(*itr)->GetBaseObject()->SetSleep(_sleep);
	}
}

void EnemyManager::GameSceneChangeState()
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		BaseObject* obj = (*itr)->GetBaseObject();
		//ボスの行動を制御
		if (obj != nullptr && obj->GetTag() == "Boss") {
			StateManager* stateManager = obj->Component()->GetComponent<StateManager>();
			switch (gameContorler->GetStateNumber())
			{
			case GameControler::GameState::BEFORE:
				// stateManager->ChangeState(StateID::PLAYER_BEFORE_S);
				break;

			case GameControler::GameState::PLAY:
				stateManager->ChangeState(StateID::BOSS_IDOL_S);
				break;

			case GameControler::GameState::BOSS_PLAY_BEFORE:
				 stateManager->ChangeState(StateID::BOSS_APPEAR_S);
				 //stateManager->SetNoStateChange(true);
				break;

			case GameControler::GameState::WIN:
				stateManager->NowChangeState(StateID::BOSS_LOSE_S);
				stateManager->SetNoStateChange(true);
				break;

			case GameControler::GameState::LOSE:
				stateManager->NowChangeState(StateID::BOSS_WIN_S);
				stateManager->SetNoStateChange(true);
				break;

			default:
				break;
			}
			return;
		}
	}
	
}

int EnemyManager::PlayerFovEnemyNum(Transform* _pTransform, float _angle)
{
	int num = 0;
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		VECTOR3 dir = VECTOR3(0,0,1) * MGetRotY(_pTransform->rotation.y);
		dir = dir.Normalize();

		VECTOR3 target = (*itr)->GetBaseObject()->GetTransform()->position - _pTransform->position;
		float dist = target.Size();

		if (VDot(dir, target.Normalize()) >= cosf(_angle * DegToRad)) {
			num++;
		}
	}
	return num;
}

bool EnemyManager::ObjectIsEnemy(BaseObject* _base)
{
	for (auto itr = chara.begin(); itr != chara.end(); itr++) {
		if ((*itr)->GetBaseObject() == _base) {
			return true;
		}
	}
	return false;
}




	/*for (auto itr = enemy.begin(); itr != enemy.end(); itr++) {
		CharaBase* e;
		if ((*itr)->GetTag() == "ENEMY") {
			e = (*itr)->Component()->GetComponent<Enemy>();
		}
		else {
			e = (*itr)->Component()->GetComponent<Boss>();
		}
		
		if (e == nullptr) {
			continue;
		}
		if (e->GetLastTarget()) {
			lastItr = itr;
			continue;
		}
		VECTOR3 target = (*itr)->GetTransform()->position - player->GetTransform()->position;
		float dist = target.Size();
		
		if (VDot(dir.Normalize(), target.Normalize()) < cosf(60.0f * DegToRad)) {
			continue;
		}
		if (distance > dist) {
			distance = dist;
			keepItr = itr;
		}
	}

	if (distance < 100000) {
		
		if ((*lastItr)->GetTag() == "ENEMY") {
			(*lastItr)->Component()->GetComponent<Enemy>()->LastTargetOut();
		}
		else {
			(*lastItr)->Component()->GetComponent<Boss>()->LastTargetOut();
		}
		if ((*keepItr)->GetTag() == "ENEMY") {
			(*keepItr)->Component()->GetComponent<Enemy>()->LastTargetIn();
		}
		else {
			(*keepItr)->Component()->GetComponent<Boss>()->LastTargetIn();
		}
		player->Component()->GetComponent<Player>()->TargetObjSet(*keepItr);
		camera->TargetSet(*keepItr);
		return true;
	}
	if ((*lastItr)->GetTag() == "ENEMY") {
		(*lastItr)->Component()->GetComponent<Enemy>()->LastTargetOut();
	}
	else {
		(*lastItr)->Component()->GetComponent<Boss>()->LastTargetOut();
	}
	player->Component()->GetComponent<Player>()->TargetObjSet(nullptr);*/
