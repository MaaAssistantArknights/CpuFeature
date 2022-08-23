//
// Created by bakashigure on 2022-08-15.
// Code from https://stackoverflow.com/a/22521619/922184
//
#include <napi.h>
#include <stdint.h>
#include <immintrin.h>
#include <iostream>

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
void cpuid(int32_t out[4], int32_t eax, int32_t ecx)
{
    __cpuid_count(eax, ecx, out[0], out[1], out[2], out[3]);
}
#endif

struct x86_features
{
    //  Misc.
    bool HW_MMX;
    bool HW_x64;
    bool HW_ABM;
    bool HW_RDRAND;
    bool HW_RDSEED;
    bool HW_BMI1;
    bool HW_BMI2;
    bool HW_ADX;
    bool HW_MPX;
    bool HW_PREFETCHW;
    bool HW_PREFETCHWT1;
    bool HW_RDPID;

    //  SIMD: 128-bit
    bool HW_SSE;
    bool HW_SSE2;
    bool HW_SSE3;
    bool HW_SSSE3;
    bool HW_SSE41;
    bool HW_SSE42;
    bool HW_SSE4a;
    bool HW_AES;
    bool HW_SHA;

    //  SIMD: 256-bit
    bool HW_AVX;
    bool HW_XOP;
    bool HW_FMA3;
    bool HW_FMA4;
    bool HW_AVX2;

    //  SIMD: 512-bit
    bool HW_AVX512_F;
    bool HW_AVX512_CD;

    //  Knights Landing
    bool HW_AVX512_PF;
    bool HW_AVX512_ER;

    //  Skylake Purley
    bool HW_AVX512_VL;
    bool HW_AVX512_BW;
    bool HW_AVX512_DQ;

    //  Cannon Lake
    bool HW_AVX512_IFMA;
    bool HW_AVX512_VBMI;

    //  Knights Mill
    bool HW_AVX512_VPOPCNTDQ;
    bool HW_AVX512_4FMAPS;
    bool HW_AVX512_4VNNIW;

    //  Cascade Lake
    bool HW_AVX512_VNNI;

    //  Cooper Lake
    bool HW_AVX512_BF16;

    //  Ice Lake
    bool HW_AVX512_VBMI2;
    bool HW_GFNI;
    bool HW_VAES;
    bool HW_AVX512_VPCLMUL;
    bool HW_AVX512_BITALG;
};

struct x86_features DetectFeatures()
{
    struct x86_features features
    {
    };
    int info[4];
    cpuid(info, 0, 0);
    int nIds = info[0];

    cpuid(info, 0x80000000, 0);
    uint32_t nExIds = info[0];

