#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"

#include "AvoidWarning.h"

namespace NYMath
{
	//�x���烉�W�A���ɕϊ�����萔
	const float PI = 3.141592654f;
	constexpr float degree2Radius = 3.1415926535897931f / 180.0f;
	constexpr float PIHalf = 3.1415926535897931f / 2.0f;

	//��(�C�v�V����)
	constexpr float EPSILON = 2.2204460492503131E-16f;

	// �P�ʍs������߂�
	NYMath::Matrix4 MakeIdentity();
	// �g��k�������߂�
	NYMath::Matrix4 MakeScaling(const NYMath::Vector3& scale);
	// ��]�s������߂�
	NYMath::Matrix4 MakeRotation(const NYMath::Vector3& rot);
	// ���s�ړ��s������߂�
	NYMath::Matrix4 MakeTranslation(const NYMath::Vector3& trans);

	// �r���[�s��̍쐬
	NYMath::Matrix4 MakeLookAtLH(NYMath::Vector3& eye, NYMath::Vector3& target, NYMath::Vector3& up);
	// �������e�쐬
	NYMath::Matrix4 MakePerspective(float fogAngleY, float aspectRatio, float nearZ, float farZ);
	// �t�s��
	NYMath::Matrix4 MakeInverse(NYMath::Matrix4& mat);
}