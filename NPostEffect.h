#pragma once
#include "NSprite.h"

class NPostEffect :
    public NSprite
{
private:
    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff_[2];
    //SRV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

    //�[�x�o�b�t�@
    ComPtr<ID3D12Resource> depthBuff_;
    //RTV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
    //DSV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV_;

    //��ʃN���A�J���[
    static const float kClearColor[4];

    //�|�X�g�G�t�F�N�g�p�̃p�C�v���C��
    PipelineSet pipeline;

public:
    NPostEffect();

    void Init();
    void TexChange();
    void Draw();

    //�e�N�X�`������
    void CreateTexture();
    //�����_�[�^�[�Q�b�g�r���[����
    void CreateRTV();
    //�[�x�o�b�t�@����
    void CreateDepthBuff();
    //�f�v�X�X�e���V���r���[����
    void CreateDSV();

    void PreDrawScene();
    void PostDrawScene();
};

