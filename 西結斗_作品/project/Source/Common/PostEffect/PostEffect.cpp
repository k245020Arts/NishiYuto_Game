#include "PostEffect.h"

#include "../../Screen.h"
#include "../../../Library/myDxLib.h"

namespace
{
	constexpr int bloomDownScale = 2;
	constexpr int edgeDownScale = 4;
	constexpr int edgeGaussPixelWidth = 18;
	constexpr int edgeGaussParam = 900;

	constexpr PostEffect::Parameters defaultParameters =
	{
		180,	// bloomBrightThreshold
		14,	// bloomGaussPixelWidth
		700,	// bloomGaussParam
		58,	// bloomAlpha
		2,	// bloomPasses

		10,	// edgeThreshold

		8,	 // rimSoftGaussWidth
		260,	 // rimSoftGaussParam
		42,	 // rimAlpha
		2,	 // rimPasses
		150,	 // rimColorR
		205,	 // rimColorG
		255,	 // rimColorB

		10,	 // aoSoftGaussWidth
		340,	 // aoSoftGaussParam
		28,	 // aoAlpha
		1,	 // aoPasses

		8,	 // colorLevelMin
		242,	 // colorLevelMax
		115,	 // colorGamma
		0,	 // colorAfterMin
		255,	 // colorAfterMax
		0,	 // colorHueShift
		18,	 // colorSaturation
		4,	 // colorBrightness
		255,	 // colorTintR
		245,	 // colorTintG
		235,	 // colorTintB
		8,	 // colorTintAlpha

		70,	 // fogColorR
		82,	 // fogColorG
		100,	 // fogColorB
		1300,	 // fogStart
		2900	 // fogEnd
	};

	

	PostEffect::Parameters params = defaultParameters;

	int sceneScreen = -1;
	int compositeScreen = -1;

	int bloomHighBrightScreen = -1;
	int bloomDownScaleScreen = -1;
	int bloomGaussScreen = -1;

	int edgeDownScaleScreen = -1;
	int edgeBlurScreen = -1;
	int edgeExpandedScreen = -1;
	int edgeDiffScreen = -1;
	int edgeMaskScreen = -1;

	int colorLevelScreen = -1;
	int colorHsbScreen = -1;

	int sceneWidth = 0;
	int sceneHeight = 0;
	int bloomDownScaleWidth = 0;
	int bloomDownScaleHeight = 0;
	int edgeDownScaleWidth = 0;
	int edgeDownScaleHeight = 0;

	bool bloomEnabled = true;
	bool rimLightEnabled = true;
	bool aoEnabled = true;
	bool colorGradingEnabled = true;
	bool fogEnabled = true;
	bool useThisFrame = false;

	void DeleteScreenHandle(int& handle)
	{
		if (handle >= 0)
		{
			DeleteGraph(handle);
			handle = -1;
		}
	}

	//bool IsTargetScene()
	//{
	//	return SceneManager::Instance().GetCurrentSceneName()== "PLAY";
	//}

