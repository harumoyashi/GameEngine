#pragma once
#include <d3d12.h>

#include <string>
#include <map>
#include <wrl.h>

class NShader
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3DBlob> vsBlob_;	// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob_;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob_;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob_; // �G���[�I�u�W�F�N�g

	std::map<std::string, NShader> shaderMap_;

public:
	static NShader* GetInstance();

	//�p�X�ɍ��킹�����_�V�F�[�_�[�̓ǂݍ���
	void LoadVS(std::string vsPath);
	//�p�X�ɍ��킹���s�N�Z���V�F�[�_�[�̓ǂݍ���
	void LoadPS(std::string psPath);
	//�p�X�ɍ��킹���W�I���g���V�F�[�_�[�̓ǂݍ���
	void LoadGS(std::string gsPath);

	//�V�F�[�_�[�𐶐����ă}�b�v�ɓo�^
	void CreateShader(std::string id, std::string path, const bool isLoadGS);
	//�w�肵��id�̃V�F�[�_�[���擾
	NShader* GetShader(std::string id);
};