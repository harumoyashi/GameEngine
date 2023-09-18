#pragma once
#include "NSprite.h"

class NPostEffect
	: public NSprite
{
protected:
	static NVertexBuff<NVertexUV> vertexBuff_;		//���_�o�b�t�@

	//�萔�o�b�t�@�܂��//
	static std::unique_ptr<NConstBuff<ConstBuffDataTransform2D>> cbTrans_;	//2D�ϊ��s��
	static std::unique_ptr<NConstBuff<ConstBuffDataColor>> cbColor_;

	//�s��//
	static NMatrix4 matWorld_;		//�ϊ��s��
	static NMatrix4 matProjection_;	//���s���e�ۊǗp

	//�ϊ��p//
	static float rotation_;		//Z���̉�]�p
	static NVec2 position_;		//���W
	static NColor color_;		//�F

	//�e�N�X�`���o�b�t�@
	static const uint32_t texNum_ = 2;
	static ComPtr<ID3D12Resource> texBuff_[texNum_];
	//SRV�p�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	//�[�x�o�b�t�@
	static ComPtr<ID3D12Resource> depthBuff_;
	//RTV�p�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
	//DSV�p�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeapDSV_;

	//��ʃN���A�J���[
	static const float kClearColor[4];

	//�p�C�v���C���̖��O
	static std::string pipelineName_;

	static bool isActive_;	//�|�X�g�G�t�F�N�g�L���t���O

public:
	NPostEffect();
	virtual ~NPostEffect() = default;

	static void Init();
	static void Update();

	//�e�N�X�`������
	static void CreateTexture();
	//�����_�[�^�[�Q�b�g�r���[����
	static void CreateRTV();
	//�[�x�o�b�t�@����
	static void CreateDepthBuff();
	//�f�v�X�X�e���V���r���[����
	static void CreateDSV();
	static void Draw();

	static void PreDrawScene();
	static void PostDrawScene();

	// �Q�b�^�[ //
	static bool GetIsActive() { return isActive_; }

	// �Z�b�^�[ //
	//�|�X�g�G�t�F�N�g�L���t���O�ݒ�
	static void SetIsActive(bool isActive) { isActive_ = isActive; }
};