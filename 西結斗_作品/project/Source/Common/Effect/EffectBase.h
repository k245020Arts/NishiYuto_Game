#pragma once
#include "../../../Library/GameObject.h"
#include <EffekseerForDXLib.h>
#include "../../Component/Component.h"
#include "../../Component/Transform/Transform.h"
#include "../ID/EffectID.h"
#include "../../Component/Color/Color.h"

class BaseObject;
class EffectManager;

class EffectBase : public Component
{
public:
	EffectBase();
	~EffectBase();
	void Update()override;
	void Draw()override;

	/// <summary>
	/// エフェクトの準備をするクラス
	/// EffectManagerで自動で呼ばれるので手動で呼ぶ必要はない
	/// </summary>
	/// <param name="_transform">EffectのTransform</param>
	/// <param name="_parent">親子関係をするか</param>
	/// <param name="_id">エフェクトのID</param>
	/// <param name="_time">エフェクトのトータル再生時間</param>
	/// <param name="effect3D">エフェクトを3Dで再生するか</param>
	void EffectInit(const Transform& _transform, BaseObject* _parent, Effect_ID::EFFECT_ID _id, float _time, bool effect3D = true);

	/// <summary>
	/// エフェクトを再生させる2Dと3Dで関数を分けている
	/// </summary>
	void EffectPlay2D();
	void EffectPlay3D();
	/// <summary>
	/// エフェクトをストップさせる2Dと3Dで関数を分けている
	/// </summary>
	void EffectStop2D();
	void EffectStop3D();

	void ChangeSpeed(float _speed) { speed = _speed; }

	/// <summary>
	/// 現在使用してるエフェクトのIDをわたす
	/// </summary>
	/// <returns>エフェクトのID</returns>
	Effect_ID::EFFECT_ID GetEffectID() { return id; }
	/// <summary>
	/// エフェクトの再生を止める
	/// </summary>
	void EffectStop();
	
	/// <summary>
	/// エフェクトのスピードを調整
	/// </summary>
	/// <param name="_speed">基準を1としたときのスピード</param>
	void EffectSpeedSet(float _speed);
	/// <summary>
	/// エフェクトの色を変える2Dver
	/// </summary>
	void SetColor2D(Color::Rgb _rgb);
	/// <summary>
	///  エフェクトの色を変える3Dver
	/// </summary>
	/// <param name="_rgb">RGBの構造体</param>
	void SetColor3D(Color::Rgb _rgb);
	void SetColor(Color::Rgb _rgb);
	/// <summary>
	/// 親子関係は継続しつつ親子関係のTransformを外したいときに使う関数
	/// </summary>
	void ParentTransformRemove();

private:

	Transform* transform;
	BaseObject* parent;
	Effect_ID::EFFECT_ID id;

	typedef void (EffectBase::* EffectKind)();
	EffectKind effectPlay;
	typedef void (EffectBase::* Stop)();
	Stop effectStop;

	typedef void (EffectBase::* EfColor)(Color::Rgb _rgb);
	EfColor effectColor;
	
	int hPlayHandle;
	float time;
	float speed;
	float feedInTime;
	const float FEEDIN_TIME = 0.1f;
};
