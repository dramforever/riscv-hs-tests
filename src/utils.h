#ifndef _UTILS_H_
#define _UTILS_H_

#define FIELD(val, mask) (((val) & (mask)) / ((mask) & -(mask)))

#define csr_read(name)                                          \
	({                                                      \
		unsigned long _val;                             \
		asm volatile("csrr %0, " #name : "=r"(_val)::); \
		_val;                                           \
	})

#define csr_write(name, val)                                        \
	({                                                          \
		unsigned long _val = (val);                         \
		asm volatile("csrw " #name ", %0" : : "r"(_val) :); \
	})

void panic(const char *format, ...);

extern char _image_start[], _image_end[];

extern char playground[];

#endif
