#pragma once
#include "AvoidWarning.h"
#include "DirectXCommon.h"
#include "WinApp.h"
#include "NekoYanagiMath.h"
#include "NekoYanagiUtility.h"

class Camera
{
public:
#pragma region ビュー行列設定
	// 視点座標
	NYMath::Vector3 eye_;
	// 注視点座標
	NYMath::Vector3 target_;
	// 上方向ベクトル
	NYMath::Vector3 up_;
#pragma endregion

private:
#pragma region 射影行列設定
	// カメラ画角
	float fovAngleY = NYMath::degree2Radius * 45.0f;
	// 手前
	float nearZ_ = 0.1f;
	// 奥行き
	float farZ_ = 1000.0f;
	// アスペクト比(画面横幅/画面縦幅)
	float aspect = 0.0f;
#pragma endregion
	// ビュー行列
	NYMath::Matrix4 matView_;
	// ビュー逆行列
	NYMath::Matrix4 matViewInverse_;
	// 射影行列
	NYMath::Matrix4 matProjection_;;
	// カメラ距離
	float distance = 50.0f;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピング
	/// </summary>
	void Map();
	/// <summary>
	/// ビュー行列の更新
	/// </summary>
	void UpdateMatrix();
	/// <summary>
	/// ビュー行列の転送
	/// </summary>
	void TransferMatrix();

public:
	const NYMath::Matrix4& GetMatView();
	const NYMath::Matrix4& GetMatViewInverse();
	const NYMath::Matrix4& GetMatProjection();

	const NYMath::Vector3& GetEye();
	const NYMath::Vector3& GetTarget();
	const NYMath::Vector3& GetUp();

	void SetEye(const NYMath::Vector3& eye);
	void SetTarget(const NYMath::Vector3& target);
	void SetUp(const NYMath::Vector3& up);
	void SetFarZ(const float& farZ);
	void SetNearZ(const float& nearZ);
};