#include "TrashEnemyGroup.h"
#include "TrashEnemy.h"
#include "../../Common/Random/Random.h"
#include "../../Camera/Camera.h"
#include "../../Common/Effect/EffectManager.h"
#include "../../Common/Sound/SoundManager.h"

TrashEnemyGroup::TrashEnemyGroup()
{
	camera = FindGameObjectWithTag<Object3D>("CAMERA_OBJ")->Component()->GetComponent<Camera>();
	trashEnemyManager = FindGameObject<TrashEnemyManager>();

	attackCounter = 0;

	enemiesRunCounter = 0;

	leaderPos = VZero;
	hitEnemyPos = VZero;

	rangedDamageMove = false;

	rangedAtkCounter = 0;

	rangedJoinCounter = 0;

	startRangedAtk = false;

	startButtonImage = false;

	leaderRotY = 0.0f;

	yButtonImage = LoadGraph("data/image/YButton.png");
	SetDrawOrder(-300000);

	leaderActiveEnd = false;

	hitBack = false;

	rangedAtkCoolTime = MaxCoolTime;

	rangedAtkTime = 0.0f;
	atkCountStart = true;

	isMeleeECooperateAtk = false;

	setPrepare = false;
	prepareCounter = 0;
}

TrashEnemyGroup::~TrashEnemyGroup()
{
	DeleteGraph(yButtonImage);
	camera = nullptr;
	trashEnemyManager = nullptr;
}

void TrashEnemyGroup::Update()
{
	Separation();
	//近距離の敵関連
	EnemyDead(meleeEnemies);
	CloseWayPoint();
	for (auto melee : meleeEnemies)
	{
		EnemiesRun(melee);
		MeleeEnemyAttack(melee);
		CooperateAttackMove(melee);
		if (startRangedAtk)
			MeleeEvadeMove(melee);
	}
	atkCountStart = true;
	//遠距離の敵関連
	EnemyDead(rangedEnemies);
	for (auto ranged : rangedEnemies)
	{
		RangedEnemySetWaypoint(ranged);
	}

	NextLeader();

	if (rangedDamageMove)
		RangedDamageMove();
}

void TrashEnemyGroup::Draw()
{
	CooperateAttackLine();

	if(startButtonImage)
		DrawGraph((int)YButtonPos.x, (int)YButtonPos.y, yButtonImage, true);
}

void TrashEnemyGroup::SetMeleeEnemy(TrashEnemy* _enemy)
{
	meleeEnemies.push_back(_enemy);
}

void TrashEnemyGroup::SetRangedEnemy(TrashEnemy* _enemy)
{
	rangedEnemies.push_back(_enemy);
}

void TrashEnemyGroup::EnemyDead(std::vector<TrashEnemy*>& enemies)
{
	for (auto itr = enemies.begin(); itr != enemies.end(); )
	{
		//雑魚敵が死んでたらlistから削除する
		if (!(*itr)->GetActive())
		{
			(*itr)->GetEnemyObj()->DestroyMe();
			//次の要素のイテレータが返る
			itr = enemies.erase(itr);
		}
		else
			++itr;
	}
}

void TrashEnemyGroup::Separation()
{
	VECTOR pos1 = { 0,0,0 };
	VECTOR pos2 = { 0,0,0 };
	const float E_SIZE = 600;

	allEnemy.clear();

	for (auto& melee : meleeEnemies)
	{
		allEnemy.push_back(melee);
	}
	for (auto& ranged : rangedEnemies)
	{
		allEnemy.push_back(ranged);
	}

	for (auto& itr1 : allEnemy)
	{
		for (auto& itr2 : allEnemy)
		{
			if (itr1 == itr2)
				continue;

			pos1 = itr1->GetPos();
			pos2 = itr2->GetPos();
			VECTOR3 vec = pos1 - pos2;
			VECTOR3 vec2 = pos2 - pos1;

			vec.y = 0.0f;
			vec2.y = 0.0f;

			//エネミーの分散
			if (vec.Size() <= E_SIZE)
			{
				itr1->AddPos(vec.Normalize());
				itr2->AddPos(vec2.Normalize());
			}
		}
	}
}

