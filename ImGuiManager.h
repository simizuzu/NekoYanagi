#pragma once
#include "AvoidWarning.h"
#include "DirectXCommon.h"
#include "WinApp.h"

class ImGuiManager final
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	// シングルトン
	static ImGuiManager* GetInstance();

private:
	// SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

private:
	ImGuiManager();
	~ImGuiManager();
	ImGuiManager& operator=(const ImGuiManager&) = delete;
	ImGuiManager(const ImGuiManager&) = delete;
};

