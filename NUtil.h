#pragma once
#include <string>

enum class BlendMode
{
	None,	//»Μάά
	Alpha,	//Ώuh
	Add,	//ΑZ
	Sub,	//ΈZ
	Inv,	//½]
};

struct NUtil
{
	//wstring^πstring^ΙΟ·
	static std::string ToUTF8(const std::wstring& value);
	//g£qo
	static std::string GetExtension(const std::string& path);
	//g£qu«·¦
	static std::wstring ReplaceExtension(const std::wstring& origin, const char* ext);
};