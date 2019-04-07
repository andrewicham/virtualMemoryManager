#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PAGE_TABLE_SIZE 256
#define PAGE_SIZE 256
#define TOTAL_FRAMES 256
#define FRAME_SIZE 256
#define TLB_SIZE 16
#define PAGE_NUMBER_MASK 0xffff
#define OFFSET_MASK 0xff
#define VIRTUAL_ADDRESS_SIZE 10

int physicalMemory[TOTAL_FRAMES][FRAME_SIZE];
int pageTable[PAGE_TABLE_SIZE][PAGE_SIZE];
char virtualAddress[VIRTUAL_ADDRESS_SIZE];
void getPage(int virtualAddress);

struct Page{
    int virtualPageNumber;
    int mappedFrame;
    struct Page *next;
};


void getPage(int virtualAddress){
    int pageNumber = ((virtualAddress & PAGE_NUMBER_MASK) >> 8); //this will shift right 8 bits, after performing and on both
    int offset = (virtualAddress & OFFSET_MASK); //performs and on virtualAddress to determine offset

    printf("page number: %d offset:%d\n", pageNumber, offset);
}



int main(int argc, char const *argv[]){
    FILE *addresses; //creates a file pointer to addresses file
    FILE *backingStore;

    addresses = fopen(argv[1], "r"); //reads in file as command line argument 
    backingStore = fopen("BACKING_STORE.bin", "rb"); //opens the backing_store.bin file
    

    int logicalAddress;
    while(fgets(virtualAddress, VIRTUAL_ADDRESS_SIZE, addresses) != NULL){
        logicalAddress = atoi(virtualAddress);

        getPage(logicalAddress);
    }

    fclose(addresses);
    fclose(backingStore);
    

    

    return 0;
}
