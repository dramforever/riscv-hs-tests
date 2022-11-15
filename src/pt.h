/**
 * @file pt.h
 * @brief Page table management helpers
 */

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

/**
 * Reset all virtual memory to Bare mode and deallocate all page table nodes.
 */
void reset_pt(void);

/**
 * Initialize guest page table @c hgtap to a page table with identity mappings for the entire test binary.
 */
void init_gpt(void);

/**
 * Initialize S-mode page table @c satp to a page table with identity mappings for the entire test binary.
 */
void init_spt(void);

/**
 * Initialize VS-mode page table @c vsatp to a page table with identity mappings for the entire test binary.
 */
void init_vspt(void);

/**
 * Map a single page in guest page table @c hgatp
 *
 * @param va Guest physical address of page
 * @param pa Physical address of page
 * @param prot Flags for this page
 */
void map_gpt(unsigned long va, addr_t pa, pte_t prot);

/**
 * Map a single page in S-mode page table @c satp
 *
 * @param va Virtual address of page
 * @param pa Physical address of page
 * @param prot Flags for this page
 */
void map_spt(unsigned long va, addr_t pa, pte_t prot);

/**
 * Map a single page in VS-mode page table @c vsatp
 *
 * @param va Guest virtual address of page
 * @param pa Guest physical address of page
 * @param prot Flags for this page
 */
void map_vspt(unsigned long va, addr_t pa, pte_t prot);

#endif
