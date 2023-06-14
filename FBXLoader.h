#pragma once
#include "NekoYanagiModel.h"

class FbxLoader
{
private:
	//デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;

	std::string directoryPath;
	const aiScene* mScene;

	bool anicmatrixion = false;
	char pad1[7] = {};

	bool inverseU = false;
	bool inverseV = false;
	char pad2[6] = {};

	const size_t MAX_BONE_INDICES = 4;

	//読み込みのデフォルトフラグ
	const UINT ASSIMP_LOAD_FLAG_DEFAULT =
		aiProcess_Triangulate | //三角面化
		aiProcess_CalcTangentSpace | //接線ベクトル生成
		aiProcess_GenSmoothNormals | //スムージングベクトル生成
		aiProcess_GenUVCoords | //非マッピングを適切なUV座標に変換
		aiProcess_RemoveRedundantMaterials | //冗長なマテリアルを削除
		aiProcess_OptimizeMeshes | //メッシュ数を最適化
		aiProcess_JoinIdenticalVertices |//インデックスを生成
		aiProcess_LimitBoneWeights |//各頂点が影響を受けるボーンを4に制限
		aiProcess_ConvertToLeftHanded;//左手系に
	char pad3[4] = {};


public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	void LoadModel(fbxModel* model, const std::string& modelDirectory, bool inverseU_, bool inverseV_, bool animeFlag);

	//void LoadAnime(AliceMotionData* data, const std::string& modelDirectory);

};

