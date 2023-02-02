#pragma once
#include "NVector3.h"
#include "NMatrix4.h"

class NQuaternion {
public:
	float x;
	float y;
	float z;
	float w;

private:

public:
	//�P�ʃN�H�[�^�j�I���ł̐���
	NQuaternion()
		: x(0), y(0), z(0), w(1) {}

	//�w�肵���l�ł̐���
	//����ɒl�����肽���Ȃ�
	NQuaternion(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w){}

	//�����Z
	NQuaternion operator+(const NQuaternion& q) const;
	//�����Z
	NQuaternion operator-(const NQuaternion& q) const;
	//�|���Z(��]�̍���)
	NQuaternion operator*(const NQuaternion& q) const;
	//�l�ύX����^�C�v�̑����Z
	NQuaternion operator+=(const NQuaternion& q);
	//�l�ύX����^�C�v�̈����Z
	NQuaternion operator-=(const NQuaternion& q);
	//�l�ύX����^�C�v�̊|���Z(��]�̍���)
	NQuaternion& operator*=(const NQuaternion& q);
	//�S�Ă̒l���](������t�N�H�[�^�j�I���Ƃ͈Ⴄ)
	NQuaternion operator-() const;
	//�X�J���[�{
	NQuaternion operator*(const float& s) const;
	//�X�J���[�Ŋ���
	NQuaternion operator/(const float& s) const;
	//�l�ύX����^�C�v�̃X�J���[�{
	NQuaternion& operator*=(const float& s);
	//�l�ύX����^�C�v�̃X�J���[�Ŋ���
	NQuaternion& operator/=(const float& s);

	//�������킹��N�H�[�^�j�I�����Ƃ������ɓ���Ă̊|���Z
	NQuaternion Mul(const NQuaternion& lhs, const NQuaternion& rhs);
	//�P�ʃN�H�[�^�j�I����Ԃ�
	NQuaternion Identity();
	//�����N�H�[�^�j�I����Ԃ�
	NQuaternion Conjugate(const NQuaternion& q);
	//norm���擾����
	float GetNorm() const;
	//norm��Ԃ�
	float Norm(const NQuaternion& q);
	//����
	float Dot(const NQuaternion& q0, const NQuaternion& q1);
	//�O��
	NVector3 Cross(const NQuaternion& q0, const NQuaternion& q1);
	//���K�������̂�Ԃ�
	NQuaternion Normalize();
	NQuaternion Normalize(const NQuaternion& q);
	//�t�N�H�[�^�j�I����Ԃ�
	NQuaternion Inverse(const NQuaternion& q);

	/// <summary>
	/// �C�ӂ̉�]���Ŏw�肵���p�x������]�����p���N�H�[�^�j�I���𐶐�����
	/// </summary>
	/// <param name="axis">��]��</param>
	/// <param name="angle">�p�x(Radian)</param>
	/// <returns>�p���N�H�[�^�j�I��</returns>
	static NQuaternion MakeAxisAngle(const NVector3& axis, const float& angle);

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	/// <param name="vec">��]��</param>
	/// <param name="angle">�p�x(Radian)</param>
	/// <returns>�p���N�H�[�^�j�I��</returns>
	static void SetRota(const NVector3& vec, const float& angle);

	/// <summary>
	/// �I�C���[�p�ŕ\���ꂽ��]���N�H�[�^�j�I���ŕ\��
	/// </summary>
	/// <param name="rot">�I�C���[�p</param>
	/// <returns>��]�N�H�[�^�j�I��</returns>
	NQuaternion EulerToQuaternion(const NVector3& rot) const;

	/// <summary>
	/// �N�H�[�^�j�I������]�s��ɕϊ�
	/// </summary>
	/// <returns>��]�s��</returns>
	/// �Q�l:https://qiita.com/aa_debdeb/items/3d02e28fb9ebfa357eaf
	NMatrix4 QuaternionToMatrix() const;

	/// <summary>
	/// �x�N�g�����N�H�[�^�j�I���ŉ�]���������ʂ̃x�N�g�������߂�
	/// </summary>
	/// <returns>�x�N�g��</returns>
	NVector3 RotateVector(const NVector3& vector,const NQuaternion& quaternion) const;

	/// <summary>
	/// ���ʐ��`���
	/// </summary>
	/// <returns>�N�H�[�^�j�I��</returns>
	NQuaternion Slarp(const NQuaternion& q0, const NQuaternion& q1,float t);

	/// <summary>
	/// �x�N�g��1����x�N�g��2�ւ̉�]�𐶐�
	/// </summary>
	/// <returns>�N�H�[�^�j�I��</returns>
	NQuaternion VecToVecRota(const NVector3& u, const NVector3& v);
};