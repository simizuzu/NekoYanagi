#pragma once

namespace NYMath
{
	/// <summary>
	/// 4�����x�N�g��
	/// </summary>
	class Vector4
	{
	public:
		float x;//x����
		float y;//y����
		float z;//z����
		float w;//w

	public:
		//�R���X�g���N�^
		Vector4();//��x�N�g��
		Vector4(float x, float y, float z, float w);//�������w�肵�Đ���

		float length()const;//�m���������߂�
		float lengthSquared()const;
		Vector4 normalization()const;//���K������
		Vector4& normal();//���K������
		float dot(const Vector4& v)const;//���ς����߂�
		Vector4 cross(const Vector4& v)const;//�O�ς����߂�

		//�P�����Z�q�I�[�o�[���[�h
		Vector4 operator+() const;
		Vector4 operator-() const;

		// ������Z�q�I�[�o�[���[�h
		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4& operator*=(float s);
		Vector4& operator/=(float s);
	};

	//2�����Z�q�I�[�o�[���[�h
	//�������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă�
	const Vector4 operator+(const Vector4& v1, const Vector4& v2);

	const Vector4 operator-(const Vector4& v1, const Vector4& v2);
	const Vector4 operator*(const Vector4& v, float s);
	const Vector4 operator*(float s, const Vector4& v);
	const Vector4 operator/(const Vector4& v, float s);

	void Vector4Lerp(const Vector4& src1, const Vector4& src2, float t, Vector4& dest);

	Vector4 Vector4Lerp(const Vector4& src1, const Vector4& src2, float t);
}