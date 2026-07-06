#include "swordEffect.h"
#include "../Common/ResourceLoader.h"
#include "CharaWeapon.h"
#include "../Component/ComponentManager.h"
#include "../Component/Transform/Transform.h"
#include "../../ImGui/imgui.h"
#include <array>
#include "../Common/Easing.h"


#ifdef MY_SWORD

SwordEffect::SwordEffect()
{

	Load::LoadImageGraph(Load::IMAGE_PATH + "zlRYsodZuqpGepk1757160129_1757160135", ID::SWORD_EFFECT_B);
	Load::LoadImageGraph(Load::IMAGE_PATH + "photo_12", ID::SWORD_EFFECT);
	num.clear();
	image = -1;
	effectNum = 0;
	indexNum = 0;
	debugId = 18;
	tag = Function::GetClassNameC<SwordEffect>();
}

SwordEffect::~SwordEffect()
{
}

void SwordEffect::Update()
{
	for (auto e = num.begin(); e != num.end();) {
		/*if (e->feedInTime > 0.0f) {
			e++;
			continue;
		}*/
		e->time -= Time::DeltaTimeRate();
		if (e->time <= 0.0f) {
			e->feedOutTime -= Time::DeltaTimeRate();
			if (e->feedOutTime <= 0.0f) {
				e = num.erase(e);
			}
		}
		else {
			e++;
		}
	}
}

void SwordEffect::Draw()
{

	if (num.size() < 2) {
		return;
	}

	std::vector<VERTEX3D> vertexes;			// 頂点バッファ
	std::vector<unsigned short> indexes;	// インデックスバッファ
	float vdif = 1.0f / (num.size() - 1);
	for (int i = 0; i < num.size() - 1; i++) {

		// 補間しながら頂点をつくる
		VECTOR3 rPos[4]; // nearPosの補間用の４頂点
		rPos[0] = (i == 0) ? num[0].nearPos : num[i - 1].nearPos;
		rPos[1] = num[i].nearPos;
		rPos[2] = num[i + 1].nearPos;
		rPos[3] = (i == num.size() - 2) ? num[num.size() - 1].nearPos : num[i + 2].nearPos;

		VECTOR tPos[4]; // farPosの補間用の４頂点
		tPos[0] = (i == 0) ? num[0].farPos : num[i - 1].farPos;
		tPos[1] = num[i].farPos;
		tPos[2] = num[i + 1].farPos;
		tPos[3] = (i == num.size() - 2) ? num[num.size() - 1].farPos : num[i + 2].farPos;

		float v0 = (float)num[i].time / num[i].timeMax;
		float v1 = (float)num[i + 1].time / num[i].timeMax;

		MakeDiv(vertexes, rPos, tPos, v0, v1, i);
	}

	//インデックスの設定
	for (int i = 0; i < (vertexes.size() - 2) / 2; i++) {
		indexes.push_back(i * 2);
		indexes.push_back(i * 2 + 1);
		indexes.push_back(i * 2 + 2);
		indexes.push_back(i * 2 + 2);
		indexes.push_back(i * 2 + 1);
		indexes.push_back(i * 2 + 3);
	}

	SetUseZBuffer3D(FALSE);
	SetUseLighting(FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ADD, 128);

	int indexSize = static_cast<int>(indexes.size());
	int vertexSize = static_cast<int>(vertexes.size());

	if (indexSize >= 3)
		DrawPolygonIndexed3D(&vertexes[0], vertexSize, &indexes[0], indexSize / 3, image, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetUseLighting(TRUE);
	SetUseZBuffer3D(TRUE);

}

void SwordEffect::ImguiDraw()
{
	ImGui::Text("%d", num.size());
}

void SwordEffect::CreateEffect(VECTOR3 _nearPos, VECTOR3 _farPos, Color::Rgb _rgb, int _boneIndex, float _time)
{
	Info in;

	in.weapon = obj->Component()->GetComponent<CharaWeapon>();
	in.charaObj = in.weapon->GetChara();
	in.bone = _boneIndex;

	VECTOR3 pos = MV1GetFramePosition(in.weapon->CharaModel(), in.bone);

	in.nearPos = _nearPos * in.weapon->GetWeaponMatrix();
	in.farPos = _farPos * in.weapon->GetWeaponMatrix();
	in.midPos = (_nearPos + _farPos) / 2.0f;
	in.rgb = _rgb;
	//image = Load::GetHandle(ID::SWORD_EFFECT);

	in.count = 0;

	//time = _time;
	in.timeMax = _time;
	in.time = _time;
	in.maxAlpha = _rgb.a;
	in.feedInTime = 0.1f;
	in.feedOutTime = 0.5f;
	num.emplace_back(in);
}

int SwordEffect::adj(int idx)
{
	return 0;
}

const Points& SwordEffect::get(int idx)
{
	// TODO: return ステートメントをここに挿入します
}

void SwordEffect::SetImage(int _model)
{
	image = _model;
}

VECTOR3 SwordEffect::GetBezier(float t, VECTOR3 p0, VECTOR3 p1, VECTOR3 p2)
{
	float u = 1.0f - t;
	return u * u * p0 + 2 * u * t * p1 + t * t * p2;

}

void SwordEffect::MakeDiv(std::vector<VERTEX3D>& vs, VECTOR rPos[4], VECTOR tPos[4], float v1, float v2, int i)
{
	const int divNum = 4;

	// root補間の変数を作る
	VECTOR rV0 = (rPos[2] - rPos[0]) / 2.0f;
	VECTOR rV1 = (rPos[3] - rPos[1]) / 2.0f;
	VECTOR rPar1 = 2 * rPos[1] - 2 * rPos[2] + rV0 + rV1;
	VECTOR rPar2 = -3 * rPos[1] + 3 * rPos[2] - 2 * rV0 - rV1;

	// tip補間の変数を作る
	VECTOR tV0 = (tPos[2] - tPos[0]) / 2.0f;
	VECTOR tV1 = (tPos[3] - tPos[1]) / 2.0f;
	VECTOR tPar1 = 2 * tPos[1] - 2 * tPos[2] + tV0 + tV1;
	VECTOR tPar2 = -3 * tPos[1] + 3 * tPos[2] - 2 * tV0 - tV1;

	COLOR_U8 spcColor;
	spcColor.r = (BYTE)num[i].rgb.r;
	spcColor.g = (BYTE)num[i].rgb.g;
	spcColor.b = (BYTE)num[i].rgb.b;
	//spcColor.a = (BYTE)num[i].rgb.a;

	//フェードインとフェードアウト
	/*if (num[i].feedInTime > 0.0f) {
		num[i].feedInTime -= Time::DeltaTimeRate();
		if (num[i].feedInTime <= 0.0f) {
			num[i].feedInTime = 0.0f;
		}
		float rate = 1 - (num[i].feedInTime / 0.1f);
		spcColor.a = (BYTE)Easing::Lerp(0.0f, num[i].maxAlpha, rate);
	}
	else if (num[i].time <= 0.0f) {
		float rate = (num[i].feedOutTime / 0.5f);
		spcColor.a = (BYTE)Easing::Lerp(0.0f, num[i].maxAlpha, rate);
	}
	else {
		spcColor.a = (BYTE)num[i].maxAlpha;
	}*/

	VERTEX3D v;
	v.norm = VECTOR3(0, 1, 0);
	v.dif = spcColor;
	v.spc = spcColor;



	for (int i = 0; i < divNum; i++) {
		float t = (float)i / divNum;
		float t2 = t * t;
		float t3 = t2 * t;

		// rootの補間をしてpush_back
		v.pos = rPar1 * t3 + rPar2 * t2 + rV0 * t + rPos[1];
		v.u = 0.0f;
		v.v = (v2 - v1) * t + v1;
		vs.push_back(v);

		// tipの補間をしてpush_back
		v.pos = tPar1 * t3 + tPar2 * t2 + tV0 * t + tPos[1];
		v.u = 1.0f;
		v.v = (v2 - v1) * t + v1;
		vs.push_back(v);
	}
}

#else

SwordEffect::SwordEffect()
{
	points.resize(40);
	readp = 0;
	writep = 0;
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "BossAttackEffect", ID::SWORD_EFFECT_B);
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "SlashLocus", ID::SWORD_EFFECT);
	image = -1;
	debugId = 18;
	tag = Function::GetClassNameC<SwordEffect>();

	called = false;
	time = 0.0f;
	timeMax = 0.0f;
}

