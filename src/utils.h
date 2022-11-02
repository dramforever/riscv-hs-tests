#ifndef _UTILS_H_
#define _UTILS_H_

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

#endif
