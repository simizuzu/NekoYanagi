#include "Input.h"

Input::Input() {}
Input::~Input(){}

void Input::Initialize()
{
	HRESULT result;
	WinApp* winApp = WinApp::GetInstance();

	//DirectInput�̏�����
	result = DirectInput8Create(winApp->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// �L�[�{�[�h������
	keyboard = new Keyboard();
	keyboard->Initialize(directInput.Get());
}

void Input::Update()
{
	// �L�[�{�[�h����
	keyboard->Update();
}

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

#pragma region �L�[�{�[�h
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

bool Input::PushKey(int keyNum) { // ���������
	return keyboard->PushKey(keyNum);
}

bool Input::ReleaseKey(int keyNum) { // ���������
	return keyboard->ReleaseKey(keyNum);
}

bool Input::TriggerPushKey(int keyNum) { // �������u��
	return keyboard->TriggerPushKey(keyNum);
}

bool Input::TriggerReleaseKey(int keyNum) { // �������u��
	return keyboard->TriggerReleaseKey(keyNum);
}
#pragma endregion

