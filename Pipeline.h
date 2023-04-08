#pragma once
#include "AvoidWarning.h"
#include "DirectXCommon.h"
#include "NekoYanagiUtility.h"

class Pipeline
{
public: // �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �����o�֐�

	/// <summary>
	/// �p�C�v���C���Ăяo���p�֐�(�X�v���C�g)
	/// </summary>
	static void CreateSpritePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend,ID3D12Device* device, std::array<RootSigSetPip,6>&pipeline);

	/// <summary>
	/// �p�C�v���C���Ăяo���p�֐�(3D�I�u�W�F�N�g)
	/// </summary>
	static void CreateObjPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend, ID3D12Device* device, RootSigSetPip& pipeline);

	/// <summary>
	/// �p�C�v���C���Ăяo���p�֐�(�p�[�e�B�N��)
	/// </summary>
	static void CreatePaticlePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob,ID3DBlob* gsBlob, BlendMode blend, ID3D12Device* device, RootSigSetPip& pipeline);

private: // �����o�ϐ�

	// �N���X�Ăяo��
	DirectXCommon* dxCommon_ = nullptr;
};

