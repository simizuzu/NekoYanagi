#pragma once
#include "AvoidWarning.h"
#include "WinApp.h"

class DirectXCommon
{
private:
	//DirectX����������
	HRESULT result;
	char PADDING[4];

	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	// �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{}; // �O�ɏo���Ȃ���G���[�N����
	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	// �t�F���X�̐���
	UINT64 fenceVal = 0;
	//�o���A�[�f�X�N
	D3D12_RESOURCE_BARRIER barrierDesc{};
	// �r���[�|�[�g
	D3D12_VIEWPORT viewport{};
	// �V�U�[��`
	D3D12_RECT scissorRect{};
	// �w�i�F
	FLOAT clearColor[4] = { 0.1f,0.25f, 0.5f,0.0f }; // ���ΐF

	// DXGI�܂�菉����
	void InitializeDXGI();
	// �ŏI�I�ȃ����_�[�^�[�Q�b�g�̐���
	void InitializeRtv();
	// �X���b�v�`�F�C���̐���
	void InitializeSwapChain();
	// �R�}���h�܂�菉����
	void InitializeCommand();
	// �t�F���X����
	void InitializeFence();
	// �[�x�o�b�t�@�̏�����
	void InitializeDepthBuffer();
	//�f�o�b�O���C���[��L���ɂ���
	void EnableDebugLayer();
	void BreakOnSeverity();

	static DirectXCommon* dxCommon_;
	WinApp* winApp_ = nullptr;

public:
	static DirectXCommon* GetInstance();

	// DirectX���t���[��������������
	void Initialize();
	void PreDraw(WinApp* winApp);
	void PostDraw();
	void ExecuteCommand();
	// DirectX���t���[�����������܂�

	// �Z�b�^�[
	//�w�i�F�ύX(RGBA)
	void SetBackScreenColor(float red, float green, float blue, float alpha);

	// �Q�b�^�[
	ID3D12Device* GetDevice();
	IDXGIFactory7* GetDxgiFactory();
	IDXGISwapChain4* GetSwapChain();
	ID3D12CommandAllocator* GetCmdAllocator();
	ID3D12GraphicsCommandList* GetCommandList();
	ID3D12CommandQueue* GetCommandQueue();
	ID3D12DescriptorHeap* GetRtvHeap();
	ID3D12Fence* GetFence();
	UINT64 GetFenceVal();

	size_t GetBackBufferCount() const;

	/// <summary>
	/// �������
	/// </summary>
	void Delete();

private:
	DirectXCommon();
	~DirectXCommon();
	DirectXCommon& operator=(const DirectXCommon&) = delete;
	DirectXCommon(const DirectXCommon&) = delete;
};

