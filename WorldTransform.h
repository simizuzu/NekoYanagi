#pragma once
#include "AvoidWarning.h"
#include "Camera.h"
#include "NekoYanagiUtility.h"

class WorldTransform
{
private: //�����o�֐�
	
	void Initialize();

	void Update(Camera* camera);

	void CreateConstBuffer();

private:
	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	WorldvViewProCamera constBuffMap;

public:
	//���[�J�����W
	NYMath::Vector3 translation_ = { 0.0f,0.0f,0.0f };
	//X,Y,Z�����̃��[�J����]�p
	NYMath::Vector3 rotation_ = { 0.0f,0.0f,0.0f };
	//���[�J���X�P�[��
	NYMath::Vector3 scale_ = { 1.0f,1.0f,1.0f };

	NYMath::Matrix4 matWorld;

	const WorldTransform* parent = nullptr;
};

