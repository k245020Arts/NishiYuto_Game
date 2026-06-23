#include "PadInput.h"
#include "inputManager.h"

PadInput::PadInput()
{
	playerNumber = PLAYER_1;
	//DontDestroyOnSceneChange(true);
}

PadInput::~PadInput()
{
	//コントローラーの振動を最後に止める。
	StopJoypadVibration(playerNumber);
	input.clear();
}

void PadInput::Update()
{
	GetJoypadXInputState(playerNumber, &inputNow);
	switch (playerNumber)
	{
	case PLAYER_1:
		GetJoypadXInputState(PLAYER_1, &inputNow);
		break;
	case PLAYER_2:
		GetJoypadXInputState(PLAYER_2, &inputNow);
		break;
	case PLAYER_3:
		GetJoypadXInputState(PLAYER_3, &inputNow);
		break;
	case PLAYER_4:
		GetJoypadXInputState(PLAYER_4, &inputNow);
		break;
	}

	//現フレームの入力をリストの先頭に保存する
	input.push_front(inputNow);
	//保存する最大フレーム数以上になったら一番昔の入力を削除
	if (input.size() > SAVE_FRAME_MAX)
	{
		input.pop_back();
	}
}

bool PadInput::GetAnyButtonPush(bool _getStick, int _backFrame)
{
	if (input.size() < 1)
	{
		return false;
	}

	std::list<XINPUT_STATE>::iterator it = input.begin();

	//それ以上戻れない場合や0フレーム(現在のフレームを取ろうとしている)場合は飛ばす
	if ((int)input.size() > _backFrame + 1 && _backFrame != 0)
	{
		std::advance(it, _backFrame);
	}

	//いずれかのボタンが押されたらtrueを返す
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		if ((*it).Buttons[i])
		{
			return true;
		}

	}

	//トリガーがデッドゾーンより押されたらtrueを返す
	if ((*it).LeftTrigger > ANY_KEY_TRIGGER_DEADZONE * 255 || (*it).RightTrigger > ANY_KEY_TRIGGER_DEADZONE * 255)
	{
		return true;
	}

	//スティックも取る設定ならスティックも取る
	if (_getStick)
	{
		//GetStickKnockingの返り値がS_NO_DIRECTIONではない=いずれかの方向に入力がされている
		if (GetStickKnocking(ANY_KEY_STICK_DEADZONE, _backFrame).leftStick != S_NO_DIRECTION || GetStickKnocking(ANY_KEY_STICK_DEADZONE, _backFrame).rightStick != S_NO_DIRECTION)
		{
			return true;
		}
	}
	
	//上のどれにも当てはまらないならfalseを返す
	return false;
}

bool PadInput::GetAnyButtonPush(bool _getStick)
{
	return GetAnyButtonPush(_getStick, 0);
}

bool PadInput::GetAnyButtonPut(bool _getStick)
{
	return (GetAnyButtonPush(_getStick, 0) && !GetAnyButtonPush(_getStick, 1));
}

