#pragma once
#include "AvoidWarning.h"
#include "Shader.h"
#include "Pipeline.h"
#include "DirectXCommon.h"
#include "WorldTransform.h"


class ParticleManager
{
public:
	static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);
	static void LoadTexture(const wchar_t* texturename);
private:
	static void InitializeDescriptorHeap();
	static void CreateCraphicsPipeline();

	static void CreateModel();

public:
	void Draw(WorldTransform* transform);

	void Update();

	void Add(int life, NYMath::Vector3 position, NYMath::Vector3 velocity, NYMath::Vector3 accel, float start_scale, float end_scale, NYMath::Vector4 s_color, NYMath::Vector4 e_color);

	void RandParticle();

	ParticleManager() = default;
	~ParticleManager();

private:
	static const int vertexCount = 1024;	// 頂点数

private:
	// デバイス
	static Microsoft::WRL::ComPtr<ID3D12Device> device_;
	// コマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList_;
	// パイプラインステート
	static RootSigSetPip pip;

	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	static Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	static Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	static UINT descriptorHandleIncrementSize;
	// 頂点データ配列
	static VertexPos vertices[vertexCount];
	static D3D12_VERTEX_BUFFER_VIEW vbView;

	static NYMath::Matrix4 matView;
	static NYMath::Matrix4 matProjection;

private:
	//Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	std::forward_list<Particle> particles;
};

