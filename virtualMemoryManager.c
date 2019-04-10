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
#define CHUNK 256
//using macros for simplicity instead of functions
#define GET_PAGE_NUMBER(addr) (((addr) & (PAGE_NUMBER_MASK)) >> (OFFSET_LENGTH))
#define GET_OFFSET(addr) ((addr) & (OFFSET_MASK))
#define GET_PHYSICAL_ADDRESS(frame, offset) (((frame) << (OFFSET_LENGTH) | (offset)))

/* Steps:
First, page number extracted from Logical address (done)
Next, TLB is consulted
If there is a TLB hit, the frame number is obtained from the TLB
If there is a TLB miss, the page table must be consulted
    If there is a TLB miss:
        Either the frame number is obtained from the Page table
        or a page fault occurs
        
        When a page fault occurs:
            Read in a 256 byte page from the backing store file 
            Store this page in an available page frame in physical memory
            Once this frame is stored, Update the TLB and page table
            Any subsequent accesses to the page number that caused a page fault will be resolved by either the TLB or page table

        
 */

/* Constructing some data structures to be used. */
struct Page{
    uint8_t valid;
    uint32_t pageNumber;
    uint32_t frameNumber;
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
struct TLB tlb;
FILE *addresses; //creates a file pointer to addresses file
FILE *backingStore;
signed char buffer[CHUNK];

/* Creating a user defined data type; similar to creating a class or an object in another language */
typedef
enum tlb_lookup_status {
    TLB_MISS,
    TLB_HIT,
} tlb_lookup_status;

typedef
enum page_lookup_status {
    PAGE_FAULT,
    PAGE_HIT,
} page_lookup_status;

/* This function will determine if the page returns a page fault */
page_lookup_status
page_lookup(struct Page **p, uint32_t pageNumber){
    if (pageNumber < 0 || pageNumber > 255) {
        return NULL;
    }
    if (!pageTable[pageNumber].valid) {
        // PAGE FAULT
        // in this case, need to load from the file
        // into physical memory at next available frame
        // and update the frame number for that page
        return PAGE_FAULT;
    } else {
        // PAGE HIT
        // simply return the address of the page
        *p = &pageTable[pageNumber];
        return PAGE_HIT;
    }
    return PAGE_FAULT;
}

tlb_lookup_status
tlb_lookup(uint32_t pageNumber, uint32_t* frameNumber){
    int i;
    for(i = 0; i < TLB_SIZE; i++){
        if(tlb.pages[i].pageNumber == pageNumber){
            //if this executes, this means that there is a TLB hit
            *frameNumber = tlb.pages[i].frameNumber;
            return TLB_HIT;
        }
    }
    //otherwise, return a TLB miss
    return TLB_MISS;
}


int main(int argc, char const *argv[]){
    int i;
    int logicalAddress;
    //initializing the physical memory so that valid field of every element is false
    for(i = 0; i < TOTAL_FRAMES; i++){
        physicalMemory[i].valid = 0;
    }
    addresses = fopen(argv[1], "r"); //reads in file as command line argument 
    backingStore = fopen("BACKING_STORE.bin", "rb"); //opens the backing_store.bin file
    
    if(addresses == NULL){
        fprintf(stderr, "Error opening addresses.txt %s\n", argv[1]);
        return -1;
    }
    if(backingStore == NULL){
        fprintf(stderr, "Error opening addresses.txt %s\n", "BACKING_STORE.bin");
        return -1;
    }
    
    while(fgets(virtualAddress, VIRTUAL_ADDRESS_SIZE, addresses) != NULL){
        logicalAddress = atoi(virtualAddress);
        uint32_t pageNumber = GET_PAGE_NUMBER(logicalAddress);
        uint32_t offset = GET_OFFSET(logicalAddress);
        //printf("PageNumber: %u Offset: %u\n", pageNumber, offset);
        uint32_t frameNumber = 0;
        if(TLB_HIT == tlb_lookup(pageNumber, &frameNumber)){
            //TLB hit case
            printf("------TLB HIT-------\n");
            printf("Frame Number: %u\n", frameNumber);
            
        }else{
            //TLB miss case
            printf("-----TLB MISS-----\n");
            struct Page *lookup_results;
            if(PAGE_HIT == page_lookup(&lookup_results, pageNumber)){
                frameNumber = lookup_results->frameNumber;
            }else{//Page fault 
                if(fseek(backingStore, pageNumber * PAGE_SIZE, SEEK_SET) != 0){
                    fprintf(stderr, "error reading backing store\n");
                }
                //first read in 256 byte data from backing store
                if(fread(buffer, sizeof(signed char), CHUNK, backingStore) == 0){
                    fprintf(stderr, "Backing store read error!\n");
                }
                int i;
                for(i = 0; i < CHUNK; i++){
                    physicalMemory[frameNumber].bytes[i] = buffer[i];
                }
                //now, update the Page Table
                //now, update the TLB
            }
        }


        uint32_t physical_address = GET_PHYSICAL_ADDRESS(frameNumber, offset);


    }

    fclose(addresses);
    fclose(backingStore);

    return 0;
}
