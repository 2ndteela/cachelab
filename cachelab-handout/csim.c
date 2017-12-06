#include "cachelab.h"
#include "stdio.h"
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
//jteela

typedef unsigned long int mem_address;

typedef struct {
    int valid;
    int tag;
    mem_address adress;
    int access_counter;
} set_line;

typedef struct {
    set_line* lines;
} cache_set;

typedef struct {
    cache_set* sets;
} cache;

typedef struct {

    int s;      //original s parameter
    int b;      //original b parameter

    int Eval;   //Lines per set
    int Sval;   //Number of Sets
    int Bval;   //Size of the block

    int hits;
    int misses;
    int evictions;
} cache_info;

cache buildCache(cache_info stuff) {
    cache tempCache;
    cache_set tempSet;
    set_line tempLine;

    int i;
    int j;

    tempCache.sets = (cache_set *) malloc(sizeof(cache_set) * stuff.Sval);


    for(i = 0; i < stuff.Sval; i++) {
        tempSet.lines = (set_line *) malloc(sizeof(set_line) * stuff.Eval);
        tempCache.sets[i] = tempSet;
        for(j = 0; j < stuff.Eval; j++) {
            tempLine.valid = 0; 
            tempLine.tag = 0;
            tempLine.access_counter = 0;
            tempSet.lines[j] = tempLine;
        }
    }
    printf("cache built\n");
    return tempCache;
}

int findOpen(cache_set thisSet, cache_info stats) {
    int i;
    int numLines = stats.Eval;

    for(i = 0; i < numLines; i++) {
        if(thisSet.lines[i].valid == 0) {
            return i;
        }
    }
    return -1;
}

void clearCache(cache thisOne, cache_info stats) {
    int i;

    for(i = 0; i < stats.Sval; i++) {
        cache_set tempSet = thisOne.sets[i];
        if(tempSet.lines != NULL) free(tempSet.lines);
    }

    if(thisOne.sets != NULL) free(thisOne.sets);
    printf("cache cleared\n");
}

int getLeastUsed(cache_set thisSet, cache_info params) {
    int *tempPoint = (int*) malloc(sizeof(int)* 2);
    int i; 

    int numLines = params.Eval;
    int mostUsed = 0;
    int whiteCrayon = 0;
    int whiteCrayonIndex = 0;
    int mostUsedindex = 0;

    for(i = 0; i < numLines; i++) {
        printf("line: %d | access count: %d | most used: %d | least used: %d\n", i, thisSet.lines[i].access_counter, mostUsed, whiteCrayon);
        if(thisSet.lines[i].access_counter > mostUsed) { 
            mostUsedindex = i;
            mostUsed = thisSet.lines[i].access_counter;
        }
        else if (thisSet.lines[i].access_counter < whiteCrayon) {
            whiteCrayonIndex = i;
            printf("white crayon = %d\n", i);
            whiteCrayon = thisSet.lines[i].access_counter; 
        }
    }

    return whiteCrayonIndex;
}

cache_info readInstruction (mem_address address, cache thisCache, cache_info params) {

    int currentLine;
    int isFull = 1; 

    int numLines = params.Eval;
    int hits = params.hits;
    int tagSize = (64 - (params.b + params.s));

    long unsigned setIndexStub = address << (tagSize);
    long unsigned setIndex = setIndexStub >> (tagSize + params.b);
    
    mem_address inputTag = address >> (params.Sval + params.Bval);

    cache_set tempSet = thisCache.sets[setIndex];

    for(currentLine = 0; currentLine < numLines; currentLine++) {
        set_line tempLine = tempSet.lines[currentLine];
        if(tempLine.valid) {
            if(tempLine.tag == inputTag) {
                params.hits++;
                tempLine.access_counter++;
                thisCache.sets[setIndex].lines[currentLine] = tempLine;
                printf("%d hits on line %d of set %d\n\n",tempLine.access_counter, currentLine, setIndex);
            }
        }
        else if(!(tempLine.valid) && isFull) {
            isFull = 0;
        }
    }
    if(hits == params.hits) params.misses++;
    else {
        return params;
    }
    printf("\nmiss\n\n");

    if(isFull) {
        params.evictions++;
        int whiteCrayon = getLeastUsed(tempSet, params);
        thisCache.sets[setIndex].lines[whiteCrayon].valid = 1;
        thisCache.sets[setIndex].lines[whiteCrayon].tag = inputTag;
        thisCache.sets[setIndex].lines[whiteCrayon].access_counter = 0;
        printf("line %d of set %d updated to %d\n", whiteCrayon, setIndex, inputTag);
    }
    else {
       int open = findOpen(tempSet, params);
       tempSet.lines[open].valid = 1;
       tempSet.lines[open].tag = inputTag;
       printf("line %d of set %d updated to %d\n", open, setIndex, inputTag);

    }
    return params;
}

int main(int argc, char** argv)
{
    short i;
    int opt;
    int numSets = 1;
    int blockNum = 1; 
    int linesPerSet;
    char* sval = 0;
    char* eval = 0;
    char* bval = 0;
    char* tval = NULL;
    cache_info stats; 
    cache simulator;

    printf("\n");
    while( (opt = getopt(argc, argv, "s:E:b:t:")) != -1) {
        switch(opt) {
            case 's':
                sval = optarg;
                stats.s = atoi(sval);
                break;

            case 'E':
                eval = optarg;
                break;

            case 'b':
                bval = optarg;
                stats.b = atoi(bval);
                break;

            case 't':
                tval = optarg;
                break;

            default: 
                break;
        }
    }


    for(int k = 0; k < atoi(bval); k++) {
        blockNum *= 2;
    }

    for(int j = 0; j < atoi(sval); j++) {
        numSets *= 2;
    }
    stats.Bval = blockNum;
    stats.Sval = numSets;
    stats.Eval = atoi(eval);
    stats.hits = 0;
    stats.misses = 0;
    stats. evictions = 0;
    simulator = buildCache(stats);
    FILE *fp = fopen(tval, "r");
    int cmd;
    mem_address nextAdd;
    int size;
    char buff[255];
    while(fscanf(fp,"%c %11x, %d", &cmd, &nextAdd, &size) == 3) {
        if(cmd == "I") {
            printf("Ignoring command\n\n");
        }
        else {
            stats =    stats = readInstruction(nextAdd, simulator, stats);
        }
    }

    printf("\n*****  hits: %d misses: %d evictions: %d  *****\n", stats.hits, stats.misses, stats.evictions);
    clearCache(simulator, stats);
    //printSummary(5, 6, 7);
    return 0;
}
