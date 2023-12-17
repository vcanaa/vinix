#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include <stdint.h>

// void clear_screen() {
//   int i = 0;
//   while (i < 80 * 25 * 2) {
//     VIDEO_PTR[i] = ' ';
//     i++;
//     VIDEO_PTR[i] = 0x00;
//     i++;
//   }
// }

// void print_all_ascii_white(const int line) {
//   unsigned char l = 0;
//   unsigned char attribute = 0x07;
//   int i = line * 80 * 2;
//   do {
//     VIDEO_PTR[i++] = l++;
//     VIDEO_PTR[i++] = attribute;
//   } while (l > 0);
// }

// void print_all_attributes(const int line) {
//   unsigned char l = 'A';
//   unsigned char attribute = 0;
//   int i = line * 80 * 2;
//   do {
//     VIDEO_PTR[i++] = l;
//     VIDEO_PTR[i++] = attribute++;
//   } while (attribute > 0);
// }

// void _start() {
//   clear_screen();

//   print_all_ascii_white(0);
//   print_all_attributes(5);
// }

void _start() {
  isr_install();
  irq_install();

  asm("int $2");
  asm("int $3");

  kprint("Type something, it will go through the kernel\n"
    "Type END to halt the CPU or PAGE to request a kmalloc()\n> ");
}

void user_input(char* input) {
  if (strcmp(input, "END") == 0) {
    kprint("Stopping the CPU. Bye!\n");
    asm volatile("hlt");
  }
  else if (strcmp(input, "PAGE") == 0) {
    /* Lesson 22: Code to test kmalloc, the rest is unchanged */
    uint32_t phys_addr;
    uint32_t page = kmalloc(1000, 1, &phys_addr);
    char page_str[16] = "";
    hex_to_ascii(page, page_str);
    char phys_str[16] = "";
    hex_to_ascii(phys_addr, phys_str);
    kprint("Page: ");
    kprint(page_str);
    kprint(", physical address: ");
    kprint(phys_str);
    kprint("\n");
  }
  kprint("You said: ");
  kprint(input);
  kprint("\n> ");
}