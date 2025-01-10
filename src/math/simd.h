
#ifndef _EOKAS_BASE_SIMD_H_
#define _EOKAS_BASE_SIMD_H_

#include "base/header.h"
#include "math.h"

namespace eokas
{

#define _EOKAS_SIMD_NONE    0x0000
#define _EOKAS_SIMD_SSE     0x0001
#define _EOKAS_SIMD_SSE2    0x0002
#define _EOKAS_SIMD_SSE3    0x0004
#define _EOKAS_SIMD_SSE3_E  0x0008
#define _EOKAS_SIMD_SSE4_1  0x0010
#define _EOKAS_SIMD_SSE4_2  0x0020
#define _EOKAS_SIMD_AVX     0x0040
#define _EOKAS_SIMD_AVX2    0x0080
#define _EOKAS_SIMD_NEON    0x0100

#define _EOKAS_SIMD_ARCH_SSE (_EOKAS_SIMD_SSE)
#define _EOKAS_SIMD_ARCH_SSE2 ((_EOKAS_SIMD_SSE2) | (_EOKAS_SIMD_ARCH_SSE))
#define _EOKAS_SIMD_ARCH_SSE3 ((_EOKAS_SIMD_SSE3) | (_EOKAS_SIMD_ARCH_SSE2))
#define _EOKAS_SIMD_ARCH_SSE3_E ((_EOKAS_SIMD_SSE3_E) | (_EOKAS_SIMD_ARCH_SSE3))
#define _EOKAS_SIMD_ARCH_SSE4_1 ((_EOKAS_SIMD_SSE4_1) | (_EOKAS_SIMD_ARCH_SSE3_E))
#define _EOKAS_SIMD_ARCH_SSE4_2 ((_EOKAS_SIMD_SSE4_2) | (_EOKAS_SIMD_ARCH_SSE4_1))
#define _EOKAS_SIMD_ARCH_AVX ((_EOKAS_SIMD_AVX) | (_EOKAS_SIMD_ARCH_SSE4_2))
#define _EOKAS_SIMD_ARCH_AVX2 ((_EOKAS_SIMD_AVX2) | (_EOKAS_SIMD_ARCH_AVX))
#define _EOKAS_SIMD_ARCH_NEON (_EOKAS_SIMD_NEON)

#if defined(__AVX2__)
#define _EOKAS_SIMD _EOKAS_SIMD_ARCH_AVX2
#elif defined(__AVX__)
    #define _EOKAS_SIMD _EOKAS_SIMD_ARCH_AVX
#elif defined(__SSE4_2__)
    #define _EOKAS_SIMD _EOKAS_SIMD_ARCH_SSE4_2
#elif defined(__SSE4_1__)
    #define _EOKAS_SIMD _EOKAS_SIMD_ARCH_SSE4_1
#elif defined(__SSE4__)
    #define _EOKAS_SIMD _EOKAS_SIMD_ARCH_SSE4_1
#elif defined(__SSSE3__)
    #define _EOKAS_SIMD _EOKAS_SIMD_ARCH_SSE3_E
#elif defined(__SSE3__)
    #define _EOKAS_SIMD _EOKAS_SIMD_ARCH_SSE3
#elif defined(__SSE2__)
    #define _EOKAS_SIMD _EOKAS_SIMD_ARCH_SSE2
#elif defined(__SSE__)
    #define _EOKAS_SIMD _EOKAS_SIMD_ARCH_SSE
#elif defined(__NEON__)
    #define _EOKAS_SIMD _EOKAS_SIMD_NEON
#else
    #define _EOKAS_SIMD _EOKAS_SIMD_NONE
#endif

#if _EOKAS_SIMD & _EOKAS_SIMD_AVX2
#   include <immintrin.h>
#elif _EOKAS_SIMD & _EOKAS_SIMD_AVX
    #   include <immintrin.h>
#elif _EOKAS_SIMD & _EOKAS_SIMD_SSE4_2
#	if GLM_COMPILER & GLM_COMPILER_CLANG
#		include <popcntintrin.h>
#	endif
#	include <nmmintrin.h>
#elif _EOKAS_SIMD & _EOKAS_SIMD_SSE4_1
#	include <smmintrin.h>
#elif _EOKAS_SIMD & _EOKAS_SIMD_SSE3_E
#	include <tmmintrin.h>
#elif _EOKAS_SIMD & _EOKAS_SIMD_SSE3
#	include <pmmintrin.h>
#elif _EOKAS_SIMD & _EOKAS_SIMD_SSE2
#	include <emmintrin.h>
#elif _EOKAS_SIMD & _EOKAS_SIMD_NEON
#	include "neon.h"
#endif


#if _EOKAS_SIMD & _EOKAS_SIMD_SSE
    using f128_t = __m128;
    
