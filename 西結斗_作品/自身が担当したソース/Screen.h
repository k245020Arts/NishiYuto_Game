#pragma once

/// <summary>
/// Windowの定義をします
/// </summary>
#define FIX_FRAME_RATE  // フレームレートを固定にする
namespace Screen
{
	static const int WIDTH = 1920;
	static const int HEIGHT = 1080;
	static const bool WINDOW_MODE = true;
	static const char* WINDOW_NAME = "SKY WARS";
	static const float WINDOW_EXTEND = 1.0f;
	static const float FRAME_RATE = 60.0f;
};
