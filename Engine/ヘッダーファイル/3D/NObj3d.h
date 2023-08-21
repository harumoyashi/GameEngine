#pragma once
#include "NDX12.h"
#include "NMaterial.h"
#include "NModelManager.h"
#include "NRootParam.h"
#include "NGPipeline.h"
#include "NMatrix4.h"
#include "NLightGroup.h"
#include "NConstBuff.h"

#include<memory>
#include <wrl.h>

class NBaseCollider;

class NObj3d
{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_HEAP_PROPERTIES heapProp_{};	//�q�[�v
	D3D12_RESOURCE_DESC resDesc_{};		//���\�[�X

	//�萔�o�b�t�@//
	std::unique_ptr<NConstBuff<ConstBuffDataTransform>> cbTrans_;
	std::unique_ptr<NConstBuff<ConstBuffDataColor>> cbColor_;
	std::unique_ptr<NConstBuff<ConstBuffDataMaterial>> cbMaterial_;
	std::unique_ptr<NConstBuff<ConstBuffDataSkin>> cbSkin_;

	NMatrix4 matWorld_;	//3D�ϊ��s��

	NTexture* texture_;				//�e�N�X�`��
	IModel* model_;					//���f��
	static ModelFormat modelFormat;	//���f���̌`���i�[�p
	std::string objName_;			//�f�o�b�O�p�ɖ��O����

	// ���C�g
	static NLightGroup* sLightGroup;

public:
	NVec3 scale_ = { 1.0f,1.0f,1.0f };		//�X�P�[�����O�{��
	NVec3 rotation_ = { 0.0f,0.0f,0.0f };	//��]�p
	NVec3 position_ = { 0.0f,0.0f,0.0f };	//���W

	NObj3d* parent_ = nullptr;	//�e�̃|�C���^
	NColor color_;				//�F

public:
	NObj3d();
	virtual ~NObj3d();
	//������
	virtual bool Init();
#pragma region �X�V�܂��
	//�X�V
	virtual void Update();
	//�L�[�{�[�h����
	void MoveKey();
	//���[���h�s��̍���
	void UpdateMatrix();
	//�萔�o�b�t�@�֑��M
	void TransferMatrix();
	//�F���]��
	void TransferColor();
	//�����]��
	void TransferMaterial();
	//�X�L�����]��(FBX�̂�)
	void TransferSkin();
#pragma endregion
#pragma region �`��܂��
	//���ʃO���t�B�b�N�X�R�}���h
	static void CommonBeginDraw();
	//�u�����h���[�h�ݒ�
	static void SetBlendMode(BlendMode blendMode);
	//�`��
	virtual void Draw();
	//�V�F�[�_�[���\�[�X�r���[�̐ݒ�R�}���h
	void SetSRVHeap(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	void SetVB(const D3D12_VERTEX_BUFFER_VIEW* vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	void SetIB(const D3D12_INDEX_BUFFER_VIEW& ibView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	void SetCBV();
	//�`��R�}���h
	void DrawCommand(const uint32_t indexSize);
#pragma endregion

	// �Q�b�^�[ //
	//���[���h�s��擾
	const NMatrix4& GetMatWorld() { return matWorld_; }

	// �Z�b�^�[ //
	//���f���̐ݒ�
	void SetModel(const std::string& modelname);
	//�e�N�X�`���̐ݒ�
	void SetTexture(const std::string& texname);
	//���[���h�s��ݒ�
	void SetMatWorld(const NMatrix4& matWorld) { matWorld_ = matWorld; }
	//���C�g��ݒ�
	static void SetLightGroup(NLightGroup* lightGroup) { sLightGroup = lightGroup; }
};