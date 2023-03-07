#pragma once
#include "AvoidWarning.h"
#include "WinApp.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Keyboard
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="directInput">:DirectInput�I�u�W�F�N�g</param>
	void Initialize(IDirectInput8* directInput);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �L�[����������Ԃ̏���
	/// </summary>
	/// <param name="keyNum">:�L�[�i���o�[</param>
	/// <returns>�L�[�i���o�[</returns>
	bool PushKey(int keyNum);
	/// <summary>
	/// �L�[�𗣂�����Ԃ̏���
	/// </summary>
	/// <param name="keyNum">:�L�[�i���o�[</param>
	/// <returns>�L�[�i���o�[</returns>
	bool ReleaseKey(int keyNum);
	/// <summary>
	/// �L�[���������u�Ԃ̏���
	/// </summary>
	/// <param name="keyNum">:�L�[�i���o�[</param>
	/// <returns>�L�[�i���o�[</returns>
	bool TriggerPushKey(int keyNum);
	/// <summary>
	/// �L�[�𗣂����u�Ԃ̏���
	/// </summary>
	/// <param name="keyNum">:�L�[�i���o�[</param>
	/// <returns>�L�[�i���o�[</returns>
	bool TriggerReleaseKey(int keyNum);

private:

	WinApp* winApp_ = nullptr;

	ComPtr<IDirectInputDevice8> keyboard;

	BYTE oldkey[256] = {};
	BYTE key[256] = {};
};

