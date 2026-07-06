#pragma once
#include "../../../Library/GameObject.h"
#include "../../../Library/myDxLib.h"
#include <list>


class MouseInput
{
public:

	enum MouseButton
	{
		MOUSE_NONE = -1,
		LEFT_CLICK = 0,
		RIGHT_CLICK,
		WHEEL_BUTTON,
		BUTTON_4,
		BUTTON_5,
		BUTTON_6,
		BUTTON_7,
		BUTTON_8,

		BUTTON_NUM,
	};

	struct MouseButtonInput
	{
		bool input[BUTTON_NUM];
	};

	MouseInput();
	~MouseInput();
	void Update();

	///<summary>マウスカーソルの位置を取得する</summary>
	/// <param name="_canScreenOut">TRUEなら画面外の座標も得られる,FALSEなら画面端が最大(最小)値になる</param>
	/// <returns>マウスカーソルの座標</returns>
	VECTOR2F GetMousePos(bool _canScreenOut);
	///<summary>マウスカーソルの位置を移動させる</summary>
	/// <param name="_pos">マウスカーソルの移動先の座標</param>
	void SetMousePos(VECTOR2F _pos);
	
	///<summary></summary>
	/// <param name="_button"></param>
	/// <returns></returns>
	bool GetMousePushing(MouseButton _button);
	///<summary></summary>
	/// <param name="_button"></param>
	/// <returns></returns>
	bool GetMousePut(MouseButton _button);

	///<summary>現在のフレームのマウスホイールの移動量を取得する</summary>
	/// <returns>奥に動かしたなら正の値・手前に動かしたなら負の値が返り動かしていないなら0が返される</returns>
	int GetMouseWheelFrame()const;

	///<summary>マウスカーソルと引数で示した正方形の範囲が当たっているかを取得する</summary>
	/// <param name="_centerPos">範囲の中心の座標</param>
	/// <param name="_size">当たり判定を取りたい中心からの範囲</param>
	/// <returns>当たったならTrue,当たっていないならFalse</returns>
	bool IsCursorSquareCollision(VECTOR2F _centerPos, VECTOR2F _size);

private:
	//現フレームのボタン入力記録する
	void SetCurrentButtonInput();
	//現フレームのボタン入力をリストに記録する
	void PushInputList();

	int nowMouseWheelVol;
	
	std::list<MouseButtonInput>buttonInput;
	MouseButtonInput currentButtonInput;
};