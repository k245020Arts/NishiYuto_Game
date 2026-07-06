#include "inputManager.h"
#include "../Debug/Debug.h"
#include "../../../ImGui/imgui.h"

InputManager::InputManager()
{
	controller = nullptr;
	keyboard = nullptr;
	mouse = nullptr;
	if (controller == nullptr)
	{
		controller = new PadInput();
	}
	if (keyboard == nullptr)
	{
		keyboard = new KeyboardInput();
	}
	if (mouse == nullptr)
	{
		mouse = new MouseInput();
	}

	//DontDestroyOnSceneChange(true);

	//------------------------------inputの情報データを格納--------------------------------------------------------

	//入力を受け付けない値
	const int NOT_INPUT = -1;

	CreateInputData(KeyConfigData("attack", KEY_INPUT_E, XINPUT_BUTTON_B, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("avoid", KEY_INPUT_X, XINPUT_BUTTON_A, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("SceneChange", KEY_INPUT_P, XINPUT_BUTTON_A, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("debugChange", KEY_INPUT_Z, NOT_INPUT, MouseInput::RIGHT_CLICK));
	CreateInputData(KeyConfigData("camera", KEY_INPUT_SPACE, XINPUT_BUTTON_RIGHT_THUMB, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("changeMode", KEY_INPUT_TAB, NOT_INPUT, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("SpecialAttack", KEY_INPUT_Q, XINPUT_BUTTON_X, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("heavyAttack", KEY_INPUT_C, XINPUT_BUTTON_Y, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("Mouse", KEY_INPUT_F1, NOT_INPUT, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("playerParam", KEY_INPUT_LCONTROL, NOT_INPUT, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("BossParamWindow", KEY_INPUT_F2, NOT_INPUT, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("PauseScreen", KEY_INPUT_F3, XINPUT_BUTTON_START, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("PauseSelect", KEY_INPUT_SPACE, XINPUT_BUTTON_A, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("PauseUp", KEY_INPUT_UP, XINPUT_BUTTON_DPAD_UP, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("PauseDown", KEY_INPUT_DOWN, XINPUT_BUTTON_DPAD_DOWN, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("StageSelectRight", KEY_INPUT_RIGHT, XINPUT_BUTTON_DPAD_RIGHT, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("StageSelectLeft", KEY_INPUT_LEFT, XINPUT_BUTTON_DPAD_LEFT, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("GameBack", KEY_INPUT_SPACE, XINPUT_BUTTON_A, MouseInput::MOUSE_NONE));
	CreateInputData(KeyConfigData("PauseBack", KEY_INPUT_F3, XINPUT_BUTTON_B, MouseInput::MOUSE_NONE));

	//-------------------------------------------------------------------------------------------------------------
}

InputManager::~InputManager()
{
	inputData.clear();

	if (controller != nullptr)
	{
		delete controller;
		controller = nullptr;
	}
	if (keyboard != nullptr)
	{
		delete keyboard;
	}
	if (mouse != nullptr)
	{
		delete mouse;
	}
}

void InputManager::Update()
{
	if (controller != nullptr)
	{
		controller->Update();
	}
	if (keyboard != nullptr)
	{
		keyboard->Update();
	}
	if (mouse != nullptr)
	{
		mouse->Update();
	}
}

PadInput* InputManager::GetControllerInput()
{
	if (controller != nullptr)
	{
		return controller;
	}
	return nullptr;
}

KeyboardInput* InputManager::GetKeyboardInput()
{
	if (keyboard != nullptr)
	{
		return keyboard;
	}
	return nullptr;
}

MouseInput* InputManager::GetMouseInput()
{
	if(mouse!=nullptr)
	{
		return mouse;
	}
	return nullptr;
}

void InputManager::CreateInputData(const KeyConfigData& _data)
{
	auto it = inputData.find(_data.action);
	if (inputData.end() == it) {
		inputData[_data.action] = _data;
	
	}
	else {
		Debug::DebugLog("そのInputDataは存在しています" + _data.action);
	}
}

bool InputManager::KeyInputDown(std::string _name)
{
	//Imguiで入力中なら入力を無視する
	if (!IsInputCan()) {
		return false;
	}
	bool data = IsData(_name);
	if (!data) {//データが存在していないならキーを入力をしていないものとして返す
		Debug::DebugLog("noAction");
		return false;
	}
	PadInput* c = GetControllerInput();
	KeyboardInput* k = GetKeyboardInput();
	MouseInput* m = GetMouseInput();
	bool control = false;
	bool key = false;
	bool mouse = false;

	if (c != nullptr && inputData[_name].padNumber >= 0) {
		control = c->GetIsButtonPutNow(inputData[_name].padNumber);
	}
	if (k != nullptr && inputData[_name].keyNumber >= 0) {
		key = k->GetIsKeyboardPut(inputData[_name].keyNumber);
	}
	if (m != nullptr && inputData[_name].mouceNumber >= 0) {
		mouse = m->GetMousePut(inputData[_name].mouceNumber);
	}
	//どれかしらがInputされていたらtrue
	if (control || key || mouse) {
		//printfDx("入力");
		return true;
	}
	return false;
}

bool InputManager::KeyInput(std::string _name)
{
	//Imguiで入力中なら入力を無視する
	if (!IsInputCan()) {
		return false;
	}
	bool data = IsData(_name);
	if (!data) { //データが存在していないならキーを入力をしていないものとして返す
		Debug::DebugLog("noAction");
		return false;
	}
	PadInput* c = GetControllerInput();
	KeyboardInput* k = GetKeyboardInput();
	MouseInput* m = GetMouseInput();
	bool control = false;
	bool key = false;
	bool mouse = false;

	if (c != nullptr && inputData[_name].padNumber >= 0) {
		control = c->GetIsButtonPushingNow(inputData[_name].padNumber);
	}
	if (k != nullptr && inputData[_name].keyNumber >= 0) {
		key = k->GetIsKeyboardPushing(inputData[_name].keyNumber);
	}
	if (m != nullptr && inputData[_name].mouceNumber >= 0) {
		mouse = m->GetMousePushing(inputData[_name].mouceNumber);
	}
	//どれかしらがInputされていたらtrue
	if (control || key || mouse) {
		return true;
	}
	return false;
}

bool InputManager::KeyInputUp(std::string _name)
{
	//Imguiで入力中なら入力を無視する
	if (!IsInputCan()) {
		return false;
	}
	bool data = IsData(_name);
	if (!data) {//データが存在していないならキーを入力をしていないものとして返す
		Debug::DebugLog("noAction");
		return false;
	}
	PadInput* c = GetControllerInput();
	KeyboardInput* k = GetKeyboardInput();
	MouseInput* m = GetMouseInput();
	bool control = false;
	bool key = false;
	bool mouse = false;

	if (c != nullptr && inputData[_name].padNumber >= 0) {
		control = c->GetIsButtonReleaseNow(inputData[_name].padNumber);
	}
	if (k != nullptr && inputData[_name].keyNumber >= 0) {
		key = k->GetIsKeyboardRelease(inputData[_name].keyNumber);
	}
	if (m != nullptr && inputData[_name].mouceNumber >= 0) {
		mouse = m->GetMousePut(inputData[_name].mouceNumber);
	}
	//どれかしらがInputされていたらtrue
	if (control || key || mouse) {
		return true;
	}
	return false;
}

bool InputManager::IsData(std::string _name)
{
	auto it = inputData.find(_name);
	if (inputData.end() == it) {//データが存在していないならfalse
		return false;
	}
	else {
		return true;
	}
}

bool InputManager::IsInputCan()
{
	ImGuiIO& io = ImGui::GetIO();
	return !io.WantCaptureKeyboard;
}

/*bool InputManager::GetIsKeyOrButtonPushingNow(int _key, int _button)
{
	if(GetIsKeyPushingNow(_button)||GetIsKeyboardPushing(_key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::GetIsKeyOrButtonPutNow(int _key, int _button)
{
	if (GetIsKeyPutNow(_button) || GetIsKeyboardPut(_key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::GetIsKeyOrButtonReleaseNow(int _key, int _button)
{
	if (GetIsKeyReleaseNow(_button) || GetIsKeyboardRelease(_key))
	{
		return true;
	}
	else
	{
		return false;
	}
}*/