	bool CreateScreens()
	{
		sceneWidth = Screen::WIDTH;
		sceneHeight = Screen::HEIGHT;
		bloomDownScaleWidth = (sceneWidth + bloomDownScale - 1) / bloomDownScale;
		bloomDownScaleHeight = (sceneHeight + bloomDownScale - 1) / bloomDownScale;
		edgeDownScaleWidth = (sceneWidth + edgeDownScale - 1) / edgeDownScale;
		edgeDownScaleHeight = (sceneHeight + edgeDownScale - 1) / edgeDownScale;

		DeleteScreenHandle(sceneScreen);
		DeleteScreenHandle(compositeScreen);
		DeleteScreenHandle(bloomHighBrightScreen);
		DeleteScreenHandle(bloomDownScaleScreen);
		DeleteScreenHandle(bloomGaussScreen);
		DeleteScreenHandle(edgeDownScaleScreen);
		DeleteScreenHandle(edgeBlurScreen);
		DeleteScreenHandle(edgeExpandedScreen);
		DeleteScreenHandle(edgeDiffScreen);
		DeleteScreenHandle(edgeMaskScreen);
		DeleteScreenHandle(colorLevelScreen);
		DeleteScreenHandle(colorHsbScreen);

		sceneScreen = MakeScreen(sceneWidth, sceneHeight, FALSE);
		compositeScreen = MakeScreen(sceneWidth, sceneHeight, FALSE);
		bloomHighBrightScreen = MakeScreen(sceneWidth, sceneHeight, FALSE);
		bloomDownScaleScreen = MakeScreen(bloomDownScaleWidth, bloomDownScaleHeight, FALSE);
		bloomGaussScreen = MakeScreen(bloomDownScaleWidth, bloomDownScaleHeight, FALSE);
		edgeDownScaleScreen = MakeScreen(edgeDownScaleWidth, edgeDownScaleHeight, FALSE);
		edgeBlurScreen = MakeScreen(edgeDownScaleWidth, edgeDownScaleHeight, FALSE);
		edgeExpandedScreen = MakeScreen(sceneWidth, sceneHeight, FALSE);
		edgeDiffScreen = MakeScreen(sceneWidth, sceneHeight, FALSE);
		edgeMaskScreen = MakeScreen(sceneWidth, sceneHeight, FALSE);
		colorLevelScreen = MakeScreen(sceneWidth, sceneHeight, FALSE);
		colorHsbScreen = MakeScreen(sceneWidth, sceneHeight, FALSE);

		const bool ok =
			sceneScreen >= 0 &&
			compositeScreen >= 0 &&
			bloomHighBrightScreen >= 0 &&
			bloomDownScaleScreen >= 0 &&
			bloomGaussScreen >= 0 &&
			edgeDownScaleScreen >= 0 &&
			edgeBlurScreen >= 0 &&
			edgeExpandedScreen >= 0 &&
			edgeDiffScreen >= 0 &&
			edgeMaskScreen >= 0 &&
			colorLevelScreen >= 0 &&
			colorHsbScreen >= 0;

		if (!ok)
		{
			DeleteScreenHandle(sceneScreen);
			DeleteScreenHandle(compositeScreen);
			DeleteScreenHandle(bloomHighBrightScreen);
			DeleteScreenHandle(bloomDownScaleScreen);
			DeleteScreenHandle(bloomGaussScreen);
			DeleteScreenHandle(edgeDownScaleScreen);
			DeleteScreenHandle(edgeBlurScreen);
			DeleteScreenHandle(edgeExpandedScreen);
			DeleteScreenHandle(edgeDiffScreen);
			DeleteScreenHandle(edgeMaskScreen);
			DeleteScreenHandle(colorLevelScreen);
			DeleteScreenHandle(colorHsbScreen);
			return false;
		}

		return true;
	}

	bool CanUseAnyEffect()
	{
		return sceneScreen >= 0 && compositeScreen >= 0 &&
			(bloomEnabled || rimLightEnabled || aoEnabled || colorGradingEnabled);
	}

	void ExpandSmallScreenToFull(int srcHandle, int destHandle)
	{
		SetDrawScreen(destHandle);
		ClearDrawScreen();
		SetDrawMode(DX_DRAWMODE_BILINEAR);
		DrawExtendGraph(0, 0, sceneWidth, sceneHeight, srcHandle, FALSE);
		SetDrawMode(DX_DRAWMODE_NEAREST);
	}

	void BuildEdgeMask()
	{
		// 画面全体を小さくしてからぼかし、元画像との差分でエッジを抽出します。
		GraphFilterBlt(sceneScreen, edgeDownScaleScreen, DX_GRAPH_FILTER_DOWN_SCALE, edgeDownScale);
		GraphFilterBlt(edgeDownScaleScreen, edgeBlurScreen, DX_GRAPH_FILTER_GAUSS, edgeGaussPixelWidth, edgeGaussParam);
		ExpandSmallScreenToFull(edgeBlurScreen, edgeExpandedScreen);
		GraphBlendBlt(edgeExpandedScreen, sceneScreen, edgeDiffScreen, 255, DX_GRAPH_BLEND_DIFFERENCE);
		GraphFilterBlt(
			edgeDiffScreen,
			edgeMaskScreen,
			DX_GRAPH_FILTER_BRIGHT_CLIP,
			DX_CMP_LESS,
			params.edgeThreshold,
			TRUE,
			GetColor(0, 0, 0),
			255);
	}

