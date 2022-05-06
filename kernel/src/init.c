#include <boot_info.h>
#include <string.h>
#include <smbios.h>
#include <stdio.h>
#include <debug.h>

static void print_info();

int kmain()
{
       printf("%y\n");
       print_info();

       return 0;
}
static void print_info()
{
       printf("BOOTBOOT signature     : %10.4s        \n", bootboot.magic   );
       printf("BOOTBOOT struct size   : %10d          \n", bootboot.size    );
       printf("BOOTBOOT protocol      : %10d          \n", bootboot.protocol);
       printf("Number of cores        : %10d          \n", bootboot.numcores);
       printf("BSPID                  : %10d          \n", bootboot.bspid   );
       printf("Timezone               : %+9d          \n", bootboot.timezone);
       printf("UTC date               : %2x%2x/%2x/%2x\n"  ,
              bootboot.datetime[0], bootboot.datetime[1],
              bootboot.datetime[2], bootboot.datetime[3]);
       printf("UTC time               :   %2x:%2x:%2x \n"  ,
              bootboot.datetime[4], bootboot.datetime[5],
              bootboot.datetime[6]                      );
       printf("Initrd ptr|size        : %10p | %d \n", 
              bootboot.initrd_ptr, bootboot.size      );
       printf("fb type|ptr|size|WxH|sa: %10p | %d | %d | %dx%d | %x\n"  ,
              bootboot.fb_ptr, bootboot.size, bootboot.fb_type           ,
              bootboot.fb_width, bootboot.fb_height, bootboot.fb_scanline);
       printf("ACPI   ptr             : %10p          \n", bootboot.arch.x86_64.acpi_ptr);
       printf("SMBIOS ptr             : %10p          \n", bootboot.arch.x86_64.smbi_ptr);
       printf("EFI    ptr             : %10p          \n", bootboot.arch.x86_64.efi_ptr );
       printf("MP     ptr             : %10p          \n", bootboot.arch.x86_64.mp_ptr  );
}
