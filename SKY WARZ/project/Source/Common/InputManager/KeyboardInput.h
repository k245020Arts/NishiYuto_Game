#pragma once
#include "../../../Library/GameObject.h"
#include "inputManager.h"

namespace
{
	//キーボードの入力を保存しておく配列の大きさ
	const int KEYBOARD_BUF_NUM = 256;
}

//キーボード入力のリスト用
struct KeyboardInputData
{
	char kInput[KEYBOARD_BUF_NUM];
};

class KeyboardInput
{
public:
	KeyboardInput();
	~KeyboardInput();
	void Update();

	///<summary>キーボード用　指定したキーが押されている間trueを返す</summary>
	/// <param name="_keyCode">キーの種類</param>
	bool GetIsKeyboardPushing(int _keyCode);
	///<summary>キーボード用　指定したキーが押され始めた1Fのみ間trueを返す</summary>
	/// <param name="_keyCode">キーの種類</param>
	bool GetIsKeyboardPut(int _keyCode);
	///<summary>キーボード用　指定したキーを押して離した1Fのみ間trueを返す</summary>
	/// <param name="_keyCode">キーの種類</param>
	bool GetIsKeyboardRelease(int _keyCode);
	/// <summary>
	/// ImguiのInput系でinputされているかどうか
	/// </summary>
	/// <returns>通常入力が出来るならtrue</returns>
	bool IsInputCan();

private:
	KeyboardInputData keyboardInputNow;
	std::list<KeyboardInputData>keyboardInput;

};