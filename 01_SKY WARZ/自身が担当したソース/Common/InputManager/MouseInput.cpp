#include "MouseInput.h"
#include "../../Screen.h"
#include "inputManager.h"

MouseInput::MouseInput()
{
	SetMouseDispFlag(true);
	//DontDestroyOnSceneChange(true);
	SetUseDirectInputFlag(true);
	nowMouseWheelVol = 0;
}

MouseInput::~MouseInput()
{
	buttonInput.clear();
}

void MouseInput::Update()
{
	nowMouseWheelVol = GetMouseWheelRotVol();

	PushInputList();
}

VECTOR2F MouseInput::GetMousePos(bool _canScreenOut)const
{
	int x, y;
	GetMousePoint(&x,&y);
	VECTOR2 result((float)x, (float)y);

	//false‚È‚ç‰æ–ÊŠO‚Ì’l‚ð‰æ–Ê’[‚É‚·‚é
	if(!_canScreenOut)
	{
		if (result.x > Screen::WIDTH)
		{
			result.x = Screen::WIDTH;
		}
		else if (result.x < 0)
		{
			result.x = 0;
		}
		if (result.y > Screen::HEIGHT)
		{
			result.y = Screen::HEIGHT;
		}
		else if (result.y < 0)
		{
			result.y = 0;
		}
	}
	return result;
}

void MouseInput::SetMousePos(const VECTOR2F& _pos)
{
	SetMousePoint((int)_pos.x, (int)_pos.y);
}

bool MouseInput::GetMousePushing(MouseButton _button)const
{
	return currentButtonInput.input[_button];
}

bool MouseInput::GetMousePut(MouseButton _button)const
{
	if(buttonInput.size()>1)
	{
		auto it = buttonInput.begin();
		MouseButtonInput currentInput = *it;
		it++;
		MouseButtonInput lastFrameInput = *it;

		return (currentInput.input[_button]) && (!lastFrameInput.input[_button]);
	}
	return false;
}

int MouseInput::GetMouseWheelFrame()const
{
	return nowMouseWheelVol;
}

bool MouseInput::IsCursorSquareCollision(const VECTOR2F& _centerPos, const VECTOR2F& _size)const
{
	const VECTOR2 leftUpPos = _centerPos - _size;
	const VECTOR2 rightDownPos = _centerPos + _size;
	const VECTOR2 mousePos = GetMousePos(false);

	if (leftUpPos.x < mousePos.x && mousePos.x < rightDownPos.x &&
		leftUpPos.y < mousePos.y && mousePos.y < rightDownPos.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MouseInput::SetCurrentButtonInput()
{
	int input= GetMouseInput();

	currentButtonInput.input[LEFT_CLICK] = (input & MOUSE_INPUT_LEFT);
	currentButtonInput.input[RIGHT_CLICK] = (input & MOUSE_INPUT_RIGHT);
	currentButtonInput.input[WHEEL_BUTTON] = (input & MOUSE_INPUT_MIDDLE);
	currentButtonInput.input[BUTTON_4] = (input & MOUSE_INPUT_4);
	currentButtonInput.input[BUTTON_5] = (input & MOUSE_INPUT_5);
	currentButtonInput.input[BUTTON_6] = (input & MOUSE_INPUT_6);
	currentButtonInput.input[BUTTON_7] = (input & MOUSE_INPUT_7);
	currentButtonInput.input[BUTTON_8] = (input & MOUSE_INPUT_8);
}

void MouseInput::PushInputList()
{
	SetCurrentButtonInput();
	buttonInput.push_front(currentButtonInput);
	if (SAVE_FRAME_MAX < buttonInput.size())
	{
		buttonInput.pop_back();
	}
}
