
#ifndef _EOKAS_BASE_SIMD_H_
#define _EOKAS_BASE_SIMD_H_

#include "header.h"

/**
SSE/AVX 提供的数据类型和函数的命名规则：

1. 数据类型通常以 _mxxx(T) 的方式进行命名，其中
    xxx 代表数据的位数，如 SSE 提供的 __m128 为 128 位， AVX 提供的 __m256 为 256 位。
    T 为类型，若为单精度浮点型则省略，若为整形则为 i，如 __m128i，若为双精度浮点型则为 d，如 __m256d。
2. 操作浮点数的内置函数命名方式为： _mm(xxx)_name_PT。
    xxx 为 SIMD 寄存器的位数，若为 __m128 则省略，如 _mm_add_ps，若为 __m256 则为 256，如 _mm256_add_ps。
    name 为函数执行的操作的名字，如加法为 _mm_add_ps，减法为 _mm_sub_ps。
    P 代表的是对矢量 (packed data vector) 还是对标量 (scalar) 进行操作，如 _mm_add_ss 是只对最低位的 32 位浮点数执行加法，而 _mm_add_ps 则是对 4 个32位浮点数执行加法操作。
    T 代表浮点数的类型，若为 s 则为单精度浮点型，若为 d 则为双精度浮点，如 _mm_add_pd 和 _mm_add_ps。
3. 操作整形的内置函数命名方式为： _mm(xxx)_name_epUY。
    xxx 为S IMD 寄存器的位数，若为 128 位则省略。
    name为函数的名字。
    U 为整数的类型，若为无符号类型则为 u，否在为 i，如 _mm_adds_epu16 和 _mm_adds_epi16。
    Y 为操作的数据类型的位数，如 _mm_cvtpd_pi32。
*/

#if ((_EOKAS_SIMD & _EOKAS_SIMD_AVX2) || (_EOKAS_SIMD & _EOKAS_SIMD_SSE4))

#if (_EOKAS_SIMD & _EOKAS_SIMD_AVX2)
#include <immintrin.h>

namespace eokas {
    struct AVX2 {
        template<typename T>
        static constexpr size_t countOf() {
            return sizeof(__m256i) / sizeof(T);
        }
    };

    template<typename T>
    struct avx2_value_t {
        using type = __m256i;
    };
    template<size_t N>
    struct avx2_part_t;


    template<>
    struct avx2_value_t<f32_t> {
        using type = __m256;
    };

    template<>
    struct avx2_value_t<f64_t> {
        using type = __m256d;
    };

    template<typename T, size_t N = AVX2::countOf<T>()>
    struct avx2_vector_t {
        using value_t = typename avx2_value_t<T>::type;

        value_t value;

        avx2_vector_t(value_t v) : value(v) {}
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

    u8x32_t simd_load(u8_t(&data)[32]) {
        return _mm256_loadu_epi8(data);
    }
    u16x16_t simd_load(u16_t(&data)[16]) {
        return _mm256_loadu_epi16(data);
    }
    u32x8_t simd_load(u32_t(&data)[8]){
        return _mm256_loadu_epi32(data);
    }
    u64x4_t simd_load(u64_t(&data)[4]) {
        return _mm256_loadu_epi64(data);
    }
    i8x32_t simd_load(i8_t(&data)[32]) {
        return _mm256_loadu_epi8(data);
    }
    i16x16_t simd_load(i16_t(&data)[16]) {
        return _mm256_loadu_epi16(data);
    }
    i32x8_t simd_load(i32_t(&data)[8]){
        return _mm256_loadu_epi32(data);
    }
    i64x4_t simd_load(i64_t(&data)[4]) {
        return _mm256_loadu_epi64(data);
    }
    f32x8_t simd_load(f32_t(&data)[8]) {
        return _mm256_loadu_ps(data);
    }
    f64x4_t simd_load(f64_t(&data)[4]) {
        return _mm256_loadu_pd(data);
    }

    template<size_t N>
    avx2_vector_t<u8_t, N> simd_abs(const avx2_vector_t<u8_t, N>& a)
    {
        return  _mm256_abs_epi8(a.value);
    }
    template<size_t N>
    avx2_vector_t<u16_t, N> simd_abs(const avx2_vector_t<u16_t, N>& a)
    {
        return  _mm256_abs_epi16(a.value);
    }
    template<size_t N>
    avx2_vector_t<u32_t, N> simd_abs(const avx2_vector_t<u32_t, N>& a)
    {
        return  _mm256_abs_epi32(a.value);
    }
    template<size_t N>
    avx2_vector_t<u64_t, N> simd_abs(const avx2_vector_t<u64_t, N>& a)
    {
        return  _mm256_abs_epi64(a.value);
    }
    template<size_t N>
    avx2_vector_t<i8_t, N> simd_abs(const avx2_vector_t<i8_t, N>& a)
    {
        return  _mm256_abs_epi8(a.value);
    }
    template<size_t N>
    avx2_vector_t<i16_t, N> simd_abs(const avx2_vector_t<i16_t, N>& a)
    {
        return  _mm256_abs_epi16(a.value);
    }
    template<size_t N>
    avx2_vector_t<i32_t, N> simd_abs(const avx2_vector_t<i32_t, N>& a)
    {
        return  _mm256_abs_epi32(a.value);
    }
    template<size_t N>
    avx2_vector_t<i64_t, N> simd_abs(const avx2_vector_t<i64_t, N>& a)
    {
        return  _mm256_abs_epi64(a.value);
    }


