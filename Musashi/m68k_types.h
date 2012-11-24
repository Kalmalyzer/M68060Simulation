#ifndef M68KTYPES_H 
#define M68KTYPES_H  

typedef unsigned int uint;

#if defined(_WIN32)

#if defined(__GNUC__)
#include <stdint.h>
#else

typedef unsigned char uint8_t;
typedef signed char int8_t;

typedef unsigned short uint16_t;
typedef signed short int16_t;

typedef unsigned int uint32_t;
typedef signed int int32_t;

typedef unsigned __int64 uint64_t;
typedef signed __int64 int64_t;

#pragma warning(disable: 4100 4127 4201 4706)
#endif

#define M68K_LIKELY(exp) exp 
#define M68K_UNLIKELY(exp) exp 
#define M68K_INLINE __forceinline
#define M68K_RESTRICT __restrict
#define M68K_ALIGN(x) __declspec(align(x))
#define M68K_ALIGNOF(t) __alignof(t)
#define M68K_BREAK __debugbreak()

#elif defined(__APPLE__)

#include <stdint.h>
#include <stdbool.h>

#define M68K_LIKELY(exp) __builtin_expect(exp, 1) 
#define M68K_UNLIKELY(exp) __builtin_expect(exp, 0) 
#define M68K_INLINE inline
#define M68K_RESTRICT __restrict
#define M68K_ALIGN(x) __attribute__((aligned(x)))
#define M68K_ALIGNOF(t) __alignof__(t)
#define M68K_BREAK ((*(volatile uint32_t *)(0)) = 0x666)

#ifndef true
#define true	1
#endif

#ifndef false
#define false	0
#endif

#endif

#define sizeof_array(array) (int)(sizeof(array) / sizeof(array[0]))

#endif

