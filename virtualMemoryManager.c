#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/* This program will translate logical to physical addresses fora virtual address s
space of size 2^16 Bytes. This program reads from a file containing logical addresses
and using a TLB and a page table, will translate each logical address to its corresponding
physical address. it will output the value of the byte stored at the translated physical
address. This project will SIMULATE the steps involved in translating logical to physical
addresses

SPECIFICS:

read file containing 32 bit integer numbers that represent logical addresses. 
You must mask the rightmost 16 bits of each logical address.

These leftmost 16 bits are are divided into an 8 bit page number and 8 bit page offset

OTHER SPECIFICS:

2^8 entries in the page table
page size of 2^8 bytes
16 entries in the TLB
Frame size of 2^8 Bytes
256 frames
physical memory of 2^16 bytes (2^8 frames * 2^8-byte frame size)


First step: Address Translation

first, translate logical to physical address using a TLB and page table. 
The page number is extracted from the logical address and the TLB is consulted.
 */

int main(int argc, char const *argv[]){
    return 0;
}
