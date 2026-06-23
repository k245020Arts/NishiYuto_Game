#pragma once
#include "../../Library/GameObject.h"
#include "../Player/PlayerInformation.h"
#include <typeinfo>
#include "../Component/Component.h"
#include "../Common/ID/StateID.h"

class BaseObject;

class StateBase
{
public:
	StateBase() {
		id = StateID::STATE_MAX;
		string = "";
		obj = nullptr;
		com = nullptr;
	}
	virtual ~StateBase(){}

	void Init(BaseObject* _base,StateID::State_ID _id) {
		obj = _base;
		id = _id;
	}

	virtual void Update(){}
	virtual void Draw(){}
	/// <summary>
	/// ステートが切り替わった時に呼ばれる
	/// </summary>
	virtual void Start() { ; }
	/// <summary>
	/// Stateの文字を取得
	/// </summary>
	/// <returns>Stateの文字</returns>
	std::string GetString()const { return string; }
	/// <summary>
	/// StateのIDを取得
	/// </summary>
	/// <returns>StateのID</returns>
	StateID::State_ID GetID()const { return id; }
	/// <summary>
	/// 回避が始まった時に呼ぶ
	/// </summary>
	virtual void AvoidStart() { ; }
	/// <summary>
	/// Stateが切り替わるときに呼ぶ
	/// </summary>
	virtual void Finish() { ; }

	/// <summary>
	/// どのオブジェクトのStateなのかがわからないためそれをセットする
	/// </summary>
	/// <typeparam name="T">どのクラス名のStateなのか</typeparam>
	/// <param name="_c">どのクラス名のStateなのか</param>
	template<typename T>
	void SetComponent(T* _c) {
		com = _c;
	}
	/// <summary>
	/// 親のオブジェクトのポインタを取得
	/// </summary>
	/// <typeparam name="T">親のオブジェクトのクラス名</typeparam>
	/// <returns>親のオブジェクトのポインタ</returns>
	template<typename T> 
	T* GetBase() {
		return static_cast<T*>(com);
	}
	/// <summary>
	/// StateVerのimgui
	/// </summary>
	virtual void StateImguiDraw() { ; };

protected:
	std::string string;
	StateID::State_ID id;
	BaseObject* obj;
	Component* com;
};