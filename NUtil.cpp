#include "NUtil.h"
#include "NWindows.h"
#include "stringapiset.h"
#include <filesystem>

std::string NUtil::ToUTF8(const std::wstring& value)
{
    auto length = WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr, nullptr);
    auto buffer = new char[length];

    WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, buffer, length, nullptr, nullptr);

    std::string result(buffer);
    delete[] buffer;
    buffer = nullptr;

    return result;
}

std::string NUtil::GetExtension(const std::string& path)
{
	std::string ext;
	size_t pos1 = path.rfind('.');
	if (pos1 != std::string::npos) {
		ext = path.substr(pos1 + 1, path.size() - pos1);
		std::string::iterator itr = ext.begin();
		while (itr != ext.end()) {
			*itr = tolower(*itr);
			itr++;
		}
		itr = ext.end() - 1;
		while (itr != ext.begin()) {
			if (*itr == 0 || *itr == 32) {
				ext.erase(itr--);
			}
			else {
				itr--;
			}
		}
	}

	return ext;
}

std::wstring NUtil::ReplaceExtension(const std::wstring& origin, const char* ext)
{
	std::filesystem::path p = origin.c_str();
	return p.replace_extension(ext).c_str();
}