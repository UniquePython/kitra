#ifndef CINDER_COMPILER_H_
#define CINDER_COMPILER_H_

// ======================================= COMPILER DETECTION =======================================

#if defined(__clang__)
#define CINDER_COMPILER_CLANG 1
#elif defined(__GNUC__)
#define CINDER_COMPILER_GCC 1
#elif defined(_MSC_VER)
#define CINDER_COMPILER_MSVC 1
#else
#define CINDER_COMPILER_UNKNOWN 1
#endif

// ======================================= ATTRIBUTES ===============================================

// Warn if return value is ignored
#if defined(CINDER_COMPILER_CLANG) || defined(CINDER_COMPILER_GCC)
#define CINDER_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define CINDER_WARN_UNUSED_RESULT
#endif

// Mark function as non-null parameters
#if defined(CINDER_COMPILER_CLANG) || defined(CINDER_COMPILER_GCC)
#define CINDER_NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))
#else
#define CINDER_NONNULL(...)
#endif

// Branch prediction hints
#if defined(CINDER_COMPILER_CLANG) || defined(CINDER_COMPILER_GCC)
#define CINDER_LIKELY(x) __builtin_expect(!!(x), 1)
#define CINDER_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define CINDER_LIKELY(x) (x)
#define CINDER_UNLIKELY(x) (x)
#endif

// Deprecation warning with message
#if defined(CINDER_COMPILER_CLANG) || defined(CINDER_COMPILER_GCC)
#define CINDER_DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(CINDER_COMPILER_MSVC)
#define CINDER_DEPRECATED(msg) __declspec(deprecated(msg))
#else
#define CINDER_DEPRECATED(msg)
#endif

// ======================================= INLINE CONTROL ===========================================

#if defined(CINDER_COMPILER_GCC) || defined(CINDER_COMPILER_CLANG)
#define CINDER_INLINE static inline __attribute__((always_inline))
#else
#define CINDER_INLINE static inline
#endif

#endif /* CINDER_COMPILER_H_ */