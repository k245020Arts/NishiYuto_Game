#include "MemoryCount.h"
#include <atomic>
#include <cstdlib>
#include <new>
#include <cstdint>
#include <iostream>

////ウィンドウズを使っているときはこれが実行される。
#ifdef _WIN32
#include <windows.h>
#endif
//どうしてもwaringが直らなかったのでこれで苦肉の策
#pragma warning(disable:28251)
#include <DxLib.h>

namespace {
    std::atomic<size_t> totalMemorySize{ 10000000 };//合計のメモリサイズ
    static const  size_t HEADER_SIZE = sizeof(size_t); // 先頭に保存するサイズの量
}

[[nodiscard]] void* operator new(std::size_t size) {
    size_t s = HEADER_SIZE;
    size_t totalSize = size + HEADER_SIZE;
    //実際のメモリ割り当てを行う(演算子のオーバーライドなので普通のnewでもしていることはしっかりとしないとインスタンスが確保されない)
    void* raw = std::malloc(totalSize);
    //メモリに格納されなかったらエラーを流す。
    if (!raw) {
        throw std::bad_alloc();
    }
    // 先頭にサイズ情報をいれる
    *static_cast<size_t*>(raw) = size;
   totalMemorySize += size;//メモリのサイズを加算
    return static_cast<char*>(raw) + HEADER_SIZE;
}

[[nodiscard]] void* operator new[](std::size_t size) {
    size_t totalSize = size + HEADER_SIZE;
    //実際のメモリ割り当てを行う(演算子のオーバーライドなので普通のnewでもしていることはしっかりとしないとインスタンスが確保されない)
    void* raw = std::malloc(totalSize);
    if (!raw) {
        throw std::bad_alloc();
    }

    *static_cast<size_t*>(raw) = size;
    totalMemorySize += size;//メモリのサイズを加算

    return static_cast<char*>(raw) + HEADER_SIZE;
}

[[nodiscard]] void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
    size_t totalSize = size + HEADER_SIZE;
    //実際のメモリ割り当てを行う(演算子のオーバーライドなので普通のnewでもしていることはしっかりとしないとインスタンスが確保されない)
    void* raw = std::malloc(totalSize);
    if (!raw) {
        return nullptr;
    }

    *static_cast<size_t*>(raw) = size;
    totalMemorySize += size;

    return static_cast<char*>(raw) + HEADER_SIZE;
}

[[nodiscard]] void* operator new[](std::size_t size, const std::nothrow_t&) noexcept {
    size_t totalSize = size + HEADER_SIZE;
    //実際のメモリ割り当てを行う(演算子のオーバーライドなので普通のnewでもしていることはしっかりとしないとインスタンスが確保されない)
    void* raw = std::malloc(totalSize);
    if (!raw) {
        return nullptr;
    }

    *static_cast<size_t*>(raw) = size;
    totalMemorySize += size;

    return static_cast<char*>(raw) + HEADER_SIZE;
}

void tracked_delete(void* ptr) {
    if (!ptr) return;
    void* raw = static_cast<char*>(ptr) - HEADER_SIZE;
    size_t size = *static_cast<size_t*>(raw);
    //totalMemorySize -= size;
   // printfDx("[DEL] ptr=%p size=%u before=%u\n", ptr, (unsigned int) size, (unsigned int)totalMemorySize.load());
    //ScreenFlip();
    totalMemorySize -= size;
   // printfDx("[DEL] after=%u\n", (unsigned int)totalMemorySize.load());
    std::free(raw);
}

void operator delete(void* ptr) noexcept {
    tracked_delete(ptr);
}
void operator delete[](void* ptr) noexcept {
    tracked_delete(ptr);
}
void operator delete(void* ptr, std::size_t) noexcept {
    tracked_delete(ptr); // 安全に統一
}
void operator delete[](void* ptr, std::size_t) noexcept {
    tracked_delete(ptr);
}

size_t GetMemory()
{
    return totalMemorySize.load();
}

size_t GetTotalMemory()
{
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        return static_cast<size_t>(memInfo.ullTotalPhys);
    }
#endif
    return 0;
}


// CRTデバッグ対応版
#ifdef _DEBUG
void* operator new(
    size_t size,
    int blockType,
    const char* file,
    int line)
{
    void* raw = _malloc_dbg(size + HEADER_SIZE, blockType, file, line);
    if (!raw) throw std::bad_alloc();
    *static_cast<size_t*>(raw) = size;
    totalMemorySize += size;
    return static_cast<char*>(raw) + HEADER_SIZE;
}

void operator delete(
    void* ptr,
    int,
    const char*,
    int) noexcept
{
    tracked_delete(ptr);
}
#endif