#pragma once
#include <string>

class NUtil
{
public:
	//wstring�^��string�^�ɕϊ�
	static std::string ToUTF8(const std::wstring& value);
};