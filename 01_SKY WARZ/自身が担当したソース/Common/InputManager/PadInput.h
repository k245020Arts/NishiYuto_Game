#pragma once
#include "../../../Library/GameObject.h"

//1P～4Pのどの入力か
enum ControllerPlayer
{
	PLAYER_1 = DX_INPUT_PAD1,
	PLAYER_2 = DX_INPUT_PAD2,
	PLAYER_3 = DX_INPUT_PAD3,
	PLAYER_4 = DX_INPUT_PAD4,
};

enum StickDirections
{
	S_NO_DIRECTION,
	S_DOWN,
	S_DOWN_RIGHT,
	S_RIGHT,
	S_UP_RIGHT,
	S_UP,
	S_UP_LEFT,
	S_LEFT,
	S_DOWN_LEFT,

	S_DIRECTION_NUM,

};

//LRスティックの左右入力
struct StickInput
{
	DxLib::VECTOR2F leftStick;
	DxLib::VECTOR2F rightStick;

	StickInput() {
		leftStick = VECTOR2F();
		rightStick = VECTOR2F();
	}
};

//LRスティックのbool
struct StickBool
{
	bool isLeftStick;
	bool isRightStick;

	StickBool() {
		isLeftStick = false;
		isRightStick = false;
	}
};

//LRスティックのfloat
struct StickFloat
{
	float leftStick;
	float rightStick;

	StickFloat() {
		leftStick = 0.0f;
		rightStick = 0.0f;
	}
};

struct StickDirection
{
	StickDirections leftStick;
	StickDirections rightStick;

	StickDirection() {
		leftStick = S_DIRECTION_NUM;
		rightStick = S_DIRECTION_NUM;
	}
};


class PadInput
{
public:
	PadInput();
	~PadInput();
	void Update();

	/// <summary>コントローラーのボタンが押されているか確認する</summary>
	/// <param name="_getStick">スティックの傾きも取るか　通常はfalse</param>
	/// <returns>いずれかのボタンが押されたらTrueを返す</returns>
	const bool GetAnyButtonPush(bool _getStick = false)const;
	/// <summary>コントローラーのボタンが押されているか確認する</summary>
	/// <param name="_getStick">スティックの傾きも取るか 通常はfalse</param>
	/// <returns>いずれかのボタンが押されたらTrueを返す　前フレームも押されているならfalseを返す</returns>
	const bool GetAnyButtonPut(bool _getStick = false)const;

	//ボタンを押している間True
	const bool GetIsButtonPushingNow(int _button)const;
	//ボタンの押し始め1フレームのみTrue
	const bool GetIsButtonPutNow(int _button)const;
	//ボタンを押して離した1フレームのみTrue
	const bool GetIsButtonReleaseNow(int _button)const;

	/*///<summary>ボタンを押している間True(過去のフレームを取れる)
	/// ※指定したフレーム分前の入力が保存されていなかった場合、無条件でfalseを返す</summary>
	/// <param name="_button">ボタンの種類</param>
	/// <param name="_frameNum">何フレーム前の入力を取るか(SAVE_FRAME_MAX-1が最大値)</param>
	bool GetIsButtonPushingPast(int _button, int _frameNum);
	///<summary>指定したフレームがボタンを押し始めた1Fの場合True(過去のフレームを取れる)
	/// ※指定したフレーム分前の入力が保存されていなかった場合、無条件でfalseを返す</summary>
	/// <param name="_button">ボタンの種類</param>
	/// <param name="_frameNum">何フレーム前の入力を取るか(SAVE_FRAME_MAX-2が最大値)</param>
	bool GetIsButtonPutPast(int _button, int _frameNum);
	///<summary>指定したフレームがボタンを押して離した最初の1Fの場合True(過去のフレームを取れる)
	/// ※指定したフレーム分前の入力が保存されていなかった場合、無条件でfalseを返す</summary>
	/// <param name="_button">ボタンの種類</param>
	/// <param name="_frameNum">何フレーム前の入力を取るか(SAVE_FRAME_MAX-2が最大値)</param>
	bool GetIsButtonReleasePast(int _button, int _frameNum);*/

	///<summary>指定したボタンが何フレーム間押し続けられているかを確認する</summary>
	/// <param name="_button">ボタンの種類</param>
	/// <returns>押されているフレームの長さ(SAVE_FRAME_MAXが最大値)</returns>
	int GetPushButtonTime(int _button)const;

