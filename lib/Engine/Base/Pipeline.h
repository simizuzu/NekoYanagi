#pragma once
#include "DirectXCommon.h"
#include "NekoYanagiUtility.h"

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4820)
#include <wrl.h>
#include <array>
#include <d3dx12.h>
#pragma warning(pop)

class Pipeline
{
public: // エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数

	/// <summary>
	/// パイプライン呼び出し用関数(スプライト)
	/// </summary>
	static void CreateSpritePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend,ID3D12Device* device, std::array<RootSigSetPip,6>&pipeline);

	/// <summary>
	/// パイプライン呼び出し用関数(3Dオブジェクト)
	/// </summary>
	static void CreateObjPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, BlendMode blend, ID3D12Device* device, RootSigSetPip& pipeline);

	/// <summary>
	/// パイプライン呼び出し用関数(パーティクル)
	/// </summary>
	static void CreatePaticlePipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob,ID3DBlob* gsBlob, ID3D12Device* device, RootSigSetPip& pipeline);

private: // メンバ変数

	// クラス呼び出し
	DirectXCommon* dxCommon_ = nullptr;
};

