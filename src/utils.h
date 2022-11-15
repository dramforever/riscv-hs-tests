/**
 * @file utils.h
 * @brief Random helpers
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stddef.h>

/**
 * Extract bitfield from value with mask. Requires field bits to be consecutive.
 *
 * @param val Value to extract bitfield from
 * @param mask Bitmask of field to extract
 */
#define FIELD(val, mask) (((val) & (mask)) / ((mask) & -(mask)))

/**
 * Read CSR by name
 */
#define csr_read(name)                                          \
	({                                                      \
		unsigned long _val;                             \
		asm volatile("csrr %0, " #name : "=r"(_val)::); \
		_val;                                           \
	})

/**
 * Write CSR by name
 */
#define csr_write(name, val)                                        \
	({                                                          \
		unsigned long _val = (val);                         \
		asm volatile("csrw " #name ", %0" : : "r"(_val) :); \
	})

/**
 * Print message and hang
 *
 * @param format Format string for panic message
 */
void panic(const char *format, ...);

extern char _image_start[], _image_end[];

extern char playground[];

void *memcpy(char *dst, const char *src, size_t num);

extern char stack1[], stack2[], stack3[];

/**
 * Use with predefined @c stackN variables like @c STACK(stack1)
 */
#define STACK(buf) ((void *)((char *)buf + 4096))

#endif