void TrashEnemyGroup::InCameraWayPoint(WayPoint& _wayPoint)
{
	VECTOR3 camPos = camera->GetCameraTransform()->position;
	camPos.y = 0;
	const float ViewingAngle = 45.0f;
	//正面べく
	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(camera->GetCameraTransform()->rotation.y);
	VECTOR3 vec = _wayPoint.position - camPos;

	//内積
	float dotProduct = VDot(frontVec, vec.Normalize());
	//カメラに写ってるか
	if (dotProduct > cosf(ViewingAngle * DegToRad))
		_wayPoint.active = true;
	//カメラに写ってなかったら
	else
		_wayPoint.active = false;
}

int TrashEnemyGroup::GetActiveEnemy()const
{
	return GetMeleeActiveEnemy() + GetRangedActiveEnemy();
}

int TrashEnemyGroup::GetMeleeActiveEnemy()const
{
	int _counter = 0;
	float _hp = 0;

	for (auto& itr : meleeEnemies)
	{
		if(itr->GetActive())
			_counter++;
	}
	return _counter;
}

int TrashEnemyGroup::GetRangedActiveEnemy()const
{
	int _counter = 0;
	float _hp = 0;

	for (auto& itr : rangedEnemies)
	{
		if (itr->GetActive())
			_counter++;
	}

	return _counter;
}

int TrashEnemyGroup::GetMeleeZeroHpEnemy() const
{
	int _counter = 0;
	float _hp = 0;

	for (auto& itr : meleeEnemies)
	{
		if (itr->GetCurrentHp() > _hp)
			_counter++;
	}
	return _counter;
}

int TrashEnemyGroup::GetRangedZeroHpEnemy() const
{
	int _counter = 0;
	float _hp = 0;

	for (auto& itr : rangedEnemies)
	{
		if (itr->GetCurrentHp() > _hp)
			_counter++;
	}

	return _counter;
}

void TrashEnemyGroup::MeleeEnemyAttack(TrashEnemy* _enemy)
{
	if (_enemy->IsCooperateAtk() || !_enemy->IsAtkStandby() || startRangedAtk || setPrepare)
		return;
	if(atkCountStart)
	{
		attackCounter += Time::DeltaTimeRate();
		atkCountStart = false;
	}
	const float MaxAtkTime = maxAttackCounter * 2;

	if (attackCounter >= MaxAtkTime)//壁対策
	{
		_enemy->AttackCommand();
		attackCounter = 0;
		maxAttackCounter = ATK_COUNTER_MIN + ATK_COUNTER_MAX * (float)Random::GetReal();
	}
	else if (attackCounter >= maxAttackCounter)//攻撃のクールタイム
	{
		if (_enemy->IsAttack())
		{
			_enemy->AttackCommand();
			attackCounter = 0;
			maxAttackCounter = ATK_COUNTER_MIN + ATK_COUNTER_MAX * (float)Random::GetReal();
		}
	}
	else if (attackCounter >= maxAttackCounter + ATK_COUNTER_MIN)//ク－ルタイム入ってない敵がいないとき
		_enemy->AttackCoolTimeReset();
}

void TrashEnemyGroup::EnemiesRun(TrashEnemy* _enemy)
{
	if (!_enemy->GetIsRunState() && !_enemy->IsCooperateAtk())
		return;
	
	VECTOR3 _pos = VZero;

	if (_enemy->GetPointNumber() != 0)
		_pos = trashEnemyManager->GetWayPointPosition()[_enemy->GetPointNumber() - 1];
	else
		_pos = trashEnemyManager->GetPlayerPos();

	_enemy->SetWayPoint(_pos);
}

