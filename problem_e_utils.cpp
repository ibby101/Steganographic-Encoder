#include "image_header.h"

// getting the bit 
unsigned char getBitN(unsigned char val, int n) {
    unsigned char b;
    b = 1 & (val >> n); // Shifts bit 'n' to position 0, then masks it
    return b;           // Returns 0 or 1
}

// code obtained from the E_bit.c file provided


// helper to set a bit in a byte (useful for decoder)
void set_bit(unsigned char& byte, int bit_index, unsigned char bit_value) {
    if (bit_index < 0 || bit_index > 7) {
        throw out_of_range("Bit index must be between 0 and 7");
    }
    if (bit_value == 1) {
        byte |= (1 << bit_index); // Set bit to 1
    }
    else {
        byte &= ~(1 << bit_index); // Set bit to 0
    }
}