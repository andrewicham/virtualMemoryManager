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

int pageTable[TOTAL_FRAMES][FRAME_SIZE];

void getPage(int virtualAddress);




void getPage(int virtualAddress){
    int pageNumber = ((virtualAddress & PAGE_NUMBER_MASK) >> 8); //this will shift right 8 bits, after performing and on both
    int offset = (virtualAddress & OFFSET_MASK); //performs and on virtualAddress to determine offset

    printf("page number: %d\noffset:%d\n", pageNumber, offset);
}



int main(int argc, char const *argv[]){
    //as per the instructions, use bitmask and bit shifting to extract address and offset on this array
    int nums[7] = {1, 256, 32768, 32769, 128, 65534, 33153}; 
    int i;

    for(i = 0; i < 7; i++){
        getPage(nums[i]);
    }
    

    FILE *addresses; //creates a file pointer to addresses file
    FILE *backingStore;

    addresses = fopen(argv[1], "r"); //reads in file as command line argument 
    backingStore = fopen("BACKING_STORE.bin", "rb"); //opens the backing_store.bin file



    


    return 0;
}