void TrashEnemyGroup::CloseWayPoint()
{
	if (!setPrepare)
		return;
	if (prepareCounter <= 1.0f)
	{
		prepareCounter += Time::DeltaTimeRate();
		return;
	}

	std::vector<WayPoint> wayPoint = trashEnemyManager->PlayerWayPoint();

	VECTOR3 position = camera->GetCameraTransform()->position;
	position.y = 0;
	//正面べく
	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(camera->GetCameraTransform()->rotation.y);
	float counter = 0;
	VECTOR3 savePos = INFINITY;

	for (auto enemy : meleeEnemies)
	{
		for (auto& itr : wayPoint)
		{
			//使っていいウェイポイントを一回だけ探す
			if (counter == 0)
			{
				InCameraWayPoint(itr);
			}
			//一番近いウェイポイントを探す
			if (itr.active)
			{
				float vec = VSize(itr.position - enemy->GetPos());
				if (savePos.Size() > vec)
					savePos = itr.position;
			}
		}
		enemy->SetCooperateWayPoint(savePos, StateID::COOPERATEATTACK1);
		counter = 1;
	}

	setPrepare = false;
	prepareCounter = 0;
}

void TrashEnemyGroup::CooperateAttackMove(TrashEnemy* _enemy)
{
	if (!_enemy->IsCooperateAtk())
	{
		isMeleeECooperateAtk = false;
		return;
	}
	
	int enemiesMax = (int)meleeEnemies.size();
	isMeleeECooperateAtk = true;

	//連携攻撃のときにその敵が準備完了したかどうか
	if (_enemy->GetStandby())
		standbyCounter++;
	if (standbyCounter >= 1)
		cooperateCounter += Time::DeltaTimeRate();

	const float MaxTime = 3.0f;
	//敵全員が準備完了するか時間経過で攻撃に移る
	if (standbyCounter >= enemiesMax || cooperateCounter >= MaxTime)
	{
		AllChangeMeleeState(StateID::T_ENEMY_RUN_S);
		standbyCounter = 0;
		cooperateCounter = 0;
	}

	if (_enemy->GetCAttack())
		AllChangeMeleeState(StateID::COOPERATEATTACK1);
}

void TrashEnemyGroup::AllChangeMeleeState(StateID::State_ID _id)
{
	if (GetActiveEnemy() == 0)
		return;

	for (auto& itr : meleeEnemies)
	{
		if (itr->IsMovingToPlayer())
			itr->ChangeState(_id);
		else
			itr->CooperateAtkFinish();
	}
}

void TrashEnemyGroup::CooperateAttackLine()
{
	if (meleeEnemies.size() <= 0)
		return;

	std::vector<VECTOR3> copyPos;
	const float PosY = 300.0f;

	for (auto& itr : meleeEnemies)
	{
		if (!itr->IsCooperateAtk())
			return;
		if (itr->GetCurrentHp() <= 0)
			continue;

		copyPos.push_back(VECTOR3(itr->GetPos().x, itr->GetPos().y + PosY, itr->GetPos().z));
	}

	const size_t Counter = copyPos.size() - 1;
	const int Color = GetColor(255, 0, 0);
	const float Size = 20.0f;

	for (int i = 0; i < Counter; i++)
	{
		DrawCapsule3D(copyPos[i], copyPos[i + 1] , Size, 0, Color, Color, true);
	}

	copyPos.clear();
}

void TrashEnemyGroup::MeleeEvadeMove(TrashEnemy* _enemy)
{
	if (_enemy->GetCurrentHp() <= 0.0f)
		return;

	_enemy->SetLeaderPos(leaderPos);
	_enemy->SetLeaderRotY(leaderRotY);

	_enemy->ChangeState(StateID::T_ENEMY_EVADE);
}

