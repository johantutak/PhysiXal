#pragma once

#include <locale>
#include <codecvt>

namespace PhysiXal::Utilities {

	class String
	{
	public:
		// String utilities
		static std::wstring ToWideString(const std::string& str);
		static std::string ToUtf8String(const std::wstring& wstr);
		static std::string ExtractFileName(const std::string& filePath);
	private:
	};
}