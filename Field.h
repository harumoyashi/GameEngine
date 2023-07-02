#pragma once
#include "NObj3d.h"
#include "NSprite.h"
#include "NEasing.h"

//���B�^�C�����O�ƍs���͈͈ȊO�F�ς������p�̃V�F�[�_�[������B
class Field
{
public:
	enum class SpriteType
	{
		Line,	//����
		Start,	//�X�^�[�g�n�_�̂��

		MaxSize
	};

private:
	std::unique_ptr<NObj3d> obj_;

	std::vector<std::unique_ptr<NSprite>> sprites_;	//�X�v���C�g�Q

	const float kStartPos = 5.0f;	//�X�^�[�g�n�_(�c����)
	float linePos_ = 0.0f;			//�����̈ʒu

	float slidePos_ = 0.0f;			//���ɑ|���Ă����̃X���C�h�ʒu
	NEasing::EaseTimer slideTimer_ = 0.1f;	//�X���C�h���̃C�[�W���O�p

	bool isStart = false;			//�X�^�[�g�������t���O

public:
	static Field* GetInstance();

	void Init();
	void Update();
	void DrawObj();
	void DrawSprite();

	// �Q�b�^�[ //
	//�X�^�[�g�n�_���W�擾
	inline float GetStartPos() const { return kStartPos; }

	// �Z�b�^�[ //

};