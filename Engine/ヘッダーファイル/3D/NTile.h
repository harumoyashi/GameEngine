#pragma once
#include "NObj3d.h"

class NTile final :
	public NObj3d
{
private:
	//�萔�o�b�t�@//
	std::unique_ptr<NConstBuff<ConstBuffDataTile>> cbTile_;

	float divide_;			//�^�C���̕�����
	float activityArea_;	//�s���͈�

public:
	bool Init()override;
	void Update()override;
	//���ʃO���t�B�b�N�X�R�}���h
	static void CommonBeginDraw();
	//�`��
	void Draw()override;

	// �Z�b�^�[ //
	//�^�C���̕������ݒ�
	void SetDivide(float divide) { divide_ = divide; }
	//�s���͈͐ݒ�
	void SetActivityArea(float activityArea) { activityArea_ = activityArea; }

private:
	//�^�C���p�萔�o�b�t�@�]��
	void TransferCBTile();
};