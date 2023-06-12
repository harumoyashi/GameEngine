#pragma once
#include "NDX12.h"
#include "NMaterial.h"
#include "NModel.h"
#include "NRootParam.h"
#include "NGPipeline.h"
#include "NMatrix4.h"
#include "NDirectionalLight.h"
#include "NPointLight.h"
#include "NSpotLight.h"
#include "NCircleShadow.h"
#include "NConstBuff.h"

#include<memory>
#include <wrl.h>

class NObj3d
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_HEAP_PROPERTIES heapProp_{};	//�q�[�v
	D3D12_RESOURCE_DESC resDesc_{};		//���\�[�X

	//�萔�o�b�t�@//
	std::unique_ptr<NConstBuff<ConstBuffDataTransform>> cbTrans_;
	std::unique_ptr<NConstBuff<ConstBuffDataColor>> cbColor_;
	std::unique_ptr<NConstBuff<ConstBuffDataMaterial>> cbMaterial_;

	NMatrix4 matWorld_;	//3D�ϊ��s��

	// ���C�g
	static NDirectionalLight* sDirectionalLight;
	static NPointLight* sPointLight;
	static NSpotLight* sSpotLight;
	static NCircleShadow* sCircleShadow;

public:
	NVector3 scale_ = { 1.0f,1.0f,1.0f };	//�X�P�[�����O�{��
	NVector3 rotation_ = { 0.0f,0.0f,0.0f };	//��]�p
	NVector3 position_ = { 0.0f,0.0f,0.0f };	//���W

	NObj3d* parent_ = nullptr;	//�e�̃|�C���^

	uint32_t texNum_ = 0;	//�e�N�X�`���w��p

	NModel model_;
	NColor color_;

public:
	NObj3d();
	~NObj3d();
#pragma region �������܂��
	//������
	bool Init();

	NObj3d* Create();

#pragma endregion
#pragma region �X�V�܂��
	//�X�V
	void Update();
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

#pragma endregion
#pragma region �`��܂��
	//���ʃO���t�B�b�N�X�R�}���h
	void CommonBeginDraw();
	//�`��
	void Draw();
	void SetSRVHeap();
	void SetSRVHeap(D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	void SetVB(D3D12_VERTEX_BUFFER_VIEW vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	void SetIB(D3D12_INDEX_BUFFER_VIEW ibView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	void SetCBV();
	//�`��R�}���h
	void DrawCommand(uint32_t indexSize);
#pragma endregion
	inline void SetModel(NModel model) { model_ = model; }
	//���C�g�̃Z�b�g
	static void SetNDirectionalLight(NDirectionalLight* directionalLight) {
		NObj3d::sDirectionalLight = directionalLight;
	}
	static void SetNPointLight(NPointLight* pointLight) {
		NObj3d::sPointLight = pointLight;
	}
	static void SetNSpotLight(NSpotLight* spotLight) {
		NObj3d::sSpotLight = spotLight;
	}
	static void SetNCircleShadow(NCircleShadow* circleShadow) {
		NObj3d::sCircleShadow = circleShadow;
	}

	inline void SetMatWorld(NMatrix4 matWorld) { matWorld_ = matWorld; }
};