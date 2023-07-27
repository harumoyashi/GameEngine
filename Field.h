#pragma once
#include "NObj3d.h"
#include "NTile.h"
#include "NEasing.h"

//��
class Field final
{
public:
	enum class LineType
	{
		Start,	//�X�^�[�g
		Goal,	//�S�[��

		MaxSize
	};

	//�t�B�[���h�ɂ����(�X�^�[�g�A�S�[���A�`�F�b�N�|�C���g)
	struct Line
	{
		std::unique_ptr<NObj3d> line;			//��
		std::unique_ptr<NObj3d> text;			//����

		float linePosZ = 0.0f;					//�����̈ʒu
		float offset = 5.0f;					//�����ǂ��ɒu����(������̃I�t�Z�b�g)

		bool isSlide = false;					//�X���C�h���邩�t���O
		float slidePos = 0.0f;					//���ɑ|���Ă����̃X���C�h�ʒu
		NEasing::EaseTimer slideTimer = 0.1f;	//�X���C�h���̃C�[�W���O�p
	};

private:
	std::unique_ptr<NTile> fieldObj_;			//��
	std::vector<Line> lines_;					//�t�B�[���h�ɂ��������
	std::vector<Line> checkPoints_;				//�t�B�[���h�ɂ���`�F�b�N�|�C���g����

	float tileDivide_ = 0.5f;					//�^�C���̕�����
	float activityAreaX_ = 10.0f;				//�s���͈͐���
	uint32_t checkPosintNum = 3;				//�`�F�b�N�|�C���g������
	
	float startPosZ_ = 5.0f;					//�X�^�[�g�n�_(�c����)
	float goalPosZ_ = 20.0f;					//�S�[���n�_(�c����)

	bool isStart_ = false;						//�X�^�[�g�������t���O
	bool isGoal_ = false;						//�S�[���������t���O

public:
	static Field* GetInstance();

	void Init();
	void Update();
	void Draw();

	// �Q�b�^�[ //
	//�X�^�[�g�n�_���W�擾
	float GetStartPos() const { return startPosZ_; }
	//�t�B�[���h�̑傫���擾
	NVector3 GetScale() const { return fieldObj_->scale_; }
	//�X�^�[�g�������t���O�擾
	bool GetIsStart() const { return isStart_; }
	//�S�[���������t���O�擾
	bool GetIsGoal() const { return isGoal_; }
	//�s���͈͎擾
	float GetActivityAreaX() const { return activityAreaX_; }

	// �Z�b�^�[ //


private:
	Field() = default;
	Field(const Field&) = delete;
	~Field() = default;
	Field& operator=(const Field&) = delete;
};