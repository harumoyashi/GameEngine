#pragma once
#include "NDX12.h"
#include "NRootParam.h"
#include "NGPipeline.h"
#include "NMatrix4.h"
#include "NColor.h"
#include "NConstBuff.h"
#include "NVertex.h"
#include "NVertexBuff.h"

#include <d3dx12.h>
#include <wrl.h>

class NSprite
{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//���_�܂��//
	uint32_t singleSizeVB_;						//���_�o�b�t�@1������̃T�C�Y
	uint32_t singleVB_;							//���_�o�b�t�@�S�̂̃T�C�Y
	std::vector<NVertexUV> vertices_{ 4 };		//���_����p
	D3D12_HEAP_PROPERTIES heapPropVert_{};	//�q�[�v
	D3D12_RESOURCE_DESC resDescVert_{};		//���\�[�X
	NVertexBuff<NVertexUV> vertexBuff_;		//���_�o�b�t�@

	//�萔�o�b�t�@�܂��//
	std::unique_ptr<NConstBuff<ConstBuffDataTransform2D>> cbTrans_;	//2D�ϊ��s��
	std::unique_ptr<NConstBuff<ConstBuffDataColor>> cbColor_;	//�F

	//�s��//
	NMatrix4 matWorld_{};		//�ϊ��s��
	NMatrix4 matProjection_{};	//���s���e�ۊǗp

	//�������̐ݒ�p//
	NVector2 anchorPoint_ = { 0.5f,0.5f };	//�A���J�[�|�C���g(0.0f~1.0f����Ȃ��Ɖ摜����o��)
	bool isFlipX_ = false;	//���E���]�t���O
	bool isFlipY_ = false;	//�㉺���]�t���O
	NVector2 texLeftTop_ = { 0,0 };			//�e�N�X�`��������W
	NVector2 texSize_ = { 100,100 };			//�e�N�X�`���؂�o���T�C�Y

	NVector2 size_ = { 100,100 };		//�X�v���C�g�̑傫��

public:
	//�ϊ��p//
	float rotation_ = 0.0f;	//Z���̉�]�p
	NVector2 position_ = { 0.0f,0.0f };	//���W
	bool isInvisible_ = false;	//��\���ɂ���t���O

	std::string texHandle_ = "";	//�e�N�X�`���w��p
	NColor color_;

public:
	NSprite();
	~NSprite();
#pragma region ������
	//�X�v���C�g����
	void CreateSprite(const std::string& texHandle = "error");
	//�e�N�X�`���T�C�Y�ɍ��킹�ăX�v���C�g����
	//�A���J�[�|�C���g���ݒ�ł���(�ݒ肵�Ȃ��ƒ��S�ɂȂ�)
	//�㉺���E�̔��]�t���O���ݒ�ł���(�ݒ肵�Ȃ��Ɣ��]���Ȃ�)
	void CreateSprite(const std::string& texHandle,
		const NVector2& anchorPoint, bool isFlipX = false, bool isFlipY = false);
	//�e�N�X�`����؂����ăX�v���C�g����(�A�j���[�V�����A�t�H���g�Ȃ�)
	//�A���J�[�|�C���g���ݒ�ł���(�ݒ肵�Ȃ��ƒ��S�ɂȂ�)
	//�㉺���E�̔��]�t���O���ݒ�ł���(�ݒ肵�Ȃ��Ɣ��]���Ȃ�)
	void CreateClipSprite(const std::string& texHandle, const NVector2& texLeftTop,
		const NVector2& texSize, const NVector2& anchorPoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

private:
	//�e�N�X�`���T�C�Y���擾���Đݒ�
	void SetTexSize(const ComPtr<ID3D12Resource>& texBuff);
	//�A���J�[�|�C���g�K�p
	void SetAncor(const NVector2& anchorPoint);
	//�㉺���E���]�t���O�K�p
	void SetIsFlip(bool isFlipX, bool isFlipY);
	//�؂蔲���͈͓K�p
	void SetClipRange(const NVector2& texLeftTop, const NVector2& texSize);

	//�e�N�X�`���n���h�����Z�b�g
	void SetTexHandle(const std::string& texHandle);
#pragma endregion
public:
#pragma region �X�V
	//���[���h�s��̍���
	void Update();
	//���_�o�b�t�@�]��
	void TransferVertex();
	//�T�C�Y�w��
	void SetSize(const float x, const float y);
	//���W�w��
	void SetPos(const float x, const float y);
#pragma endregion
#pragma region �`��
	//���ʃO���t�B�b�N�X�R�}���h
	static void CommonBeginDraw();
	//�u�����h���[�h�ݒ�
	static void SetBlendMode(BlendMode blendMode);
	//�`��R�}���h
	void Draw();
#pragma endregion
	const NVector2& GetTexSize()const { return texSize_; }
	const NVector2& GetSize()const { return size_; }
};