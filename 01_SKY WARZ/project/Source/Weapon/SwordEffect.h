#pragma once
#include "../Component/Color/Color.h"
#include <vector>

class CharaWeapon;
class BaseObject;

//#define MY_SWORD

#ifdef MY_SWORD

class SwordEffect : public Component
{
public:
	SwordEffect();
	~SwordEffect();

	void Update()override;
	void Draw()override;
	void ImguiDraw()override;
	/// <summary>
	/// 剣の軌跡の生成
	/// </summary>
	/// <param name="_nearPos">剣の軌跡の末端</param>
	/// <param name="_farPos">剣の軌跡の先端</param>
	/// <param name="_rgb">軌跡の色</param>
	/// <param name="_boneIndex">骨</param>
	/// <param name="_time">時間</param>
	void CreateEffect(VECTOR3 _nearPos, VECTOR3 _farPos, Color::Rgb _rgb, int _boneIndex, float _time);
	void SetImage(int _model);

	VECTOR3 GetBezier(float t, VECTOR3 p0, VECTOR3 p1, VECTOR3 p2);

private:

	struct Info
	{
		Info() {
			time = 0.0f;
			weapon = nullptr;
			charaObj = nullptr;
			bone = -1;
			nearPos = VZero;
			farPos = VZero;
			midPos = VZero;
			rgb = Color::Rgb();
			count = 0;
			timeMax = 0.0f;
			feedInTime = 0.0f;
			feedOutTime = 0.0f;
			maxAlpha = 0.0f;
		}
		float time;
		float timeMax;

		CharaWeapon* weapon;
		BaseObject* charaObj;
		int bone;

		VECTOR3 nearPos;
		VECTOR3 farPos;
		VECTOR3 midPos;
		Color::Rgb rgb;

		int count;

		float feedInTime;
		float feedOutTime;
		float maxAlpha;
	};



	int effectNum;
	int indexNum;

	static const int EFFECT_NUM = 100;
	static const int INDEX_NUM = (EFFECT_NUM - 2) * 3;;

	int image;

	std::vector<Info> num;

	void MakeDiv(std::vector<VERTEX3D>& vs, VECTOR rPos[4], VECTOR tPos[4], float v1, float v2, int i);

};

#else

class SwordEffect : public Component
{
public:
	SwordEffect();
	~SwordEffect();

	void Update()override;
	void Draw()override;
	void ImguiDraw()override;
	/// <summary>
	/// 剣軌跡のイメージを取得
	/// </summary>
	/// <param name="_model"></param>
	void SetImage(int _model);
	/// <summary>
	/// 剣の軌跡の生成
	/// </summary>
	/// <param name="_topPos">剣の先端</param>
	/// <param name="_bottomPos">剣の末端</param>
	/// <param name="_rgb">色</param>
	/// <param name="_boneIndex">キャラクターのボーンのフレーム番号</param>
	/// <param name="_time">生成される時間</param>
	void CreateEffect(VECTOR3 _topPos, VECTOR3 _bottomPos, Color::Rgb _rgb, int _boneIndex, float _time);

private:
	struct Points
	{
		VECTOR3 top, btm;
		Color::Rgb rgb;

		int bone;

		CharaWeapon* weapon;
		BaseObject* charaObj;
	};

	std::vector<Points> points; //リングバッファ
	int writep; //読みの場所
	int readp; //書く場所
	int image;
	int adj(int idx);
	const Points& get(int idx);

	float time;
	float timeMax;

	bool called;
	VERTEX3D MakeVertex(VECTOR3 _pos, float u, float v, Color::Rgb _rgb);
};


#endif // MY_SWORD

