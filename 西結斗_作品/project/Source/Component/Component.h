#pragma once
#include <string>
#include "../Component/Object/Object2D.h"
#include "../Component/Object/Object3D.h"
#include "../Common/Function.h"

//コンポーネントの基底クラス

class Component
{
public:
	Component() {
		active = true;
		obj = nullptr;
		timeRate = 1.0f;
		debugId = -1;
	}
	virtual ~Component(){}
	virtual void Update(){}
	virtual void Draw(){}
	//コンポーネントを始めた時に設定する。
	virtual void Start(){}
	virtual void Awake(){}
	//親のオブジェクトをここで持たせている。内部的にやっているので呼び出す必要性なし)
	virtual void Init(BaseObject* _ob) { obj = _ob; }
	//アクティブ状態を記録しておく（アクティブじゃなかったら削除)
	bool IsActive() { return active; }
	//Imguiでデバック表示したいときに使う関数。
	virtual void ImguiDraw() {};
	virtual void DebugDraw() {};
	//タグを設定できるようにする「
	virtual void SetTag(std::string name) { tag = name; }
	virtual std::string GetTag() { return tag; }
	/// <summary>
	/// ヒエラルキー用のdebugID
	/// </summary>
	/// <returns>debugID</returns>
	int GetId() { return debugId; }
	/// <summary>
	/// 当たり判定用のタグを取得
	/// </summary>
	/// <returns>当たり判定用のタグ</returns>
	std::string GetCollTag() { return colltag; }
	/// <summary>
	/// 当たり判定用のタグをセット
	/// </summary>
	/// <param name="_tag">当たり判定用のタグ</param>
	void SetCollTag(std::string _tag) { colltag = _tag; }
	/// <summary>
	/// コンポーネントの親オブジェクトの取得
	/// </summary>
	/// <returns></returns>
	BaseObject* GetBaseObject() { my_null_assert(obj); return obj; }

	//void SetSlowMode(bool _slow) { slowMode = _slow;}

protected:
	BaseObject* obj; //このコンポーネントが登録されているObject
	bool active;
	std::string tag; //Componentのタグ
	std::string colltag; //当たり判定に使用するComponentのタグ

	float timeRate;
	int debugId;
};