    struct SimdVec4 {
        using Comp = f32_t;
        using Type = __m128;
        
        static Type op_set(Comp x, Comp y, Comp z, Comp w) {
            return _mm_set_ps(x, y, z, w);
        }
        static void op_get(Type v, Comp& x, Comp& y, Comp& z, Comp& w) {
            alignas(16) f32_t data[4];
            _mm_store_ps(data, v);
            x = data[0];
            y = data[1];
            z = data[2];
            w = data[3];
        }
        static Type op_add(Type a, Type b) {
            return _mm_add_ps(a, b);
        }
        static Type op_sub(Type a, Type b) {
            return _mm_sub_ps(a, b);
        }
        static Type op_mul(Type a, Type b) {
            return _mm_mul_ps(a, b);
        }
        static Type op_div(Type a, Type b) {
            return _mm_div_ps(a, b);
        }
        static Type op_and(Type x, Type y)
        {
            return _mm_and_ps(x, y);
        }
        static Type op_or(Type x, Type y)
        {
            return _mm_or_ps(x, y);
        }
        static Type op_xor(Type x, Type y)
        {
            return _mm_xor_ps(x, y);
        }
        static Type op_eq(Type x, Type y)
        {
            return _mm_cmpeq_ps(x, y);
        }
        static Type op_ne(Type x, Type y)
        {
            return _mm_cmpneq_ps(x, y);
        }
        static Type op_gt(Type x, Type y)
        {
            return _mm_cmpgt_ps(x, y);
        }
        static Type op_ge(Type x, Type y)
        {
            return _mm_cmpge_ps(x, y);
        }
        static Type op_lt(Type x, Type y)
        {
            return _mm_cmplt_ps(x, y);
        }
        static Type op_le(Type x, Type y)
        {
            return _mm_cmple_ps(x, y);
        }
        static Type op_truncate(Type x)
        {
            return _mm_cvtepi32_ps(_mm_cvttps_epi32(x));
        }
        static Type op_dot(Type a, Type b) {
            return _mm_dp_ps(a, b, 0xFF);
        }
        static Type op_length(Type a) {
            Type temp = _mm_dp_ps(a, a, 0xFF);
            return _mm_sqrt_ps(temp);
        }
        static Type op_normalize(Type a)
        {
            Type lengthSqr = _mm_dp_ps(a, a, 0xFF);
            Type length = _mm_sqrt_ps(lengthSqr);
            Type normal = _mm_div_ps(a, length);
            return normal;
        }
        static Type op_select(Type mask, Type arg0, Type arg1)
        {
            Type u = op_xor(arg0, arg1);
            Type v = op_and(mask, u);
            return op_xor(arg1, v);
        }
        
    };

#if _EOKAS_SIMD & _EOKAS_SIMD_AVX
    using f256_t = __m256d;
#endif
#if _EOKAS_SIMD & _EOKAS_SIMD_AVX2
    using i256_t = __m256i;
    using u256_t = __m256i;
#endif

#elif _EOKAS_SIMD & _EOKAS_SIMD_NEON
    using f128_t = float32x4_t;
#endif

}

#endif //_EOKAS_BASE_SIMD_H_
