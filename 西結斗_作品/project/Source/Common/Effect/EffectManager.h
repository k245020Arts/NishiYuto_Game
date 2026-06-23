#pragma once
#include "../../../Library/GameObject.h"
#include <EffekseerForDXLib.h>
#include "../../Component/Color/Color.h"
#include "../ID/EffectID.h"
#include "../Singleton/SingletonBase.h"

class Transform;
class BaseObject;

class EffectManager : public SingletonBase<EffectManager>
{
public:
	

	void Update()override;
	void Draw()override;
	/// <summary>
	/// エフェクトの生成を行う関数
	/// </summary>
	/// <param name="_transform">発生させるTransformを設定</param>
	/// <param name="_parent">親をもつなら親のポインタを持つ(親を持たない場合はnullptrを入れる)</param>
	/// <param name="_id">エフェクトのID</param>
	/// <param name="_time">エフェクトの発生させる時間</param>
	/// <param name="effect3D">エフェクト3Dにするか2Dにするかデフォルトは3D</param>
	/// <returns></returns>
	BaseObject* CreateEffekseer(Transform _transform, BaseObject* _parent, Effect_ID::EFFECT_ID _id,float _time,bool effect3D = true);
	/// <summary>
	/// エフェクトの再生を止める
	/// </summary>
	/// <param name="_id">エフェクトID</param>
	void StopEffekseer(Effect_ID::EFFECT_ID _id);
	/// <summary>
	/// エフェクトのスピード調整
	/// </summary>
	/// <param name="_id">エフェクトID</param>
	/// <param name="_speed">基準を1としたときのスピード</param>
	void SetSpeedEffekseer(Effect_ID::EFFECT_ID _id,float _speed);
	/// <summary>
	/// すべてのエフェクトをストップさせる
	/// </summary>
	void AllStopEffekseer();
	/// <summary>
	/// エフェクトが再生されているか
	/// </summary>
	/// <param name="_id">エフェクトID</param>
	/// <returns>再生されているか</returns>
	bool IsPlayIng(Effect_ID::EFFECT_ID _id);
	/// <summary>
	/// エフェクトの色を変えたいときに使う。
	/// 主にalphaで使うことが多いかも？
	/// </summary>
	/// <param name="_id">エフェクトID</param>
	/// <param name="_rgb">RGBの構造体</param>
	void SetColor(Effect_ID::EFFECT_ID _id, Color::Rgb _rgb);
	/// <summary>
	///　親のトランスフォームを外すための関数
	/// </summary>
	/// <param name="_obj">今親になっているObject</param>
	void ParentTransformRemove(BaseObject* _obj);

	/// <summary>
	/// 引数で指定したエフェクトのデータを削除
	/// </summary>
	/// <param name="_obj">エフェクトオブジェクト</param>
	void RemoveEffekseer(BaseObject* _obj);

	/// <summary>
	/// すべてのエフェクトを処理する
	/// </summary>
	void DeleteAllEffect();
	
private:
	std::list<BaseObject*> effect;

	friend class SingletonBase<EffectManager>;

	EffectManager();
	~EffectManager();
};