#pragma once
#include <string>

enum class BlendMode
{
	None,	//���̂܂�
	Alpha,	//���u�����h
	Add,	//���Z
	Sub,	//���Z
	Inv,	//���]
};

struct NUtil
{
	//wstring�^��string�^�ɕϊ�
	static std::string ToUTF8(const std::wstring& value);
	//�g���q���o
	static std::string GetExtension(const std::string& path);
	//�g���q�u����������
	static std::wstring ReplaceExtension(const std::wstring& origin, const char* ext);
};