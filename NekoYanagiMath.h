#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"

#include "AvoidWarning.h"

namespace NYMath
{
	//度からラジアンに変換する定数
	const float PI = 3.141592654f;
	constexpr float degree2Radius = 3.1415926535897931f / 180.0f;
	constexpr float PIHalf = 3.1415926535897931f / 2.0f;

	//ε(イプシロン)
	constexpr float EPSILON = 2.2204460492503131E-16f;

	// 単位行列を求める
	NYMath::Matrix4 MakeIdentity();
	// 拡大縮小を求める
	NYMath::Matrix4 MakeScaling(const NYMath::Vector3& scale);
	// 回転行列を求める
	NYMath::Matrix4 MakeRotation(const NYMath::Vector3& rot);
	// 平行移動行列を求める
	NYMath::Matrix4 MakeTranslation(const NYMath::Vector3& trans);

	// ビュー行列の作成
	NYMath::Matrix4 MakeLookAtLH(NYMath::Vector3& eye, NYMath::Vector3& target, NYMath::Vector3& up);
	// 透視投影作成
	NYMath::Matrix4 MakePerspective(float fogAngleY, float aspectRatio, float nearZ, float farZ);
	// 逆行列
	NYMath::Matrix4 MakeInverse(NYMath::Matrix4& mat);
}