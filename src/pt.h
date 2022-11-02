#ifndef _PT_H_
#define _PT_H_

#include "riscv_encodings.h"
#include <stdint.h>

typedef unsigned long pte_t;
typedef uint64_t addr_t;

extern pte_t gpt_root[];
extern pte_t spt_root[];
extern pte_t vspt_root[];
extern pte_t pt_alloc[];

extern pte_t *pt_alloc_top;

void reset_pt(void);
void init_gpt(void);
void init_spt(void);
void init_vspt(void);

#endif
