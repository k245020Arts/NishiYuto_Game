#pragma once
#include <cstddef>
#include <new>
#include <windows.h>
#pragma warning(push)
#pragma warning(disable:28251) //どうしてもwaringが直らなかったのでこれで苦肉の策
#pragma warning(error : 4834) //new演算子に入れるときに引数の値を使わなかった場合エラーを吐くようにする

#include <cstddef>
#include <new>
#include <atomic>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif

/// <summary>
/// new演算子のオーバーロード
/// 
/// </summary>
/// <param name="_size">メモリのサイズ</param>
/// <returns></returns>
[[nodiscard]] void* __CRTDECL operator new(std::size_t _size) noexcept(false);
/// <summary>
/// newの演算子のオーバーロードで例外を吐かないver
/// </summary>
/// <param name="_size"></param>
/// <param name=""></param>
/// <returns></returns>
[[nodiscard]] void* __CRTDECL operator new(std::size_t _size, const std::nothrow_t&) noexcept;
/// <summary>
/// new演算子のオーバーロード配列対応ver
/// 
/// </summary>
/// <param name="_size">メモリのサイズ</param>
/// <returns></returns>
[[nodiscard]] void* __CRTDECL operator new[](std::size_t _size) noexcept(false);
/// <summary>
/// newの演算子のオーバーロードで例外を吐かないver配列対応ver
/// </summary>
/// <param name="_size"></param>
/// <param name=""></param>
/// <returns></returns>
[[nodiscard]] void* __CRTDECL operator new[](std::size_t _size, const std::nothrow_t&) noexcept;

//----------------------以下delete ; ver--------------------------------

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete(void* _ptr, std::size_t _size) noexcept;
//void operator delete(void* _ptr, const std::nothrow_t&) noexcept;
void operator delete[](void* _ptr, std::size_t _size) noexcept;

//void operator delete[](void* _ptr, const std::nothrow_t&) noexcept;
/// <summary>
/// 合計のメモリの使用量を取得する
/// </summary>
/// <returns></returns>
size_t GetMemory();
/// <summary>
/// この環境内での最高メモリ量を取得する関数
/// </summary>
/// <returns></returns>
size_t GetTotalMemory();

template<typename C>
void SafeDelete(C*& ptr) {
	if (ptr != nullptr) {
		delete ptr;
		ptr = nullptr;
	}
}

#define SafeDelete(type,ptr)

//#pragma warning(pop)

