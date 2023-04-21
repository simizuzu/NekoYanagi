#pragma once
#include "Keyboard.h"

#pragma warning(push)
#pragma warning(disable: 4365)
#include <memory>
#pragma warning(pop)

class Input
{
private:
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;

	Keyboard* keyboard = nullptr;

public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize();

	/// <summary>
	/// XV
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

