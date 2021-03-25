#ifndef ABOV_COMPILER_H
#define ABOV_COMPILER_H

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(UNITTEST)
#define STATIC
#else
#define STATIC				static
#endif

#define unused(x)			(void)(x)

#define ABOV_UNUSED			__attribute__((unused))
#define ABOV_USED			__attribute__((used))
#define ABOV_ALWAYS_INLINE		__attribute__((always_inline))
#define ABOV_WEAK			__attribute__((weak))
#define ABOV_NORETURN			__attribute__((noreturn))
#define ABOV_PACKED			__attribute__((packed))
#define ABOV_SECTION(x)			__attribute__((section(x)))
#define ABOV_ALIAS(x)			__attribute__((alias(x)))

#define ABOV_STATIC_ASSERT(exp, msg)	_Static_assert(exp, msg)

#define barrier()			__asm__ __volatile__("" ::: "memory")
#define ACCESS_ONCE(x)			(*(volatile __typeof__(x) *)&(x))
#define WRITE_ONCE(ptr, val)
#define READ_ONCE(x)

#define NO_OPTIMIZE			__attribute__((optimize("O0")))

#define CONST_CAST(t, v)		\
	(((union { const t cval; t val; }*)&(v))->val)

#define stringify(x)			#x
#define def2str(x)			stringify(x)

/** Align down */
#define BASE(x, unit)			((x) & ~((__typeof__(x))(unit) - 1UL))
/** Align up */
#define ALIGN(x, unit)			BASE((x) + ((__typeof__(x))(unit) - 1UL), unit)

#define container_of(ptr, type, member) \
	((type *)(void *)((char *)(ptr) - offsetof(type, member)))

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_COMPILER_H */
