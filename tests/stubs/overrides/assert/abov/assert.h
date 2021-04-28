#ifndef ASSERT_OVERRIDE_H
#define ASSERT_OVERRIDE_H

#if defined(__cplusplus)
extern "C" {
#endif

#undef assert
#define assert(exp)		if (!(exp)) assert_override()

void assert_override(void);

#if defined(__cplusplus)
}
#endif

#endif /* ASSERT_OVERRIDE_H */
