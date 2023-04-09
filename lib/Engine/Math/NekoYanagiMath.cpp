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
	// アスペクト比を作成

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
	//掃き出し法を行う行列
	float sweep[4][8]{};
	//定数倍用
	float constTimes = 0.0f;
	//許容する誤差
	float MAX_ERR = 1e-10f;
	//戻り値用
	Matrix4 retMat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//weepの左側に逆行列を求める行列をセット
			sweep[i][j] = mat.m[i][j];

			//sweepの右側に単位行列をセット
			sweep[i][4 + j] = MakeIdentity().m[i][j];
		}
	}

	//全ての列の対角成分に対する繰り返し
	for (int i = 0; i < 4; i++)
	{
		//最大の絶対値を注目対角成分の絶対値と仮定
		float max = std::fabs(sweep[i][i]);
		int maxIndex = i;

		//i列目が最大の絶対値となる行を探す
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
			//逆行列は求められない
			return MakeIdentity();
		}

		//操作(1):i行目とmaxIndex行目を入れ替える
		if (i != maxIndex)
		{
			for (int j = 0; j < 8; j++)
			{
				float tmp = sweep[maxIndex][j];
				sweep[maxIndex][j] = sweep[i][j];
				sweep[i][j] = tmp;
			}
		}

		//sweep[i][i]に掛けると1になる値を求める
		constTimes = 1 / sweep[i][i];

		//操作(2):p行目をa倍する
		for (int j = 0; j < 8; j++)
		{
			//これによりsweep[i][i]が1になる
			sweep[i][j] *= constTimes;
		}

		//操作(3)によりi行目以外の行のi列目を0にする
		for (int j = 0; j < 4; j++)
		{
			if (j == i)
			{
				//i行目はそのまま
				continue;
			}

			//i行目に掛ける値を求める
			constTimes = -sweep[j][i];

			for (int k = 0; k < 8; k++)
			{
				//j行目にi行目をa倍した行を足す
				//これによりsweep[j][i]が0になる
				sweep[j][k] += sweep[i][k] * constTimes;
			}
		}
	}

	//sweepの右半分がmatの逆行列
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			retMat.m[i][j] = sweep[i][4 + j];
		}
	}

	return retMat;
}
