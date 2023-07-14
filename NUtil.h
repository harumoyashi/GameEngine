#pragma once
#include <string>

struct NUtil
{
	//wstringŒ^‚ğstringŒ^‚É•ÏŠ·
	static std::string ToUTF8(const std::wstring& value);
	//Šg’£q’Šo
	static std::string GetExtension(const std::string& path);
	//Šg’£q’u‚«Š·‚¦ˆ—
	static std::wstring ReplaceExtension(const std::wstring& origin, const char* ext);
};