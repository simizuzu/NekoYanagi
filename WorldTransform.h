#pragma once
#include "AvoidWarning.h"
#include "Camera.h"
#include "NekoYanagiUtility.h"

class WorldTransform
{
private: //メンバ関数
	
	void Initialize();

	void Update(Camera* camera);

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

