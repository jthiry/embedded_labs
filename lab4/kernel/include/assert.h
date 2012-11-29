/**
 * @file assert.h
 *
 * @brief Assertion and debugging infrastructure.
 *
 * @author Kartik Subramanian
 */

#ifndef _ASSERT_H_
#define _ASSERT_H_

/*~~~~~~~~~~~~~~~~ pulled over from lab3, dunno if it's needed ~~~~~~~~*/
#include <exports.h>
struct context {
	uint32_t r8_fiq, r9_fiq, r10_fiq, r11_fiq, r12_fiq;
	uint32_t lr, sp, cpsr;
	uint32_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, lr_abt;
};

void register_dump(struct context *reg_context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef NDEBUG

#define assert(e) ((void)0)
#define crash

#else

#include <types.h>

void panic(const char* fmt, ...) __attribute__((noreturn, format (printf, 1, 2)));
void hexdump(void* buf, size_t len);

#define crash \
	((void)(*(volatile char*)0xdeadbeef))

#include <exports.h>
#define assert(e) \
	((void)((e) ? 0 : (printf("Assertion Failure at %s:%d\n%s", __FILE__, \
		__LINE__, #e), panic(" "), 0)))
/*#define assert(e) \
	((void)((e) ? 0 : (panic("Assertion Failure at %s:%d\n%s", __FILE__, \
		__LINE__, #e), 0))) */
#endif

#endif /* _ASSERT_H_ */
