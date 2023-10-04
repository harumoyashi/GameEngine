#pragma once
#include <string>

static const int maxBoneIndices = 4;

enum class BlendMode
{
	None,	//���̂܂�
	Alpha,	//���u�����h
	Add,	//���Z
	Sub,	//���Z
	Inv,	//���]
};

// ���f���̃t�H�[�}�b�g
enum class ModelFormat
{
	Obj,
	Fbx,
};

struct NUtil
{
	//wstring�^��string�^�ɕϊ�
	static std::string ToUTF8(const std::wstring& value);
	
	//�}���`�o�C�g����������C�h������ɕϊ�
	static std::wstring ToWideString(const std::string& mString);
	//�g���q���o
	static std::string GetExtension(const std::string& path);
	//�g���q�u����������
	static std::wstring ReplaceExtension(const std::wstring& origin, const char* ext);
	//�����񂩂�uint32_t�^�ɕϊ����ĕԂ�
	static uint32_t StringToInt(const std::string& string);
	//uint32_t�^���當����ɕϊ����ĕԂ�
	static std::string IntToString(uint32_t num);
};