#pragma once
#include "../../../Library/myDxLib.h"
/// <summary>
/// シングルトンのベースクラス
/// </summary>
template<typename DerivedClass>
class SingletonBase {
public:
	/// <summary>
	/// Instanceの取得
	/// </summary>
	/// <returns></returns>
	static DerivedClass* GetInstance() {
		if (instance == nullptr) {
			instance = new DerivedClass();
		}
		return instance;
	}
	/// <summary>
	/// Instanceの破棄 今はApp.cpp のReleaseで呼ばれている
	/// </summary>
	/// <returns></returns>
	static void Destroy() {
		delete instance;
		instance = nullptr;
	}

protected:
	SingletonBase() = default;
	virtual ~SingletonBase() = default;

	virtual void Update() { ; }
	virtual void Draw() { ; }

private:
	static DerivedClass* instance;
};

template<typename DerivedClass>
DerivedClass* SingletonBase<DerivedClass>::instance = nullptr;