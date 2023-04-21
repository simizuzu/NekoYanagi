#pragma once
#pragma warning(push)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)

#include <Windows.h>
#include <cstdint>
#include <string>
#include <wrl.h>
#pragma warning(pop)

class WinApp
{
public: // 静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // 定数
	// ウィンドウ横幅
	static const int window_height = 720;
	// ウィンドウ縦幅
	static const int window_width = 1280;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	bool ProccessMessage();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Delete();

	// シングルトン
	static WinApp* GetInstance();

public: // ゲッター
	HWND GetHwnd();
	HINSTANCE GetHinstance();

private: // メンバ変数
	// ウィンドウハンドル
	HWND hwnd = nullptr;
	// ウィンドウクラスの設定
	WNDCLASSEX w{};
	static WinApp* winApp_;

private:
	WinApp();
	~WinApp();
	WinApp& operator=(const WinApp&) = delete;
	WinApp(const WinApp&) = delete;
};