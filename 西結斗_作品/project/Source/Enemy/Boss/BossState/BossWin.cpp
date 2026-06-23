#include "BossWin.h"
#include "../Boss.h"

BossWin::BossWin()
{
	animId = ID::B_WIN;
	string = Function::GetClassNameC<BossWin>();
}

BossWin::~BossWin()
{
}

void BossWin::Update()
{
}

void BossWin::Start()
{
	Boss* b = GetBase<Boss>();
	EnemyStateBase::Start();
}

void BossWin::Finish()
{
}
