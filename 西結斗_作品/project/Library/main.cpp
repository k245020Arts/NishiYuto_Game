/// <summary>
/// CoGフレームワーク
/// WinMain()から始まります
/// </summary>
/// <author>N.Hanai</author>
/// 

#define _CRTDBG_MAP_ALLOC
#ifdef _CRTDBG_MAP_ALLOC
#include "../Source/Common/Memory/MemoryCount.h"
#include <crtdbg.h>
#include <cstdlib>
#ifdef _DEBUG
#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__);
#else
#define DBG_NEW new
#endif
#endif

#define IMGUI 1
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "App.h"
#include "../Source/Screen.h"
#include "../ImGui/imgui_impl_dxlib.hpp"
#include <d3d11.h>
#include "../icon/resource.h"

#define CoGVersion "4.5F"

const char* Version() {
	return CoGVersion;
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(2495626);
#endif
	SetGraphMode(Screen::WIDTH, Screen::HEIGHT, 32);
	SetWindowIconID(ID_ICON);
	SetOutApplicationLogValidFlag(FALSE); // ログを出さない

	SetMainWindowText(Screen::WINDOW_NAME);
	SetWindowSizeExtendRate(Screen::WINDOW_EXTEND);
	ChangeWindowMode(Screen::WINDOW_MODE); // Windowモードの場合

	SetUseDirect3DVersion(DX_DIRECT3D_11);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//--【Effekseer用追加】--
	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Effekseerに2D描画の設定をする。(2Dのみ)
	Effekseer_Set2DSetting(Screen::WIDTH, Screen::HEIGHT);


	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	SetHookWinProc([](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT /*CALLBACK*/
	{
		// DxLibとImGuiのウィンドウプロシージャを両立させる
		SetUseHookWinProcReturnValue(FALSE);
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	});
#if IMGUI
	// ImGUI初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.Fonts->AddFontFromFileTTF(u8"data\\Font\\FiraCode-Regular.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());	ImGui_ImplDXlib_Init();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 2.5f;
	style.FrameRounding = 3.0f;
	style.GrabRounding = 2.0f;
	style.ScrollbarRounding = 4.0f;
	style.FramePadding = ImVec2(8, 6);
	style.ItemSpacing = ImVec2(10, 8);
	style.WindowBorderSize = 1.2f;
	style.FrameBorderSize = 1.0f;

	ImVec4* c = style.Colors;


	// 基本背景：黒に近い
	c[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.01f, 0.015f, 0.95f);
	c[ImGuiCol_ChildBg] = ImVec4(0.03f, 0.01f, 0.015f, 0.90f);
	c[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.00f, 0.02f, 0.95f);
	c[ImGuiCol_Border] = ImVec4(0.70f, 0.00f, 0.10f, 0.45f);

	// テキスト（白・赤）
	c[ImGuiCol_Text] = ImVec4(1.00f, 0.90f, 0.9f, 1.00f);
	c[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.30f, 0.35f, 1.00f);

	// ボタン：黒 → 濃赤 → 赤発光
	c[ImGuiCol_Button] = ImVec4(0.30f, 0.05f, 0.07f, 0.85f);
	c[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.10f, 0.15f, 1.00f);
	c[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.20f, 0.25f, 1.00f);

	// 入力欄
	c[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.00f, 0.03f, 1.00f);
	c[ImGuiCol_FrameBgHovered] = ImVec4(0.70f, 0.00f, 0.10f, 0.85f);
	c[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.10f, 0.20f, 1.00f);

	// スライダー、チェックマーク（赤紫系）
	c[ImGuiCol_SliderGrab] = ImVec4(0.90f, 0.10f, 0.40f, 1.00f);
	c[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.30f, 0.60f, 1.00f);
	c[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.15f, 0.35f, 1.00f);

	// タブ・タイトル
	c[ImGuiCol_TitleBg] = ImVec4(0.25f, 0.00f, 0.05f, 1.00f);
	c[ImGuiCol_TitleBgActive] = ImVec4(0.60f, 0.00f, 0.10f, 1.00f);
	c[ImGuiCol_Tab] = ImVec4(0.40f, 0.00f, 0.10f, 1.00f);
	c[ImGuiCol_TabHovered] = ImVec4(0.90f, 0.05f, 0.10f, 1.00f);
	c[ImGuiCol_TabActive] = ImVec4(1.00f, 0.10f, 0.15f, 1.00f);

	// リサイズ/セパレータ
	c[ImGuiCol_Separator] = ImVec4(0.80f, 0.00f, 0.10f, 0.75f);
	c[ImGuiCol_ResizeGrip] = ImVec4(0.50f, 0.00f, 0.10f, 0.40f);
	c[ImGuiCol_ResizeGripHovered] = ImVec4(0.90f, 0.10f, 0.20f, 0.85f);
	c[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.30f, 0.35f, 1.00f);


#endif
	AppInit();

#ifdef FIX_FRAME_RATE
	float refreshTime;
	float freq;
	LONGLONG lastTime;
	if (Screen::FRAME_RATE <= 0.0f) {
		HDC hdc = GetDC(GetMainWindowHandle());	// デバイスコンテキストの取得
		float refreshRate = (float)GetDeviceCaps(hdc, VREFRESH);	// リフレッシュレートの取得
		refreshTime = 1.0f / refreshRate;
		ReleaseDC(GetMainWindowHandle(), hdc);	// デバイスコンテキストの解放
	} else {
		int d = Screen::FRAME_RATE;
		refreshTime = 1.0f / d;
	}
	{
		LARGE_INTEGER freqL;
		QueryPerformanceFrequency(&freqL);
		freq = freqL.QuadPart;
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		lastTime = current.QuadPart;
	}
#endif
	while (true) {
#ifdef FIX_FRAME_RATE
		while(true) {
			LARGE_INTEGER current;
			QueryPerformanceCounter(&current);
			float dt = static_cast<float>(current.QuadPart - lastTime) / freq;
			if (dt >= refreshTime) {
				lastTime = current.QuadPart;
				break;
			}
		}
#endif
#if IMGUI
		ImGui_ImplDXlib_NewFrame();
		ImGui::NewFrame();
#endif

		AppUpdate();

		if (ProcessMessage() == -1 || AppIsExit())
		{
			break;
		}
		ScreenFlip();
		ClearDrawScreen();
		AppDraw();
#if IMGUI
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDXlib_RenderDrawData();
#endif
		RefreshDxLibDirect3DSetting();
#if IMGUI
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
#endif
	}
	AppRelease();
	size_t memorySize = GetMemory();
#if IMGUI
	ImGui_ImplDXlib_Shutdown();
	ImGui::DestroyContext();
#endif
	//--【Effekseer用追加】--
	// Effekseerを終了する。
	Effkseer_End();
	DxLib_End();				// ＤＸライブラリ使用の終了処理
#ifdef _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
	size_t memorySize2 = GetMemory();
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//_CrtDumpMemoryLeaks();
	
#endif
#endif
	return 0;				// ソフトの終了 
}