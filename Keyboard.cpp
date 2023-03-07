#include "Keyboard.h"

void Keyboard::Initialize(IDirectInput8* directInput)
{
	HRESULT result;
	WinApp* winApp = WinApp::GetInstance();

	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Keyboard::Update()
{
	// �S�L�[�̓��͏�Ԃ��擾����
	for (int i = 0; i < _countof(oldkey); i++)
	{
		oldkey[i] = key[i];
	}

	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(key), key);
}

bool Keyboard::PushKey(int keyNum) { // ���������
	return static_cast<bool>(key[keyNum]);
}

bool Keyboard::ReleaseKey(int keyNum) { // ���������
	return !key[keyNum] && !oldkey[keyNum];
}

bool Keyboard::TriggerPushKey(int keyNum) { // �������u��
	return key[keyNum] && !oldkey[keyNum];
}

bool Keyboard::TriggerReleaseKey(int keyNum) { // �������u��
	return !key[keyNum] && oldkey[keyNum];
}