    template<size_t N>
    avx2_vector_t<f32_t, N> simd_sqrt(const avx2_vector_t<f32_t, N>& a)
    {
        return  _mm256_sqrt_ps(a.value);
    }
    template<size_t N>
    avx2_vector_t<f64_t, N> simd_sqrt(const avx2_vector_t<f64_t, N>& a)
    {
        return  _mm256_sqrt_pd(a.value);
    }

    template<size_t N>
    avx2_vector_t<u16_t, N> simd_abs(const avx2_vector_t<u16_t, N>& a)
    {
        return  _mm256_abs_epi16(a.value);
    }
    template<size_t N>
    avx2_vector_t<u32_t, N> simd_abs(const avx2_vector_t<u32_t, N>& a)
    {
        return  _mm256_abs_epi32(a.value);
    }
    template<size_t N>
    avx2_vector_t<u64_t, N> simd_abs(const avx2_vector_t<u64_t, N>& a)
    {
        return  _mm256_abs_epi64(a.value);
    }
    template<size_t N>
    avx2_vector_t<i8_t, N> simd_abs(const avx2_vector_t<i8_t, N>& a)
    {
        return  _mm256_abs_epi8(a.value);
    }
    template<size_t N>
    avx2_vector_t<i16_t, N> simd_abs(const avx2_vector_t<i16_t, N>& a)
    {
        return  _mm256_abs_epi16(a.value);
    }
    template<size_t N>
    avx2_vector_t<i32_t, N> simd_abs(const avx2_vector_t<i32_t, N>& a)
    {
        return  _mm256_abs_epi32(a.value);
    }
    template<size_t N>
    avx2_vector_t<i64_t, N> simd_abs(const avx2_vector_t<i64_t, N>& a)
    {
        return  _mm256_abs_epi64(a.value);
    }
    template<size_t N>
    avx2_vector_t<f32_t, N> simd_abs(const avx2_vector_t<f32_t, N>& a)
    {
        return  _mm256_abs_ps(a.value);
    }
    template<size_t N>
    avx2_vector_t<f64_t, N> simd_abs(const avx2_vector_t<f64_t, N>& a)
    {
        return  _mm256_abs_pd(a.value);
    }


