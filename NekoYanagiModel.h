#pragma once
#pragma warning(push)
#pragma warning(disable: 4061)

#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4625)
#pragma warning(disable: 4619)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)
#pragma warning(disable: 5264)

#include <Windows.h>
#include <d3d12.h>
#include <vector>
#include <string>
#include <wrl.h>
#include <unordered_map>

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<assimp/cimport.h>
#pragma warning(pop)

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#include "TextureManager.h"
#include "DirectXCommon.h"
#include "NekoYanagiUtility.h"

//�{�[���̍ő吔
constexpr int MAX_BONES = 128;
constexpr int MAX_BONE_INDICES = 4;

struct ModelMaterial
{
	std::string name;				// �}�e���A����
	NYMath::Vector3 ambient = { 0.3f,0.3f,0.3f };		// �A���r�G���g�e���x
	NYMath::Vector3 diffuse = { 0.0f,0.0f,0.0f };		// �f�B�t���[�Y�e���x
	NYMath::Vector3 specular = { 0.0f,0.0f,0.0f };		// �X�y�L�����[�e���x
	float alpha = 1.0f;					// �A���t�@
	float shininess = 1.0f;
	char pad1[4];
	std::string textureFilename;	// �e�N�X�`���t�@�C����
};

/// <summary>
/// ���f���f�[�^
/// </summary>
class NekoYanagiModel
{
};

struct PosNormUvTangeColSkin
{
	NYMath::Vector4 position = { 0.0f,0.f,0.0f,1.0f }; // �ʒu���W
	NYMath::Vector3 normal; // �@��
	NYMath::Vector2 uv; // uv���W
	NYMath::Vector3 tangent; // �ڋ��
	NYMath::Vector4 color; // ���_�F

	std::array<UINT, MAX_BONE_INDICES> boneIndex;	// �{�[���̔ԍ�
	std::array<float, MAX_BONE_INDICES> boneWeight;	// �{�[���̏d��
};

struct PosNormUvTangeCol
{
	NYMath::Vector4 position = { 0.0f,0.f,0.0f,1.0f }; // �ʒu���W
	NYMath::Vector3 normal; // �@��
	NYMath::Vector2 uv; // uv���W
	NYMath::Vector3 tangent; // �ڋ��
	NYMath::Vector4 color; // ���_�F
};

struct PosNormSkin
{
	NYMath::Vector4 position; // �ʒu���W
	NYMath::Vector3 normal; // �@��

	std::array<UINT, MAX_BONE_INDICES> boneIndex;	// �{�[���̔ԍ�
	std::array<float, MAX_BONE_INDICES> boneWeight;	// �{�[���̏d��
};

struct PosNorm
{
	NYMath::Vector4 position; // �ʒu���W
	NYMath::Vector3 normal; // �@��
};
//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
struct SkinData
{
	std::vector<NYMath::Matrix4> bones;
};

struct SkinComputeInput
{
	std::vector<PosNormSkin> vertices;
	SkinData bone;
};

struct Node
{
	//���O
	std::string name;
	//���[�J���ό`�s��
	NYMath::Matrix4 transform;
	//�O���[�o���ό`�s��
	NYMath::Matrix4 globalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
	//�q�m�[�h
	std::vector<Node>childrens;
};

struct Bone
{
public:
	//���O
	std::string name;

	NYMath::Matrix4 matrix = {};

	NYMath::Matrix4 offsetMatirx = {};

	UINT index = 0;

	Bone() = default;
	~Bone() = default;

private:
	char PADING[4] = {};
};

struct BoneData
{
	//�{�[���f�[�^
	std::array<NYMath::Matrix4, MAX_BONES> boneMat;
};

class ModelMesh
{
public:

	//���O
	std::string name;

	//���O
	std::string nodeName;

	//���_�f�[�^�̔z��
	std::vector<PosNormUvTangeColSkin> vertices;

	//�C���f�b�N�X�̔z��
	std::vector<uint32_t> indices;

	//�e�N�X�`��
	std::vector <std::string> textures;

	//�e�N�X�`��
	std::vector <std::string> texturesNormal;

	std::vector<Bone> vecBones;

	std::vector<NYMath::Matrix4> deformationMat;

	//�L��
	bool enable = false;
	char PADING[3]{};

	// �J���[
	float col[4] = { 1.0f,1.0f,1.0f,1.0f };
	char PADING1[4]{};

	// �}�e���A��
	ModelMaterial material;

	Node* node = nullptr;

	//�`�撸�_�f�[�^
	std::vector<PosNormUvTangeCol> vertice;

	SkinComputeInput skinComputeInput;

	SkinData skinData;

	BoneData bonedata;

	bool dirtyFlag;
	char PADING2[7]{};

public:


};

class fbxModel
{
public:
	bool isAnimetion;
	char pad1[7];

	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<ModelMesh> meshes;
};

class NekoYanagiMotionData
{

};