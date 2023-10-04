#include "NUtil.h"
#include "NWindows.h"
#include "DirectXTex.h"
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

std::wstring NUtil::ToWideString(const std::string& mString)
{
	//���C�h������ɕϊ������ۂ̕��������v�Z
	int buffSize = MultiByteToWideChar(CP_ACP, 0U, mString.c_str(), -1, nullptr, 0);

	//���C�h������
	std::wstring wString;
	wString.resize(buffSize);

	//���C�h������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], buffSize);

	return wString;
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

uint32_t NUtil::StringToInt(const std::string& string)
{
	return (uint32_t)atoi(string.c_str());
}

std::string NUtil::IntToString(uint32_t num)
{
	return  std::to_string(num);
}
