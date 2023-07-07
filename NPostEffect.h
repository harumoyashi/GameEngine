#pragma once
#include "NSprite.h"

class NPostEffect :
    public NSprite
{
protected:
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

    //�p�C�v���C���̖��O
    std::string pipelineName_;

public:
    NPostEffect();
    virtual ~NPostEffect() = default;

    virtual void Init();
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

