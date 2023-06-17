#pragma once

#include <commdlg.h>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

namespace PhysiXal {

    class FileManager
    {
    public:
        // Windows utilities
        static bool SelectFile(std::string& filePath, const TCHAR* filter);

        static float CPUTemperature();
    private:
        inline static OPENFILENAME ofn;
        inline static TCHAR szFile[MAX_PATH];
    };
}