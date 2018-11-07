/**
 *This file is used to set up and run
 *htable.c and it's related files.
 *The program creates and fills a hashtable with
 *user input and carries out tasks based on given
 *command line interfaces
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "mylib.h"
#include "htable.h"

/**
 *This function is used to print out frequency and key
 *of a given index in a hashtable.
 *@param freq the frequency of the key
 *@param *word the key itself
 */
static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

/**
 *Function checks if a given integer is prime or not
 *@param n integer to be checked
 *@return int 1 if prime, 0 if not
 */
int is_prime(int n){
    int i;
    for(i = 2; n%i != 0; i++){
    }
    if(n == i){
        return 1;
    }else{
        return 0;
    }
}
/**
 *Function returns the first prime number that is greater
 *than or equal to an integer.
 *@param num given integer to find closest prime to.
 *@return int is the next prime number after the integer.
 */
int get_prime(int num){
    int i;

    for(i = num;;i++){
        if(is_prime(i)){
            return i;
        }
    }
}
/**
 *Prints out a help message to stderr.
 *@param *name is the name of the programme
 */
void help(char *name){
    fprintf(stderr,"Usage: %s [OPTION]... <STDIN>\n\n%s", name, 
            "Perform various operations using a hash-table.  By default, words are\n"
            "read from stdin and added to the hash table, before being printed out\n"
            "alongside their frequencies to stdout.\n\n"
            " -c FILENAME  Check spelling of words in FILENAME using words\n"
            "              from stdin as dictionary. Print unknown words to\n"
            "              stdout, timing info & count to stderr (ignore -p)\n"
            " -d           Use double hashing (linear probing is the default)\n"
            " -e           Display entire contents of hash-table on stderr\n");
    fprintf(stderr,
            " -p           Print stats info instead of frequencies & words\n"
            " -s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)\n"
            " -t TABLESIZE Use the first prime >= TABLESIZE as htable size\n\n"
            " -h           Display this message\n\n");
}

/**
 *Main function reads any command line arguments and runs
 *the programme accordingly. Keys are read in through stdin
 *and inserted to a hashtable. The data is then dealt with
 *based on the arguments given
 *@param argc is the number of arguments
 *@param argv is the arguments themselves
 *@return int specifying whether programme is successful or not
 */
int main(int argc, char **argv) {
    htable h;
    char word[256];
    const char *optstring = "c:deps:t:h";
    int capacity = 0, num_stats = 0, unknown=0;
    char option;
    char *filename = NULL;
    clock_t startfill, endfill, searchstart, searchend;
    FILE *file;
    int spell_check = 0, whole_table = 0, print_stats = 0, double_hashing = 0;

    
    while ((option = getopt(argc, argv, optstring)) != EOF){
        switch(option) {
            case 'c':
                spell_check = 1;
                filename = optarg;
                break;
            case 'd':
                double_hashing = 1;
                break;
            case 'e':
                whole_table = 1;
                break;
            case 'p':
                print_stats = 1;
                break;
            case 's':
                num_stats = atoi(optarg);
                break;
            case't':
                capacity = get_prime(atoi(optarg));
                break;
            case 'h':
                help(argv[0]);
                return EXIT_SUCCESS;
                break;
            default:
                help(argv[0]);
                return EXIT_SUCCESS;
                break;
                
        }
    }
    

    if(capacity < 1){
        capacity = 113;
    }
    h = htable_new(capacity, (double_hashing) ? DOUBLE_H : LINEAR_P);
    
    startfill = clock();
    while (getword(word, sizeof word, stdin) != EOF) {
        htable_insert(h, word);
    }
    endfill = clock();

    if(whole_table){
        htable_print_entire_table(h, stderr);
    }
    if(spell_check && filename != NULL){
        file = fopen(filename, "r");
        if (file != NULL){
            searchstart = clock();
            while (getword(word, sizeof word, file) != EOF) {
                if(htable_search(h, word) == 0){
                    printf("%s\n", word);
                    unknown++;
                }
            }
            searchend = clock();
            fprintf(stderr, "Fill time     : %f\n",
                    (endfill - startfill)/(double)CLOCKS_PER_SEC);
            fprintf(stderr, "Search time   : %f\n",
                    (searchend - searchstart)/(double)CLOCKS_PER_SEC);
            fprintf(stderr, "Unknown words = %d\n", unknown);
        }else{
            fprintf(stderr, "File %s not found.", filename);
        }
    }else  if(print_stats){
        if (num_stats <1) num_stats = 10;
        htable_print_stats(h, stdout, num_stats);
    }else{
        htable_print(h, print_info);
    }
    
    htable_free(h);
    return EXIT_SUCCESS;
}





            
            
    








