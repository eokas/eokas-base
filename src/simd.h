
#ifndef _EOKAS_BASE_SIMD_H_
#define _EOKAS_BASE_SIMD_H_

#include "header.h"

#if (_EOKAS_SIMD & _EOKAS_SIMD_AVX2)
#include <immintrin.h>

namespace eokas
{
    struct AVX2
    {
        template<typename T>
        static constexpr size_t countOf()
        {
            return sizeof(__m256i) / sizeof(T);
        }
    };

    template<typename T>
    struct avx2_value_t
    {
        using type = __m256i;
    };

    template<>
    struct avx2_value_t<f32_t>
    {
        using type = __m256;
    };

    template<>
    struct avx2_value_t<f64_t>
    {
        using type = __m256d;
    };

    template<typename T, size_t N = AVX2::countOf<T>()>
    struct avx2_vector_t
    {
        using value_t = typename avx2_value_t<T>::type;

        value_t value;
    };

    using u8x32_t = avx2_vector_t<u8_t, 32>;
    using u16x16_t = avx2_vector_t<u16_t, 16>;
    using u32x8_t = avx2_vector_t<u32_t, 8>;
    using u64x4_t = avx2_vector_t<u64_t, 4>;
    using i8x32_t = avx2_vector_t<i8_t, 32>;
    using i16x16_t = avx2_vector_t<i16_t, 16>;
    using i32x8_t = avx2_vector_t<i32_t, 8>;
    using i64x4_t = avx2_vector_t<i64_t, 4>;
    using f32x8_t = avx2_vector_t<f32_t, 8>;
    using f64x4_t = avx2_vector_t<f64_t, 4>;
}

#elif (_EOKAS_SIMD & _EOKAS_SIMD_SSE4)

#include <smmintrin.h>

namespace eokas
{
    struct SSE4 {
        template<typename T>
        static constexpr size_t countOf()
        {
            return sizeof(__m128i) / sizeof(T);
        }
    };

    template<typename T>
    struct sse4_value_t
    {
        using type = __m128i;
    };

    template<>
    struct sse4_value_t<f32_t>
    {
        using type = __m128;
    };

    template<>
    struct sse4_value_t<f64_t>
    {
        using type = __m128d;
    };

    template<typename T, size_t N = SSE4::countOf<T>()>
    struct sse4_vector_t
    {
        using value_t = typename sse4_value_t<T>::type;

        value_t value;
    };

    using u8x16_t = sse4_vector_t<u8_t, 16>;
    using u16x8_t = sse4_vector_t<u16_t, 8>;
    using u32x4_t = sse4_vector_t<u32_t, 4>;
    using u64x2_t = sse4_vector_t<u64_t, 2>;
    using i8x16_t = sse4_vector_t<i8_t, 16>;
    using i16x8_t = sse4_vector_t<i16_t, 8>;
    using i32x4_t = sse4_vector_t<i32_t, 4>;
    using i64x2_t = sse4_vector_t<i64_t, 2>;
    using f32x4_t = sse4_vector_t<f32_t, 4>;
    using f64x2_t = sse4_vector_t<f64_t, 2>;

    using u8x8_t = sse4_vector_t<u8_t, 8>;
    using u16x4_t = sse4_vector_t<u16_t, 4>;
    using u32x2_t = sse4_vector_t<u32_t, 2>;
    using u64x1_t = sse4_vector_t<u64_t, 1>;
    using i8x8_t = sse4_vector_t<i8_t, 8>;
    using i16x4_t = sse4_vector_t<i16_t, 4>;
    using i32x2_t = sse4_vector_t<i32_t, 2>;
    using i64x1_t = sse4_vector_t<i64_t, 1>;
    using f32x2_t = sse4_vector_t<f32_t, 2>;
    using f64x1_t = sse4_vector_t<f64_t, 1>;

