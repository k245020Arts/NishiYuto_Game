#pragma once
#include <string>
#include <string_view>
#include <cassert>
#include <iostream>
#include <string.h>
#include"../../../strconv-master/strconv.h"

//アサートを自作してみた
//従来のやつと違うところがAssertの&&をなくす
//あとはnullPtrやロードミスなどのエラー文を統一化、＆判定をdefineの中ですることなどを入れています

//安全対策でDoWhileを使っている

#ifdef _DEBUG
// 通常assert err = 式 , msg = メッセージ
#define my_assert(err,msg) do { \
	const char* m = #err #msg; \
	if((!err)) ErrorAssert(m);\
}while(0)

//nullPtrだったらassertを吐くようにする ptr = ポインタ
#define my_null_assert(ptr) do { \
    const char* msg = #ptr "はnullPtrです"; \
    if ((ptr) == nullptr) ErrorAssert(msg); \
} while(0)

//Load確認用Assert err = ハンドラを入れる変数
#define my_load_assert(err) do { \
	const char* m = #err "はロードミスしています"; \
	if((err <= -1)) ErrorAssert(m);\
}while(0)

//valueの範囲がminより小さい、またはmaxよりも大きいとアサートを吐くようにした value = 値, mins = 最小値 max = 最大値
#define my_range_assert(value,mins,maxs)do{ \
	const char* m = #value"の値は範囲外です";\
	if ((mins > value) || (value > maxs)) ErrorAssert(m);\
}while (0)\

//assert(false)がかっこわるいのでこの関数を呼べば強制的にアサートを吐くようにした msg = 出したいエラー文
#define my_error_assert(msg)do{\
	ErrorAssert(msg);\
}while(0)\

#else
// 通常assert err = 式 , msg = メッセージ
#define my_assert(err,msg){}

//nullPtrだったらassertを吐くようにする ptr = ポインタ
#define my_null_assert(ptr) {}

//Load確認用Assert err = ハンドラを入れる変数
#define my_load_assert(err) {}

//valueの範囲がminより小さい、またはmaxよりも大きいとアサートを吐くようにした value = 値, mins = 最小値 max = 最大値
#define my_range_assert(value,mins,maxs){}

//assert(false)がかっこわるいのでこの関数を呼べば強制的にアサートを吐くようにした msg = 出したいエラー文
#define my_error_assert(msg){}

#endif // _DEBUG



/// <summary>
/// 呼ばないでDefine経由で関数を読んでください
/// </summary>
/// <param name="errMsg"></param>
/// <param name="file"></param>
/// <param name="line"></param>
void ErrorAssert(std::string_view errMsg, const char* file = __FILE__,
	int line = __LINE__);