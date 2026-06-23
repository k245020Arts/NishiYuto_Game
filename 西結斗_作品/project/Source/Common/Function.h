#pragma once
#include <string>
#include <typeinfo>

namespace Function {
	//列挙型の数値を足したいときに使う
	inline int EnumPlus(int _a, int _b) {
		return _a + _b;
	}
	//入れ替えの関数
	template <typename T>
	void swap(T& _a, T& _b) {
		T tmp = _a;
		_a = _b;
		_b = tmp;
	}

	inline int EnumMaxPlus(int _b, int max) {
		int result = 0;
		for (int i = 0; i < _b; i++) {
			result += max;
		}
		return result;
	}

	inline int EnumTag(int _a, int _b,int max) {
		if (_a < _b) {
			swap(_a, _b);
		}
		return EnumMaxPlus(_b, max) + _a;
	}

	/// <summary>
	/// クラス名をStringにする関数
	/// </summary>
	/// <typeparam name="C">このクラス名をString型にする</typeparam>
	/// <returns></returns>
	template <typename C>
	inline std::string GetClassNameC() {
		std::string string = typeid(C).name();
		const std::string prefix = "class ";
		if (string.find(prefix) == 0) {
			string = string.substr(prefix.length());
		}
		return string;
	}
}