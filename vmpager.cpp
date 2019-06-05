//Srinivas C Lingutla
//655115444
//slingu2


#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>
#include <iomanip>
using namespace std;

//#define FRAMETABLESIZE 256
#define PAGETABLESIZE 256
#define NPROCESSES 256

typedef struct
{
    uint8_t pid;
    uint8_t page;
} MemoryAccess;

typedef struct
{
    int frame; // -1 if not currently loaded
    int finitePageHits;
    int finitePageMisses;
    int infinitePageHits;
    int infinitePageMisses;
} PageTableEntry;

PageTableEntry pageTables[ PAGETABLESIZE ][ NPROCESSES ];

typedef struct
{
    uint8_t pid;
    uint8_t page;
    bool vacant;
} FrameTableEntry;

void initializeTables(FrameTableEntry *table, int frametablesize)
{

  for(int i = 0; i < PAGETABLESIZE; i++)
  {
    for(int j = 0; j < NPROCESSES; j++)
    {
      pageTables[i][j].frame = -1;
      pageTables[i][j].finitePageHits = 0;
      pageTables[i][j].finitePageMisses = 0;
      pageTables[i][j].infinitePageHits = 0;
      pageTables[i][j].infinitePageMisses = 0;
    }
  }

  for(int i = 0; i < frametablesize; i++)
  {
    table[i].pid = -1;
    table[i].page = -1;
    table[i].vacant = true;
  }

}

int main (int argc , char ** argv )
{

    cout << endl << "*********** Srinivas Lingutla, Project 3, slingu2 ************" << endl;

    char * filename;
    long int memoryAccesses, frameTableSize = 256, filesize;
    unsigned long int InfiniteMemoryHits = 0, InfiniteMemoryMisses = 0,  FIFOMemoryHits = 0, FIFOMemoryMisses = 0;
    int fd;
    struct stat statBuffer;
    MemoryAccess* filestart;
    FrameTableEntry* frameTable;

    //Parse the arguments
    if(argc < 2) {
        cout  << "Too few arguments" << endl;
        exit(-1);
    }
    //get the arguments
    if(argc >= 2){
        filename = (char *)malloc(sizeof(char) * (strlen(argv[1]) + 1));
        strcpy(filename, argv[1]);

        if(argc == 3)
            memoryAccesses = atoi(argv[2]);

        if(argc == 4) {
            if(atoi(argv[3]) > 0)
            {
                memoryAccesses = atoi(argv[2]);
                frameTableSize = atoi(argv[3]);
            }
            else
            {
                perror("Frametable size must be greater than 0");
                exit(-5);
            }
          }
    }

    if((fd = open(filename, O_RDWR)) < 0){ //open the file
        perror( "Error opening the file" );
        exit(-2);
    }

    if( fstat(fd, &statBuffer) < 0 ) { //fstat
        perror( "fstat error" );
        exit( -3 );
    }

    filesize = statBuffer.st_size; //get the size
    if(memoryAccesses <=0 || memoryAccesses > ((filesize)/ (sizeof(MemoryAccess)))) //reset the memory access if it is out of bounds
        memoryAccesses = ((filesize)/ (sizeof(MemoryAccess)));

    cout << endl << "File: " << filename << endl;
    cout << "Frame table size: " << frameTableSize << endl;
    cout << "No. of Memory Accesses: " << memoryAccesses << endl << endl;

    if( (filestart = (MemoryAccess*) mmap(0, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) { //memory map
        perror( "mmap error" );
        exit( -4 );
    }

    //Create the frametable of a certain size
    frameTable = (FrameTableEntry *) malloc (sizeof(FrameTableEntry) * frameTableSize);

    // for (int x=0; x <= memoryAccesses; x++) //for testing purposes
    // {
    //     cout << filestart[x].pid << endl;
    // }

    //Simulate Infinite memory

    initializeTables(frameTable, frameTableSize); //reset all the tables(page table and the frame table)

    for( int i = 0; i < memoryAccesses; i++)
    {
      if(pageTables[ filestart[i].pid ][ filestart[i].page ].frame == 1 ) //if its a hit
      {
        pageTables[ filestart[i].pid ][ filestart[i].page ].infinitePageHits++;
      }
      else //if its a miss
      {
        pageTables[ filestart[i].pid ][ filestart[i].page ].infinitePageMisses++;
        pageTables[ filestart[i].pid ][ filestart[i].page ].frame = 1; //page is loaded

      }
     }



    //Simulate FIFO Results

    for(int i = 0; i < PAGETABLESIZE; i++) //reset all the tables(page table and the frame table)
      for(int j = 0; j < NPROCESSES; j++)
        pageTables[i][j].frame = -1;

    int top = 0; //keeping track of the queue

    for(int i = 0; i < memoryAccesses; i++)
    {
        if(pageTables[filestart[i].pid][filestart[i].page].frame == 1 ) //if its a hit update the count
        {
          pageTables[filestart[i].pid][filestart[i].page].finitePageHits++;
        }
        else if(pageTables[filestart[i].pid][filestart[i].page].frame < 0 ) //if its a miss swap the pages
        {
          pageTables[filestart[i].pid][filestart[i].page].finitePageMisses++;

          if(frameTable[top].vacant == false)
            pageTables[frameTable[top].pid][frameTable[top].page].frame = -1;

          //load the new page
          frameTable[top].pid = filestart[i].pid;
          frameTable[top].page = filestart[i].page;
          frameTable[top].vacant = false;

          pageTables[frameTable[top].pid][frameTable[top].page].frame = 1;

          if((++top) == frameTableSize) top = 0;
        }
    }

    for(int i = 0; i < PAGETABLESIZE; ++i)
    {
      for(int j = 0; j < NPROCESSES; ++j)
      {
        InfiniteMemoryHits += pageTables[i][j].infinitePageHits;
        InfiniteMemoryMisses += pageTables[i][j].infinitePageMisses;

        FIFOMemoryHits += pageTables[i][j].finitePageHits;
        FIFOMemoryMisses += pageTables[i][j].finitePageMisses;

      }
    }

    //Finished the infinite memory simulation
    cout << "-------------------------------------" << endl;

    cout << "** Infinite Memory Simulation **" << endl;
    cout << "Hits: " << InfiniteMemoryHits << endl;
    cout << "Misses: " << InfiniteMemoryMisses << endl;

    //Finished the FIFO memory simulation
    cout << "-------------------------------------" << endl;

    cout << "** FIFO Memory Simulation **" << endl;
    cout << "Hits: " << FIFOMemoryHits << endl;
    cout << "Misses: " << FIFOMemoryMisses << endl;

    // cout << "-------------------------------------" << endl;
    // cout << "Total hits: " << InfiniteMemoryHits + FIFOMemoryHits << endl;
    // cout << "Total Misses: " << InfiniteMemoryMisses + FIFOMemoryMisses << endl;

}
