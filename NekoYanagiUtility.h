#pragma once
#include "AvoidWarning.h"

#include "Matrix4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

//2D変換行列(定数バッファ)
struct ConstBufferData
{
	NYMath::Vector4 color;
	NYMath::Matrix4 mat;
};

//頂点データ構造体
struct PosUvColor
{
	NYMath::Vector3 pos;
	NYMath::Vector2 uv;
	NYMath::Vector4 color;
};

//定数バッファ用データ構造体
struct ConstBufferDataWorldTransform
{
	NYMath::Matrix4 mat;	// 3D変換行列
};

//ビュープロジェクション定数バッファデータ
struct ConstBufferDataViewProjection
{
	//ワールド行列
	NYMath::Matrix4 world;
	//ワールド座標
	NYMath::Matrix4 matWorld;

	//ワールド → ビュー変換行列
	NYMath::Matrix4 view;
	//ビュー → プロジェクション変換行列
	NYMath::Matrix4 projection;
	//カメラ座標（ワールド座標）
	NYMath::Vector3 cameraPos;
};

//カメラ構造体
struct WorldvViewProCamera
{
	//ワールド行列
	NYMath::Matrix4 world;
	//ワールド座標
	NYMath::Matrix4 matWorld;
	//カメラ座標(ワールド座標)
	NYMath::Vector3 cameraPos;
};

//ビュープロジェクション
struct ViewProjection
{
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	//マッピング済みアドレス
	ConstBufferDataViewProjection* constBuffMap = nullptr;
};

struct VertexPos
{
	NYMath::Vector3 pos;	//xyz座標
	float scale;			//スケール
	NYMath::Vector4 color;	//色
};

struct VertexPosNormalUv
{
	NYMath::Vector3 pos;	//xyz座標
	NYMath::Vector3 normal;	//法線ベクトル
	NYMath::Vector2 uv;		//uv座標
};

//ブレンド名前
enum class BlendMode
{
	None,	// ブレンド無し
	Alpha,	// アルファ
	Add,	// 加算
	Sub,	// 減算
	Mul,	// 乗算
	Inv,	// 色反転
};

struct RootSigSetPip
{
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
};

//テクスチャデータ
struct TextureData
{
	// テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
	//GPUデスクリプタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle{};
	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	char PAD[4];

	// 横幅
	size_t width = 0;
	// 縦幅
	size_t height = 0;
};