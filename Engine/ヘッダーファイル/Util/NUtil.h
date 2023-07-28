#pragma once
#include <string>

enum class BlendMode
{
	None,	//‚»‚Ì‚Ü‚Ü
	Alpha,	//ƒ¿ƒuƒŒƒ“ƒh
	Add,	//‰ÁZ
	Sub,	//Œ¸Z
	Inv,	//”½“]
};

struct NUtil
{
	//wstringŒ^‚ğstringŒ^‚É•ÏŠ·
	static std::string ToUTF8(const std::wstring& value);
	//Šg’£q’Šo
	static std::string GetExtension(const std::string& path);
	//Šg’£q’u‚«Š·‚¦ˆ—
	static std::wstring ReplaceExtension(const std::wstring& origin, const char* ext);
};