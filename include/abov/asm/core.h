#ifndef ABOV_ASM_CORE_H
#define ABOV_ASM_CORE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#if defined(armv7m3)
#define abov_get_pc_core() __extension__ \
	({ \
		uintptr_t mypc; \
		__asm__ __volatile__("mov %0, pc" : "=r"(mypc)); \
		mypc; \
	})
#define abov_raise_trap_core(id) \
	__asm__ __volatile__("bkpt %0" :: "I"(id) : "cc", "memory")
#else
#error "unknown cpu architecture"
#endif

#define abov_get_pc()			abov_get_pc_core()
#define abov_get_lr()			__builtin_return_address(0)
#define abov_raise_trap(id)		abov_raise_trap_core(id)

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_ASM_CORE_H */
