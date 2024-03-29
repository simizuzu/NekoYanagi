#pragma once
#include "AvoidWarning.h"
#include "Keyboard.h"

class Input
{
private:
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;

	Keyboard* keyboard = nullptr;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	unsigned char GetKeyTime(int keyName);
	bool PushKey(int keyNum);
	bool ReleaseKey(int keyNum);
	bool TriggerPushKey(int keyNum);
	bool TriggerReleaseKey(int keyNum);

	static Input* GetInstance();

private:
	BYTE oldkey[256] = {};
	BYTE key[256] = {};

	Input();
	~Input();
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;
};