    template<size_t N>
    avx2_vector_t<u8_t, N> operator+(const avx2_vector_t<u8_t, N>& a, const avx2_vector_t<u8_t, N>& b)
    {
        return  _mm256_add_epi8(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<u16_t, N> operator+(const avx2_vector_t<u16_t, N>& a, const avx2_vector_t<u16_t, N>& b)
    {
        return  _mm256_add_epi16(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<u32_t, N> operator+(const avx2_vector_t<u32_t, N>& a, const avx2_vector_t<u32_t, N>& b)
    {
        return  _mm256_add_epi32(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<u64_t, N> operator+(const avx2_vector_t<u64_t, N>& a, const avx2_vector_t<u64_t, N>& b)
    {
        return  _mm256_add_epi64(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i8_t, N> operator+(const avx2_vector_t<i8_t, N>& a, const avx2_vector_t<i8_t, N>& b)
    {
        return  _mm256_add_epi8(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i16_t, N> operator+(const avx2_vector_t<i16_t, N>& a, const avx2_vector_t<i16_t, N>& b)
    {
        return  _mm256_add_epi16(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i32_t, N> operator+(const avx2_vector_t<i32_t, N>& a, const avx2_vector_t<i32_t, N>& b)
    {
        return  _mm256_add_epi32(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i64_t, N> operator+(const avx2_vector_t<i64_t, N>& a, const avx2_vector_t<i64_t, N>& b)
    {
        return  _mm256_add_epi64(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<f32_t, N> operator+(const avx2_vector_t<f32_t, N>& a, const avx2_vector_t<f32_t, N>& b)
    {
        return  _mm256_add_ps(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<f64_t, N> operator+(const avx2_vector_t<f64_t, N>& a, const avx2_vector_t<f64_t, N>& b)
    {
        return  _mm256_add_pd(a.value, b.value);
    }


    template<size_t N>
    avx2_vector_t<u8_t, N> operator-(const avx2_vector_t<u8_t, N>& a, const avx2_vector_t<u8_t, N>& b)
    {
        return  _mm256_sub_epi8(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<u16_t, N> operator-(const avx2_vector_t<u16_t, N>& a, const avx2_vector_t<u16_t, N>& b)
    {
        return  _mm256_sub_epi16(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<u32_t, N> operator-(const avx2_vector_t<u32_t, N>& a, const avx2_vector_t<u32_t, N>& b)
    {
        return  _mm256_sub_epi32(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<u64_t, N> operator-(const avx2_vector_t<u64_t, N>& a, const avx2_vector_t<u64_t, N>& b)
    {
        return  _mm256_sub_epi64(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i8_t, N> operator-(const avx2_vector_t<i8_t, N>& a, const avx2_vector_t<i8_t, N>& b)
    {
        return  _mm256_sub_epi8(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i16_t, N> operator-(const avx2_vector_t<i16_t, N>& a, const avx2_vector_t<i16_t, N>& b)
    {
        return  _mm256_sub_epi16(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i32_t, N> operator-(const avx2_vector_t<i32_t, N>& a, const avx2_vector_t<i32_t, N>& b)
    {
        return  _mm256_sub_epi32(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i64_t, N> operator-(const avx2_vector_t<i64_t, N>& a, const avx2_vector_t<i64_t, N>& b)
    {
        return  _mm256_sub_epi64(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<f32_t, N> operator-(const avx2_vector_t<f32_t, N>& a, const avx2_vector_t<f32_t, N>& b)
    {
        return  _mm256_sub_ps(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<f64_t, N> operator-(const avx2_vector_t<f64_t, N>& a, const avx2_vector_t<f64_t, N>& b)
    {
        return  _mm256_sub_pd(a.value, b.value);
    }


    template<size_t N>
    avx2_vector_t<u8_t, N> operator*(const avx2_vector_t<u8_t, N>& a, const avx2_vector_t<u8_t, N>& b)
    {
        return  _mm256_mul_epi8(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<u16_t, N> operator*(const avx2_vector_t<u16_t, N>& a, const avx2_vector_t<u16_t, N>& b)
    {
        return  _mm256_mul_epi16(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<u32_t, N> operator*(const avx2_vector_t<u32_t, N>& a, const avx2_vector_t<u32_t, N>& b)
    {
        return  _mm256_mul_epi32(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<u64_t, N> operator*(const avx2_vector_t<u64_t, N>& a, const avx2_vector_t<u64_t, N>& b)
    {
        return  _mm256_mul_epi64(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i8_t, N> operator*(const avx2_vector_t<i8_t, N>& a, const avx2_vector_t<i8_t, N>& b)
    {
        return  _mm256_mul_epi8(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i16_t, N> operator*(const avx2_vector_t<i16_t, N>& a, const avx2_vector_t<i16_t, N>& b)
    {
        return  _mm256_mul_epi16(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i32_t, N> operator*(const avx2_vector_t<i32_t, N>& a, const avx2_vector_t<i32_t, N>& b)
    {
        return  _mm256_mul_epi32(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i64_t, N> operator*(const avx2_vector_t<i64_t, N>& a, const avx2_vector_t<i64_t, N>& b)
    {
        return  _mm256_mul_epi64(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<f32_t, N> operator*(const avx2_vector_t<f32_t, N>& a, const avx2_vector_t<f32_t, N>& b)
    {
        return  _mm256_mul_ps(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<f64_t, N> operator*(const avx2_vector_t<f64_t, N>& a, const avx2_vector_t<f64_t, N>& b)
    {
        return  _mm256_mul_pd(a.value, b.value);
    }


    template<size_t N>
    avx2_vector_t<u8_t, N> operator/(const avx2_vector_t<u8_t, N>& a, const avx2_vector_t<u8_t, N>& b)
    {
        return  _mm256_div_epi8(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<u16_t, N> operator/(const avx2_vector_t<u16_t, N>& a, const avx2_vector_t<u16_t, N>& b)
    {
        return  _mm256_div_epi16(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<u32_t, N> operator/(const avx2_vector_t<u32_t, N>& a, const avx2_vector_t<u32_t, N>& b)
    {
        return  _mm256_div_epi32(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<u64_t, N> operator/(const avx2_vector_t<u64_t, N>& a, const avx2_vector_t<u64_t, N>& b)
    {
        return  _mm256_div_epi64(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i8_t, N> operator/(const avx2_vector_t<i8_t, N>& a, const avx2_vector_t<i8_t, N>& b)
    {
        return  _mm256_div_epi8(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i16_t, N> operator/(const avx2_vector_t<i16_t, N>& a, const avx2_vector_t<i16_t, N>& b)
    {
        return  _mm256_div_epi16(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i32_t, N> operator/(const avx2_vector_t<i32_t, N>& a, const avx2_vector_t<i32_t, N>& b)
    {
        return  _mm256_div_epi32(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<i64_t, N> operator/(const avx2_vector_t<i64_t, N>& a, const avx2_vector_t<i64_t, N>& b)
    {
        return  _mm256_div_epi64(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<f32_t, N> operator/(const avx2_vector_t<f32_t, N>& a, const avx2_vector_t<f32_t, N>& b)
    {
        return  _mm256_div_ps(a.value, b.value);
    }
    template<size_t N>
    avx2_vector_t<f64_t, N> operator/(const avx2_vector_t<f64_t, N>& a, const avx2_vector_t<f64_t, N>& b)
    {
        return  _mm256_div_pd(a.value, b.value);
    }

}
#endif

#if (_EOKAS_SIMD & _EOKAS_SIMD_SSE4)

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
#endif

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
