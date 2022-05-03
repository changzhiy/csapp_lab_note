#include "cachelab.h"
#include<stdlib.h>
#include<stdio.h>
#include<getopt.h>
#include<string.h>
int help_mode, verbose_mode, s, E, b, S,number_hits, number_miss, number_eviction;
//S is the number of sets, E is the associativity, b is number of block bits
char filename[1000];
char buffer[1000];

typedef struct
{
    int valid_bit, tag, stamp;//cold miss
}cache_line;
cache_line **cache = NULL;

int get_max_stamp(int s_n){
    int max = cache[s_n][0].stamp;
    for (int i = 1; i < E; ++i) {
        if((cache[s_n][i].valid_bit == 1 && cache[s_n][i].stamp > max) || (cache[s_n][0].valid_bit == 0 && cache[s_n][i].valid_bit == 1))
            max = cache[s_n][i].stamp;
    }
    return max;
}

void update(unsigned int address)
{
    int max_stamp_id = -1;
    int  t_address, s_address;// The t value and s value of address
    s_address = (address >> b) & ((-1U) >> (32 - s));//use bit manipulation to get s_address, -1U equals to INT_MAX
    t_address = address >> (s + b);

    for(int i = 0; i < E; i++)//check whether there is a hit
        if(cache[s_address][i].tag == t_address)//which means a hit
        {
            //is_placed = 1;
            cache[s_address][i].stamp = 0;//restart the time stamp control unit
            number_hits++;
            //printf("hit\n");
            return ;//just return now
        }
    //to check whether is an empty line
    for(int i = 0; i < E; i++)
        if(cache[s_address][i].valid_bit == 0)
        {
            cache[s_address][i].valid_bit = 1;
            cache[s_address][i].tag = t_address;
            cache[s_address][i].stamp = 0;
            number_miss++;//compulsory miss
            //printf("miss\n");
            return;
        }
    //If there is not any empty line, then an eviction will occur
    number_eviction++;
    number_miss++;
    for(int i = 0; i < E; i++)
        if(cache[s_address][i].stamp== get_max_stamp(s_address))
        {
            max_stamp_id = i;
        }
    cache[s_address][max_stamp_id].tag = t_address;
    cache[s_address][max_stamp_id].stamp = 0;
    return;
}

void update_time(void)
{
    for(int i = 0; i < S; i++)
        for(int j = 0; j < E; j++)
            if(cache[i][j].valid_bit == 1)
                ++cache[i][j].stamp;
}

int main(int argc,char* argv[])
{
    int opt, temp; //The getopt return value
    char type; //type of a single trace record
    unsigned int address; //address of memory
    number_hits = number_miss = number_eviction = 0; //initialization
    while(-1 != (opt = (getopt(argc, argv, "hvs:E:b:t:"))))
    {
        switch(opt)
        {
            case 'h':help_mode = 1;
                break;
            case 'v':verbose_mode = 1;
                break;
            case 's':s = atoi(optarg);
                break;
            case 'E':E = atoi(optarg);
                break;
            case 'b':b = atoi(optarg);
                break;
            case 't':strcpy(filename, optarg);
                break;
        }
    }
    if(help_mode == 1)
    {
        system("cat help_info");//"help_info" is a text file containing help information
        exit(0);
    }
    FILE* fp = fopen(filename,"r");
    if(fp == NULL)
    {
        fprintf(stderr,"The File is wrong!\n");
        exit(-1);
    }
    S = (1 << s); // S equals to 2^s
    cache = (cache_line**)malloc(sizeof(cache_line*) * S);
    for(int i = 0; i < S; i++)
        cache[i] = (cache_line*)malloc(sizeof(cache_line) * E);//Important! malloc each row of cache
    for(int i = 0; i < S; i++)
        for(int j = 0; j < E; j++)
        {
            cache[i][j].valid_bit = 0;
            cache[i][j].tag = cache[i][j].stamp = -1;
        }//initialization

    while(fgets(buffer,1000,fp))//get a whole line
    {
        sscanf(buffer," %c %xu,%d", &type, &address, &temp);//hexdecimal
        switch(type)
        {
            case 'L':update(address);
                break;
            case 'M':update(address);//just let it fall through, do twice
            case 'S':update(address);
                break;
        }
        update_time();
    }
    for(int i = 0; i < S; i++)
        free(cache[i]);//free allocated space first
    free(cache);
    fclose(fp);
    printSummary(number_hits, number_miss, number_eviction);
    return 0;
}