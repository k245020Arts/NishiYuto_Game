#include "TrashEnemyGroup.h"
#include "TrashEnemy.h"
#include "../../Common/Random.h"
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
}

TrashEnemyGroup::~TrashEnemyGroup()
{
	DeleteGraph(yButtonImage);
	camera = nullptr;
	trashEnemyManager = nullptr;
}

void TrashEnemyGroup::Update()
{
	EnemyDead(meleeEnemies);
	EnemyDead(rangedEnemies);
	Separation();
	
	//近距離の敵関連
	for (auto melee : meleeEnemies)
	{
		EnemiesRun(melee);
		MeleeEnemyAttack(melee);
		CooperateAttackMove(melee);
		if (startRangedAtk)
			MeleeEvadeMove(melee);
	}
	//遠距離の敵関連
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
	VECTOR3 position = camera->GetCameraTransform()->position;
	position.y = 0;
	//正面べく
	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(camera->GetCameraTransform()->rotation.y);
	VECTOR3 vec = _wayPoint.position - position;

	//内積
	float dotProduct = VDot(frontVec, vec.Normalize());
	//カメラに写ってるか
	if (dotProduct > cosf(45 * DegToRad))
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
	float* _hp = 0;

	for (auto& itr : meleeEnemies)
	{
		if (itr->GetHp() > _hp)//Activeでやると死んでるモーション挟んでる敵もカウントされるため
			_counter++;
	}
	return _counter;
}

int TrashEnemyGroup::GetRangedActiveEnemy()const
{
	int _counter = 0;
	float* _hp = 0;

	for (auto& itr : rangedEnemies)
	{
		if (itr->GetHp() > _hp)
			_counter++;
	}

	return _counter;
}

void TrashEnemyGroup::MeleeEnemyAttack(TrashEnemy* _enemy)
{
	if (_enemy->IsCooperateAtk() || !_enemy->IsAttack() || startRangedAtk)
		return;

	attackCounter += Time::DeltaTimeRate();

	if (attackCounter >= ATK_COUNTER_MIN + maxAttackCounter)//攻撃のクールタイム
	{
		if (_enemy->IsAttack())
		{
			_enemy->AttackCommand();
			attackCounter = 0;
			maxAttackCounter = ATK_COUNTER_MAX * (float)Random::GetReal();
		}
	}
	else if (attackCounter >= ATK_COUNTER_MAX)
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

void TrashEnemyGroup::CooperateAttackMove(TrashEnemy* _enemy)
{
	if (!_enemy->IsCooperateAtk())
		return;
	int enemiesMax = (int)meleeEnemies.size();

	//連携攻撃のときにその敵が準備完了したかどうか
	if (_enemy->GetStandby())
		standbyCounter++;
	if (standbyCounter >= 1)
		cooperateCounter += Time::DeltaTimeRate();

	//敵全員が準備完了するか時間経過で攻撃に移る
	if (standbyCounter == enemiesMax || cooperateCounter >= 3)
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

		copyPos.push_back(VECTOR3(itr->GetPos().x, PosY, itr->GetPos().z));
	}

	const size_t Counter = copyPos.size() - 1;

	for (int i = 0; i < Counter; i++)
	{
		DrawCapsule3D(copyPos[i], copyPos[i + 1] , 20, 0, GetColor(255, 0, 0), GetColor(255, 0, 0), true);
	}

	copyPos.clear();
}

void TrashEnemyGroup::MeleeEvadeMove(TrashEnemy* _enemy)
{
	if (_enemy->GetNowHp() <= 0.0f)
		return;

	_enemy->SetLeaderPos(leaderPos);
	_enemy->SetLeaderRotY(leaderRotY);

	_enemy->ChangeState(StateID::T_ENEMY_EVADE);
}

void TrashEnemyGroup::CloseWayPoint(std::vector<WayPoint>& wayPoint)
{
	VECTOR3 position = camera->GetCameraTransform()->position;
	position.y = 0;
	//正面べく
	VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(camera->GetCameraTransform()->rotation.y);
	float counter = 0;
	VECTOR3 savePos = INFINITY;
	/*float _a = INFINITY;*/
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
				/*if (_a > vec)
				{
					savePos = itr.position;
					_a = vec;
				}*/
			}
		}
		enemy->SetCooperateWayPoint(savePos, StateID::COOPERATEATTACK1);
		counter = 1;
	}
}

void TrashEnemyGroup::RangedEnemyAttack()
{
	if (rangedAtkCoolTime <= MaxCoolTime)
	{
		rangedAtkCoolTime += Time::DeltaTimeRate();
		return;
	}

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

	if (rangedJoinCounter <= rangedAtkCounter)//敵全員が攻撃を終えた後の処理
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
