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
#define OFFSET_LENGTH 8 //the number of bits to shift when finding physical address
#define CHUNK 256

//using macros for simplicity instead of functions
#define GET_PAGE_NUMBER(addr) (((addr) & (PAGE_NUMBER_MASK)) >> (OFFSET_LENGTH))
#define GET_OFFSET(addr) ((addr) & (OFFSET_MASK))
#define GET_PHYSICAL_ADDRESS(frame, offset) ((frame << OFFSET_LENGTH) | (offset))

FILE *addresses; //creates a file pointer to addresses file and backingStore file
FILE *backingStore;
int numberOfPageFaults = 0;
int translatedAddresses = 0;
int tlbInsertions = 0;
int tlbIndex = 0;

/* first we need frame and page data structures */
struct Page{
    uint32_t pageNumber;
    uint32_t frameNumber;
    uint8_t valid; 
};

struct Frame{
    uint8_t free; //if the memory is free, will be 1. otherwise 0.
    signed char bytes[FRAME_SIZE];
};

/* Now implementing a TLB data structure */
struct TLB {
    struct Page pages[TLB_SIZE];
};

struct Page pageTable[PAGE_TABLE_SIZE];
struct Frame physicalMemory[TOTAL_FRAMES];
char virtualAddress[VIRTUAL_ADDRESS_SIZE];
struct TLB tlb;

/* creating a user defined data type to add more meaningful return values to functions */
typedef
enum page_lookup_status {
    PAGE_FAULT,
    PAGE_HIT,
} page_lookup_status;

typedef
enum tlb_lookup_status {
    TLB_MISS,
    TLB_HIT,
} tlb_lookup_status;


page_lookup_status
page_lookup(struct Page **p, uint32_t pageNumber){
    //point p to null if there is a page fault
    *p = NULL;
    if (pageNumber < 0 || pageNumber > 255) {
        return PAGE_FAULT;
    }
    //if there is a valid pageTable entry, then get its address
    if (pageTable[pageNumber].valid == 1) {
        *p = &pageTable[pageNumber];
        return PAGE_HIT;
    }
    return PAGE_FAULT;
}

tlb_lookup_status
tlb_lookup(uint32_t pageNumber, uint32_t* frameNumber){
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb.pages[i].pageNumber == pageNumber) {
            // HIT
            *frameNumber = tlb.pages[i].frameNumber;
            return TLB_HIT;
        }
    }
    return TLB_MISS; // MISS
}

void insert_into_tlb(pageNumber, frameNumber){
    
}


int main(int argc, char const *argv[]){
    addresses = fopen(argv[1], "r"); //reads in file as command line argument 
    backingStore = fopen("BACKING_STORE.bin", "rb"); //opens the backing_store.bin file
    int logicalAddress;
    uint32_t frameNumber = 0;
    
    //initializing the page table so that valid field of every element is false
    for(int i = 0; i < PAGE_TABLE_SIZE; i++){
        pageTable[i].valid = 0;
    }

    //physical memory is free at the beginning
    for(int i = 0; i < TOTAL_FRAMES; i++){
        physicalMemory[i].free = 1;
    }
    //error check conditions for file opening
    if(addresses == NULL){
        fprintf(stderr, "Error opening addresses.txt %s\n", argv[1]);
        return -1;
    }
    if(backingStore == NULL){
        fprintf(stderr, "Error opening addresses.txt %s\n", "BACKING_STORE.bin");
        return -1;
    }
    
    //iterate through the addresses.txt file line by line
    while(fgets(virtualAddress, VIRTUAL_ADDRESS_SIZE, addresses) != NULL){
        logicalAddress = atoi(virtualAddress); //set each line of logical address to type int
        
        uint32_t pageNumber = GET_PAGE_NUMBER(logicalAddress); //translating page number from logical address
        uint32_t offset = GET_OFFSET(logicalAddress); //translating offset from logical address

        struct Page *lookup_results; //a placeholder variable to get the frame number in case of a PAGE_HIT
        
        if(TLB_HIT == tlb_lookup(pageNumber, &frameNumber)){ //if the page number is in the TLB, get its frame number
            //now that the frame number has been retrieved, 
        }
        else{ //if the TLB search fails, do a page table lookup
            if(PAGE_HIT == page_lookup(&lookup_results, pageNumber)){ //if the page is in the page table, get its frame number
                frameNumber = lookup_results->frameNumber;
                insert_into_tlb(pageNumber, frameNumber);
            }
            else{ //if its not in the page table, look for available frame number
                for (int i = 0; i < TOTAL_FRAMES; i++) {
                    if (physicalMemory[i].free) {
                        // found a free frame, use it
                        frameNumber = i;
                        physicalMemory[i].free = 0;
                        break;
                    }
                }
                
                //now, read in the page from the disk (BACKING_STORE.bin)
                if(fseek(backingStore, pageNumber * CHUNK, SEEK_SET) != 0){
                    fprintf(stderr, "Error! cannot seek through backing store!\n");
                    // continue with next element, there is nothing to do for current input
                    continue;
                }
                //read the proper page directly into physical memory
                if(fread(physicalMemory[frameNumber].bytes, sizeof(signed char), CHUNK, backingStore) == 0){
                    fprintf(stderr, "Error! cannot read backing store!\n");
                    // continue with next element, there is nothing to do for current input
                    continue;
                }
                /* the following three lines updates the page table entry for this pageNumber and frameNumber */
                pageTable[pageNumber].pageNumber = pageNumber; 
                pageTable[pageNumber].frameNumber = frameNumber;
                pageTable[pageNumber].valid = 1;
                insert_into_tlb(pageNumber, frameNumber);
                //insert the page into the tlb
            }
        }
        uint32_t physical_address = GET_PHYSICAL_ADDRESS(frameNumber, offset);
        //printf("virtual address: %u physical address: %u\n", logicalAddress, physical_address);
    }
    
    int j = 15 % 16;
    printf("J: %d", j);
    fclose(addresses);
    fclose(backingStore);
    return 0;
}