void TrashEnemyGroup::RangedEnemyAttack()
{
	if (rangedAtkCoolTime <= MaxCoolTime)
	{
		rangedAtkCoolTime += Time::DeltaTimeRate();
		return;
	}

	if (isMeleeECooperateAtk)
		return;

	for (auto& enemy : rangedEnemies)
	{
		if (enemy->GetDeadMove() || enemy->IsPlayerSpecialMove())
			return;

		if (enemy->GetEnemyType() == EnemyType::RANGED_LEADER)
			AttackLeaderMove(enemy);
		else
			AttackRangedMove(enemy);
	}
	
	rangedAtkTime += Time::DeltaTimeRate();
}

void TrashEnemyGroup::DeadMeleeEnemy()	
{
	for (auto& itr : meleeEnemies)
	{
		itr->ChangeHp(-itr->GetMaxHp());
	}
}

void TrashEnemyGroup::DeadRangedEnemy(bool _readerDead)
{
	for (auto& itr : rangedEnemies)
	{
		if (_readerDead)
		{
			if (itr->GetEnemyType() == EnemyType::RANGED_LEADER)
			{
				itr->ChangeHp(-itr->GetMaxHp());
				return;
			}
		}
		else
		{
			itr->ChangeHp(-itr->GetMaxHp());
		}
	}
}

void TrashEnemyGroup::RangedEnemySetWaypoint(TrashEnemy* _enemy)
{
	if (_enemy->GetEnemyType() == EnemyType::RANGED)
	{
		VECTOR3 lPos = leaderPos;
		lPos.y = 0.0f;
		_enemy->SetWayPoint(lPos);
	}
	else
	{
		const VECTOR3 CameraPos = camera->GetCameraTransform()->position;
		const float CamRotY = camera->GetCameraTransform()->rotation.y;
		const VECTOR3 Forward = VECTOR3(sinf(CamRotY), 0, cosf(CamRotY));
		const float Distance = 5000;

		VECTOR3 _wayPointPos = CameraPos + Forward * Distance;

		_wayPointPos.y = 0;

		_enemy->SetWayPoint(_wayPointPos);

		leaderPos = _enemy->GetPos();
	}
}

void TrashEnemyGroup::RangedDamageMove()
{
	for (auto& enemy : rangedEnemies)
	{
		const float Damage = -enemy->MaxHp();
		if (enemy->GetEnemyType() == EnemyType::RANGED_LEADER)
		{
			enemy->ChangeHp(Damage);
		}
		else
		{
			if (!enemy->IsMovingToPlayer())
				enemy->ChangeHp(Damage);
			else
				enemy->ChangeState(StateID::T_ENEMY_WAITSEE);
		}
	}

	rangedDamageMove = false;

	trashEnemyManager->SetStartRangedAttack(false);
}

void TrashEnemyGroup::AllChangeRangedState(StateID::State_ID _id)
{
	for (auto& enemy : rangedEnemies)
		enemy->ChangeState(_id);
}

void TrashEnemyGroup::NextLeader()
{
	bool readerActive = false;
	const size_t Max = rangedEnemies.size();

	if (Max <= 0)
		return;

	for (auto& enemy : rangedEnemies)
	{
		if (enemy->GetEnemyType() == EnemyType::RANGED_LEADER)
		{
			readerActive = true;
			return;
		}
	}
	//リーダーが死んでた時別の敵をリーダーにする
	if (!readerActive)
	{
		rangedEnemies[0]->SetEnemyType(EnemyType::RANGED_LEADER);
		AllChangeRangedState(StateID::T_ENEMY_WAITSEE);
	}
}

void TrashEnemyGroup::DeadRangedEnemy(TrashEnemy* _enemy)
{
	rangedDamageMove = true;
	_enemy->ChangeHp(-_enemy->MaxHp());

	EndRangedAttack(_enemy);
	
	FindGameObject<TrashEnemyManager>()->SetStartRangedAttack(false);
}

