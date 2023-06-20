#pragma once
#include <string>

class NUtil
{
public:
	//wstring�^��string�^�ɕϊ�
	static std::string ToUTF8(const std::wstring& value);
	//�g���q���o
	static std::string GetExtension(const std::string& path);
	//�g���q�u����������
	static std::wstring ReplaceExtension(const std::wstring& origin, const char* ext);
};