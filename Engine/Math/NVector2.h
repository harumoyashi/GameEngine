#pragma once

struct NVector2
{
    // �����o�ϐ�
    float x; // x����
    float y; // y����

    // �R���X�g���N�^
    NVector2();                            // ��x�N�g���Ƃ��Đ���
    NVector2(const float x, const float y);            // x,y�������w�肵�Ă̐���

    // �����o�֐�
    float Length() const;                // �m����(����)�����߂�
    NVector2& Normalize();                // ���K��
    float Dot(const NVector2& v) const;    // ���ς����߂�
    float Cross(const NVector2& v) const;// �O�ς����߂�

    // �P�����Z�q�I�[�o�[���[�h
    NVector2 operator+() const;
    NVector2 operator-() const;

    // ������Z�q�I�[�o�[���[�h
    NVector2& operator+=(const NVector2& v);
    NVector2& operator-=(const NVector2& v);
    NVector2& operator*=(const float s);
    NVector2& operator/=(const float s);
};

// 2�����Z�q�I�[�o�[���[�h
// �������Ȉ����̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɏ������Ă���
const NVector2 operator+(const NVector2& v1, const NVector2& v2);
const NVector2 operator-(const NVector2& v1, const NVector2& v2);
const NVector2 operator*(const NVector2& v, const float s);
const NVector2 operator*(const float s, const NVector2& v);
const NVector2 operator/(const NVector2& v, const float s);