#include "Input.h"

Input::Input() {}
Input::~Input(){}

void Input::Initialize()
{
	HRESULT result;
	WinApp* winApp = WinApp::GetInstance();

	//DirectInputの初期化
	result = DirectInput8Create(winApp->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// キーボード初期化
	keyboard = new Keyboard();
	keyboard->Initialize(directInput.Get());
}

void Input::Update()
{
	// キーボード処理
	keyboard->Update();
}

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

#pragma region キーボード
unsigned char Input::GetKeyTime(int keyName)
{
	if (PushKey(keyName))
	{
		key[keyName]++;
		if (key[keyName] >= 255)
		{
			key[keyName] = 254;
		}
	}
	else
	{
		key[keyName] = 0;
	}
	return key[keyName];
}

bool Input::PushKey(int keyNum) { // 押した状態
	return keyboard->PushKey(keyNum);
}

bool Input::ReleaseKey(int keyNum) { // 離した状態
	return keyboard->ReleaseKey(keyNum);
}

bool Input::TriggerPushKey(int keyNum) { // 押した瞬間
	return keyboard->TriggerPushKey(keyNum);
}

bool Input::TriggerReleaseKey(int keyNum) { // 離した瞬間
	return keyboard->TriggerReleaseKey(keyNum);
}
#pragma endregion