SwordEffect::~SwordEffect()
{
	//DeleteGraph(image);
	points.clear();
}

void SwordEffect::Update()
{
	if (not called) {
		if (readp != writep) {
			readp = adj(readp + 1);
		}
	}
	called = false;
	
	/*if (time >= 0.0f) {
		time -= Time::DeltaTimeRate();
	}*/
}

VECTOR3 CatmullRom(float rate, VECTOR3 pb, VECTOR3 p0, VECTOR3 p1, VECTOR3 p2)
{
	float t3 = rate * rate * rate; //3乗
	float t2 = rate * rate;//2乗
	float t1 = rate;//1乗
	VECTOR3 rV0 = (p1 - pb) / 2.0f; //p0の傾き
	VECTOR3 rV1 = (p2 - p0) / 2.0f; //p1の傾き
	VECTOR3 a = p0 * 2 - p1 * 2 + rV0 + rV1;
	VECTOR3 b = p0 * -3 + p1 * 3 - rV0 * 2 - rV1;

	return a * t3 + b * t2 + rV0 * rate + p0;//三次曲線の式
}

void SwordEffect::Draw()
{
	std::vector<VERTEX3D> vertecxes; //頂点
	std::vector<unsigned short> indexes;

	//if (readp != writep) {//ポリゴンがある
	//	Points p1 = points[writep];
	//	int before = (writep + points.size() - 1) % points.size();
	//	Points p0 = points[before];
	//	
	//	vertecxes.push_back(MakeVertex(p0.btm, 0.0f, 0.1f));
	//	vertecxes.push_back(MakeVertex(p0.top, 0.0f, 1.0f));
	//	vertecxes.push_back(MakeVertex(p1.top, 0.0f, 1.0f));
	//	vertecxes.push_back(MakeVertex(p0.btm, 0.0f, 0.1f));
	//	vertecxes.push_back(MakeVertex(p1.top, 0.0f, 1.0f));
	//	vertecxes.push_back(MakeVertex(p1.btm, 0.0f, 0.1f));
	//	DrawPolygon3D(&vertecxes[0], 2, image, true);
	//}

	/*VECTOR3 splineTop[4];
	splineTop[0] = points[readp].top;
	splineTop[1] = points[readp + 1].top;
	splineTop[2] = points[readp + 2].top;
	splineTop[3] = points[readp + 3].top;*/

	int s = (int)points.size();

	if (readp == writep)
		return;
	for (int idx = readp; idx != writep; idx = (idx + 1) % s) {
		for (int j = 0; j < 4; j++) {
			VECTOR3 pb, p0, p1, p2;
			if (idx == readp) {
				pb = get(idx).btm;
			}
			else {
				pb = get(idx - 1).btm;
			}
			p0 = get(idx).btm;
			p1 = get(idx + 1).btm;
			if (idx + 1 == writep) {
				p2 = get(idx + 1).btm;
			}
			else {
				p2 = get(idx + 2).btm;
			}
			//p2 = points[i + 1].btm;
			VECTOR3 btm = CatmullRom((float)j / 4, pb, p0, p1, p2);
			vertecxes.push_back(MakeVertex(btm, 0.0f, 1.0f, points[idx].rgb));
			if (idx == readp) {
				pb = get(idx).top;
			}
			else {
				pb = get(idx - 1).top;
			}
			p0 = get(idx).top;
			p1 = get(idx + 1).top;
			if (idx + 1 == writep) {
				p2 = get(idx + 1).top;
			}
			else {
				p2 = get(idx + 2).top;
			}
			VECTOR3 top = CatmullRom((float)j / 4, pb, p0, p1, p2);
			vertecxes.push_back(MakeVertex(top, 0.0f, 1.0f,points[idx].rgb));

		}
		//vertecxes.push_back(MakeVertex(points[idx].top, 0.0f, 1.0f));
	}
	if (vertecxes.size() <= 4) {
		return;
	}
	int pointSize = (int)vertecxes.size();
	float add = 1.0f / pointSize;
	for (int i = 0; i < pointSize; i++) {
		vertecxes[i].v -= add * i;
	}
	for (int i = 0; i < (vertecxes.size() - 2) / 2; i++) {
		indexes.push_back(i * 2);
		indexes.push_back(i * 2 + 1);
		indexes.push_back(i * 2 + 2);
		indexes.push_back(i * 2 + 2);
		indexes.push_back(i * 2 + 1);
		indexes.push_back(i * 2 + 3);
	}

	SetUseZBuffer3D(FALSE);
	SetUseLighting(FALSE);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
	DrawPolygonIndexed3D(vertecxes.data(), (int)vertecxes.size(), indexes.data(), (int)indexes.size() / 3, image, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	SetUseLighting(TRUE);
	SetUseZBuffer3D(TRUE);

}

void SwordEffect::ImguiDraw()
{
	ImGui::Text("%d", points.size());
}

void SwordEffect::CreateEffect(VECTOR3 _nearPos, VECTOR3 _farPos, Color::Rgb _rgb, int _boneIndex, float _time)
{
	Points in;

	in.weapon = obj->Component()->GetComponent<CharaWeapon>();
	in.charaObj = in.weapon->GetChara();
	in.bone = _boneIndex;

	VECTOR3 pos = MV1GetFramePosition(in.weapon->CharaModel(), in.bone);

	in.top = _nearPos * in.weapon->GetWeaponMatrix();
	in.btm = _farPos * in.weapon->GetWeaponMatrix();
	//in.midPos = (_nearPos + _farPos) / 2.0f;
	in.rgb = _rgb;
	//image = Load::GetHandle(ID::SWORD_EFFECT);

	//time = _time;
	timeMax = _time;
	time = _time;
	/*in.maxAlpha = _rgb.a;
	in.feedInTime = 0.1f;
	in.feedOutTime = 0.5f;*/
	points[writep] = in;
	called = true;
	writep = (writep + 1) % points.size();
	if (writep == readp) {
		readp = (readp + 1) % points.size();
	}
}

VERTEX3D SwordEffect::MakeVertex(VECTOR3 _pos, float u, float v,Color::Rgb _rgb)
{
	VERTEX3D vertex;
	vertex.pos = _pos;
	vertex.u = u;
	vertex.v = v;
	vertex.norm = VECTOR3(0.0f, 1.0f, 0.0f);
	vertex.dif = GetColorU8((int)_rgb.r, (int)_rgb.g, (int)_rgb.b, 255);
	vertex.spc = GetColorU8(0, 0, 0, 0);
	
	return vertex;
}

int SwordEffect::adj(int idx)
{
	int s = (int)points.size();
	return (idx + s) % s;
}

const SwordEffect::Points& SwordEffect::get(int idx)
{
	return points[adj(idx)];
}

void SwordEffect::SetImage(int _model)
{
	image = _model;
}



#endif // MY_SWORD

