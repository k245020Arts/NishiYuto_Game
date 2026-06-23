#include "Debug.h"
#include "../../../ImGui/imgui.h"
#include "../InputManager/InputManager.h"
#include "../PostEffect/PostEffect.h"

namespace {
	std::list<std::string>logger;

	float fps;
	float drawFps;
	int num;
	bool shaker;
	float rate;

	bool sphereColliderDraw;
	bool aabbColliderDraw;
	bool modelColliderDraw;
	bool rayColliderDraw;
	InputManager* inputManager;
	bool debug;
	bool mouse;
	bool uiDraw;
}

void Debug::CreateMessageBox(const std::string& _error, const std::string& _title)
{
//#if DebugIf
	MessageBox(NULL, _error.c_str(), _title.c_str(), MB_ICONERROR | MB_OK);
//#endif
}

void Debug::InitDebug(InputManager* _input)
{
	drawFps				= 60.0f;
	shaker				= true;

	sphereColliderDraw	= false;
	aabbColliderDraw	= false;
	modelColliderDraw	= false;
	rayColliderDraw		= false;
	inputManager		= _input;

	logger.clear();
	uiDraw = true;

#ifdef _DEBUG
	debug = false;
	mouse = true;
#else
	debug = false;
	mouse = false;
#endif
}

//void Debug::UpdateDebug()
//{
//#ifdef _DEBUG
//	ImGui::Begin("debug");
//	if (ImGui::TreeNode("ger")) {
//		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(500, 200));
//		for (std::string& str : logger) {
//			ImGui::Text("%s", str.c_str());
//			ImGui::Separator();
//		}
//		ImGui::EndChild();
//		ImGui::TreePop();
//	}
//	if (logger.size() >= 100) {
//		logger.pop_front();
//	}
//	ImGui::End();
//#endif
//}

