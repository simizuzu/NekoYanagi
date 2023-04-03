#pragma once
#include "DirectXCommon.h"
#include "NekoYanagiUtility.h"

#include "AvoidWarning.h"

/// <summary>
/// テクスチャマネージャ
/// </summary>
class TextureManager
{
public: //定数
	static const size_t MaxSRVCount = 256; // テクスチャの最大枚数

public: //メンバ変数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// テクスチャデータ
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	TextureData LoadTexture(const std::string& fileName);

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	static TextureData Load(const std::string& fileName);

	/// <summary>
	/// テクスチャバッファの生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTexBuff(DirectX::TexMetadata& metadata, DirectX::ScratchImage& scrachImg);

	/// <summary>
	/// シェーダリソースビューの生成
	/// </summary>
	/// <param name="texbuff">テクスチャバッファ</param>
	/// <param name="metadata"></param>
	D3D12_GPU_DESCRIPTOR_HANDLE CreateShaderResourceView(ID3D12Resource* texBuff, DirectX::TexMetadata& metadata);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Delete();

	// インスタンス
	static TextureManager* GetInstance();

	// Getter
	ID3D12DescriptorHeap* GetSrvHeap();

private: // メンバ変数
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	// テクスチャバッファ
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, MaxSRVCount> texBuff_;
	// テクスチャ数
	UINT texCount;
	char PAD[4];

	// デフォルトテクスチャ格納ディレクトリ
	static std::string DefaultTextureDirectoryPath;

	DirectXCommon* dxCommon_ = nullptr;
	static TextureManager* textureManager_;

private:
	TextureManager();
	~TextureManager();
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager(const TextureManager&) = delete;
};

