#pragma once
#include "AvoidWarning.h"
#include "Camera.h"
#include "NekoYanagiUtility.h"

class WorldTransform
{

public:
	//初期化
	void Initialize();
	//更新
	void Update(Camera* camera,bool billboradFlag = false);

	//ゲッター
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress();

private: //メンバ関数
	//定数バッファ生成
	void CreateConstBuffer();

private:
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	WorldvViewProCamera constBuffMap;

public:
	//ローカル座標
	NYMath::Vector3 translation_ = { 0.0f,0.0f,0.0f };
	//X,Y,Z軸回りのローカル回転角
	NYMath::Vector3 rotation_ = { 0.0f,0.0f,0.0f };
	//ローカルスケール
	NYMath::Vector3 scale_ = { 1.0f,1.0f,1.0f };

	NYMath::Matrix4 matWorld;

	const WorldTransform* parent = nullptr;
};

