#pragma once

#include "Windows.h"
#include <string>

namespace EngineUtils
{
	std::string WstringToString(const std::wstring& WstringToConvert)
	{
		if (!WstringToConvert.empty())
		{
			const int WstringSize = static_cast<int>(WstringToConvert.size());
			const int Size = WideCharToMultiByte(CP_UTF8, 0, &WstringToConvert[0], WstringSize, nullptr, 0, nullptr, nullptr);
			std::string ConvertedString(Size, 0);
			WideCharToMultiByte(CP_UTF8, 0, &WstringToConvert[0], WstringSize, &ConvertedString[0], Size, nullptr, nullptr);
			return ConvertedString;
		}

		return std::string();
	}

	std::wstring StringToWstring(const std::string& StringToConvert)
	{
		if (!StringToConvert.empty())
		{
			const int StringSize = static_cast<int>(StringToConvert.size());
			const int Size = MultiByteToWideChar(CP_UTF8, 0, &StringToConvert[0], StringSize, nullptr, 0);
			std::wstring ConvertedWstring(Size, 0);
			MultiByteToWideChar(CP_UTF8, 0, &StringToConvert[0], StringSize, &ConvertedWstring[0], Size);
			return ConvertedWstring;
		}

		return std::wstring();
	}

}