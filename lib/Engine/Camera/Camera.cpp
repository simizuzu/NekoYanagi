#include "Camera.h"

ViewProjection viewPro;
ConstBufferDataViewProjection ConstMap;

Camera::~Camera()
{
	viewPro.constBuff_.Reset();
}

void Camera::Initialize()
{
	eye_ = { 0,0,-distance };//視点座標
	target_ = { 0,0,0 };//注視点座標
	up_ = { 0,1,0 };//上方向ベクトル

	nearZ_ = 0.1f;
	farZ_ = 1000.0f;

	// アスペクト比を計算(画面横幅/画面縦幅)
	aspect = 
		static_cast<float>(WinApp::GetInstance()->window_width) /
		static_cast<float>(WinApp::GetInstance()->window_height);

	CreateConstBuffer();
	Map();
	UpdateMatrix();
}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::CreateConstBuffer()
{
	HRESULT result;

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferDataViewProjection) + 0xff) & ~0xff;//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(viewPro.constBuff_.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
}

void Camera::Map()
{
	HRESULT result;

	//定数バッファのマッピング
	result = viewPro.constBuff_->Map(0, nullptr, (void**)&viewPro.constBuffMap);//マッピング
	assert(SUCCEEDED(result));

	viewPro.constBuff_->SetName(L"viewProConstBuff");
}

void Camera::UpdateMatrix()

{
	// ビュー行列の生成
	matView_ = NYMath::MakeLookAtLH(eye_, target_, up_);
	// 逆行列
	matViewInverse_ = NYMath::MakeInverse(matView_);
	// 透視投影の生成
	matProjection_ = NYMath::MakePerspective(fovAngleY, aspect, nearZ_, farZ_);
	// 定数バッファに転送
	TransferMatrix();
}

void Camera::TransferMatrix()
{
	// 定数バッファに書き込み
	ConstMap.view = matView_;
	ConstMap.projection = matProjection_;
	ConstMap.cameraPos = eye_;
}

#pragma region Getter&Setter
const NYMath::Matrix4& Camera::GetMatView()
{
	return matView_;
}

const NYMath::Matrix4& Camera::GetMatViewInverse()
{
	return matViewInverse_;
}

const NYMath::Matrix4& Camera::GetMatProjection()
{
	return matProjection_;
}

const NYMath::Vector3& Camera::GetEye()
{
	return eye_;
}

const NYMath::Vector3& Camera::GetTarget()
{
	return target_;
}

const NYMath::Vector3& Camera::GetUp()
{
	return up_;
}

void Camera::SetEye(const NYMath::Vector3& eye)
{
	eye_ = eye;
}

void Camera::SetTarget(const NYMath::Vector3& target)
{
	target_ = target;
}

void Camera::SetUp(const NYMath::Vector3& up)
{
	up_ = up;
}

void Camera::SetFarZ(const float& farZ)
{
	farZ_ = farZ;
}

void Camera::SetNearZ(const float& nearZ)
{
	nearZ_ = nearZ;
}
#pragma endregion