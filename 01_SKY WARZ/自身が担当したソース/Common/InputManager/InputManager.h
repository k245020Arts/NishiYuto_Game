#pragma once
#include <list>
#include "../../../Library/gameObject.h"
#include "PadInput.h"
#include "KeyboardInput.h"
#include "MouseInput.h"
#include <unordered_map>
#include "../Singleton/SingletonBase.h"

class PadInput;
class KeyboardInput;

//#define KEY_GUIDE //キーボードのガイドを付けるならdefineを定義

/// <summary>
/// キーコンフィグ用のデータ
/// これを使うと直接ボタンの種類をやるのではなく、文字列でキーボード、コントローラー、マウスが対応可能になる
/// </summary>
struct KeyConfigData
{
	
	std::string action;
	int keyNumber;
	int padNumber;
	MouseInput::MouseButton mouceNumber;

	KeyConfigData(): KeyConfigData("", -1, -1, MouseInput::MouseButton::MOUSE_NONE){}

	KeyConfigData(std::string _act, int _key, int _pad, MouseInput::MouseButton _mouce) {
		action = _act;
		keyNumber = _key;
		padNumber = _pad;
		mouceNumber = _mouce;
	}
};

namespace
{
	//何フレーム前の入力まで覚えておくか
	constexpr int SAVE_FRAME_MAX = 600;
	//スティック入力の最大値
	constexpr int STICK_INPUT_MAX = 32767;
	
}

/// <summary>
/// プレイヤーの操作の入力を管理するクラス
///	シーンを跨いで存在できるようにしているためBootSceneで生成推奨
/// </summary>
class InputManager : public SingletonBase<InputManager>
{
public:
	/// <summary>
	/// 使用したいデバイスのみ生成推奨
	/// 生成したデバイスはGet関数で取得可能
	/// </summary>
	/// <param name="_useController">コントローラー入力を取るか</param>
	/// <param name="_useKeyboard">キーボード入力を取るか</param>
	/// <param name="_useMouse">マウス入力を取るか</param>
	
	void Update()override;

	///<summary>指定したキーボードのキーかコントローラーのボタンが押されている間Trueを返す</summary>
	//bool GetIsKeyOrButtonPushingNow(int _key, int _button);
	///<summary>指定したキーボードのキーかコントローラーのボタンの押し始め1F目のみTrueを返す</summary>
	//bool GetIsKeyOrButtonPutNow(int _key, int _button);
	///<summary>指定したキーボードのキーかコントローラーのボタンを押して離し始めた1F目のみTrueを返す</summary>
	//bool GetIsKeyOrButtonReleaseNow(int _key, int _button);

	const PadInput* GetControllerInput()const;
	const KeyboardInput* GetKeyboardInput()const;
	const MouseInput* GetMouseInput()const;
	/// <summary>
	/// キーコンフィグ用データを生成する。
	/// </summary>
	/// <param name="_data"></param>
	void CreateInputData(const KeyConfigData& _data);
	//キーコンフィグ用のボタンの種類
	const bool KeyInputDown(const std::string& _name)const;//最初の一回のみ
	const bool KeyInput(const std::string& _name)const;//入力し続けている間
	const bool KeyInputUp(const std::string& _name)const;//話した瞬間

	/// <summary>
	/// データが存在しているかどうか
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <returns>登録しているかどうか</returns>
	const bool IsData(const std::string& _name)const;
	/// <summary>
	/// ImguiのInput系でinputされているかどうか
	/// </summary>
	/// <returns>通常入力が出来るならtrue</returns>
	const bool IsInputCan()const;

private:

	InputManager();
	~InputManager();

	friend class SingletonBase<InputManager>;

	PadInput* controller;
	KeyboardInput* keyboard;
	MouseInput* mouse;

	std::unordered_map<std::string, KeyConfigData> inputData;
};