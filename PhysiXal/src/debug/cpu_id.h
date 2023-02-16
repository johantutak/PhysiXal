#pragma once

// Pradeep Garigipati aka 9prady9 on GitHUB.
// Revisoned to fit application

#include <algorithm>
#include <iostream>
#include <string>

#ifdef PX_PLATFORM_WINDOWS
    #include <limits.h>
    #include <intrin.h>
    typedef unsigned __int32  uint32_t;
#else
    #include <stdint.h>
#endif

class CPUInfo 
{
public:
    // CPU info 
    CPUInfo();
    std::string  vendor()            const { return mVendorId; }
    std::string  model()             const { return mModelName; }
    int     cores()             const { return mNumCores; }
    float   cpuSpeedInMHz()     const { return mCPUMHz; }
    bool    isSSE()             const { return mIsSSE; }
    bool    isSSE2()            const { return mIsSSE2; }
    bool    isSSE3()            const { return mIsSSE3; }
    bool    isSSE41()           const { return mIsSSE41; }
    bool    isSSE42()           const { return mIsSSE42; }
    bool    isAVX()             const { return mIsAVX; }
    bool    isAVX2()            const { return mIsAVX2; }
    bool    isHyperThreaded()   const { return mIsHTT; }
    int     logicalCpus()       const { return mNumLogCpus; }

private:
    // Bit positions for data extractions
    static const uint32_t SSE_POS = 0x02000000;
    static const uint32_t SSE2_POS = 0x04000000;
    static const uint32_t SSE3_POS = 0x00000001;
    static const uint32_t SSE41_POS = 0x00080000;
    static const uint32_t SSE42_POS = 0x00100000;
    static const uint32_t AVX_POS = 0x10000000;
    static const uint32_t AVX2_POS = 0x00000020;
    static const uint32_t LVL_NUM = 0x000000FF;
    static const uint32_t LVL_TYPE = 0x0000FF00;
    static const uint32_t LVL_CORES = 0x0000FFFF;

    // Attributes
    std::string mVendorId;
    std::string mModelName;
    int    mNumSMT;
    int    mNumCores;
    int    mNumLogCpus;
    float  mCPUMHz;
    bool   mIsHTT;
    bool   mIsSSE;
    bool   mIsSSE2;
    bool   mIsSSE3;
    bool   mIsSSE41;
    bool   mIsSSE42;
    bool   mIsAVX;
    bool   mIsAVX2;
};

//#### EXAMPLE #### How to use with example below

#if 0

CPUInfo cinfo;

PX_CORE_TRACE("VENDOR: {0}", cinfo.vendor());
PX_CORE_TRACE("DEVICE (CPU): {0}", cinfo.model());
PX_CORE_TRACE("CORES: {0}", cinfo.cores());
PX_CORE_TRACE("LOGICAL CORES: {0}", cinfo.logicalCpus());
PX_CORE_TRACE("SPEED (MHz): {0}", cinfo.cpuSpeedInMHz());
PX_CORE_TRACE("HYPER THREADED CPU: {0}", cinfo.isHyperThreaded());
PX_CORE_TRACE("CPU SSE: {0}", cinfo.isSSE());
PX_CORE_TRACE("CPU SSE2: {0}", cinfo.isSSE2());
PX_CORE_TRACE("CPU SSE3: {0}", cinfo.isSSE3());
PX_CORE_TRACE("CPU SSE41: {0}", cinfo.isSSE41());
PX_CORE_TRACE("CPU SSE42 : {0}", cinfo.isSSE42());
PX_CORE_TRACE("CPU AVX: {0}", cinfo.isAVX());
PX_CORE_TRACE("CPU AVX2: {0}", cinfo.isAVX2());

#endif