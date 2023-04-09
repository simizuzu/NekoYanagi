#include "NekoYanagiMath.h"

NYMath::Matrix4 NYMath::MakeIdentity()
{
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1.0f;
	matIdentity.m[1][1] = 1.0f;
	matIdentity.m[2][2] = 1.0f;
	matIdentity.m[3][3] = 1.0f;

	return matIdentity;
}

NYMath::Matrix4 NYMath::MakeScaling(const NYMath::Vector3& scale)
{
	Matrix4 matScale;
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;

	return matScale;
}

NYMath::Matrix4 NYMath::MakeRotation(const NYMath::Vector3& rot)
{
	Matrix4 matRotX;
	matRotX = MakeIdentity();
	matRotX.m[1][1] = cos(rot.x);
	matRotX.m[1][2] = sin(rot.x);
	matRotX.m[2][1] = -sin(rot.x);
	matRotX.m[2][2] = cos(rot.x);

	Matrix4 matRotY;
	matRotY = MakeIdentity();
	matRotY.m[0][0] = cos(rot.y);
	matRotY.m[0][2] = -sin(rot.y);
	matRotY.m[2][0] = sin(rot.y);
	matRotY.m[2][2] = cos(rot.y);

	Matrix4 matRotZ;
	matRotZ = MakeIdentity();
	matRotZ.m[0][0] = cos(rot.z);
	matRotZ.m[0][1] = sin(rot.z);
	matRotZ.m[1][0] = -sin(rot.z);
	matRotZ.m[1][1] = cos(rot.z);

	Matrix4 matRot;
	matRot = MakeIdentity();

	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;
	return matRot;
}

NYMath::Matrix4 NYMath::MakeTranslation(const NYMath::Vector3& trans)
{
	Matrix4 matTrans;
	matTrans = MakeIdentity();
	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = trans.z;
	return matTrans;
}

NYMath::Matrix4 NYMath::MakeLookAtLH(NYMath::Vector3& eye, NYMath::Vector3& target, NYMath::Vector3& up)
{
	Vector3 zVec = target - eye;
	zVec.normalize();

	Vector3 xVec = up.cross(zVec);
	xVec.normalize();

	Vector3 yVec = zVec.cross(xVec);
	yVec.normalize();

	Matrix4 matrix;
	matrix.m[0][0] = xVec.x;
	matrix.m[0][1] = xVec.y;
	matrix.m[0][2] = xVec.z;

	matrix.m[1][0] = yVec.x;
	matrix.m[1][1] = yVec.y;
	matrix.m[1][2] = yVec.z;

	matrix.m[2][0] = zVec.x;
	matrix.m[2][1] = zVec.y;
	matrix.m[2][2] = zVec.z;

	matrix.m[3][0] = -eye.x;
	matrix.m[3][1] = -eye.y;
	matrix.m[3][2] = -eye.z;

	return matrix;
}

NYMath::Matrix4 NYMath::MakePerspective(float fogAngleY, float aspectRatio, float nearZ, float farZ)
{
	// �A�X�y�N�g����쐬

	float sinFov = std::sin(0.5f * fogAngleY);
	float cosFov = std::cos(0.5f * fogAngleY);

	float height = cosFov / sinFov;
	float range = farZ / (farZ - nearZ);

	Matrix4 matrix;
	matrix.m[0][0] = height / aspectRatio;
	matrix.m[1][1] = cosFov / sinFov;
	matrix.m[2][2] = range;
	matrix.m[2][3] = 1.0f;
	matrix.m[3][2] = -range * nearZ;

	matrix.m[0][1] = 0.0f;
	matrix.m[0][2] = 0.0f;
	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = 0.0f;
	matrix.m[1][2] = 0.0f;
	matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
	matrix.m[3][0] = 0.0f;
	matrix.m[3][1] = 0.0f;
	matrix.m[3][3] = 0.0f;

	return matrix;
}

NYMath::Matrix4 NYMath::MakeInverse(NYMath::Matrix4& mat)
{
	//�|���o���@���s���s��
	float sweep[4][8]{};
	//�萔�{�p
	float constTimes = 0.0f;
	//���e����덷
	float MAX_ERR = 1e-10f;
	//�߂�l�p
	Matrix4 retMat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//weep�̍����ɋt�s������߂�s����Z�b�g
			sweep[i][j] = mat.m[i][j];

			//sweep�̉E���ɒP�ʍs����Z�b�g
			sweep[i][4 + j] = MakeIdentity().m[i][j];
		}
	}

	//�S�Ă̗�̑Ίp�����ɑ΂���J��Ԃ�
	for (int i = 0; i < 4; i++)
	{
		//�ő�̐�Βl�𒍖ڑΊp�����̐�Βl�Ɖ���
		float max = std::fabs(sweep[i][i]);
		int maxIndex = i;

		//i��ڂ��ő�̐�Βl�ƂȂ�s��T��
		for (int j = i + 1; j < 4; j++)
		{
			if (std::fabs(sweep[j][i]) > max)
			{
				max = std::fabs(sweep[j][i]);
				maxIndex = j;
			}
		}

		if (fabs(sweep[maxIndex][i]) <= MAX_ERR)
		{
			//�t�s��͋��߂��Ȃ�
			return MakeIdentity();
		}

		//����(1):i�s�ڂ�maxIndex�s�ڂ����ւ���
		if (i != maxIndex)
		{
			for (int j = 0; j < 8; j++)
			{
				float tmp = sweep[maxIndex][j];
				sweep[maxIndex][j] = sweep[i][j];
				sweep[i][j] = tmp;
			}
		}

		//sweep[i][i]�Ɋ|�����1�ɂȂ�l�����߂�
		constTimes = 1 / sweep[i][i];

		//����(2):p�s�ڂ�a�{����
		for (int j = 0; j < 8; j++)
		{
			//����ɂ��sweep[i][i]��1�ɂȂ�
			sweep[i][j] *= constTimes;
		}

		//����(3)�ɂ��i�s�ڈȊO�̍s��i��ڂ�0�ɂ���
		for (int j = 0; j < 4; j++)
		{
			if (j == i)
			{
				//i�s�ڂ͂��̂܂�
				continue;
			}

			//i�s�ڂɊ|����l�����߂�
			constTimes = -sweep[j][i];

			for (int k = 0; k < 8; k++)
			{
				//j�s�ڂ�i�s�ڂ�a�{�����s�𑫂�
				//����ɂ��sweep[j][i]��0�ɂȂ�
				sweep[j][k] += sweep[i][k] * constTimes;
			}
		}
	}

	//sweep�̉E������mat�̋t�s��
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			retMat.m[i][j] = sweep[i][4 + j];
		}
	}

	return retMat;
}