    using u8x4_t = sse4_vector_t<u8_t, 4>;
    using u16x2_t = sse4_vector_t<u16_t, 2>;
    using u32x1_t = sse4_vector_t<u32_t, 1>;
    using i8x4_t = sse4_vector_t<i8_t, 4>;
    using i16x2_t = sse4_vector_t<i16_t, 2>;
    using i32x1_t = sse4_vector_t<i32_t, 1>;
    using f32x1_t = sse4_vector_t<f32_t, 1>;
}

#elif (_EOKAS_SIMD & _EOKAS_SIMD_NEON)

#include <arm_neon.h>

namespace eokas
{
    struct NEON {
    template<typename T>
        static constexpr size_t countOf()
        {
            return sizeof(int8x16_t) / sizeof(T);
        }
    };

    template<typename T, size_t N>
    struct neon_value_t;

    template<>
    struct neon_value_t<u8_t, 16>
    {
        using type = uint8x16_t;
    };

    template<>
    struct neon_value_t<u16_t, 8>
    {
        using type = uint16x8_t;
    };

    template<>
    struct neon_value_t<u32_t, 4>
    {
        using type = uint32x4_t;
    };

    template<>
    struct neon_value_t<u64_t, 2>
    {
        using type = uint64x2_t;
    };

    template<>
    struct neon_value_t<i8_t, 16>
    {
        using type = int8x16_t;
    };

    template<>
    struct neon_value_t<i16_t, 8>
    {
        using type = int16x8_t;
    };

    template<>
    struct neon_value_t<i32_t, 4>
    {
        using type = int32x4_t;
    };

    template<>
    struct neon_value_t<i64_t, 2>
    {
        using type = int64x2_t;
    };

    template<>
    struct neon_value_t<f32_t, 4>
    {
        using type = float32x4_t;
    };

    template<>
    struct neon_value_t<f64_t, 2>
    {
        using type = float64x2_t;
    };

    template<typename T, size_t N = NEON::countOf<T>()>
    struct neon_vector_t
    {
        using value_t = typename neon_value_t<T, N>::type;

        value_t value;
    };

    using u8x16_t = neon_vector_t<u8_t, 16>;
    using u16x8_t = neon_vector_t<u16_t, 8>;
    using u32x4_t = neon_vector_t<u32_t, 4>;
    using u64x2_t = neon_vector_t<u64_t, 2>;
    using i8x16_t = neon_vector_t<i8_t, 16>;
    using i16x8_t = neon_vector_t<i16_t, 8>;
    using i32x4_t = neon_vector_t<i32_t, 4>;
    using i64x2_t = neon_vector_t<i64_t, 2>;
    using f32x4_t = neon_vector_t<f32_t, 4>;
    using f64x2_t = neon_vector_t<f64_t, 2>;

    using u8x8_t = neon_vector_t<u8_t, 8>;
    using u16x4_t = neon_vector_t<u16_t, 4>;
    using u32x2_t = neon_vector_t<u32_t, 2>;
    using u64x1_t = neon_vector_t<u64_t, 1>;
    using i8x8_t = neon_vector_t<i8_t, 8>;
    using i16x4_t = neon_vector_t<i16_t, 4>;
    using i32x2_t = neon_vector_t<i32_t, 2>;
    using i64x1_t = neon_vector_t<i64_t, 1>;
    using f32x2_t = neon_vector_t<f32_t, 2>;
    using f64x1_t = neon_vector_t<f64_t, 1>;

    using u8x4_t = neon_vector_t<u8_t, 4>;
    using u16x2_t = neon_vector_t<u16_t, 2>;
    using u32x1_t = neon_vector_t<u32_t, 1>;
    using i8x4_t = neon_vector_t<i8_t, 4>;
    using i16x2_t = neon_vector_t<i16_t, 2>;
    using i32x1_t = neon_vector_t<i32_t, 1>;
    using f32x1_t = neon_vector_t<f32_t, 1>;
}

#else
    #error "Unsupport SIMD platform."
#endif

#endif //_EOKAS_BASE_SIMD_H_