	void ApplyBloom()
	{
		GraphFilterBlt(
			sceneScreen,
			bloomHighBrightScreen,
			DX_GRAPH_FILTER_BRIGHT_CLIP,
			DX_CMP_LESS,
			params.bloomBrightThreshold,
			TRUE,
			GetColor(0, 0, 0),
			255);

		GraphFilterBlt(bloomHighBrightScreen, bloomDownScaleScreen, DX_GRAPH_FILTER_DOWN_SCALE, bloomDownScale);
		GraphFilterBlt(bloomDownScaleScreen, bloomGaussScreen, DX_GRAPH_FILTER_GAUSS, params.bloomGaussPixelWidth, params.bloomGaussParam);

		SetDrawMode(DX_DRAWMODE_BILINEAR);
		SetDrawBlendMode(DX_BLENDMODE_ADD, params.bloomAlpha);
		for (int i = 0; i < params.bloomPasses; ++i)
		{
			DrawExtendGraph(0, 0, sceneWidth, sceneHeight, bloomGaussScreen, FALSE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		SetDrawMode(DX_DRAWMODE_NEAREST);
	}

	void ApplyAmbientOcclusionLike()
	{
		// 抽出済みエッジを少し広げ、黒で重ねるだけの軽量 AO 風処理。
		GraphFilterBlt(edgeMaskScreen, edgeExpandedScreen, DX_GRAPH_FILTER_GAUSS, params.aoSoftGaussWidth, params.aoSoftGaussParam);

		SetDrawMode(DX_DRAWMODE_BILINEAR);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, params.aoAlpha);
		SetDrawBright(0, 0, 0);
		for (int i = 0; i < params.aoPasses; ++i)
		{
			DrawGraph(0, 0, edgeExpandedScreen, FALSE);
		}
		SetDrawBright(255, 255, 255);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		SetDrawMode(DX_DRAWMODE_NEAREST);
	}

	void ApplyRimLightLike()
	{
		// エッジを少しだけ太らせて青白く加算し、輪郭が読み取りやすい見た目にします。
		GraphFilterBlt(edgeMaskScreen, edgeExpandedScreen, DX_GRAPH_FILTER_GAUSS, params.rimSoftGaussWidth, params.rimSoftGaussParam);

		SetDrawMode(DX_DRAWMODE_BILINEAR);
		SetDrawBlendMode(DX_BLENDMODE_ADD, params.rimAlpha);
		SetDrawBright(params.rimColorR, params.rimColorG, params.rimColorB);
		for (int i = 0; i < params.rimPasses; ++i)
		{
			DrawGraph(0, 0, edgeExpandedScreen, FALSE);
		}
		SetDrawBright(255, 255, 255);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		SetDrawMode(DX_DRAWMODE_NEAREST);
	}

	void ApplyColorGrading()
	{
		// レベル補正でコントラストを整え、HSB で彩度と明るさを少しだけ味付け。
		GraphFilterBlt(
			compositeScreen,
			colorLevelScreen,
			DX_GRAPH_FILTER_LEVEL,
			params.colorLevelMin,
			params.colorLevelMax,
			params.colorGamma,
			params.colorAfterMin,
			params.colorAfterMax);
		
		GraphFilterBlt(
			colorLevelScreen,
			colorHsbScreen,
			DX_GRAPH_FILTER_HSB,
			0,
			params.colorHueShift,
			params.colorSaturation,
			params.colorBrightness);

		SetDrawScreen(compositeScreen);
		ClearDrawScreen();
		DrawGraph(0, 0, colorHsbScreen, FALSE);

		if (params.colorTintAlpha > 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, params.colorTintAlpha);
			SetDrawBright(params.colorTintR, params.colorTintG, params.colorTintB);
			DrawBox(0, 0, sceneWidth, sceneHeight, GetColor(255, 255, 255), TRUE);
			SetDrawBright(255, 255, 255);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}

	void ApplyFogState()
	{
		const bool enableFogThisFrame = fogEnabled/* && IsTargetScene()*/;
		SetFogEnable(enableFogThisFrame ? TRUE : FALSE);
		if (!enableFogThisFrame)
		{
			return;
		}

		SetFogColor(params.fogColorR, params.fogColorG, params.fogColorB);
		SetFogStartEnd(static_cast<float>(params.fogStart), static_cast<float>(params.fogEnd));
	}

	void ResetDrawStateForUi()
	{
		// ImGui や 2D UI 側へ影響が残らないよう、描画状態を戻す。
		SetFogEnable(FALSE);
		SetDrawScreen(DX_SCREEN_BACK);
		SetDrawArea(0, 0, sceneWidth - 1, sceneHeight - 1);
		SetDrawMode(DX_DRAWMODE_NEAREST);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		SetDrawBright(255, 255, 255);
		SetUseZBufferFlag(TRUE);
		SetWriteZBufferFlag(TRUE);
		SetUseZBuffer3D(TRUE);
		SetWriteZBuffer3D(TRUE);
	}
}

namespace PostEffect
{
	void Init()
	{
		//ApplyShowcasePreset();
		params = defaultParameters;
		if (!CreateScreens())
		{
			bloomEnabled = false;
			rimLightEnabled = false;
			aoEnabled = false;
			colorGradingEnabled = false;
			fogEnabled = false;
		}
	}

