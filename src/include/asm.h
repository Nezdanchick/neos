#pragma once
#include <stdint.h>
#include <stdbool.h>

extern uint32_t farpeekl(uint16_t sel, void* off);
extern void farpokeb(uint16_t sel, void* off, uint8_t v);
extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);
extern void io_wait(void);
extern bool are_interrupts_enabled();
extern unsigned long save_irqdisable(void);
extern void irqrestore(unsigned long flags);
static void intended_usage(void);
extern void lidt(void* base, uint16_t size);
extern void cpuid(int code, uint32_t* a, uint32_t* d);
extern uint64_t rdtsc();
extern unsigned long read_cr0(void);