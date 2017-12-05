#include "cachelab.h"
#include "stdio.h"
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
//jteela

typedef unsigned long int mem_adress;

typedef struct {
    int valid;
    int tag;
    mem_adress adress;
    int access_counter;
} set_line;

typedef struct {
    set_line* lines;
} cache_set;

typedef struct {
    cache_set* sets;
} cache;

typedef struct {
    int Eval; 
    int Sval;
    int Bval;

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

void clearCache(cache thisOne, cache_info stats) {
    int i;

    for(i = 0; i < stats.Sval; i++) {
        cache_set tempSet = thisOne.sets[i];
        if(tempSet.lines != NULL) free(tempSet.lines);
    }

    if(thisOne.sets != NULL) free(thisOne.sets);
    printf("cache cleared\n");
}

int findOpen(cache_set thisSet, cache_info stats) {
    int i;
    int numLines = stats.Eval;

    for(i = 0; i < numLines; i++) {
        if(thisSet.lines[i].valid == 0) {
            printf("returning %d\n", i);
            return i;
        }
    }
    printf("no empty sets found\n");
    return -1;
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
                break;

            case 'E':
                eval = optarg;
                break;

            case 'b':
                bval = optarg;
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
    simulator = buildCache(stats);
    simulator.sets[2].lines[0].valid = 1;
    findOpen(simulator.sets[2], stats);

    printf("Sets = %d | Lines per set = %d | Block size = %d\n", stats.Sval, stats.Eval, stats.Bval);

    printf("\n");
    FILE *fp = fopen(tval, "r");
    char buff[255];
    while(fgets(buff, 255, (FILE*) fp)) {
        if(buff[0] == 'I') {

        }
        else {
            printf("%s", buff);
        }
    }

    clearCache(simulator, stats);
    //printSummary(5, 6, 7);
    return 0;
}
