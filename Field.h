#pragma once
#include "NObj3d.h"
#include "NSprite.h"

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

	const float startPos_ = 5.0f;	//�X�^�[�g�n�_(�c����)
	float linePos_ = 0.0f;			//�����̈ʒu

public:
	static Field* GetInstance();

	void Init();
	void Update();
	void DrawObj();
	void DrawSprite();

	// �Q�b�^�[ //
	//�X�^�[�g�n�_���W�擾
	inline float GetStartPos() const { return startPos_; }

	// �Z�b�^�[ //

};