	void Release()
	{
		DeleteScreenHandle(sceneScreen);
		DeleteScreenHandle(compositeScreen);
		DeleteScreenHandle(bloomHighBrightScreen);
		DeleteScreenHandle(bloomDownScaleScreen);
		DeleteScreenHandle(bloomGaussScreen);
		DeleteScreenHandle(edgeDownScaleScreen);
		DeleteScreenHandle(edgeBlurScreen);
		DeleteScreenHandle(edgeExpandedScreen);
		DeleteScreenHandle(edgeDiffScreen);
		DeleteScreenHandle(edgeMaskScreen);
		DeleteScreenHandle(colorLevelScreen);
		DeleteScreenHandle(colorHsbScreen);
		useThisFrame = false;
	}

	void BeginScene()
	{
		useThisFrame = /*IsTargetScene() &&*/ CanUseAnyEffect();

		if (useThisFrame)
		{
			SetDrawScreen(sceneScreen);
			ClearDrawScreen();
		}
		else
		{
			SetDrawScreen(DX_SCREEN_BACK);
		}

		ApplyFogState();
	}

	void EndScene()
	{
		if (!useThisFrame)
		{
			ResetDrawStateForUi();
			return;
		}

		ValidateParameters();

		// まず sceneScreen を合成用スクリーンへコピーし、
		// 以降のエフェクトは compositeScreen に順番に積む。
		SetDrawScreen(compositeScreen);
		ClearDrawScreen();
		DrawGraph(0, 0, sceneScreen, FALSE);

		if (rimLightEnabled || aoEnabled)
		{
			BuildEdgeMask();
		}

		SetDrawScreen(compositeScreen);

		if (aoEnabled)
		{
			ApplyAmbientOcclusionLike();
		}

		if (rimLightEnabled)
		{
			ApplyRimLightLike();
		}

		if (bloomEnabled)
		{
			ApplyBloom();
		}

		if (colorGradingEnabled)
		{
			ApplyColorGrading();
		}

		SetDrawScreen(DX_SCREEN_BACK);
		DrawGraph(0, 0, compositeScreen, FALSE);
		ResetDrawStateForUi();
	}

	void ToggleBloomEnabled()
	{
		bloomEnabled = !bloomEnabled;
	}

	void ToggleRimLightEnabled()
	{
		rimLightEnabled = !rimLightEnabled;
	}

	void ToggleAmbientOcclusionEnabled()
	{
		aoEnabled = !aoEnabled;
	}

