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

	D3D12_HEAP_PROPERTIES heapProp{};	//�q�[�v
	D3D12_RESOURCE_DESC resDesc{};		//���\�[�X

	NConstBuff<ConstBuffDataTransform>* cbTrans;		//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^

	NMatrix4 matWorld;	//3D�ϊ��s��

	// ���C�g
	static NDirectionalLight* directionalLight;
	static NPointLight* pointLight;
	static NSpotLight* spotLight;
	static NCircleShadow* circleShadow;

public:
	NVector3 scale = { 1.0f,1.0f,1.0f };	//�X�P�[�����O�{��
	NVector3 rotation = { 0.0f,0.0f,0.0f };	//��]�p
	NVector3 position = { 0.0f,0.0f,0.0f };	//���W

	NObj3d* parent = nullptr;	//�e�̃|�C���^

	int texNum = 0;	//�e�N�X�`���w��p

	NModel* model;

public:
	NObj3d();
	~NObj3d();
#pragma region �������܂��
	//������
	bool Init();

	NObj3d* Create();

#pragma endregion
#pragma region �X�V�܂��
	//�L�[�{�[�h����
	void MoveKey();
	//���[���h�s��̍���
	void UpdateMatrix();
	//�萔�o�b�t�@�֑��M
	void TransferMatrix();
#pragma endregion
#pragma region �`��܂��
	//���ʃO���t�B�b�N�X�R�}���h
	void CommonBeginDraw();
	//�`��
	void Draw();
	//�萔�o�b�t�@�r���[(CRV)�̐ݒ�R�}���h(�}�e���A��)
	void SetMaterialCBV(NMaterial material);
	void SetSRVHeap();
	void SetSRVHeap(D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	void SetVB(D3D12_VERTEX_BUFFER_VIEW vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	void SetIB(D3D12_INDEX_BUFFER_VIEW ibView);
	//�萔�o�b�t�@�r���[(CRV)�̐ݒ�R�}���h(3D�ϊ��s��)
	void SetMatCBV();
	//�`��R�}���h
	void DrawCommand(UINT indexSize);
#pragma endregion
	inline void SetModel(NModel* model) { this->model = model; }
	//���C�g�̃Z�b�g
	static void SetNDirectionalLight(NDirectionalLight* directionalLight) {
		NObj3d::directionalLight = directionalLight;
	}
	static void SetNPointLight(NPointLight* pointLight) {
		NObj3d::pointLight = pointLight;
	}
	static void SetNSpotLight(NSpotLight* spotLight) {
		NObj3d::spotLight = spotLight;
	}
	static void SetNCircleShadow(NCircleShadow* circleShadow) {
		NObj3d::circleShadow = circleShadow;
	}

	void SetMatWorld(NMatrix4 matWorld) { this->matWorld = matWorld; }
};