bool PadInput::GetIsButtonPushingNow(int _button)
{
	if (input.front().Buttons[_button])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PadInput::GetIsButtonPutNow(int _button)
{
	if (input.size() > 1)
	{
		std::list<XINPUT_STATE>::iterator it = input.begin();
		XINPUT_STATE now = *it;
		std::advance(it, 1);
		XINPUT_STATE lastFrame = *it;

		if (now.Buttons[_button] && !lastFrame.Buttons[_button])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool PadInput::GetIsButtonReleaseNow(int _button)
{
	if (input.size() > 1)
	{
		std::list<XINPUT_STATE>::iterator it = input.begin();
		XINPUT_STATE now = *it;
		std::advance(it, 1);
		XINPUT_STATE lastFrame = *it;

		if (!now.Buttons[_button] && lastFrame.Buttons[_button])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/*bool PadInput::GetIsButtonPushingPast(int _button, int _frameNum)
{
	std::list<XINPUT_STATE>::iterator it = input.begin();
	XINPUT_STATE now = *it;

	//最大値より大きい値が入ったらそのままfalseを返す
	if (_frameNum >= input.size())
	{
		return false;
	}
	std::advance(it, _frameNum);

	if (it->Buttons[_button])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PadInput::GetIsButtonPutPast(int _button, int _frameNum)
{
	std::list<XINPUT_STATE>::iterator it = input.begin();

	//最大値より大きい値が入ったらそのままfalseを返す
	if (_frameNum >= input.size() - 1)
	{
		return false;
	}
	std::advance(it, _frameNum);

	XINPUT_STATE now = *it;
	std::advance(it, 1);
	XINPUT_STATE lastFrame = *it;

	if (now.Buttons[_button] && !lastFrame.Buttons[_button])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PadInput::GetIsButtonReleasePast(int _button, int _frameNum)
{
	std::list<XINPUT_STATE>::iterator it = input.begin();

	//最大値より大きい値が入ったらそのままfalseを返す
	if (_frameNum >= input.size() - 1)
	{
		return false;
	}
	std::advance(it, _frameNum);

	XINPUT_STATE now = *it;
	std::advance(it, 1);
	XINPUT_STATE lastFrame = *it;

	if (!now.Buttons[_button] && lastFrame.Buttons[_button])
	{
		return true;
	}
	else
	{
		return false;
	}
}*/

int PadInput::GetPushButtonTime(int _button)
{
	if (input.size() < _button)
	{
		return -1;
	}
	std::list<XINPUT_STATE>::iterator it = input.begin();
	XINPUT_STATE now = *it;

	if (!now.Buttons[_button])
	{
		return 0;
	}

	int time = 0;
	do
	{
		time++;
		if (time >= input.size())
		{
			break;
		}
		std::advance(it, 1);
		now = *it;
	} while (now.Buttons[_button]);

	return time;
}

StickInput PadInput::GetStickInput()
{
	StickInput result;
	result.leftStick = { (float)inputNow.ThumbLX / STICK_INPUT_MAX,(float)inputNow.ThumbLY / STICK_INPUT_MAX };
	result.rightStick = { (float)inputNow.ThumbRX / STICK_INPUT_MAX,(float)inputNow.ThumbRY / STICK_INPUT_MAX };

	return result;
}

StickInput PadInput::GetStickInput(int _backFrame)
{
	StickInput result;
	auto it = input.begin();
	//std::next(it, _frame);
	if (_backFrame > 0 && input.size() > _backFrame)
	{
		std::advance(it, _backFrame);//入力されたフレーム分イテレータを進める
	}

	result.leftStick = { (float)(*it).ThumbLX / STICK_INPUT_MAX,(float)(*it).ThumbLY / STICK_INPUT_MAX };
	result.rightStick = { (float)(*it).ThumbRX / STICK_INPUT_MAX,(float)(*it).ThumbRY / STICK_INPUT_MAX };

	return result;
}

StickDirection PadInput::GetStickKnocking(float _deadZone)
{
	StickDirection result{ S_NO_DIRECTION };
	StickFloat stickRot = GetStickRot();
	//ベクトルの量を求める
	StickFloat stickSize{ 0 };
	stickSize.leftStick = sqrtf(GetStickInput().leftStick.x * GetStickInput().leftStick.x + GetStickInput().leftStick.y * GetStickInput().leftStick.y);
	stickSize.rightStick = sqrtf(GetStickInput().rightStick.x * GetStickInput().rightStick.x + GetStickInput().rightStick.y * GetStickInput().rightStick.y);

	if (stickSize.leftStick > _deadZone)
	{
		result.leftStick = GetDirection(stickRot.leftStick);
	}
	if (stickSize.rightStick > _deadZone)
	{
		result.rightStick = GetDirection(stickRot.rightStick);
	}

	return result;
}

StickDirection PadInput::GetStickKnocking(float _deadZone, int _backFrame)
{
	StickDirection result{ S_NO_DIRECTION };
	StickFloat stickRot = GetStickRot(_backFrame);
	//ベクトルの量を求める
	StickFloat stickSize{ 0 };
	stickSize.leftStick = sqrtf(GetStickInput(_backFrame).leftStick.x * GetStickInput(_backFrame).leftStick.x + GetStickInput(_backFrame).leftStick.y * GetStickInput(_backFrame).leftStick.y);
	stickSize.rightStick = sqrtf(GetStickInput(_backFrame).rightStick.x * GetStickInput(_backFrame).rightStick.x + GetStickInput(_backFrame).rightStick.y * GetStickInput(_backFrame).rightStick.y);

	if (stickSize.leftStick > _deadZone)
	{
		result.leftStick = GetDirection(stickRot.leftStick);
	}
	if (stickSize.rightStick > _deadZone)
	{
		result.rightStick = GetDirection(stickRot.rightStick);
	}

	return result;
}

StickDirection PadInput::GetStickKnockingReverce(float _deadZone, int _backFrame)
{
	StickDirection result =  GetStickKnocking(_deadZone, _backFrame);
	result.leftStick = ReverceDirection(result.leftStick);
	result.rightStick = ReverceDirection(result.rightStick);
	return result;
}

StickDirection PadInput::GetStickKnockingPut(float _deadZone)
{
	StickDirection result{ S_NO_DIRECTION,S_NO_DIRECTION };

	//現フレームの入力
	StickDirection nowInput = GetStickKnocking(_deadZone);
	//過去フレームの入力
	StickDirection lastInput = GetStickKnocking(_deadZone, 1);

	if (nowInput.leftStick != lastInput.leftStick)
	{
		result.leftStick = nowInput.leftStick;
	}
	if (nowInput.rightStick != lastInput.rightStick)
	{
		result.rightStick = nowInput.rightStick;
	}

	return result;
}

StickFloat PadInput::GetStickRot()
{
	StickFloat result{ 0 };
	StickInput stick = GetStickInput();

	result.leftStick = atan2f((float)inputNow.ThumbLY, (float)inputNow.ThumbLX);
	result.rightStick = atan2f((float)inputNow.ThumbRY, (float)inputNow.ThumbRX);

	return result;
}

StickFloat PadInput::GetStickRot(int _backFrame)
{
	StickFloat result{ 0 };
	StickInput stick = GetStickInput(_backFrame);

	result.leftStick = atan2f((float)stick.leftStick.y, (float)stick.leftStick.x);
	result.rightStick = atan2f((float)stick.rightStick.y, (float)stick.rightStick.x);

	return result;
}

StickFloat PadInput::GetTriggerNow()
{
	StickFloat trigger{ inputNow.LeftTrigger / 255.0f, inputNow.RightTrigger / 255.0f };
	return trigger;
}

void PadInput::ControlVibrationStartTime(int _power, int _timeSecond)
{
	StartJoypadVibration(playerNumber, _power, _timeSecond * 1000);
}

void PadInput::ControlVibrationStartFrame(int _power, int _timeFrame)
{
	StartJoypadVibration(playerNumber, _power, _timeFrame * 1000 / 60);
}

void PadInput::StopControlVibrationStartFrame()
{
	StopJoypadVibration(playerNumber);
}

StickDirections PadInput::ReverceDirection(StickDirections _direction)
{
	StickDirections direction = S_NO_DIRECTION;
	switch (_direction)
	{
	case S_NO_DIRECTION:
		break;
	case S_DOWN:
		direction = S_UP;
		break;
	case S_DOWN_RIGHT:
		direction = S_UP_LEFT;
		break;
	case S_RIGHT:
		direction = S_LEFT;
		break;
	case S_UP_RIGHT:
		direction = S_DOWN_LEFT;
		break;
	case S_UP:
		direction = S_DOWN;
		break;
	case S_UP_LEFT:
		direction = S_DOWN_RIGHT;
		break;
	case S_LEFT:
		direction = S_RIGHT;
		break;
	case S_DOWN_LEFT:
		direction = S_UP_RIGHT;
		break;
	}
	return direction;
}

StickDirections PadInput::GetDirection(float _rot)
{
	_rot += DX_PI_F;//計算しやすい用に整数にする

	const float directionAngleMax = DX_PI_F / 8;

	if (_rot < directionAngleMax * 1)
	{
		return S_LEFT;
	}
	else if (_rot < directionAngleMax * 3)
	{
		return S_DOWN_LEFT;
	}
	else if (_rot < directionAngleMax * 5)
	{
		return S_DOWN;
	}
	else if (_rot < directionAngleMax * 7)
	{
		return S_DOWN_RIGHT;
	}
	else if (_rot < directionAngleMax * 9)
	{
		return S_RIGHT;
	}
	else if (_rot < directionAngleMax * 11)
	{
		return S_UP_RIGHT;
	}
	else if (_rot < directionAngleMax * 13)
	{
		const float a = DX_PI_F + DX_PI_F / 6;
		return S_UP;
	}
	else if (_rot < directionAngleMax * 15)
	{
		return S_UP_LEFT;
	}

	return S_LEFT;

}