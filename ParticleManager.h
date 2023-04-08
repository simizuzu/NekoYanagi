#pragma once
#include "AvoidWarning.h"
#include "Shader.h"
#include "Pipeline.h"
#include "DirectXCommon.h"



class ParticleManager
{
private:
	static void CrreateCraphicsPipeline();
	static void LoadTexture(const wchar_t* texturename);
	static void CreateModel();

private:
	void InitializeVerticesBuff();

public:
	void Draw();

private:
	static const int vertexCount = 1024;	// ���_��

private:
	// �f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D12Device> device_;
	// �R�}���h���X�g
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList_;
	// �p�C�v���C���X�e�[�g
	static RootSigSetPip pip;

	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	static Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	static Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	static UINT descriptorHandleIncrementSize;
	// ���_�f�[�^�z��
	static VertexPos vertices[vertexCount];
	static D3D12_VERTEX_BUFFER_VIEW vbView{};

};