void Debug::DebugUpdate()
{
	MouseDrawUpdate();
#ifdef _DEBUG
	if (inputManager->KeyInputDown("debugChange")) {
		debug = !debug;
	}
#ifdef POST_EFFECT
	PostEffectImgui();
#endif // POST_EFFECT

	if (!debug) { //デバックウィンドウ非表示モードならリターン
		return;
	}
	num++;
	if (num >= 60) {
		fps = GetFPS();
		num = 0;
	}
	ImGui::Text("FPS : %.1f", fps);
	DrawMemory();
	if (ImGui::TreeNode("Debug")) {
		if (ImGui::Button("breakPoint")) {
			int a = 0;
		}
		if (shaker) {
			if (ImGui::Button("shakeStop")) {
				shaker = false;
			}
		}
		else {
			if (ImGui::Button("shakeAgain")) {
				shaker = true;
			}
		}
		ImGui::DragFloat("timeRate", &rate, 0.1f, 0.0f, 2.0f);
		if (ImGui::Button("timeChange")) {
			Time::DebugSlowTimeSet(rate);
		}
		if (ImGui::TreeNode("DebugCollsion")) {
			
			if (ImGui::Button("sphereColliderDraw")) {
				sphereColliderDraw = !sphereColliderDraw;
			}
			if (ImGui::Button("aabbColliderDraw")) {
				aabbColliderDraw = !aabbColliderDraw;
			}
			if (ImGui::Button("modelColliderDraw")) {
				modelColliderDraw = !modelColliderDraw;
			}
			if (ImGui::Button("rayColliderDraw")) {
				rayColliderDraw = !rayColliderDraw;
			}
			if (ImGui::Button("UiDraw")) {
				uiDraw = !uiDraw;
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	UpdateLogger();
#endif // _DEBUG
}

void Debug::DebugLog(std::string _log)
{
	logger.emplace_back(_log);
}

void Debug::DebugLogPrintfArgs(const TCHAR* _log,...)
{
	std::string name;
	name = printfString(_log);
	logger.emplace_back(name);
}

void Debug::DebugLogPrintf(const TCHAR* _log)
{
	std::string name;
	name = _log;
	logger.emplace_back(name);
}

void Debug::UpdateLogger()
{
	if (ImGui::TreeNode("logger")) {
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(500, 400));
		for (std::string& str : logger) {
			ImGui::Text("%s", str.c_str());
			ImGui::Separator();
		}
		ImGui::EndChild();
		ImGui::TreePop();
	}
	const int LOG_MAX = 1000;
	//ログの蓄積が最大よりたまったら、先頭から削除する
	if (logger.size() >= LOG_MAX) {
		logger.pop_front();
	}
	
}

bool Debug::GetDebugMode()
{
	return debug;
}

void Debug::ClearLogger()
{
	logger.clear();
}

bool Debug::ShakeStop()
{
	return shaker;
}

bool Debug::SphereColliderDraw()
{
	return sphereColliderDraw;
}

bool Debug::AABBColliderDraw()
{
	return aabbColliderDraw;
}

bool Debug::ModelColliderDraw()
{
	return modelColliderDraw;
}

bool Debug::RayColliderDraw()
{
	return rayColliderDraw;
}

void Debug::DrawMemory()
{
	if(ImGui::TreeNode("Memory")) {
		//現在の使用メモリ
		ImGui::Text("Memory : %zu", GetMemory());
		size_t memory = GetMemory();
		//メガバイト表記
		float memoryMB = (float)memory / (1024.0f * 1024.0f);
		ImGui::Text("Memory: %.2f MB\n", memoryMB);
		//最大メモリ
		ImGui::Text("Total : Memory : %zu", GetTotalMemory());
		size_t totalMemory = GetTotalMemory();
		//メガバイト表記
		float totalMemoryMB = (float)totalMemory / (1024.0f * 1024.0f);
		ImGui::Text("Total : Memory: %.2f MB\n", totalMemoryMB);
		ImGui::TreePop();
	}
	
}

bool Debug::UIDraw()
{
	return uiDraw;
}

void Debug::MouseDrawUpdate()
{
	SetMouseDispFlag(mouse);
	if (inputManager->KeyInputDown("Mouse")) {
		mouse = !mouse;
	}
}

void Debug::PostEffectImgui()
{
	if (ImGui::Begin("PostEffect"))
	{
		auto& pe = PostEffect::GetParameters();

		ImGui::Separator();

		if (ImGui::Button("Reset PostEffect Params"))
		{
			PostEffect::ResetParameters();
		}

		ImGui::Separator();
		ImGui::Text("Bloom");
		ImGui::SliderInt("Bloom Threshold", &pe.bloomBrightThreshold, 0, 255);
		ImGui::SliderInt("Bloom Blur Width", &pe.bloomGaussPixelWidth, 1, 64);
		ImGui::SliderInt("Bloom Blur Strength", &pe.bloomGaussParam, 1, 2000);
		ImGui::SliderInt("Bloom Alpha", &pe.bloomAlpha, 0, 255);
		ImGui::SliderInt("Bloom Passes", &pe.bloomPasses, 1, 8);

		ImGui::Separator();
		ImGui::Text("Shared Edge Mask");
		ImGui::SliderInt("Edge Threshold", &pe.edgeThreshold, 0, 255);

		ImGui::Separator();
		ImGui::Text("Rim Light");
		ImGui::SliderInt("Rim Blur Width", &pe.rimSoftGaussWidth, 1, 64);
		ImGui::SliderInt("Rim Blur Strength", &pe.rimSoftGaussParam, 1, 2000);
		ImGui::SliderInt("Rim Alpha", &pe.rimAlpha, 0, 255);
		ImGui::SliderInt("Rim Passes", &pe.rimPasses, 1, 8);
		ImGui::SliderInt("Rim Color R", &pe.rimColorR, 0, 255);
		ImGui::SliderInt("Rim Color G", &pe.rimColorG, 0, 255);
		ImGui::SliderInt("Rim Color B", &pe.rimColorB, 0, 255);

		ImGui::Separator();
		ImGui::Text("AO Like");
		ImGui::SliderInt("AO Blur Width", &pe.aoSoftGaussWidth, 1, 64);
		ImGui::SliderInt("AO Blur Strength", &pe.aoSoftGaussParam, 1, 2000);
		ImGui::SliderInt("AO Alpha", &pe.aoAlpha, 0, 255);
		ImGui::SliderInt("AO Passes", &pe.aoPasses, 1, 8);

		PostEffect::ValidateParameters();
	}
	ImGui::End();
}