    //  Detect Features
    if (nIds >= 0x00000001)
    {
        cpuid(info, 0x00000001, 0);
        features.HW_MMX = (info[3] & ((int)1 << 23)) != 0;
        features.HW_SSE = (info[3] & ((int)1 << 25)) != 0;
        features.HW_SSE2 = (info[3] & ((int)1 << 26)) != 0;
        features.HW_SSE3 = (info[2] & ((int)1 << 0)) != 0;

        features.HW_SSSE3 = (info[2] & ((int)1 << 9)) != 0;
        features.HW_SSE41 = (info[2] & ((int)1 << 19)) != 0;
        features.HW_SSE42 = (info[2] & ((int)1 << 20)) != 0;
        features.HW_AES = (info[2] & ((int)1 << 25)) != 0;

        features.HW_AVX = (info[2] & ((int)1 << 28)) != 0;
        features.HW_FMA3 = (info[2] & ((int)1 << 12)) != 0;

        features.HW_RDRAND = (info[2] & ((int)1 << 30)) != 0;
    }
    if (nIds >= 0x00000007)
    {
        cpuid(info, 0x00000007, 0);
        features.HW_AVX2 = (info[1] & ((int)1 << 5)) != 0;

        features.HW_BMI1 = (info[1] & ((int)1 << 3)) != 0;
        features.HW_BMI2 = (info[1] & ((int)1 << 8)) != 0;
        features.HW_ADX = (info[1] & ((int)1 << 19)) != 0;
        features.HW_MPX = (info[1] & ((int)1 << 14)) != 0;
        features.HW_SHA = (info[1] & ((int)1 << 29)) != 0;
        features.HW_RDSEED = (info[1] & ((int)1 << 18)) != 0;
        features.HW_PREFETCHWT1 = (info[2] & ((int)1 << 0)) != 0;
        features.HW_RDPID = (info[2] & ((int)1 << 22)) != 0;

        features.HW_AVX512_F = (info[1] & ((int)1 << 16)) != 0;
        features.HW_AVX512_CD = (info[1] & ((int)1 << 28)) != 0;
        features.HW_AVX512_PF = (info[1] & ((int)1 << 26)) != 0;
        features.HW_AVX512_ER = (info[1] & ((int)1 << 27)) != 0;

        features.HW_AVX512_VL = (info[1] & ((int)1 << 31)) != 0;
        features.HW_AVX512_BW = (info[1] & ((int)1 << 30)) != 0;
        features.HW_AVX512_DQ = (info[1] & ((int)1 << 17)) != 0;

        features.HW_AVX512_IFMA = (info[1] & ((int)1 << 21)) != 0;
        features.HW_AVX512_VBMI = (info[2] & ((int)1 << 1)) != 0;

        features.HW_AVX512_VPOPCNTDQ = (info[2] & ((int)1 << 14)) != 0;
        features.HW_AVX512_4FMAPS = (info[3] & ((int)1 << 2)) != 0;
        features.HW_AVX512_4VNNIW = (info[3] & ((int)1 << 3)) != 0;

        features.HW_AVX512_VNNI = (info[2] & ((int)1 << 11)) != 0;

        features.HW_AVX512_VBMI2 = (info[2] & ((int)1 << 6)) != 0;
        features.HW_GFNI = (info[2] & ((int)1 << 8)) != 0;
        features.HW_VAES = (info[2] & ((int)1 << 9)) != 0;
        features.HW_AVX512_VPCLMUL = (info[2] & ((int)1 << 10)) != 0;
        features.HW_AVX512_BITALG = (info[2] & ((int)1 << 12)) != 0;

        cpuid(info, 0x00000007, 1);
        features.HW_AVX512_BF16 = (info[0] & ((int)1 << 5)) != 0;
    }
    if (nExIds >= 0x80000001)
    {
        cpuid(info, 0x80000001, 0);
        features.HW_x64 = (info[3] & ((int)1 << 29)) != 0;
        features.HW_ABM = (info[2] & ((int)1 << 5)) != 0;
        features.HW_SSE4a = (info[2] & ((int)1 << 6)) != 0;
        features.HW_FMA4 = (info[2] & ((int)1 << 16)) != 0;
        features.HW_XOP = (info[2] & ((int)1 << 11)) != 0;
        features.HW_PREFETCHW = (info[2] & ((int)1 << 8)) != 0;
    }
    return features;
}

