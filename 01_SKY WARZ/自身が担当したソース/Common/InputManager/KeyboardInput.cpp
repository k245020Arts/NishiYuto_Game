#include "KeyboardInput.h"
#include "../../../ImGui/imgui.h"

KeyboardInput::KeyboardInput()
{
	//DontDestroyOnSceneChange(true);
}

KeyboardInput::~KeyboardInput()
{
	keyboardInput.clear();
}

void KeyboardInput::Update()
{
	GetHitKeyStateAll(keyboardInputNow.kInput);

	keyboardInput.push_front(keyboardInputNow);
	if (keyboardInput.size() > SAVE_FRAME_MAX)
	{
		keyboardInput.pop_back();
	}
}


bool KeyboardInput::GetIsKeyboardPushing(const int _keyCode)const
{
	if (!IsInputCan()) {
		return false;
	}
	if (keyboardInput.front().kInput[_keyCode])
	{
		return true;
	}
	return false;
}

bool KeyboardInput::GetIsKeyboardPut(const int _keyCode)const
{
	if (!IsInputCan()) {
		return false;
	}
	if(keyboardInput.size()>1)
	{
		auto it = keyboardInput.begin();
		const KeyboardInputData now = *it;
		std::advance(it, 1);
		const KeyboardInputData lastFrame = *it;

		if (now.kInput[_keyCode] && !lastFrame.kInput[_keyCode])
		{
			return true;
		}
	}
	return false;
}

bool KeyboardInput::GetIsKeyboardRelease(const int _keyCode)const
{
	if (!IsInputCan()) {
		return false;
	}
	if (keyboardInput.size() > 1)
	{
		std::list<KeyboardInputData>::const_iterator it = keyboardInput.begin();
		const KeyboardInputData now = *it;
		std::advance(it, 1);
		const KeyboardInputData lastFrame = *it;

		if (!now.kInput[_keyCode] && lastFrame.kInput[_keyCode])
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	return false;
}

bool KeyboardInput::IsInputCan()const
{
	const ImGuiIO& io = ImGui::GetIO();
	return !io.WantCaptureKeyboard;
}