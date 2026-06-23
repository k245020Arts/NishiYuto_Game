#pragma once
#include <list>
#include <typeinfo>
#include "../Component/Object/BaseObject.h"
#include "Component.h"

//コンポーネントを取りまとめるクラスです。

class ComponentManager
{
public:
	ComponentManager() { obj = nullptr; }
	~ComponentManager();
	void Update();
	void Draw();
	void Init(BaseObject* _ob) { obj = _ob; }

	//コンポーネントを作る。
	template <typename T>
	T* AddComponent() {
		T* com = new T();
		component.emplace_back(com);
		com->Init(obj);
		com->Awake();
		return com;
	}
	//コンポーネントを削除する。
	template <typename T>
	T* RemoveComponent() {
		for (auto c = component.begin(); c != component.end();) {
			if (typeid(*(*c)) == typeid(T)) {
				delete *c;
				c = component.erase(c);
				return nullptr;
			}
			else {
				c++;
			}
		}
		return nullptr;
	}

	//コンポーネントを削除する。
	template <typename T>
	T* RemoveAllComponent() {
		for (auto c = component.begin(); c != component.end();) {
			if (typeid(*(*c)) == typeid(T)) {
				delete* c;
				c = component.erase(c);
			}
			else {
				c++;
			}
		}
		return nullptr;
	}

	//tagで検索して一致したコンポーネントを削除
	template <typename T>
	T* RemoveComponentWithTag(std::string tag) {
		for (auto c = component.begin(); c != component.end();) {
			if ((*c)->GetTag() == tag) {
				if (typeid(*(*c)) == typeid(T)) {
					delete* c;
					c = component.erase(c);
					return nullptr;
				}
				else {
					c++;
				}
			}
			else {
				c++;
			}
		}
		return nullptr;
	}

	//tagで検索して一致したコンポーネントを削除
	template <typename T>
	T* RemoveComponentWithTagIsCollsion(std::string tag) {
		for (auto c = component.begin(); c != component.end();) {
			if ((*c)->GetCollTag() == tag) {
				if (typeid(*(*c)) == typeid(T)) {
					delete* c;
					c = component.erase(c);
					return nullptr;
				}
				else {
					c++;
				}
			}
			else {
				c++;
			}
		}
		return nullptr;
	}
	/// <summary>
	/// コンポーネントを取得
	/// </summary>
	/// <typeparam name="T">指定する型</typeparam>
	/// <returns>指定したコンポーネントのポインタ</returns>
	template <typename T>
	T* GetComponent() {
		for (Component* c:component) {
			if (typeid(*c) == typeid(T)) {
				return dynamic_cast<T*>(c);
			}
		}
		return nullptr;
	}
	/// <summary>
	/// その型に含まれるすべてのコンポーネントの取得
	/// </summary>
	/// <typeparam name="T">指定する型</typeparam>
	/// <returns>指定したコンポーネントのポインタ</returns>
	template <typename T>
	std::list<T*> GetComponents() {
		std::list<T*> list;
		for (Component* c : component) {
			if (typeid(*c) == typeid(T)) {
				list.push_back(dynamic_cast<T*>(c));
			}
		}
		return list;
	}
	/// <summary>
	/// コンポーネントとタグが一致したら返す
	/// </summary>
	/// <typeparam name="T">取りたい型</typeparam>
	/// <param name="_tag">取りたいタグ</param>
	/// <returns>指定したコンポーネントのポインタ</returns>
	template <typename T>
	T* GetComponentWithTag(std::string _tag) {
		for (Component* c:component) {
			if (c->GetTag() == _tag) {
				if (typeid(*c) == typeid(T)) {
					return dynamic_cast<T*>(c);
				}
			}
		}
		return nullptr;
	}
	/// <summary>
	/// コンポーネントとタグが一致したすべてのポインタを返す
	/// </summary>
	/// <typeparam name="T">取りたい型</typeparam>
	/// <param name="_tag">取りたいタグ</param>
	/// <returns>指定したコンポーネントのポインタのリスト</returns>
	template <typename T>
	std::list<T*> GetComponentsWithTag(std::string _tag) {
		std::list<T*> list;
		for (Component* c : component) {
			if (c->GetTag() == _tag) {
				if (typeid(*c) == typeid(T)) {
					list.push_back(dynamic_cast<T*>(c));
				}
			}
		}
		return list;
	}

	/// <summary>
	/// すべてのリストのコンポーネントを取得
	/// </summary>
	/// <returns></returns>
	const std::list<Component*>& GetAllComponent() {
		return component;
	}

private:
	std::list<Component*> component; //登録しているコンポーネント
	BaseObject* obj; //このコンポーネントManagerを持っているオブジェクト
	
};
