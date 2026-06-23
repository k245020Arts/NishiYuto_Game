#pragma once
#include "../../../Library/GameObject.h"
#define DebugIf (1);
#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>

class InputManager;

namespace Debug {

	//デバック用機能を備えたクラス

	/// <summary>///
	/// MassageBoxを簡易的に使いやすくしたものエラー文とタイトルを指定してメッセージを出すことが可能
	/// 本来のMassageBoxはさらにmassageに対するアクションを変えることが出来るが基本変えないので抜きました
	/// </summary>/// 
	/// <param name="_error">エラー文</param>
	/// /// <param name="_title">左上に出る文章</param>

	void CreateMessageBox(const std::string& _error, const std::string& _title);

	/// <summary>
	/// Debugの初期化、bootSceneにて使用
	/// </summary>
	/// <param name="_input">inputManagerのポインタ</param>
	void InitDebug(InputManager* _input);

	/// <summary>
	/// DebugModeのUpdate
	/// </summary>
	/*void UpdateDebug();*/

	/// <summary>
	/// DebugModeのUpdate
	/// </summary>
	void DebugUpdate();

	/// <summary>
	/// Log出力する値を挿入する関数
	/// </summary>
	/// <param name="_log"></param>
	void DebugLog(std::string _log);
	/// <summary>
	/// Log出力する値を挿入する関数
	/// Printfみたいな形で出力したいときに使う
	/// 上の関数はエスケープシーケンス対応ver
	/// 下の関数は非対応ver
	/// </summary>
	/// <param name="_log">ログ内容</param>
	/// <param name="">エスケープシーケンス</param>
	void DebugLogPrintfArgs(const TCHAR* _log,...);
	/// <summary>
	/// Log出力する値を挿入する関数
	/// Printfみたいな形で出力したいときに使う
	/// エスケープシーケンス非対応ver
	/// </summary>
	/// <param name="_log">ログ内容</param>
	void DebugLogPrintf(const TCHAR* _log);

	/// <summary>
	/// Logを流すようのUpdate
	/// </summary>
	void UpdateLogger();

	/// <summary>
	/// DebugWindowを流すかどうかの判定
	/// </summary>
	/// <returns></returns>
	bool GetDebugMode();

	//ログ出力をリセットする
	void ClearLogger();
	//ヒットストップを削除
	bool ShakeStop();

	//当たり判定の描画をするかどうかを管理しているクラス
	bool SphereColliderDraw();
	bool AABBColliderDraw();
	bool ModelColliderDraw();
	bool RayColliderDraw();
	void DrawMemory();

	bool UIDraw();

	//printfのエスケープシーケンスの文字列をデバックログで出したいときに使う関数。
	inline const TCHAR* printfString(const TCHAR* _format, ...) {
		static TCHAR strPrintf[1024];
		va_list args;
		va_start(args, _format);
		_vstprintf_s(strPrintf, _format, args);
		return strPrintf;
	}
	/// <summary>
	/// OutPutDebugStringにて出力をするクラス
	/// </summary>
	/// <param name="fmt">ログ内容</param>
	/// <param name="">エスケープシーケンス</param>
	inline void DebugOutPutPrintf(const char* fmt, ...)
	{
		char buf[1024];
		va_list ap;
		va_start(ap, fmt);
		vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap);
		va_end(ap);
		OutputDebugStringA(buf);
		OutputDebugStringA("\n");
	}
	/// <summary>
	/// マウスの表示、非表示を管理
	/// </summary>
	void MouseDrawUpdate();

	void PostEffectImgui();
}