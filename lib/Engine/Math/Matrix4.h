#pragma once
#include <cmath>
#include "Vector3.h"

namespace NYMath
{
	class Matrix4
	{
	public:
		float m[4][4];

		// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
		Matrix4();

		Matrix4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

		/// <summary>
		/// ‘ã“ü‰‰Zq
		/// </summary>
		Matrix4& operator=(const Matrix4& _m);

		// ‰ÁZ
		Matrix4& operator+=(const Matrix4& mat);
		// Œ¸Z
		Matrix4& operator-=(const Matrix4& mat);
		// æZ
		Matrix4& operator*=(const Matrix4& mat);
		// ‰ÁZ
		Matrix4 operator+(const Matrix4& mat) const;
		// Œ¸Z
		Matrix4 operator-(const Matrix4& mat) const;
		// æZ
		Matrix4 operator*(const Matrix4& mat) const;
	};
}


