#pragma once

/// <summary>
/// ゲーム画面用のポストエフェクトと軽量な見た目調整を管理します。
/// </summary>
namespace PostEffect
{
	/// <summary>
	/// ポストエフェクトの調整用パラメータ。
	/// </summary>
	struct Parameters
	{
		int bloomBrightThreshold;// 明るい部分の抽出閾値（0-255）
		int bloomGaussPixelWidth;// ブルームのぼかし幅（ピクセル単位）
		int bloomGaussParam;	 // ブルームのぼかし強さ（大きいほどぼかしが強くなる）
		int bloomAlpha;			 // ブルームの合成アルファ（0-255）
		int bloomPasses;		 // ブルームの重ね回数（1-8）

		int edgeThreshold;	// エッジとはみなす輝度差の閾値（0-255）

		int rimSoftGaussWidth;	 // リムライトのぼかし幅（ピクセル単位）
		int rimSoftGaussParam;	 // リムライトのぼかし強さ（大きいほどぼかしが強くなる）
		int rimAlpha;			 // リムライトの合成アルファ（0-255）
		int rimPasses;			 // リムライトの重ね回数（1-8）
		int rimColorR;			 // リムライトの色 R（0-255）
		int rimColorG;			 // リムライトの色 G（0-255）
		int rimColorB;			 // リムライトの色 B（0-255）

		int aoSoftGaussWidth;	 // AO 風処理のぼかし幅（ピクセル単位）
		int aoSoftGaussParam;	 // AO 風処理のぼかし強さ（大きいほどぼかしが強くなる）
		int aoAlpha;			 // AO 風処理の合成アルファ（0-255）
		int aoPasses;			 // AO 風処理の重ね回数（1-8）

		int colorLevelMin;		 // レベル補正の入力下限（0-255）
		int colorLevelMax;		 // レベル補正の入力上限（0-255）
		int colorGamma;			 // レベル補正のガンマ値（100 で 1.0）
		int colorAfterMin;		 // レベル補正後の最小値（0-255）
		int colorAfterMax;		 // レベル補正後の最大値（0-255）
		int colorHueShift;		 // 色相の相対シフト（-180〜180）
		int colorSaturation;	 // 彩度の相対補正（-255〜255）
		int colorBrightness;	 // 輝度の相対補正（-255〜255）
		int colorTintR;			 // 仕上げに重ねる色味 R（0-255）
		int colorTintG;			 // 仕上げに重ねる色味 G（0-255）
		int colorTintB;			 // 仕上げに重ねる色味 B（0-255）
		int colorTintAlpha;		 // 仕上げ色味のアルファ（0-255）

		int fogColorR;			 // 距離フォグの色 R（0-255）
		int fogColorG;			 // 距離フォグの色 G（0-255）
		int fogColorB;			 // 距離フォグの色 B（0-255）
		int fogStart;			 // フォグ開始距離
		int fogEnd;				 // フォグ終了距離
	};
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 解放
	/// </summary>
	void Release();

	/// <summary>
	/// 3D シーン描画の開始前に呼び出します。
	/// 距離フォグの設定もここで適用します。
	/// </summary>
	void BeginScene();

	/// <summary>
	/// 3D シーン描画の終了後に呼び出します。
	/// </summary>
	void EndScene();

	// エフェクトのオンオフ切り替え
	void ToggleBloomEnabled();
	void ToggleRimLightEnabled();
	void ToggleAmbientOcclusionEnabled();
	void ToggleColorGradingEnabled();
	void ToggleFogEnabled();

	// エフェクトのオンオフ状態を参照
	bool IsBloomEnabled();
	bool IsRimLightEnabled();
	bool IsAmbientOcclusionEnabled();
	bool IsColorGradingEnabled();
	bool IsFogEnabled();

	/// <summary>
	/// 現在の調整値を参照します。
	/// </summary>
	Parameters& GetParameters();

	/// <summary>
	/// 調整値を初期値へ戻します。
	/// </summary>
	void ResetParameters();

	/// <summary>
	/// 調整後に安全な範囲へ丸め込みます。
	/// </summary>
	void ValidateParameters();
}
