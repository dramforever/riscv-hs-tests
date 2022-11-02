#include <stddef.h>

#include "printf.h"
#include "pt.h"
#include "utils.h"
#include <stdbool.h>

__attribute__((section(".data.pagesize4")))
pte_t gpt_root[PAGE_SIZE * 4 / sizeof(pte_t)];

__attribute__((section(".data.pagesize")))
pte_t spt_root[PAGE_SIZE / sizeof(pte_t)];

__attribute__((section(".data.pagesize")))
pte_t vspt_root[PAGE_SIZE / sizeof(pte_t)];

__attribute__((section(".data.pagesize")))
pte_t pt_alloc[64 * PAGE_SIZE / sizeof(pte_t)];

pte_t *pt_alloc_top;

struct ptw_mode {
	unsigned long mode;
	bool addr_signed;
	char parts[8];
};

struct ptw_mode ptw_sv39   = { .mode	    = 8,
			       .addr_signed = 1,
			       .parts	    = { 12, 9, 9, 9, 0 } };
struct ptw_mode ptw_sv39x4 = { .mode	    = 8,
			       .addr_signed = 0,
			       .parts	    = { 12, 9, 9, 11, 0 } };

static void clear_pt_node(pte_t *node, size_t num)
{
	for (size_t i = 0; i < num; i++)
		node[i] = 0;
}

void reset_pt()
{
	csr_write(hgatp, 0);
	csr_write(vsatp, 0);
	csr_write(satp, 0);

	asm volatile("sfence.vma");
	asm volatile("hfence.gvma");

	clear_pt_node(gpt_root, sizeof(gpt_root) / sizeof(gpt_root[0]));
	clear_pt_node(spt_root, sizeof(spt_root) / sizeof(spt_root[0]));
	clear_pt_node(vspt_root, sizeof(vspt_root) / sizeof(vspt_root[0]));

	pt_alloc_top = pt_alloc;
}

pte_t *alloc_node()
{
	if (pt_alloc_top == pt_alloc + sizeof(pt_alloc) / sizeof(pt_alloc[0])) {
		panic("Ran out of pt nodes");
	}

	pte_t *res = pt_alloc_top;
	pt_alloc_top += PAGE_SIZE / sizeof(pte_t);
	clear_pt_node(res, PAGE_SIZE / sizeof(pte_t));
	return res;
}

static inline bool addr_valid(addr_t addr, const struct ptw_mode *mode,
			      int va_bits)
{
	if (mode->addr_signed) {
		int64_t a = ((int64_t)addr) >> (va_bits - 1);
		return a == 0 || a == -1;
	} else {
		return (addr >> va_bits) == 0;
	}
}

static void map_page(pte_t *root, const struct ptw_mode *mode, addr_t addr,
		     pte_t leaf_pte)
{
	int num_levels = 0, va_bits = 0;
	int level, shift;
	addr_t addr_part, mask;
	pte_t *node = root, *pte, *new_node;

	while (mode->parts[num_levels]) {
		va_bits += mode->parts[num_levels];
		num_levels++;
	}
	shift = va_bits;

	if (!addr_valid(addr, mode, va_bits)) {
		panic("Invalid va 0x%llx", addr);
	}

	for (level = num_levels - 1; level >= 1; level--) {
		shift -= mode->parts[level];

		mask	  = (1UL << mode->parts[level]) - 1;
		addr_part = (addr >> shift) & mask;

		pte = (pte_t *)(node + addr_part);

		if (level > 1) {
			if (!(*pte & PTE_V)) {
				new_node = alloc_node();

				*pte = PTE_V |
				       ((pte_t)((addr_t)new_node >> PAGE_SHIFT)
					<< PTE_PPN_SHIFT);
			}

			node = (pte_t *)(((*pte >> PTE_PPN_SHIFT) &
					  PTE_PPN_MASK)
					 << PAGE_SHIFT);
		} else {
			*pte = leaf_pte;
		}
	}
}

static void identity_map(pte_t *root, const struct ptw_mode *mode, pte_t prot)
{

	for (char *addr = _image_start; addr < _image_end; addr += PAGE_SIZE)
		map_page(root, mode, (addr_t)addr,
			 prot | (pte_t)((addr_t)addr >> PAGE_SHIFT)
					 << PTE_PPN_SHIFT);
}

void init_gpt(void)
{
	pte_t prot = PTE_V | PTE_R | PTE_W | PTE_X | PTE_U | PTE_A | PTE_D;
	identity_map(gpt_root, &ptw_sv39x4, prot);
	csr_write(hgatp, ptw_sv39x4.mode << HGATP_MODE_SHIFT |
				 ((pte_t)((addr_t)gpt_root >> PAGE_SHIFT)));
	asm volatile("hfence.gvma");
}

void init_spt(void)
{
	pte_t prot = PTE_V | PTE_R | PTE_W | PTE_X | PTE_A | PTE_D;
	identity_map(spt_root, &ptw_sv39, prot);
	csr_write(satp, ptw_sv39.mode << SATP_MODE_SHIFT |
				((pte_t)((addr_t)spt_root >> PAGE_SHIFT)));
	asm volatile("sfence.vma");
}

void init_vspt(void)
{
	pte_t prot = PTE_V | PTE_R | PTE_W | PTE_X | PTE_A | PTE_D;
	identity_map(vspt_root, &ptw_sv39, prot);
	csr_write(vsatp, ptw_sv39.mode << SATP_MODE_SHIFT |
				 ((pte_t)((addr_t)vspt_root >> PAGE_SHIFT)));
	asm volatile("hfence.vvma");
}

void map_gpt(unsigned long va, addr_t pa, pte_t prot)
{
	map_page(gpt_root, &ptw_sv39x4, va,
		 pa >> PAGE_SHIFT << PTE_PPN_SHIFT | prot);
	asm volatile("hfence.gvma");
}

void map_spt(unsigned long va, addr_t pa, pte_t prot)
{
	map_page(spt_root, &ptw_sv39, va,
		 pa >> PAGE_SHIFT << PTE_PPN_SHIFT | prot);
	asm volatile("sfence.vma");
}

void map_vspt(unsigned long va, addr_t pa, pte_t prot)
{
	map_page(vspt_root, &ptw_sv39, va,
		 pa >> PAGE_SHIFT << PTE_PPN_SHIFT | prot);
	asm volatile("hfence.vvma");
}