void TrashEnemyGroup::AttackLeaderMove(TrashEnemy* _enemy)
{
	if (hitBack)
		return;
	
	if (rangedJoinCounter == 0)//リーダー以外の敵を数える
	{
		const int _Counter = (int)rangedEnemies.size() - 1;
		if (_Counter > 0)
			rangedJoinCounter = _Counter;//リーダーをのぞくため
		else
			return;
	}

	_enemy->ChangeState(StateID::T_ENEMY_STAYSKY);
	leaderRotY = _enemy->GetRot().y;

	startRangedAtk = true;

	if (_enemy->GetStandby())//リーダーが他の奴に指示を出す
	{
		if (rangedJoinCounter > 0)
			leaderActiveEnd = true;
		else
			DeadRangedEnemy(_enemy);
	}

	leaderPos = _enemy->GetPos();

	if (rangedJoinCounter <= rangedAtkCounter || (int)rangedEnemies.size() - 1 <= 0)//敵全員が攻撃を終えた後の処理または移動中に他が死んだとき
	{
		if (rangedAtkTime >= MaxAttackCounter )
		{
			//遠距離の敵のステートを変える
			AllChangeRangedState(StateID::T_ENEMY_WAITSEE);
			EndRangedAttack(_enemy);
			//近距離の敵のステートを変える
			AllChangeMeleeState(StateID::T_ENEMY_RUN_S);
			FindGameObject<TrashEnemyManager>()->SetStartRangedAttack(false);
		}
	}
}

void TrashEnemyGroup::AttackRangedMove(TrashEnemy* _enemy)
{
	float pointCounter = 0.0f;
	const float MaxPoint = 8.0f;
	const float Range = 700.0f;

	//リーダーの周りにポイント配置
	if (pointCounter < MaxPoint && !_enemy->GetStandby() && !_enemy->IsMovingToPlayer())
	{
		//均等に割って円形に配置
		float angle = (2.0f * DX_PI_F) * pointCounter / MaxPoint;

		//回転を反映した方向
		VECTOR3 rotatedDir = VECTOR3(cosf(angle), 0, sinf(angle));
		//リーダーからの絶対座標
		VECTOR3 target = leaderPos + rotatedDir * Range;
		//指定したポイントを渡す
		_enemy->SetCooperateWayPoint(target);
		_enemy->ChangeState(StateID::T_ENEMY_STAYSKY);

		startButtonImage = true;

		pointCounter++;
	}

	//敵がダメージをくらった後の処理
	if (_enemy->GetCooperateDamageMove())
	{
		const VECTOR3 enemyPos = _enemy->GetPos();
		hitEnemyPos = enemyPos;
		hitBack = true;
		
		const float LeaderVecMax = 1200.0f;
		if (VSize(leaderPos - enemyPos) <= LeaderVecMax)//リーダーと打ち返された敵との距離が近くなったら倒す
		{
			SoundManager::GetInstance()->PlaySe(Sound_ID::SOUND_ID::RANGED_ATK_FIN);
			EffectManager::GetInstance()
				->CreateEffekseer(*_enemy->GetEnemyObj()->GetTransform(), nullptr, Effect_ID::ROCK_BLAST, 3.0f);
	
			DeadRangedEnemy(_enemy);

			hitBack = false;
			EndRangedAttack(_enemy);
			//近距離の敵を元のステートに戻す
			AllChangeMeleeState(StateID::T_ENEMY_RUN_S);
		}
		return;
	}

	//敵がプレイヤーに攻撃する処理
	if (rangedAtkTime >= MaxAttackCounter && !_enemy->IsMovingToPlayer())
	{	
		if (hitBack)
			return;
		
		_enemy->SetLeaderPos(leaderPos);
		_enemy->RangedAttack();
		rangedAtkTime = 0.0f;
		rangedAtkCounter++;
	}

}

void TrashEnemyGroup::EndRangedAttack(TrashEnemy* _enemy)
{
	startButtonImage = false;
	startRangedAtk = false;
	rangedJoinCounter = 0;
	rangedAtkCounter = 0;
	rangedAtkTime = 0.0f;
	rangedAtkCoolTime = 0.0f;
}
