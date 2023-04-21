#pragma once
#include "WinApp.h"

#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)

#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include <DirectXMath.h>
#include <array>
#include <wrl.h>
#include <cassert>
#pragma warning(pop)

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

