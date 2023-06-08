#pragma once
#include "NDX12.h"
#include "NRootParam.h"
#include "NGPipeline.h"
#include "NMatrix4.h"
#include "NColor.h"
#include "NConstBuff.h"
#include "NVertex.h"

#include <d3dx12.h>
#include <wrl.h>

class NSprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//���_�܂��//
	uint32_t singleSizeVB;						//���_�o�b�t�@1������̃T�C�Y
	uint32_t sizeVB;							//���_�o�b�t�@�S�̂̃T�C�Y
	NVertexUV vertices[4]{};				//���_����p
	D3D12_HEAP_PROPERTIES heapPropVert{};	//�q�[�v
	D3D12_RESOURCE_DESC resDescVert{};		//���\�[�X
	ComPtr<ID3D12Resource> vertBuff;		//���_�o�b�t�@
	NVertexUV* vertMap = nullptr;			//�}�b�v�p
	D3D12_VERTEX_BUFFER_VIEW vbView{};		//���_�o�b�t�@�r���[

	//�萔�o�b�t�@�܂��//
	std::unique_ptr<NConstBuff<ConstBuffDataTransform2D>> cbTrans;	//2D�ϊ��s��
	std::unique_ptr<NConstBuff<ConstBuffDataColor>> cbColor;	//�F

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
	NColor color;

public:
	NSprite();
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
	
	//�e�N�X�`���n���h�����Z�b�g
	void SetTexHandle(std::string texHandle);
#pragma endregion
public:
#pragma region �X�V
	//���[���h�s��̍���
	void Update();
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