#include "Camera.h"

ViewProjection viewPro;
ConstBufferDataViewProjection ConstMap;

Camera::~Camera()
{
	viewPro.constBuff_.Reset();
}

void Camera::Initialize()
{
	eye_ = { 0,0,-distance };//���_���W
	target_ = { 0,0,0 };//�����_���W
	up_ = { 0,1,0 };//������x�N�g��

	nearZ_ = 0.1f;
	farZ_ = 1000.0f;

	// �A�X�y�N�g����v�Z(��ʉ���/��ʏc��)
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

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferDataViewProjection) + 0xff) & ~0xff;//���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
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

	//�萔�o�b�t�@�̃}�b�s���O
	result = viewPro.constBuff_->Map(0, nullptr, (void**)&viewPro.constBuffMap);//�}�b�s���O
	assert(SUCCEEDED(result));

	viewPro.constBuff_->SetName(L"viewProConstBuff");
}

void Camera::UpdateMatrix()

{
	// �r���[�s��̐���
	matView_ = NYMath::MakeLookAtLH(eye_, target_, up_);
	// �t�s��
	matViewInverse_ = NYMath::MakeInverse(matView_);
	// �������e�̐���
	matProjection_ = NYMath::MakePerspective(fovAngleY, aspect, nearZ_, farZ_);
	// �萔�o�b�t�@�ɓ]��
	TransferMatrix();
}

void Camera::TransferMatrix()
{
	// �萔�o�b�t�@�ɏ�������
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