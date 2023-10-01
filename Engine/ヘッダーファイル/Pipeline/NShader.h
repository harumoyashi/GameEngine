#pragma once
#include <d3d12.h>

#include <string>
#include <map>
#include <wrl.h>

class NShader final
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3DBlob> vsBlob_;		// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob_;		// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob_;		// �W�I���g���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> csBlob_;		// �R���s���[�g�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob_;	// �G���[�I�u�W�F�N�g

	static std::map<std::string, NShader> shaderMap_;

public:
	//�p�X�ɍ��킹�����_�V�F�[�_�[�̓ǂݍ���
	void LoadVS(std::string vsPath);
	//�p�X�ɍ��킹���s�N�Z���V�F�[�_�[�̓ǂݍ���
	void LoadPS(std::string psPath);
	//�p�X�ɍ��킹���W�I���g���V�F�[�_�[�̓ǂݍ���
	void LoadGS(std::string gsPath);
	//�p�X�ɍ��킹���R���s���[�g�V�F�[�_�[�̓ǂݍ���
	void LoadCS(std::string csPath);

	//�V�F�[�_�[�𐶐����ă}�b�v�ɓo�^
	//id:�V�F�[�_�[�}�b�v�ɓo�^���閼�O
	//path:�V�F�[�_�[�}�b�v�ɓo�^����V�F�[�_�[�̃p�X
	//isLoadGS:�W�I���g���V�F�[�_�[�ǂݍ��ނ�
	//isLoadCS:�R���s���[�g�V�F�[�_�[�ǂݍ��ނ�
	static void CreateShader(std::string id, std::string path, bool isLoadGS = false, bool isLoadCS = false);
	//�w�肵��id�̃V�F�[�_�[���擾
	static NShader* GetShader(std::string id);

	ID3DBlob* GetVSBlob() { return vsBlob_.Get(); }
	ID3DBlob* GetPSBlob() { return psBlob_.Get(); }
	ID3DBlob* GetGSBlob() { return gsBlob_.Get(); }
	ID3DBlob* GetCSBlob() { return csBlob_.Get(); }
};