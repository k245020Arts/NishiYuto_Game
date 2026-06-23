#pragma once
#include "../../../Library/myDxLib.h"
#include "../../../Library/Time.h"
#include "../Component.h"
#include "../Transform/Transform.h"

class Shaker : public Component
{
public:
	enum ShakePattern
	{
		NONE = -1,
		VERTICAL_SHAKE,
		HORIZONAL_SHAKE,
		HEIGHT_SHAKE,
		MIX_SHAKE,
	};
	Shaker();
	~Shaker();

	void Update()override;
	void Draw()override;

	/// <summary>
	/// シェイクを始めたいときに呼ぶとshakeが出来る。
	/// </summary>
	/// <param name="_power">強さ</param>
	/// <param name="_pattern">縦揺れか横揺れかランダムか</param>
	/// <param name="rok">trueにするとヒットストップになってアニメーションとかも全部止まるようになる。falseにすると普通に動く</param>
	/// <param name="_second">秒数</param>
	/// <param name="_baseTransform">自身のtransform(obj->GetTransform())を使うのではなく、別のtransformを使いたいときはこれを書く。</param>
	void ShakeStart(const VECTOR3& _power,ShakePattern _pattern,bool rok,float _second);
	/// <summary>
	/// シェイクを始めたいときに呼ぶとshakeが出来る。
	/// </summary>
	/// <param name="_power">強さ</param>
	/// <param name="_pattern">縦揺れか横揺れかランダムか</param>
	/// <param name="rok">trueにするとヒットストップになってアニメーションとかも全部止まるようになる。falseにすると普通に動く</param>
	/// <param name="_second">秒数</param>
	/// <param name="_baseTransform">自身のtransform(obj->GetTransform())を使うのではなく、別のtransformを使いたいときはこれを書く。</param>
	void ShakeStart(const VECTOR3& _power,ShakePattern _pattern,float rok,float _second,Transform* _baseTransform);
	
	/// <summary>
	/// shakeの強さをセットっする
	/// </summary>
	/// <param name="_power">振動のパワー</param>
	void SetShakePower(const VECTOR3& _power);
	/// <summary>
	/// shakeの強さを足していく
	/// </summary>
	/// <param name="_power">振動の追加パワー</param>
	void AddShakePower(const VECTOR3& _power);
	
	/// <summary>
	/// 振動の終了
	/// </summary>
	void ShakeFinish();
	/// <summary>
	/// Shake中かどうか
	/// timeが0より大きいまたはマイナス1より小さかったら今shake中と判定する
	/// </summary>
	/// <returns></returns>
	bool IsShakeing()const { return (time > 0 || time < -1.0f); }
	/// <summary>
	/// 現在の振動の位置を取得
	/// </summary>
	/// <returns></returns>
	VECTOR3 GetShakePos() const ;
	/// <summary>
	/// 今のトランスフォームを取得
	/// </summary>
	/// <returns>今のトランスフォーム</returns>
	VECTOR3 GetCurrentPos()const  { return currentTransform.position; }
	/// <summary>
	/// 今のシェイクの強さを取得
	/// </summary>
	/// <returns> 今のシェイクの強さ</returns>
	VECTOR3 GetShakePower()const  { return currentShakePower; }
	/// <summary>
	/// 今のシェイクの強さを取得
	/// </summary>
	/// <returns>今のシェイクの強さ</returns>
	VECTOR3 GetShakeOffset() const;

	void ImguiDraw()override;

private:

	Transform currentTransform;
	Transform* transform;

	VECTOR3 power;
	VECTOR3 powerSub;
	float time;
	ShakePattern pattern;
	bool stop;

	void ShakePower();
	VECTOR3 GetShake(const VECTOR3& _shake);

	VECTOR3 shakePos;

	VECTOR3 shakeSubPosition;
	VECTOR3 currentShakePower;
};
