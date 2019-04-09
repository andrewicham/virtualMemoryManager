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

/* Steps:
First, page number extracted from Logical address (done)
Next, TLB is consulted
If there is a TLB hit, the frame number is obtained from the TLB
If there is a TLB miss, the page table must be consulted
    If there is a TLB miss:
        Either the frame number is obtained from the Page table
        or a page fault occurs
        
        When a page fault occurs:
            this means that the 
            Read in a 256 byte page from the backing store file 
            Store this page in an available page frame in physical memory
            Once this frame is stored, Update the TLB and page table
            Any subsequent accesses to the page number that caused a page fault will be resolved by either the TLB or page table

        
 */

//page table contains page number and frame number
struct Page{
    int pageNumber;
    int frameNumber;
};

struct TLB{
    struct Page pages[TLB_SIZE];
};

struct Frame {
    uint32_t valid;
    char bytes[FRAME_SIZE];
};

struct Frame physicalMemory[TOTAL_FRAMES];
struct Page pageTable[PAGE_TABLE_SIZE];
char virtualAddress[VIRTUAL_ADDRESS_SIZE];
struct TLB tlb;
int firstAvailableFrameNumber = 0;

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
    //default return value
    if(pageNumber < 0 || pageNumber > 255){
        return NULL;
    }

    return PAGE_FAULT;
}




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
        uint32_t frameNumber;
        uint32_t physical_address = GET_PHYSICAL_ADDRESS(frameNumber, offset);
        



    }

    fclose(addresses);
    fclose(backingStore);

    return 0;
}
