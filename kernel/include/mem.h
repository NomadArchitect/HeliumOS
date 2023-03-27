#ifndef HELIUM_MEM_H
#define HELIUM_MEM_H

#include <attributes.h>
#include <stdbool.h>
#include <stddef.h>
#include <error.h>

#include <arch/mem.h>

#define MEM_PSEG_MAGIC (0xA55AA55AA55AA55A)

struct MEM_PSEG_HEADER;
typedef struct MEM_PSEG_HEADER mem_pseg_header;
struct MEM_PSEG_HEADER
{
  size_t magic;
  void *padr;
  size_t size;
} pack;

struct MEM_PALLOCATION;
typedef struct MEM_PALLOCATION mem_pallocation;
struct MEM_PALLOCATION
{
  void *padr;
  size_t header_off;
  size_t size;
  errno_t error;
};

void mem_init();

/* mem_p* */

#define PALLOC_STD_HEADER ((void *) UINTPTR_MAX)
mem_pallocation mem_ppalloc(
  void *pheader,
  size_t size,
  size_t alignement,
  bool cont,
  void *below
);
void mem_ppfree(void *pheader, mem_pallocation alloc);

/* mem_v* */
// ERR_MEM memory operations errors
#define ERR_MEM_ALN          (-1) /* Alignment error */
#define ERR_MEM_NO_PHY_SPACE (-2) /* No physical space */
#define ERR_MEM_NULL_SIZE    (-3) /* Allocation/mapping of size 0 */
#define ERR_MEM_INV_VADR     (-4) /* Invalid virtual address(eg. non canonical
                                     address on systems that require it) */
#define ERR_MEM_MANAGED      (-5) /* Memory area to be mapped is managed by
                                     another kernel system(eg; vcache) */
#define ERR_MEM_NO_VC_SPACE  (-6) /* Couldn't allocate a VCache page */

errno_t mem_vmap(void *vadr, void *padr, size_t size, int flags);
errno_t mem_vumap(void *vadr, size_t size);

// Kernel virtual space
#define KVMSPACE ((void *)(0xFFFF800000000000)) // not to be mistaken with
                                                // linux's kvm, this is
                                                // Kernel Virtual Memory
#define PHEADER_VPTR ((void *) (0xFFFF804000000000)) // Virtual address of
                                                     // physical memory header
// KHEAP is 512 Gib in size, 1 PML4 page
#define KHEAP    ((void *)(0xFFFF808000000000)) // Kernel heap

// MAPF memory mapping flags
#define MAPF_R   (1<<0) /* Read */
#define MAPF_W   (1<<1) /* Write */
#define MAPF_X   (1<<2) /* Execute */

#define MAPF_U   (1<<3) /* User access */

#define MAPF_P2M (1<<4) /* Map using 2 Mib page entries */
#define MAPF_P1G (1<<5) /* Map using 1 Gib page entries */

#define MAPF_G   (1<<6) /* Global page */
#endif