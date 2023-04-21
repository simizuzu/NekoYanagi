#pragma once
#include "Camera.h"
#include "NekoYanagiUtility.h"

#pragma warning(push)
#include <d3d12.h>
#include <wrl.h>
#pragma warning(pop)

class WorldTransform
{

public:
	//������
	void Initialize();
	//�X�V
	void Update(Camera* camera,bool billboradFlag = false);

	//�Q�b�^�[
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress();

private: //�����o�֐�
	//�萔�o�b�t�@����
	void CreateConstBuffer();

private:
	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	WorldvViewProCamera constBuffMap;

public:
	//���[�J�����W
	NYMath::Vector3 translation_ = { 0.0f,0.0f,0.0f };
	//X,Y,Z�����̃��[�J����]�p
	NYMath::Vector3 rotation_ = { 0.0f,0.0f,0.0f };
	//���[�J���X�P�[��
	NYMath::Vector3 scale_ = { 1.0f,1.0f,1.0f };

	NYMath::Matrix4 matWorld;

	const WorldTransform* parent = nullptr;
};

