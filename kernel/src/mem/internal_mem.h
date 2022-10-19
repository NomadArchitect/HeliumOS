#ifndef HELIUM_INT_MEM_H
#define HELIUM_INT_MEM_H

#include <stddef.h>
#include <math.h>

#include <arch/mem.h>


// TODO: find optimal value
// Threshold, if a virtual mapping needs to modify more than this much pages
// then reloading cr3 is preseffered over invlpg (unless the paging is global)
#define RLCR3_THRESHOLD (32)

#define BITMAP_SIZE(seg_size) (ALIGN_UP((seg_size) / MEM_PS, 64) / 8)

#define ADR_MASK(n)  (uintptr_t) ((uintptr_t) 0x1FF << (12 + 9 * (n)))
#define ADR_SHIFT(n) (size_t) (12 + 9 * (n))

#define ENTRY_IDX(n, ptr) (size_t) (((uintptr_t) (ptr) & ADR_MASK(n)) >> ADR_SHIFT(n))

// TODO: when porting to other architectures, this should be considered
#define ORDER_COUNT (4)
#define MAX_ORDER   (ORDER_COUNT - 1)


// TODO: calculate at compile time if possible
#define ORDER_PS(n) (size_t)(4096 * powi(512, (n)))

// Kernel virtual space
#define KVMSPACE ((void*)(0xFFFF800000000000)) // not to be mistaken with linux's kvm, this is Kernel Virtual Memory

extern void* i_pmm_header;
extern size_t i_mmap_usable_len;
extern mem_vpstruct_ptr* i_pmlmax;

extern size_t i_order_ps[ORDER_COUNT];

#endif