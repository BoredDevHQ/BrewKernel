/*
 * Brew Kernel
 * Copyright (C) 2024-2025 boreddevhq
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

// Main kernel file for Brew kernel.
// This file contains the main function and the kernel's entry point.
// It prints a welcome message and then loops all VGA characters to the screen.

#include "print.h"

static inline void brewing(int iterations) {
    for (volatile int i = 0; i < iterations; i++) {
        __asm__ __volatile__("nop");
    }
}

void kernel_main() {
    print_clear();
    
    print_init_palette();

    print_set_palette_color(1, 0, 113, 255);   // Blue
    print_set_palette_color(1, 108, 198, 74);   // Green
    print_set_palette_color(2, 245, 194, 45);   // Yellow
    print_set_palette_color(3, 255, 129, 63);   // Orange
    print_set_palette_color(4, 237, 28, 36);    // Red
    print_set_palette_color(5, 163, 73, 164);   // Purple
    print_set_palette_color(6, 108, 198, 74);  // Green
    print_set_palette_color(7, 172, 140, 104);  // Latte

    print_set_color(PRINT_INDEX_7, PRINT_INDEX_0);
    print_str("Brew kernel v1.0\n");
    print_str("Copyright (C) 2024-2025 boreddevhq.\n \n");

    print_set_color(PRINT_INDEX_1, PRINT_INDEX_0);  
    print_str("( (\n");
    print_set_color(PRINT_INDEX_2, PRINT_INDEX_0);  
    print_str("    ) )\n");
    print_set_color(PRINT_INDEX_3, PRINT_INDEX_0);  
    print_str("  ........\n");
    print_set_color(PRINT_INDEX_4, PRINT_INDEX_0); 
    print_str("  |      |]\n");
    print_set_color(PRINT_INDEX_5, PRINT_INDEX_0); 
    print_str("  \\      /\n");
    print_set_color(PRINT_INDEX_9, PRINT_INDEX_0); 
    print_str("   `----'\n\n");
    print_set_color(PRINT_INDEX_7, PRINT_INDEX_0);

    // Print VGA color indexes 0-15
    for (int i = 0; i < 16; i++) {
        print_set_color(i, i);  
        print_char(' ');       
    }
    print_set_color(PRINT_INDEX_7, PRINT_INDEX_0);  // Reset to default color
    print_str("\n\n");

    // Infinite loop to continuously display ASCII characters
    while (1) {
        // First Loop: Display standard ASCII characters (0-126)
        for (unsigned char c = 0; c <= 126; c++) { 
            print_char(c);
            if ((c - 31) % 80 == 0) {
                print_char('\n');
            }
            brewing(10000000); 
            // Lazy delay for readability, PIT will not be implemented since
            // there really is no need for it in this kernel.
        }
        
        // Second Loop: Display extended ASCII characters (128-255)
        for (unsigned char c = 128; c != 0; c++) {
            print_char(c);
            if ((c - 127) % 80 == 0) {
                print_char('\n');
            }
            brewing(10000000);  
        }
    }
}
