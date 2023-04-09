#pragma once
#include "AvoidWarning.h"
#include "DirectXCommon.h"
#include "WinApp.h"
#include "NekoYanagiMath.h"
#include "NekoYanagiUtility.h"

class Camera
{
public:
#pragma region �r���[�s��ݒ�
	// ���_���W
	NYMath::Vector3 eye_;
	// �����_���W
	NYMath::Vector3 target_;
	// ������x�N�g��
	NYMath::Vector3 up_;
#pragma endregion

private:
#pragma region �ˉe�s��ݒ�
	// �J������p
	float fovAngleY = NYMath::degree2Radius * 45.0f;
	// ��O
	float nearZ_ = 0.1f;
	// ���s��
	float farZ_ = 1000.0f;
	// �A�X�y�N�g��(��ʉ���/��ʏc��)
	float aspect = 0.0f;
#pragma endregion
	// �r���[�s��
	NYMath::Matrix4 matView_;
	// �r���[�t�s��
	NYMath::Matrix4 matViewInverse_;
	// �ˉe�s��
	NYMath::Matrix4 matProjection_;;
	// �J��������
	float distance = 50.0f;

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �萔�o�b�t�@����
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// �}�b�s���O
	/// </summary>
	void Map();
	/// <summary>
	/// �r���[�s��̍X�V
	/// </summary>
	void UpdateMatrix();
	/// <summary>
	/// �r���[�s��̓]��
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