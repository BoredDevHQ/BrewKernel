#include "print.h"

extern void load_idt(void* idt_ptr);
extern void keyboard_handler(void);  

volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

void kernel_puts(const char* str) {
    static uint16_t cursor_position = 0;

    for (size_t i = 0; str[i] != '\0'; ++i) {
        char c = str[i];
        if (c == '\n') {
            cursor_position += 80 - (cursor_position % 80); 
        } else {
            vga_buffer[cursor_position++] = (uint16_t)(0x0F00 | c); 
        }
    }
}

#define IDT_SIZE 256
#define KBD_PORT 0x60
#define IRQ1 33

struct IDTEntry {
    uint16_t base_low;  
    uint16_t sel;       
    uint8_t always0;  
    uint8_t flags;      
    uint16_t base_high; 
} __attribute__((packed));

struct IDTPtr {
    uint16_t limit;  
    uintptr_t base;   
} __attribute__((packed));

struct IDTEntry idt[IDT_SIZE];
struct IDTPtr idt_ptr;

char key_buffer[256];
uint16_t cursor_position = 0;

void set_idt_entry(int num, uintptr_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void init_idt() {
    idt_ptr.limit = (sizeof(struct IDTEntry) * IDT_SIZE) - 1;
    idt_ptr.base = (uintptr_t)&idt;

    set_idt_entry(IRQ1, (uintptr_t)keyboard_handler, 0x08, 0x8E);

    load_idt(&idt_ptr);
}

unsigned char keyboard_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_handler() {
    uint8_t scancode = inb(KBD_PORT);

    if (scancode & 0x80) {
        return;
    } else {
        char c = keyboard_map[scancode];

        if (c != 0) {
            key_buffer[cursor_position++] = c; 
            key_buffer[cursor_position] = '\0';  
            kernel_puts(&c);
        }
    }
}

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_MAGENTA, PRINT_COLOR_BLACK);
    print_str("       _..._                         \n");
    print_str("      .'     '.      _               \n");
    print_str("     /    .-\"\"-\\   _/ \\          \n");
    print_str("   .-|   /:.   |  |   |              \n");
    print_str("   |  \\  |:.   /.-'-./              \n");
    print_str("   | .-'-;:__.'    =/                \n");
    print_str("   .'=  *=|CC0  _.='                 \n");
    print_str("  /   _.  |    ;                     \n");
    print_str(" ;-.-'|    \\   |                    \n");
    print_str("/   | \\    _\\  _\\                 \n");
    print_str("\\__/'._;.  ==' ==\\                 \n");
    print_str("         \\    \\   |                \n");
    print_str("         /    /   /                  \n");
    print_str("         /-._/-._/                   \n");
    print_str("         \\   `\\  \\                \n");
    print_str("          `-._/._/                   \n");

    init_idt();

    print_set_color(PRINT_COLOR_MAGENTA, PRINT_COLOR_BLACK);
    print_str("Void Kernel Booted.\n");

    while (1) {
        __asm__ __volatile__("hlt");
    }
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "dN"(port));
}
