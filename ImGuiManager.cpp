#include "ImGuiManager.h"

void ImGuiManager::Initialize(WinApp* winApp, DirectXCommon* dxCommon)
{
	HRESULT result;

	// ImGui�̃R���e�L�X�g�𐶐�
	ImGui::CreateContext();
	// Imgui�̃X�^�C����ݒ�
	ImGui::StyleColorsDark();

	// Win32�p�̏������֐�
	ImGui_ImplWin32_Init(winApp->GetHwnd());

	// �f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	// �f�X�N���v�^�q�[�v����
	result = dxCommon->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	// DirectX12�p�������֐�
	ImGui_ImplDX12_Init(
		dxCommon->GetDevice(),
		static_cast<int>(dxCommon->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

	// �t�H���g�̒ǉ�
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Draw(DirectXCommon* dxCommon)
{
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	// �f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �`��R�}���h�𔭍s
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}

void ImGuiManager::Begin()
{
	// ImGui�t���[���J�n
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	// �`��O����
	ImGui::Render();
}

void ImGuiManager::Finalize()
{
	// �I������
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// �f�X�N���v�^�q�[�v�����
	srvHeap_.Reset();
}

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager imGuiManager;

	return &imGuiManager;
}

ImGuiManager::ImGuiManager(){}
ImGuiManager::~ImGuiManager(){}
