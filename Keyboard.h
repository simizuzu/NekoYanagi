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
	/// 初期化
	/// </summary>
	/// <param name="directInput">:DirectInputオブジェクト</param>
	void Initialize(IDirectInput8* directInput);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーを押した状態の処理
	/// </summary>
	/// <param name="keyNum">:キーナンバー</param>
	/// <returns>キーナンバー</returns>
	bool PushKey(int keyNum);
	/// <summary>
	/// キーを離した状態の処理
	/// </summary>
	/// <param name="keyNum">:キーナンバー</param>
	/// <returns>キーナンバー</returns>
	bool ReleaseKey(int keyNum);
	/// <summary>
	/// キーを押した瞬間の処理
	/// </summary>
	/// <param name="keyNum">:キーナンバー</param>
	/// <returns>キーナンバー</returns>
	bool TriggerPushKey(int keyNum);
	/// <summary>
	/// キーを離した瞬間の処理
	/// </summary>
	/// <param name="keyNum">:キーナンバー</param>
	/// <returns>キーナンバー</returns>
	bool TriggerReleaseKey(int keyNum);

private:

	WinApp* winApp_ = nullptr;

	ComPtr<IDirectInputDevice8> keyboard;

	BYTE oldkey[256] = {};
	BYTE key[256] = {};
};

