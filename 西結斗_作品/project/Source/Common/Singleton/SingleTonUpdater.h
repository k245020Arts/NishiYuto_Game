#pragma once

/// <summary>
/// シングルトンの更新処理を回す場所 App.cppから直接回す
/// </summary>
class SingleTonUpdater
{
public:
	SingleTonUpdater();
	~SingleTonUpdater();

	/// <summary>
	/// 更新処理
	/// </summary>
	static void Update();
	/// <summary>
	/// 描画
	/// </summary>
	static void Draw();
	/// <summary>
	/// 破棄
	/// </summary>
	static void Release();

private:

};
