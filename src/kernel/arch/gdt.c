#include <stdint.h>

#include "src/includes/gdt.h"
#include "src/includes/serial.h"

segment gdt[5];
gdt_pointer gdtr;

void init_gdt
(
    void
)
{
    gdt[1] = (segment)
    {
        .access = KERNEL | CODE,
	.granularity = 0b00100000
    };
    gdt[2] = (segment)
    {
        .access = KERNEL | DATA,
	.granularity = 0
    };
    gdt[3] = (segment)
    {
        .access = USER | CODE,
	.granularity = 0b00100000
    };
    gdt[4] = (segment)
    {
        .access = USER | DATA,
	.granularity = 0
    };
    gdtr.adress = (uint64_t)&gdt;
    gdtr.size = sizeof
    (
         gdt
     )-1;
    serial_puts
    (
         COM1,
         "Loading GDT... \n"
     );
    GDT_load
    (
         (uintptr_t)&gdtr
    );
    serial_puts
    (
        COM1,
        "-->GDT successfully initialized \n"
    );
};

void GDT_load()
{
    __asm__ volatile("lgdt %0"
                     :
                     : "m"(gdtr)
                     : "memory");
    __asm__ volatile(
        "mov %%rsp, %%rax\n"
        "push $0x10\n"
        "push %%rax\n"
        " pushf\n"
        " push $0x8\n"
        " push $1f\n"
        " iretq\n"
        " 1:\n"
        " mov $0x10, %%ax\n"
        "  mov %%ax, %%ds\n"
        " mov %%ax, %%es\n"
        "mov %%ax, %%ss\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"

        :
        :
        : "rax", "memory");
}
