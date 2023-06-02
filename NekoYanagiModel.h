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

//ボーンの最大数
constexpr int MAX_BONES = 128;
constexpr int MAX_BONE_INDICES = 4;

struct ModelMaterial
{
	std::string name;				// マテリアル名
	NYMath::Vector3 ambient = { 0.3f,0.3f,0.3f };		// アンビエント影響度
	NYMath::Vector3 diffuse = { 0.0f,0.0f,0.0f };		// ディフューズ影響度
	NYMath::Vector3 specular = { 0.0f,0.0f,0.0f };		// スペキュラー影響度
	float alpha = 1.0f;					// アルファ
	float shininess = 1.0f;
	char pad1[4];
	std::string textureFilename;	// テクスチャファイル名
};

/// <summary>
/// モデルデータ
/// </summary>
class NekoYanagiModel
{
};

struct PosNormUvTangeColSkin
{
	NYMath::Vector4 position = { 0.0f,0.f,0.0f,1.0f }; // 位置座標
	NYMath::Vector3 normal; // 法線
	NYMath::Vector2 uv; // uv座標
	NYMath::Vector3 tangent; // 接空間
	NYMath::Vector4 color; // 頂点色

	std::array<UINT, MAX_BONE_INDICES> boneIndex;	// ボーンの番号
	std::array<float, MAX_BONE_INDICES> boneWeight;	// ボーンの重み
};

struct PosNormUvTangeCol
{
	NYMath::Vector4 position = { 0.0f,0.f,0.0f,1.0f }; // 位置座標
	NYMath::Vector3 normal; // 法線
	NYMath::Vector2 uv; // uv座標
	NYMath::Vector3 tangent; // 接空間
	NYMath::Vector4 color; // 頂点色
};

struct PosNormSkin
{
	NYMath::Vector4 position; // 位置座標
	NYMath::Vector3 normal; // 法線

	std::array<UINT, MAX_BONE_INDICES> boneIndex;	// ボーンの番号
	std::array<float, MAX_BONE_INDICES> boneWeight;	// ボーンの重み
};

struct PosNorm
{
	NYMath::Vector4 position; // 位置座標
	NYMath::Vector3 normal; // 法線
};
//定数バッファ用データ構造体(スキニング)
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
	//名前
	std::string name;
	//ローカル変形行列
	NYMath::Matrix4 transform;
	//グローバル変形行列
	NYMath::Matrix4 globalTransform;
	//親ノード
	Node* parent = nullptr;
	//子ノード
	std::vector<Node>childrens;
};

struct Bone
{
public:
	//名前
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
	//ボーンデータ
	std::array<NYMath::Matrix4, MAX_BONES> boneMat;
};

class ModelMesh
{
public:

	//名前
	std::string name;

	//名前
	std::string nodeName;

	//頂点データの配列
	std::vector<PosNormUvTangeColSkin> vertices;

	//インデックスの配列
	std::vector<uint32_t> indices;

	//テクスチャ
	std::vector <std::string> textures;

	//テクスチャ
	std::vector <std::string> texturesNormal;

	std::vector<Bone> vecBones;

	std::vector<NYMath::Matrix4> deformationMat;

	//有効
	bool enable = false;
	char PADING[3]{};

	// カラー
	float col[4] = { 1.0f,1.0f,1.0f,1.0f };
	char PADING1[4]{};

	// マテリアル
	ModelMaterial material;

	Node* node = nullptr;

	//描画頂点データ
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

	//モデル名
	std::string name;
	//ノード配列
	std::vector<ModelMesh> meshes;
};

class NekoYanagiMotionData
{

};