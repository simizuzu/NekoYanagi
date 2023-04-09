#pragma once
#include "AvoidWarning.h"
#include "DirectXCommon.h"
#include "WinApp.h"

class ImGuiManager final
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	/// <summary>
	/// ImGui��t�J�n
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui��t�I��
	/// </summary>
	void End();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	// �V���O���g��
	static ImGuiManager* GetInstance();

private:
	// SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

private:
	ImGuiManager();
	~ImGuiManager();
	ImGuiManager& operator=(const ImGuiManager&) = delete;
	ImGuiManager(const ImGuiManager&) = delete;
};

