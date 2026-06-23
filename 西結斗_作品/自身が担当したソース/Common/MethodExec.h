#pragma once

template <typename T>
class MethodExec {
public:
    using MethodPtr = MethodExec<T>(T::*)();

    MethodExec(MethodPtr f = nullptr) : func(f) {}
    /// <summary>
   /// 関数の実行部分
   /// </summary>
   /// <param name="obj"></param>
   /// <returns></returns>
    MethodExec<T> Act(T* obj) {
        return (obj->*func)();  // 呼び出し
    }
    /// <summary>
   /// 現在の関数のアドレスを取得
   /// </summary>
   /// <returns></returns>
    MethodPtr GetMethodFunc() const {
        return func;  // 関数ポインタを返す
    }

private:
    MethodPtr func; // 実行するT型クラスのメソッドポインタ
};