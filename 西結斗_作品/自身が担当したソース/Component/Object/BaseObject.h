#pragma once
#include "../../../Library/GameObject.h"

class ComponentManager;
class Color;
class TransitorManager;

class Transform;
class GameControler;

//Objectの基底クラス
class BaseObject : public GameObject
{
public:
	BaseObject();
	virtual ~BaseObject();
	virtual void Update()override;
	virtual void Draw()override;
	/// <summary>
	/// コンポーネントManagerの取得
	/// </summary>
	/// <returns></returns>
	ComponentManager* Component() const { return componentManager; }

	/// <summary>
	/// オブジェクトに持たせたりする再生速度の重み、引数のスピード
	/// </summary>
	void SetObjectTimeRate(float _rate) { objectTimeRate = _rate; }
	/// <summary>
	/// オブジェクトに持たせたりする再生速度の重み、基準のスピードで設定
	/// </summary>
	void SetObjectTimeRate() { SetObjectTimeRate(baseTimeSpeed); }
	/// <summary>
	/// objectごとのスピードの重みをセットする関数
	/// </summary>
	/// <returns>objectごとのスピードをセット</returns>
	float GetObjectTimeRate() { return objectTimeRate * Time::DeltaTimeRate(); }
	
	/// <summary>
	/// オブジェクトごとのidの取得
	/// </summary>
	/// <returns></returns>
	int GetId() const { return id; }

	/// <summary>
	/// Transformのポインタの取得
	/// </summary>
	/// <returns></returns>
	Transform* GetTransform() const { return transform; }

	/// <summary>
	/// 初期化を行う
	/// </summary>
	/// <param name="name">オブジェクトのTag</param>
	void Init(std::string name);

	/// <summary>
	/// 子オブジェクトを追加する
	/// </summary>
	/// <param name="child">子供のオブジェクト</param>
	void AddChild(BaseObject* child);
	/// <summary>
	/// 子オブジェクトを追加する
	/// </summary>
	/// <param name="child">子供のオブジェクト</param>
	/// <param name="_transformParent">Transformを親子関係にするかどうかTrueなら親子関係にする</param>
	void AddChild(BaseObject* child, bool _transformParent);
	/// <summary>
	/// 子のオブジェクトを取り外す
	/// </summary>
	/// <param name="child"></param>
	void RemoveChild(BaseObject* child);

	// 子のオブジェクトを削除する
	void DeleteChild(BaseObject* child);

	// 子のオブジェクトをすべて削除する
	void DeleteAllChildren();

	// 親から外れる
	void RemoveParent();

	/// <summary>
	/// 親が子供のリストを取り出したい場合はこの関数を使う
	/// </summary>
	/// <returns></returns>
	std::list<BaseObject*> GetChildren() { return children; }
	/// <summary>
	/// 子が親の情報をとりたい場合はこの関数を使うととれる
	/// </summary>
	/// <returns></returns>
	BaseObject* GetParent() { return parent; }

	/// <summary>
	/// 基準のスピードを再設定する
	/// </summary>
	/// <param name="_speed">基準のスピード、デフォルトは1</param>
	void SetBaseTimeSpeed(float _speed) { baseTimeSpeed = _speed; }
	/// <summary>
	/// 基準のスピードを取得する
	/// </summary>
	/// <returns>基準のスピード <returns>
	float GetBaseTimeSpeed(float _speed)const { return baseTimeSpeed; }

	/// <summary>
	/// trueにするとUpdateが止まるfalseにすると通常通りに動く
	/// </summary>
	/// <param name="_sleep">trueにするとUpdateが止まるfalseにすると通常通りに動く</param>
	void SetSleep(bool _sleep) { sleep = _sleep; }
	/// <summary>
	/// trueにするとDrawが動くfalseにすると止まる
	/// </summary>
	/// <param name="_draw"> trueにするとDrawが動くfalseにすると止まる</param>
	void SetDraw(bool _draw) { draw = _draw; }

protected:
	ComponentManager* componentManager;
	float objectTimeRate;
	TransitorManager* transitor;
	int id;
	static int idPlus; //idの加算用の変数

	Transform* transform;

	BaseObject* parent;
	std::list<BaseObject*> children;
	bool transformParent;
	float baseTimeSpeed;

	bool sleep;
	bool draw;

	GameControler* gameControler;
};
