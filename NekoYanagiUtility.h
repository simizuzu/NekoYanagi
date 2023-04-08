#pragma once
#include "AvoidWarning.h"

#include "Matrix4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

//2D�ϊ��s��(�萔�o�b�t�@)
struct ConstBufferData
{
	NYMath::Vector4 color;
	NYMath::Matrix4 mat;
};

//���_�f�[�^�\����
struct PosUvColor
{
	NYMath::Vector3 pos;
	NYMath::Vector2 uv;
	NYMath::Vector4 color;
};

//�萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferDataWorldTransform
{
	NYMath::Matrix4 mat;	// 3D�ϊ��s��
};

//�r���[�v���W�F�N�V�����萔�o�b�t�@�f�[�^
struct ConstBufferDataViewProjection
{
	//���[���h�s��
	NYMath::Matrix4 world;
	//���[���h���W
	NYMath::Matrix4 matWorld;

	//���[���h �� �r���[�ϊ��s��
	NYMath::Matrix4 view;
	//�r���[ �� �v���W�F�N�V�����ϊ��s��
	NYMath::Matrix4 projection;
	//�J�������W�i���[���h���W�j
	NYMath::Vector3 cameraPos;
};

//�J�����\����
struct WorldvViewProCamera
{
	//���[���h�s��
	NYMath::Matrix4 world;
	//���[���h���W
	NYMath::Matrix4 matWorld;
	//�J�������W(���[���h���W)
	NYMath::Vector3 cameraPos;
};

//�r���[�v���W�F�N�V����
struct ViewProjection
{
	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	//�}�b�s���O�ς݃A�h���X
	ConstBufferDataViewProjection* constBuffMap = nullptr;
};

struct VertexPos
{
	NYMath::Vector3 pos;	//xyz���W
	float scale;			//�X�P�[��
	NYMath::Vector4 color;	//�F
};

struct VertexPosNormalUv
{
	NYMath::Vector3 pos;	//xyz���W
	NYMath::Vector3 normal;	//�@���x�N�g��
	NYMath::Vector2 uv;		//uv���W
};

//�u�����h���O
enum class BlendMode
{
	None,	// �u�����h����
	Alpha,	// �A���t�@
	Add,	// ���Z
	Sub,	// ���Z
	Mul,	// ��Z
	Inv,	// �F���]
};

struct RootSigSetPip
{
	// ���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
};

//�e�N�X�`���f�[�^
struct TextureData
{
	// �e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	// �f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
	//GPU�f�X�N���v�^�n���h��
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle{};
	// �f�X�N���v�^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	char PAD[4];

	// ����
	size_t width = 0;
	// �c��
	size_t height = 0;
};