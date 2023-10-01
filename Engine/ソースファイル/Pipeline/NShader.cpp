#include "NShader.h"

#include <cassert>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

std::map<std::string, NShader> NShader::shaderMap_;

void NShader::LoadVS(std::string vsPath)
{
	HRESULT result;

	std::string shaderFolder = "Resources/shaders/";
	std::string shaderType = "VS.hlsl";
	vsPath = shaderFolder + vsPath + shaderType;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		std::wstring(vsPath.begin(), vsPath.end()).c_str(),  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob_, &errorBlob_);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob_����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NShader::LoadPS(std::string psPath)
{
	HRESULT result;

	std::string shaderFolder = "Resources/shaders/";
	std::string shaderType = "PS.hlsl";
	psPath = shaderFolder + psPath + shaderType;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		std::wstring(psPath.begin(), psPath.end()).c_str(),  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob_, &errorBlob_);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob_����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NShader::LoadGS(std::string gsPath)
{
	HRESULT result;

	std::string shaderFolder = "Resources/shaders/";
	std::string shaderType = "GS.hlsl";
	gsPath = shaderFolder + gsPath + shaderType;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		std::wstring(gsPath.begin(), gsPath.end()).c_str(),  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "gs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&gsBlob_, &errorBlob_);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob_����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NShader::LoadCS(std::string csPath)
{
	HRESULT result;

	std::string shaderFolder = "Resources/shaders/";
	std::string shaderType = "CS.hlsl";
	csPath = shaderFolder + csPath + shaderType;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		std::wstring(csPath.begin(), csPath.end()).c_str(),  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "cs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&csBlob_, &errorBlob_);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob_����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void NShader::CreateShader(std::string id, std::string path, bool isLoadGS, bool isLoadCS)
{
	shaderMap_[id] = NShader();
	shaderMap_[id].LoadVS(path);
	shaderMap_[id].LoadPS(path);
	if (isLoadGS)
	{
		shaderMap_[id].LoadGS(path);
	}

	if (isLoadCS)
	{
		shaderMap_[id].LoadCS(path);
	}
}

NShader* NShader::GetShader(std::string id)
{
	return &shaderMap_[id];
}
