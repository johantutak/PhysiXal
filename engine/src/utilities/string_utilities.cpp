#include "px_pch.h"
#include "utilities/string_utilities.h"

namespace PhysiXal::Utilities {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// String utilities
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::wstring String::ToWideString(const std::string& str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    }

    std::string String::ToUtf8String(const std::wstring& wstr)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }

    std::string String::ExtractFileName(const std::string& filePath)
    {
        std::string fileName = filePath;
        size_t lastSlashPos = fileName.find_last_of("\\/");
        
        if (lastSlashPos != std::string::npos)
        {
            fileName = fileName.substr(lastSlashPos + 1);
        }
        return fileName;
    }
}
