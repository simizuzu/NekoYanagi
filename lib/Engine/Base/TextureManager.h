#pragma once
#include "DirectXCommon.h"
#include "NekoYanagiUtility.h"

#include "AvoidWarning.h"

/// <summary>
/// �e�N�X�`���}�l�[�W��
/// </summary>
class TextureManager
{
public: //�萔
	static const size_t MaxSRVCount = 256; // �e�N�X�`���̍ő喇��

public: //�����o�ϐ�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// �e�N�X�`���f�[�^
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	TextureData LoadTexture(const std::string& fileName);

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	static TextureData Load(const std::string& fileName);

	/// <summary>
	/// �e�N�X�`���o�b�t�@�̐���
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTexBuff(DirectX::TexMetadata& metadata, DirectX::ScratchImage& scrachImg);

	/// <summary>
	/// �V�F�[�_���\�[�X�r���[�̐���
	/// </summary>
	/// <param name="texbuff">�e�N�X�`���o�b�t�@</param>
	/// <param name="metadata"></param>
	D3D12_GPU_DESCRIPTOR_HANDLE CreateShaderResourceView(ID3D12Resource* texBuff, DirectX::TexMetadata& metadata);

	/// <summary>
	/// �������
	/// </summary>
	void Delete();

	// �C���X�^���X
	static TextureManager* GetInstance();

	// Getter
	ID3D12DescriptorHeap* GetSrvHeap();

private: // �����o�ϐ�
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	// �e�N�X�`���o�b�t�@
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, MaxSRVCount> texBuff_;
	// �e�N�X�`����
	UINT texCount;
	char PAD[4];

	// �f�t�H���g�e�N�X�`���i�[�f�B���N�g��
	static std::string DefaultTextureDirectoryPath;

	DirectXCommon* dxCommon_ = nullptr;
	static TextureManager* textureManager_;

private:
	TextureManager();
	~TextureManager();
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager(const TextureManager&) = delete;
};

