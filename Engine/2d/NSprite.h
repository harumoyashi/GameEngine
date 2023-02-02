#pragma once
#include "NDX12.h"
#include "NRootParam.h"
#include "NGPipeline.h"
#include "NMatrix4.h"

#include <d3dx12.h>
#include <wrl.h>

//�X�v���C�g�p���_�f�[�^�\����
struct VertexUV
{
	NVector3 pos;		//xyz���W
	NVector2 uv;		//uv���W
};

//�萔�o�b�t�@�p�f�[�^�\���́i3D�ϊ��s��j
struct  SpriteCBDataTransform
{
	XMFLOAT4 color;	//�F(RGBA)
	NMatrix4 mat;	//3D�ϊ��s��
};

class NSprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//���_�܂��//
	UINT singleSizeVB;						//���_�o�b�t�@1������̃T�C�Y
	UINT sizeVB;							//���_�o�b�t�@�S�̂̃T�C�Y
	VertexUV vertices[4]{};					//���_����p
	D3D12_HEAP_PROPERTIES heapPropVert{};	//�q�[�v
	D3D12_RESOURCE_DESC resDescVert{};		//���\�[�X
	ComPtr<ID3D12Resource> vertBuff;		//���_�o�b�t�@
	VertexUV* vertMap = nullptr;			//�}�b�v�p
	D3D12_VERTEX_BUFFER_VIEW vbView{};		//���_�o�b�t�@�r���[

	//�萔�o�b�t�@�܂��//
	D3D12_HEAP_PROPERTIES heapPropConst{};		//�q�[�v
	D3D12_RESOURCE_DESC resDescConst{};			//���\�[�X
	SpriteCBDataTransform* constMapTransform;	//3D�ϊ��s��
	ComPtr<ID3D12Resource> constBuffTransform;	//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^

	//�s��//
	NMatrix4 matWorld{};		//�ϊ��s��
	NMatrix4 matProjection{};	//���s���e�ۊǗp

	//�������̐ݒ�p//
	NVector2 anchorPoint = { 0.5f,0.5f };	//�A���J�[�|�C���g(0.0f~1.0f����Ȃ��Ɖ摜����o��)
	bool isFlipX = false;	//���E���]�t���O
	bool isFlipY = false;	//�㉺���]�t���O
	NVector2 texLeftTop = { 0,0 };			//�e�N�X�`��������W
	NVector2 texSize = { 100,100 };			//�e�N�X�`���؂�o���T�C�Y

	NVector2 size = {};		//�X�v���C�g�̑傫��

public:
	//�ϊ��p//
	float rotation = 0.0f;	//Z���̉�]�p
	NVector2 position = { 0.0f,0.0f };	//���W
	bool isInvisible = false;	//��\���ɂ���t���O

	std::string texHandle = "";	//�e�N�X�`���w��p

public:
	~NSprite();
#pragma region ������
	//�X�v���C�g����
	void CreateSprite(std::string texHandle = "error");
	//�e�N�X�`���T�C�Y�ɍ��킹�ăX�v���C�g����
	//�A���J�[�|�C���g���ݒ�ł���(�ݒ肵�Ȃ��ƒ��S�ɂȂ�)
	//�㉺���E�̔��]�t���O���ݒ�ł���(�ݒ肵�Ȃ��Ɣ��]���Ȃ�)
	void CreateSprite(std::string texHandle,
		NVector2 anchorPoint, bool isFlipX = false, bool isFlipY = false);
	//�e�N�X�`����؂����ăX�v���C�g����(�A�j���[�V�����A�t�H���g�Ȃ�)
	//�A���J�[�|�C���g���ݒ�ł���(�ݒ肵�Ȃ��ƒ��S�ɂȂ�)
	//�㉺���E�̔��]�t���O���ݒ�ł���(�ݒ肵�Ȃ��Ɣ��]���Ȃ�)
	void CreateClipSprite(std::string texHandle, NVector2 texLeftTop,
		NVector2 texSize, NVector2 anchorPoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

private:
	//���_�f�[�^�ݒ�
	void SetVert();
	//���_�p�q�[�v�ݒ�
	void SetVertHeap();
	//���_�p���\�[�X�ݒ�
	void SetVertResource();
	//�o�b�t�@�쐬
	void CreateVertBuff();
	//�e�N�X�`���T�C�Y�ɍ��킹��
	void MatchTexSize(ComPtr<ID3D12Resource> texBuff);
	//�A���J�[�|�C���g�K�p
	void SetAncor(NVector2 anchorPoint);
	//�㉺���E���]�t���O�K�p
	void SetIsFlip(bool isFlipX, bool isFlipY);
	//�؂蔲���͈͓K�p
	void SetClipRange(NVector2 texLeftTop, NVector2 texSize);
	//�؂蔲���Ȃ��ꍇ�e�N�X�`���T�C�Y�ɍ��킹��
	void SetClipRange();
	//�}�b�s���O
	void VertMaping();
	//���_�o�b�t�@�r���[�쐬
	void CreateVertBuffView();
	//�萔�p�q�[�v�ݒ�
	void SetConstHeap();
	//�萔�p���\�[�X�ݒ�
	void SetConstResource();
	//�萔�o�b�t�@�̐���
	void CreateCB();
	//�萔�o�b�t�@�̃}�b�s���O
	void MappingCB();
	//�e�N�X�`���n���h�����Z�b�g
	void SetTexHandle(std::string texHandle);
#pragma endregion
public:
#pragma region �X�V
	//�X�v���C�g�̐F�ύX(int�^0~255)
	void SetColor(int R = 255, int G = 255, int B = 255, int A = 255);
	//�}�b�v����
	void Unmap();
	//���[���h�s��̍���
	void UpdateMatrix();
	//�萔�o�b�t�@�֑��M
	void TransferMatrix();
	//���_�o�b�t�@�]��
	void TransferVertex();
	//�T�C�Y�w��
	void SetSize(float x, float y);
	//���W�w��
	void SetPos(float x, float y);
#pragma endregion
#pragma region �`��
	//���ʃO���t�B�b�N�X�R�}���h
	void CommonBeginDraw();
	//�`��R�}���h
	void Draw();
#pragma endregion
	inline NVector2 GetTexSize() { return texSize; }
	inline NVector2 GetSize() { return size; }
};