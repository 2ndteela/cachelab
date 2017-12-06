#include "cachelab.h"
#include "stdio.h"
#include "unistd.h"
#include "math.h"
#include "stdlib.h"
#include <getopt.h>
#define _POSIX_C_SOURCE 2
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
}

int getLeastUsed(cache_set thisSet, cache_info params, int* usedLines) {
    int num_lines = params.Eval;

    int mostUsed = thisSet.lines[0].access_counter; 
    int whiteCrayon = thisSet.lines[0].access_counter;
    

    int whiteCrayonIndex = 0;
    
    int lineIndex;
    set_line line; 

    for (lineIndex = 1; lineIndex < num_lines; lineIndex ++) {
    
        line = thisSet.lines[lineIndex];

        if (whiteCrayon > line.access_counter) {
            whiteCrayonIndex = lineIndex; 
            whiteCrayon = line.access_counter;
        }

        if (mostUsed < line.access_counter) {
            mostUsed = line.access_counter;
        }
    }
    usedLines[0] = whiteCrayon;
    usedLines[1] = mostUsed;
    
    return whiteCrayonIndex;
}
cache_info simulateCache (cache this_cache, cache_info par, mem_address address) {
            int lineIndex;
            int cache_full = 1;
    
            int num_lines = par.Eval;
            int prev_hits = par.hits;
    
            int tag_size = (64 - (par.s + par.b));
    
            unsigned long temp = address << (tag_size);
            unsigned long setIndex = temp >> (tag_size + par.b);
            mem_address input_tag = address >> (par.s + par.b);
    
            cache_set query_set = this_cache.sets[setIndex];
            for (lineIndex = 0; lineIndex < num_lines; lineIndex ++) {
                set_line line = query_set.lines[lineIndex];
                if (line.valid) {
                    if (line.tag == input_tag) {
                        line.access_counter++;
                        par.hits++;
                        query_set.lines[lineIndex] = line;
                    }
    
                } else if (!(line.valid) && (cache_full)) {
                    cache_full = 0;     
                }
            }   
    
            if (prev_hits == par.hits) {
                par.misses++; 
                
            } else {
                return par; 
            }
    
    
            int *used_lines = (int*) malloc(sizeof(int) * 2);
            int whiteCrayonIndex = getLeastUsed(query_set, par, used_lines);   
    
            if (cache_full) {
                par.evictions++;
                query_set.lines[whiteCrayonIndex].tag = input_tag;
                query_set.lines[whiteCrayonIndex].access_counter = used_lines[1] + 1;
            } else { 
    
                int empty_line_index = findOpen(query_set, par);
                query_set.lines[empty_line_index].tag = input_tag;
                query_set.lines[empty_line_index].valid = 1;
                query_set.lines[empty_line_index].access_counter = used_lines[1] + 1;
            }                       
    
            free(used_lines);
            return par;
    
    } /* end simulate_cache */

int main(int argc, char** argv)
{
    int opt;
    int numSets = 1;
    int blockNum = 1; 
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
    stats.s = atoi(sval);
    stats.b = atoi(bval);
    stats.hits = 0;
    stats.misses = 0;
    stats. evictions = 0;
    simulator = buildCache(stats);
    FILE *fp = fopen(tval, "r");
    char cmd;
    mem_address nextAdd;
    int size;
    while(fscanf(fp," %c %lx, %d", &cmd, &nextAdd, &size) == 3) {
        if(cmd == 'I') {
        }
        else if (cmd == 'L' || cmd == 'S') {
            stats = simulateCache(simulator, stats, nextAdd);
        }
        else {
            stats = simulateCache(simulator, stats, nextAdd);
            stats = simulateCache(simulator, stats, nextAdd);
        }
    }
    clearCache(simulator, stats);
    printSummary( stats.hits, stats.misses, stats.evictions);
    fclose(fp);
    return 0;
}
