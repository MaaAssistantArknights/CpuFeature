//
// Created by bakashigure on 2022-08-15.
// Code from https://stackoverflow.com/a/22521619/922184
//
#include <napi.h>
#include <stdint.h>
#include <immintrin.h>
#include <iostream>

#ifdef _MSVC_VER
#define XCR_XFEATURE_ENABLED_MASK _XCR_XFEATURE_ENABLED_MASK
#else
#define XCR_XFEATURE_ENABLED_MASK uint8_t(0)
#endif

#ifdef _WIN32
//  Windows
#include <intrin.h>
#define cpuid(info, x) __cpuidex(info, x, 0)
#else
//  GCC Intrinsics
#include <cpuid.h>
void cpuid(int info[4], int InfoType)
{
    __cpuid_count(InfoType, 0, info[0], info[1], info[2], info[3]);
}
#endif


bool AVXSupport()
{
    bool avxSupported = false;

    int cpuInfo[4];
    cpuid(cpuInfo, 1);

    bool osUsesXSAVE_XRSTORE = cpuInfo[2] & (1 << 27) || false;
    bool cpuAVXSuport = cpuInfo[2] & (1 << 28) || false;

    if (osUsesXSAVE_XRSTORE && cpuAVXSuport)
    {
        unsigned long long xcrFeatureMask = _xgetbv(XCR_XFEATURE_ENABLED_MASK);
        avxSupported = (xcrFeatureMask & 0x6) == 0x6;
    }
    return avxSupported;
}

Napi::Value GetAVXSupport(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, AVXSupport());
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "GetAVXSupport"), Napi::Function::New(env, GetAVXSupport));
    return exports;
}

NODE_API_MODULE(addon, Init)
