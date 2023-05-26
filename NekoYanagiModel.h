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

