#pragma once

struct NVec2
{
    // �����o�ϐ�
    float x; // x����
    float y; // y����

    // �R���X�g���N�^
    NVec2();                            // ��x�N�g���Ƃ��Đ���
    NVec2(const float x, const float y);            // x,y�������w�肵�Ă̐���

    // �����o�֐�
    float Length() const;                // �m����(����)�����߂�
    NVec2& Normalize();                // ���K��
    float Dot(const NVec2& v) const;    // ���ς����߂�
    float Cross(const NVec2& v) const;// �O�ς����߂�

    // �P�����Z�q�I�[�o�[���[�h
    NVec2 operator+() const;
    NVec2 operator-() const;

    // ������Z�q�I�[�o�[���[�h
    NVec2& operator+=(const NVec2& v);
    NVec2& operator-=(const NVec2& v);
    NVec2& operator*=(const float s);
    NVec2& operator/=(const float s);
};

// 2�����Z�q�I�[�o�[���[�h
// �������Ȉ����̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɏ������Ă���
const NVec2 operator+(const NVec2& v1, const NVec2& v2);
const NVec2 operator-(const NVec2& v1, const NVec2& v2);
const NVec2 operator*(const NVec2& v, const float s);
const NVec2 operator*(const float s, const NVec2& v);
const NVec2 operator/(const NVec2& v, const float s);