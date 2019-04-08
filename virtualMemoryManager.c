#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#define PAGE_TABLE_SIZE 256
#define PAGE_SIZE 256
#define TOTAL_FRAMES 256
#define FRAME_SIZE 256
#define TLB_SIZE 16
#define PAGE_NUMBER_MASK 0xff00 //could be either 0xffff or 0xff00, as we are losing right most two f's 
#define OFFSET_MASK 0xff
#define VIRTUAL_ADDRESS_SIZE 10
#define OFFSET_LENGTH 8

//using macros for simplicity instead of functions
#define GET_PAGE_NUMBER(addr) (((addr) & (PAGE_NUMBER_MASK)) >> (OFFSET_LENGTH))
#define GET_OFFSET(addr) ((addr) & (OFFSET_MASK))
#define GET_PHYSICAL_ADDRESS(frame, offset) (((frame) << (OFFSET_LENGTH) | (offset)))


struct Page{
    int pageNumber;
    int frameNumber;
};

struct TLB{
    struct Page pages[TLB_SIZE];
};

struct Frame {
    uint8_t valid;
    char bytes[FRAME_SIZE];
};

struct Frame physicalMemory[TOTAL_FRAMES];
struct Page pageTable[PAGE_TABLE_SIZE];
char virtualAddress[VIRTUAL_ADDRESS_SIZE];


int main(int argc, char const *argv[]){
    FILE *addresses; //creates a file pointer to addresses file
    FILE *backingStore;

    addresses = fopen(argv[1], "r"); //reads in file as command line argument 
    backingStore = fopen("BACKING_STORE.bin", "rb"); //opens the backing_store.bin file
    

    int logicalAddress;
    while(fgets(virtualAddress, VIRTUAL_ADDRESS_SIZE, addresses) != NULL){
        logicalAddress = atoi(virtualAddress);
        uint32_t pageNumber = GET_PAGE_NUMBER(logicalAddress);
        uint32_t offset = GET_OFFSET(logicalAddress);

        printf("PageNumber: %u Offset: %u\n", pageNumber, offset);

    }

    fclose(addresses);
    fclose(backingStore);

    return 0;
}