Napi::Object GetFeatures(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    const auto features = DetectFeatures();
    Napi::Object object = Napi::Object::New(env);
    // Misc.
    object.Set("mmx", Napi::Boolean::New(env, features.HW_MMX));
    object.Set("x64", Napi::Boolean::New(env, features.HW_x64));
    object.Set("abm", Napi::Boolean::New(env, features.HW_ABM));
    object.Set("rdrand", Napi::Boolean::New(env, features.HW_RDRAND));
    object.Set("rdseed", Napi::Boolean::New(env, features.HW_RDSEED));
    object.Set("bmi1", Napi::Boolean::New(env, features.HW_BMI1));
    object.Set("bmi2", Napi::Boolean::New(env, features.HW_BMI2));
    object.Set("adx", Napi::Boolean::New(env, features.HW_ADX));
    object.Set("mpx", Napi::Boolean::New(env, features.HW_MPX));
    object.Set("prefetchw", Napi::Boolean::New(env, features.HW_PREFETCHW));
    object.Set("prefetchwt1", Napi::Boolean::New(env, features.HW_PREFETCHWT1));
    object.Set("rdpid", Napi::Boolean::New(env, features.HW_RDPID));

    // SIMD: 128-bit
    object.Set("sse", Napi::Boolean::New(env, features.HW_SSE));
    object.Set("sse2", Napi::Boolean::New(env, features.HW_SSE2));
    object.Set("sse3", Napi::Boolean::New(env, features.HW_SSE3));
    object.Set("ssse3", Napi::Boolean::New(env, features.HW_SSSE3));
    object.Set("sse41", Napi::Boolean::New(env, features.HW_SSE41));
    object.Set("sse42", Napi::Boolean::New(env, features.HW_SSE42));
    object.Set("sse4a", Napi::Boolean::New(env, features.HW_SSE4a));
    object.Set("aes", Napi::Boolean::New(env, features.HW_AES));
    object.Set("sha", Napi::Boolean::New(env, features.HW_SHA));

    // SIMD: 256-bit
    object.Set("avx", Napi::Boolean::New(env, features.HW_AVX));
    object.Set("xop", Napi::Boolean::New(env, features.HW_XOP));
    object.Set("fma3", Napi::Boolean::New(env, features.HW_FMA3));
    object.Set("fma4", Napi::Boolean::New(env, features.HW_FMA4));
    object.Set("avx2", Napi::Boolean::New(env, features.HW_AVX2));

    // SIMD: 512-bit
    object.Set("avx512f", Napi::Boolean::New(env, features.HW_AVX512_F));
    object.Set("avx512cd", Napi::Boolean::New(env, features.HW_AVX512_CD));

    object.Set("avx512pf", Napi::Boolean::New(env, features.HW_AVX512_PF));
    object.Set("avx512er", Napi::Boolean::New(env, features.HW_AVX512_ER));

    object.Set("avx512vl", Napi::Boolean::New(env, features.HW_AVX512_VL));
    object.Set("avx512bw", Napi::Boolean::New(env, features.HW_AVX512_BW));
    object.Set("avx512dq", Napi::Boolean::New(env, features.HW_AVX512_DQ));

    object.Set("avx512ifma", Napi::Boolean::New(env, features.HW_AVX512_IFMA));
    object.Set("avx512vbmi", Napi::Boolean::New(env, features.HW_AVX512_VBMI));

    object.Set("avx512vpopcntdq", Napi::Boolean::New(env, features.HW_AVX512_VPOPCNTDQ));
    object.Set("avx5124fmaps", Napi::Boolean::New(env, features.HW_AVX512_4FMAPS));
    object.Set("avx5124vnniw", Napi::Boolean::New(env, features.HW_AVX512_4VNNIW));

    object.Set("avx512vnni", Napi::Boolean::New(env, features.HW_AVX512_VNNI));

    object.Set("avx512bf16", Napi::Boolean::New(env, features.HW_AVX512_BF16));

    object.Set("avx512vbmi2", Napi::Boolean::New(env, features.HW_AVX512_VBMI2));
    object.Set("gfni", Napi::Boolean::New(env, features.HW_GFNI));
    object.Set("vaes", Napi::Boolean::New(env, features.HW_VAES));
    object.Set("avx512vpclmul", Napi::Boolean::New(env, features.HW_AVX512_VPCLMUL));
    object.Set("avx512bitalg", Napi::Boolean::New(env, features.HW_AVX512_BITALG));

    return object;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    // exports.Set(Napi::String::New(env, "GetAVXSupport"), Napi::Function::New(env, GetAVXSupport));
    exports.Set("GetFeatures", Napi::Function::New(env, GetFeatures));
    return exports;
}

NODE_API_MODULE(addon, Init)
