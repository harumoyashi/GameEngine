#pragma once
#include "NDX12.h"
#include <array>
#include <map>

#include <wrl.h>

typedef std::string TextureHandle;

class NTexture
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> texBuff_;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle_ = D3D12_CPU_DESCRIPTOR_HANDLE(); //SRV�̃n���h��(CPU��)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_ = D3D12_GPU_DESCRIPTOR_HANDLE(); //SRV�̃n���h��(GPU��)
	std::string fileName_; //�t�@�C����

	NTexture();
	~NTexture();
};

class NTextureManager
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	TexMetadata metadata_{};
	ScratchImage scratchImg_{};
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc_{};
	D3D12_HEAP_PROPERTIES texHeapProp_{};	//�q�[�v
	D3D12_RESOURCE_DESC texResDesc_{};		//���\�[�X
	static const size_t sMaxSRVCount = 2056;	//SRV�̍ő��
	ScratchImage mipChain_{};				//�~�b�v�}�b�v

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};	//�ݒ�\����

	std::map<TextureHandle, NTexture> textureMap_;	//�e�N�X�`���Q
	ComPtr<ID3D12Resource> tb_;	//�]���p�̃e�N�X�`���o�b�t�@

public:
	//TextureManager���擾����
	static NTextureManager* GetInstance();

	//������
	void Init();

	//�G���[�e�N�X�`���𐶐�����
	NTexture CreateErrorTexture();

	//�e�N�X�`���̓ǂݍ���
	//index�����Ȃ�������0�ԖڂɊi�[
	//�ǂݍ��ނ��тɏ����ς�邩��A��񂫂�̎������ɂ���
	NTexture LoadTexture(const std::string& pictureName, const std::string& handle = "");

private:
	//WIC�e�N�X�`���̃��[�h
	bool Load(const std::string& pictureName);
	//�~�b�v�}�b�v����
	void CreateMipmap();

	//�q�[�v�ݒ�
	void SetTBHeap();
	//���\�[�X�ݒ�
	void SetTBResource();
	//�e�N�X�`���o�b�t�@�̐���
	ID3D12Resource* CreateTexBuff();
	//�~�b�v�}�b�v�f�[�^�̓]��
	void TexBuffMaping(ID3D12Resource* texBuff);
	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	void SetSRV();
	//�V�F�[�_�[���\�[�X�r���[����
	NTexture CreateSRV(NTexture& tex);
};

