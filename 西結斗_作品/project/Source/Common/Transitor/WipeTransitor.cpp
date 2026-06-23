#include "WipeTransitor.h"
#include <array>

WipeTransitor::WipeTransitor()
{
	dir			= UP;
	hGradation = -1;
}

WipeTransitor::~WipeTransitor()
{
	if (hGradation != -1) {
		DeleteGraph(hGradation);
	}
}

void WipeTransitor::Update()
{
	switch (transitorType)
	{
	case Transitor::STAY:
		break;
	case Transitor::PLAY:

		if (time == timeMax) {
			SetDrawScreen(DX_SCREEN_BACK);
		}
		time -= Time::DeltaTime();
		if (time <= 0.0f) {
			SetDrawScreen(newWindow);
			transitorType = FINISH;
			time = 0.0f;
		}
		else {
			SetDrawScreen(newWindow);
		}
		break;
	case Transitor::FINISH:
		SetDrawScreen(DX_SCREEN_BACK);
		transitorType = STAY;
		break;
	default:
		break;
	}
}

void WipeTransitor::Draw()
{
	if (IsEnd()) {
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, oldWindow, true);
	float rate = time / timeMax;
	DrawBlendGraph(0, 0, newWindow, true, hGradation, static_cast<int>( 255 * rate), 255);
}

void WipeTransitor::WipeStart(std::string _sceneName, float _time, Direction _dir)
{
	Transitor::StartTransitor(_sceneName, _time);
	dir = _dir;
	SetGradation();
}

void WipeTransitor::SetGradation()
{
	float left, right, top, bottom;
	left = right = top = bottom = 1.0f;

	switch (dir) {
	case LEFT:
		right = 0.0f;
		break;
	case RIGHT:
		left = 0.0f;
		break;
	case UP:
		bottom = 0.0f;
		break;
	case DOWN:
		top = 0.0f;
		break;
	default:
		break;
	}
	std::array<COLOR_U8, 4> colors;
	colors[0].r = colors[0].g = colors[0].b = colors[0].a = static_cast<BYTE>(255 * left * top);//左上
	colors[1].r = colors[1].g = colors[1].b = colors[1].a = static_cast<BYTE>(255 * right * top);//右上
	colors[2].r = colors[2].g = colors[2].b = colors[2].a = static_cast<BYTE>(255 * left * bottom);//左下
	colors[3].r = colors[3].g = colors[3].b = colors[3].a = static_cast<BYTE>(255 * right * bottom);//右下

	std::array<VERTEX2D, 6> vertices;

	// 左上の頂点の情報をセット( 1ポリゴン目の第1頂点 )
	vertices[0].pos.x	= 0;
	vertices[0].pos.y	= 0;
	vertices[0].pos.z	= 0.0f;
	vertices[0].rhw		= 1.0f;
	vertices[0].dif		= colors[0];
	vertices[0].u		= 0.0f;
	vertices[0].v		= 0.0f;

	// 右上の頂点の情報をセット( 1ポリゴン目の第2頂点 )
	vertices[1].pos.x	= static_cast<float>(size.x);
	vertices[1].pos.y	= 0;
	vertices[1].pos.z	= 0.0f;
	vertices[1].rhw		= 1.0f;
	vertices[1].dif		= colors[1];
	vertices[1].u		= 0.0f;
	vertices[1].v		= 0.0f;

	// 左下の頂点の情報をセット( 1ポリゴン目の第3頂点 )
	vertices[2].pos.x	= 0;
	vertices[2].pos.y	= static_cast<float>(size.y);
	vertices[2].pos.z	= 0.0f;
	vertices[2].rhw		= 1.0f;
	vertices[2].dif		= colors[2];
	vertices[2].u		= 0.0f;
	vertices[2].v		= 0.0f;

	// 右下の頂点の情報をセット( 2ポリゴン目の第1頂点 )
	vertices[3].pos.x	= static_cast<float>(size.x);
	vertices[3].pos.y	= static_cast<float>(size.y);
	vertices[3].pos.z	= 0.0f;
	vertices[3].rhw		= 1.0f;
	vertices[3].dif		= colors[3];
	vertices[3].u		= 0.0f;
	vertices[3].v		= 0.0f;

	// 2ポリゴン目の第2頂点は左下の頂点なのでコピー
	vertices[4]			= vertices[2];

	// 2ポリゴン目の第3頂点は右上の頂点なのでコピー
	vertices[5]			= vertices[1];
	hGradation			= MakeScreen(size.x, size.y, true);
	int bkScrH			= GetDrawScreen();
	//レンダ―ターゲットを変更し、グラデーションの描画
	SetDrawScreen(hGradation);
	DrawPolygon2D(vertices.data(), 2, DX_NONE_GRAPH, true);
	//グラデーション描いたらまた元に戻す
	SetDrawScreen(bkScrH);
}
