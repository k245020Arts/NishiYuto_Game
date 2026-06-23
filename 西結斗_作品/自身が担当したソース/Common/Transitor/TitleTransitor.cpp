#include "TitleTransitor.h"

TitleTransitor::TitleTransitor()
{
	tileSize	= 50;
	gravity		= 0.2f;
	gravityTime = 0.15f;
	erase		= false;
	useGravity	= false;
	tile.clear();

}

TitleTransitor::~TitleTransitor()
{
	tile.clear();
}

void TitleTransitor::Update()
{
	switch (transitorType)
	{
	case Transitor::STAY:
		break;
	case Transitor::PLAY:
		time -= Time::DeltaTimeRate();
		if (time > 0) {

		}
		else {
			SetDrawScreen(newWindow);
			transitorType = FINISH;
		}
		break;
	case Transitor::FINISH:
		transitorType = STAY;
		SetDrawScreen(DX_SCREEN_BACK);
		break;
	default:
		break;
	}
	if (transitorType == STAY) {
		return;
	}

	int xNum = (size.x / tileSize) + 1; //タイルの数の設定
	int yNum = (size.y / tileSize) + 1;
	float eraseRate = (xNum * yNum) / timeMax;

	// DeltaTime をかけて時間に応じた消去量を計算（小数）
	erase += eraseRate * Time::DeltaTimeRate();

	// tileの数だけ消せるだけ消す
	int eraseCount = static_cast<int>(erase);
	if (tile.size() > erase) {
		tile.erase(tile.end() - eraseCount, tile.end());
	}
	else {
		tile.clear();
	}

	erase -= eraseCount;
	if (!useGravity) {
		return;
	}
	for (int i = 0; i < tile.size(); i++) {
		//重力が働いていたら加速
		if (tile[i].velocity > 0.0f) {
			tile[i].velocity += gravity;
			tile[i].gravity += tile[i].velocity;
		}
	}

	auto e = tile.begin();

	for (int i = 0; i < eraseCount && e != tile.end(); ++e) {
		//重力がすでに働いていたらスルー
		if (e->velocity > 0.0f || e->gravity > 0) {
			continue;
		}
		else {
			//重力の始まり
			++i;
			e->velocity += gravity;
			e->gravity += e->velocity;
		}
	}
}
void TitleTransitor::Draw()
{
	if (transitorType == STAY) {
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	float rate = time / timeMax;
	DrawRectGraph(0, 0, 0, 0,size.x, size.y, newWindow, true);
	//分割してシャッフルされた古いキャプチャを表示
	for (const TitleInfo& cell : tile) {
		DrawRectGraph(cell.pos.x * tileSize, static_cast<int>(cell.pos.y * tileSize + cell.gravity), cell.pos.x * tileSize, cell.pos.y * tileSize,tileSize, tileSize,oldWindow, true);
	}
}

void TitleTransitor::SetTile()
{
	int xNum = (size.x / tileSize) + 1; //タイルの数の設定
	int yNum = (size.y / tileSize) + 1;
	tile.reserve(xNum * yNum);
	for (int y = 0; y < yNum; ++y) {
		for (int x = 0; x < xNum; ++x) {
			tile.emplace_back(TitleInfo(VECTOR2I(x,y),0.0f,0.0f));
		}
	}
	std::shuffle(tile.begin(), tile.end(), mt);//コンテナの中身をシャッフルする。
}

void TitleTransitor::StartTile(std::string _sceneName, float _time, bool _useGravity)
{
	Transitor::StartTransitor(_sceneName, _time);
	SetTile();
	erase = 0.0f;
	useGravity	= _useGravity;
	if (useGravity) {
		timeMax += gravityTime;
		time	= timeMax;
	}
}
