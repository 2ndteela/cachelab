#include "cachelab.h"
#include "stdio.h"
#include <unistd.h>
#include <math.h>
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
} cache_block;

typedef struct {
    cache_block* sets;
} cache;

int main(int argc, char** argv)
{
    short i;
    int opt;
    int numSets = 1;
    int blockNum = 1; 
    char* sval = 0;
    char* Eval = 0;
    char* bval = 0;
    char* tval = NULL;
    short hflag;
    short vflag;
    short sflag;
    short eflag;
    short bflag;

    printf("\n");
    while( (opt = getopt(argc, argv, "h::v::s:E:b:t:")) != -1) {
        switch(opt) {
            case 'h':
                printf("got h!");
                break;
            
            case 'v':
                printf("got v!");
                break;
            
            case 's':
                printf("got s!");
                sval = optarg;
                break;

            case 'E':
                printf("got E!");
                Eval = optarg;
                break;

            case 'b':
                printf("got b!");
                bval = optarg;
                break;

            case 't':
            printf("got t!");
                tval = optarg;
                break;

            default: 
                printf("not it");
                break;
        }
        printf(" | ");
    }

    blockNum = atoi(bval);

    for(int j = 0; j < atoi(sval); j++) {
        numSets *= 2;
    }

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

    //printSummary(0, 0, 0);
    return 0;
}
