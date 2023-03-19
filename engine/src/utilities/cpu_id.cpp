#include "px_pch.h"
#include "utilities/cpu_id.h"

#define MAX_INTEL_TOP_LVL 4

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPUID
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CPUID
{
    U32 regs[4];

public:
    explicit CPUID(unsigned funcId, unsigned subFuncId)
    {
#ifdef PX_PLATFORM_WINDOWS
        __cpuidex((int*)regs, (int)funcId, (int)subFuncId);
#else
        asm volatile
            ("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
                : "a" (funcId), "c" (subFuncId));
        // ECX is set to zero for CPUID function 4
#endif
    }
    const U32& EAX() const { return regs[0]; }
    const U32& EBX() const { return regs[1]; }
    const U32& ECX() const { return regs[2]; }
    const U32& EDX() const { return regs[3]; }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPU Info
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CPUInfo::CPUInfo()
{
    // Get vendor name EAX=0
    CPUID cpuID0(0, 0);
    U32 HFS = cpuID0.EAX();
    mVendorId += std::string((const char*)&cpuID0.EBX(), 4);
    mVendorId += std::string((const char*)&cpuID0.EDX(), 4);
    mVendorId += std::string((const char*)&cpuID0.ECX(), 4);
    // Get SSE instructions availability
    CPUID cpuID1(1, 0);
    mIsHTT = cpuID1.EDX() & AVX_POS;
    mIsSSE = cpuID1.EDX() & SSE_POS;
    mIsSSE2 = cpuID1.EDX() & SSE2_POS;
    mIsSSE3 = cpuID1.ECX() & SSE3_POS;
    mIsSSE41 = cpuID1.ECX() & SSE41_POS;
    mIsSSE42 = cpuID1.ECX() & SSE41_POS;
    mIsAVX = cpuID1.ECX() & AVX_POS;
    // Get AVX2 instructions availability
    CPUID cpuID7(7, 0);
    mIsAVX2 = cpuID7.EBX() & AVX2_POS;

    std::string upVId = mVendorId;
    for_each(upVId.begin(), upVId.end(), [](char& in) { in = ::toupper(in); });
    // Get num of cores
    if (upVId.find("INTEL") != std::string::npos) 
    {
        if (HFS >= 11) 
        {
            for (int lvl = 0; lvl < MAX_INTEL_TOP_LVL; ++lvl) 
            {
                CPUID cpuID4(0x0B, lvl);
                U32 currLevel = (LVL_TYPE & cpuID4.ECX()) >> 8;
                switch (currLevel) {
                case 0x01: mNumSMT = LVL_CORES & cpuID4.EBX(); break;
                case 0x02: mNumLogCpus = LVL_CORES & cpuID4.EBX(); break;
                default: break;
                }
            }
            mNumCores = mNumLogCpus / mNumSMT;
        }
        else 
        {
            if (HFS >= 1) 
            {
                mNumLogCpus = (cpuID1.EBX() >> 16) & 0xFF;
                if (HFS >= 4) 
                {
                    mNumCores = 1 + (CPUID(4, 0).EAX() >> 26) & 0x3F;
                }
            }
            if (mIsHTT) 
            {
                if (!(mNumCores > 1)) 
                {
                    mNumCores = 1;
                    mNumLogCpus = (mNumLogCpus >= 2 ? mNumLogCpus : 2);
                }
            }
            else 
            {
                mNumCores = mNumLogCpus = 1;
            }
        }
    }
    else if (upVId.find("AMD") != std::string::npos) 
    {
        if (HFS >= 1) 
        {
            mNumLogCpus = (cpuID1.EBX() >> 16) & 0xFF;
            if (CPUID(0x80000000, 0).EAX() >= 8) 
            {
                mNumCores = 1 + (CPUID(0x80000008, 0).ECX() & 0xFF);
            }
        }
        if (mIsHTT) 
        {
            if (!(mNumCores > 1)) 
            {
                mNumCores = 1;
                mNumLogCpus = (mNumLogCpus >= 2 ? mNumLogCpus : 2);
            }
        }
        else 
        {
            mNumCores = mNumLogCpus = 1;
        }
    }
    else 
    {
        PX_CORE_TRACE("Unexpected vendor id");
    }
    // Get processor brand string
    // This seems to be working for both Intel & AMD vendors
    for (int i = 0x80000002; i < 0x80000005; ++i) 
    {
        CPUID cpuID(i, 0);
        mModelName += std::string((const char*)&cpuID.EAX(), 4);
        mModelName += std::string((const char*)&cpuID.EBX(), 4);
        mModelName += std::string((const char*)&cpuID.ECX(), 4);
        mModelName += std::string((const char*)&cpuID.EDX(), 4);
    }
}