	///<summary>スティックの倒し具合を(-1~1)取る</summary>
	StickInput GetStickInput()const;
	///<summary>スティックの倒し具合を(-1~1)取る</summary>
	/// <param name="_backFrame">何フレーム前の入力を取るか</param>
	StickInput GetStickInput(int _backFrame)const;

	///<summary>スティックが倒されている方向を調べる</summary>
	/// <param name="_deadZone">デッドゾーン(全体の何割が倒れていたら反応させるか)</param>
	StickDirection GetStickKnocking(float _deadZone)const;
	///<summary>スティックが倒されている方向を調べる</summary>
	/// <param name="_deadZone">デッドゾーン(全体の何割が倒れていたら反応させるか)</param>
	/// /// <param name="_backFrame">何フレーム前の入力を取るか</param>
	StickDirection GetStickKnocking(float _deadZone, int _backFrame)const;
	///<summary>スティックが倒されている方向の逆方向をしらべる</summary>
	/// <param name="_deadZone">デッドゾーン(全体の何割が倒れていたら反応させるか)</param>
	/// /// <param name="_backFrame">何フレーム前の入力を取るか</param>
	StickDirection GetStickKnockingReverce(float _deadZone, int _backFrame)const;

	///<summary>スティックが倒されている方向を調べる</summary>
	/// <param name="_deadZone">デッドゾーン(全体の何割が倒れていたら反応させるか)</param>
	/// <returns>前回のフレームと今のフレームで倒した角度が違うなら値を返し、同じならS_NO_DIRECTIONが返る</returns>
	StickDirection GetStickKnockingPut(float _deadZone)const;

	///<summary>倒されているスティックの角度を入手する</summary>
	const StickFloat GetStickRot()const;
	///<summary>倒されているスティックの角度を入手する</summary>
	/// <param name="_backFrame">何フレーム前の入力を取るか</param>
	const StickFloat GetStickRot(int _backFrame)const;

	///<summary>現フレームのLRトリガーの押し込まれ具合を入手する(0~1)</summary>
	const StickFloat GetTriggerNow()const;

	///<summary>コントローラーの振動を行う</summary>
	/// <param name="_power">振動の強さ(0~1000)</param>
	///	<param name="_timeSecond">振動時間(秒数)</param>
	void ControlVibrationStartTime(int _power, int _timeSecond)const;

	///<summary>コントローラーの振動を行う(1秒=60フレームで計算)</summary>
	/// <param name="_power">振動の強さ(0~1000)</param>
	///	<param name="_timeFrame">振動時間(フレーム数)</param>
	void ControlVibrationStartFrame(int _power, int _timeFrame)const;
	/// <summary>
	/// コントローラーの振動をストップする
	/// </summary>
	/// <param name="_power"></param>
	/// <param name="_timeFrame"></param>
	void StopControlVibrationStartFrame()const;

	//何Pのコントローラーの情報を入手するかを設定する(何もしなかったら1P)
	void SetControllerPlayer(ControllerPlayer _playerNumber) { playerNumber = _playerNumber; }

	/// <summary>
	/// コントローラーが何台接続されているか返す
	/// </summary>
	/// <returns></returns>
	const int GetPadInputNum()const { return GetJoypadNum(); }
	/// <summary>
	/// コントローラーが接続されているかを返す
	/// </summary>
	/// <returns>tuueなら接続されている</returns>
	bool GetIsPadInput()const { return (GetJoypadNum() > 0); }

private:
	/// <summary>コントローラーのボタンが押されているか確認する</summary>
	/// <param name="_getStick">スティックの傾きも取るか</param>
	/// <param name="_backFrame">何フレーム前のものを取るか</param>
	/// <returns>いずれかのボタンが押されたらTrueを返す</returns>
	const bool GetAnyButtonPush(bool _getStick, int _backFrame)const;

	const StickDirections GetDirection(float _rot)const;

	const StickDirections ReverceDirection(StickDirections _direction)const;

	const int BUTTON_MAX = 16;//XINPUTで使うボタンの数
	const float ANY_KEY_TRIGGER_DEADZONE = 0.5f;//AnyButtonPushで使うトリガーのデッドゾーン
	const float ANY_KEY_STICK_DEADZONE = 0.5f;//AnyButtonPushで使うスティックのデッドゾーン

	//何Pか
	ControllerPlayer playerNumber;

	//現フレームの入力
	XINPUT_STATE inputNow;

	//過去の入力をまとめて保存しておくリスト
	std::list<XINPUT_STATE> input;
};