	void ToggleColorGradingEnabled()
	{
		colorGradingEnabled = !colorGradingEnabled;
	}

	void ToggleFogEnabled()
	{
		fogEnabled = !fogEnabled;
	}

	bool IsBloomEnabled()
	{
		return bloomEnabled;
	}

	bool IsRimLightEnabled()
	{
		return rimLightEnabled;
	}

	bool IsAmbientOcclusionEnabled()
	{
		return aoEnabled;
	}

	bool IsColorGradingEnabled()
	{
		return colorGradingEnabled;
	}

	bool IsFogEnabled()
	{
		return fogEnabled;
	}

	Parameters& GetParameters()
	{
		return params;
	}

	void ResetParameters()
	{
		params = defaultParameters;
	}

	void ValidateParameters()
	{
		params.bloomBrightThreshold = std::clamp(params.bloomBrightThreshold, 0, 255);
		params.bloomGaussPixelWidth = std::clamp(params.bloomGaussPixelWidth, 1, 64);
		params.bloomGaussParam = std::clamp(params.bloomGaussParam, 1, 2000);
		params.bloomAlpha = std::clamp(params.bloomAlpha, 0, 255);
		params.bloomPasses = std::clamp(params.bloomPasses, 1, 8);

		params.edgeThreshold = std::clamp(params.edgeThreshold, 0, 255);

		params.rimSoftGaussWidth = std::clamp(params.rimSoftGaussWidth, 1, 64);
		params.rimSoftGaussParam = std::clamp(params.rimSoftGaussParam, 1, 2000);
		params.rimAlpha = std::clamp(params.rimAlpha, 0, 255);
		params.rimPasses = std::clamp(params.rimPasses, 1, 8);
		params.rimColorR = std::clamp(params.rimColorR, 0, 255);
		params.rimColorG = std::clamp(params.rimColorG, 0, 255);
		params.rimColorB = std::clamp(params.rimColorB, 0, 255);

		params.aoSoftGaussWidth = std::clamp(params.aoSoftGaussWidth, 1, 64);
		params.aoSoftGaussParam = std::clamp(params.aoSoftGaussParam, 1, 2000);
		params.aoAlpha = std::clamp(params.aoAlpha, 0, 255);
		params.aoPasses = std::clamp(params.aoPasses, 1, 8);

		params.colorLevelMin = std::clamp(params.colorLevelMin, 0, 255);
		params.colorLevelMax = std::clamp(params.colorLevelMax, 0, 255);
		if (params.colorLevelMin > params.colorLevelMax)
		{
			std::swap(params.colorLevelMin, params.colorLevelMax);
		}
		params.colorGamma = std::clamp(params.colorGamma, 1, 300);
		params.colorAfterMin = std::clamp(params.colorAfterMin, 0, 255);
		params.colorAfterMax = std::clamp(params.colorAfterMax, 0, 255);
		if (params.colorAfterMin > params.colorAfterMax)
		{
			std::swap(params.colorAfterMin, params.colorAfterMax);
		}
		params.colorHueShift = std::clamp(params.colorHueShift, -180, 180);
		params.colorSaturation = std::clamp(params.colorSaturation, -255, 255);
		params.colorBrightness = std::clamp(params.colorBrightness, -255, 255);
		params.colorTintR = std::clamp(params.colorTintR, 0, 255);
		params.colorTintG = std::clamp(params.colorTintG, 0, 255);
		params.colorTintB = std::clamp(params.colorTintB, 0, 255);
		params.colorTintAlpha = std::clamp(params.colorTintAlpha, 0, 255);

		params.fogColorR = std::clamp(params.fogColorR, 0, 255);
		params.fogColorG = std::clamp(params.fogColorG, 0, 255);
		params.fogColorB = std::clamp(params.fogColorB, 0, 255);
		params.fogStart = std::clamp(params.fogStart, 0, 200000);
		params.fogEnd = std::clamp(params.fogEnd, 0, 200000);
		if (params.fogStart > params.fogEnd)
		{
			std::swap(params.fogStart, params.fogEnd);
		}
	}
}
