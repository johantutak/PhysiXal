#include "px_pch.h"
#include "platform/win/win_utilities.h"

#include <commdlg.h>
#include "utilities/string_utilities.h"

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Windows utilities
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool FileManager::SelectFile(std::string& filePath, const TCHAR* filter)
    {
        ZeroMemory(&ofn, sizeof(ofn));
        ZeroMemory(szFile, sizeof(szFile));

        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn))
        {
            filePath = Utilities::String::ToUtf8String(szFile);
            filePath = Utilities::String::ExtractFileName(filePath);
            return true;
        }
        return false;
    }

    float FileManager::CPUTemperature()
    {
        IWbemLocator* pLoc = NULL;
        IWbemServices* pSvc = NULL;
        IEnumWbemClassObject* pEnumerator = NULL;

        HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
        if (FAILED(hres))
            return -1.0f;

        hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
        if (FAILED(hres)) {
            CoUninitialize();
            return -1.0f;
        }

        hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\WMI"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
        if (FAILED(hres)) {
            pLoc->Release();
            CoUninitialize();
            return -1.0f;
        }

        hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
        if (FAILED(hres)) {
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return -1.0f;
        }

        hres = pSvc->ExecQuery(
            bstr_t("WQL"),
            bstr_t("SELECT * FROM MSAcpi_ThermalZoneTemperature"),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator);
        if (FAILED(hres)) {
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return -1.0f;
        }

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;
        float temperature = -1.0f;

        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (0 == uReturn)
                break;

            VARIANT vtProp;
            hr = pclsObj->Get(L"CurrentTemperature", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr))
                temperature = vtProp.intVal;

            VariantClear(&vtProp);
            pclsObj->Release();
        }

        pSvc->Release();
        pLoc->Release();
        pEnumerator->Release();
        CoUninitialize();

        return temperature;
    }
}