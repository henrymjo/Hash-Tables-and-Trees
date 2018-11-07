
/**
 *This is an ADT that is a representation of a hashtable, The methods below prints information on.
 *the hash table as data is inserted. Data is added to the table by either using linear probing or
 *double hashing methods.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

/**
 *This struct consists of datafields that make up the hashtable.
 */
   
struct htablerec {
    char **keys; /*This is an array consistings of characters that represent keys/stirngs*/
    int *frequencies;/*The integer stores the frequency of the inputed data */
    int *stats;/*The integer stores the amount of Collisions before a cell is filled*/
    int num_keys;/* The number of keys in the hash table*/
    int capacity;/* The capacity that the hash table can hold*/
    hashing_t method;/* This will pass the collision resolution method which will fill the hash table*/
};

/**
 *This is a constructor method that initialises a new hash table. The parameters
 *included are type integer,capacity and type hashing_t, method.
 *@param capacity sets the maximum capacity for the hashtable.
 *@param method sets the collision resolution method the hashtable will use as data is inputed.
 *@return result, returns a hastable initialised with a capacity, method, frequencies and stats
 *set to 0, and keys set at NULL.
 */
htable htable_new(int capacity, hashing_t method){
    htable result = emalloc(sizeof *result);
    int i;
    result->capacity = capacity;
    result->num_keys = 0;
    result->keys = emalloc(capacity * sizeof result->keys[0]);
    result->frequencies = emalloc(capacity * sizeof result->frequencies[0]);
    result->stats = emalloc(capacity * sizeof result->stats[0]);
    result->method = method;

    for (i=0; i < capacity; i++){
        result->keys[i]= NULL;
        result->frequencies[i]=0;
        result->stats[i] = 0;
    }
    return result;

}

/**
 *This method calculates the step which is used in double hashing.
 *This is done using the key and the htable's capacity
 *@param h is the htable we are retrieving a step for
 *@param i_key the the key for a partiular word that is from the
 *htable_word_to_int() method.
 *@return an integer the stores the step for double hashing.
 */
static unsigned int htable_step(htable h, unsigned int i_key){
    return 1 + (i_key % (h->capacity-1));
}

/**
 *A method to free the memory the is used in the hashtable arrays and its.
 *paramters
 *@param h is the hash table that needs to be freed
 */
void htable_free(htable h){
    int i;
    for (i=0; i < h->capacity; i++){
        free(h->keys[i]);
    }
    free (h->stats);
    free (h->keys);
    free (h->frequencies);
    free (h);
}

/**
 *A method that converts a word to a corresponding integer/number.
 *@param word is the word we want to convert to int
 *@return result is a integer that is an index to the word
 */
static unsigned int htable_word_to_int(char *word) {
    unsigned int result= 0;

    while(*word != '\0'){
        result = (*word++ + 31 * result);
    }
    return result;
}

/**
 *A method that trys to insert a word into a certain index in the hashtable.
 *the meothod will continue tracking through unless it comes to either an empty cell
 *to insert into or it has tracked through the whole array
 *@param h is the hashtable that we want to insert the word into
 *@param word is the word we want to insert
 *@return is the number of times the words occurs in the hashtable
 */
int htable_insert(htable h, char *word){
    unsigned int conversion = htable_word_to_int(word);
    int index = (conversion % h->capacity);
    unsigned int step = htable_step(h, htable_word_to_int(word));
    int collisions = 0;
    while (collisions < h->capacity){
        if (0 == h->frequencies[index]) {
            h->keys[index] = emalloc(sizeof *word);
            strcpy(h->keys[index], word);
            h->frequencies[index]++;
            h->stats[h->num_keys] = collisions;
            h->num_keys++;
            return 1;
        } else if (strcmp(word, h->keys[index]) == 0){
            h->frequencies[index]++;
            return h->frequencies[index];
        }
        if (h->method != LINEAR_P){
            index = (index + step) % h->capacity;
        } else {
            index = index + 1;
            if(index == h->capacity){
                index = 0;
            }
        }
        collisions ++;
    }
    return 0;
}

/**
 *This method prints the keys in the hash table but.
 *will not print NULL keys
 *@param freq the frequency in keys
 *@param h the hatble we are printing from
 *@param word the word that will be printed
 */
void htable_print(htable h, void f(int freq, char *word)){
    int i;
    for (i=0; i < h->capacity; i++){
        if (h->keys[i] != NULL){
            f(h->frequencies[i], h->keys[i]);
        }
    }
}

/**
 *This method searches the hash table for a prticular word and.
 *how many times it occurs in the hash table
 *@param h the hash table we are searching in
 *@param word the word we are looking for
 *@return returns the frequency of the particular word we were searching for
 */
int htable_search(htable h, char *word){
    int collisions = 0;
    unsigned int conversion = htable_word_to_int(word);
    unsigned int step = htable_step(h, htable_word_to_int(word));
    int key = (conversion % h->capacity);


    while(collisions < h->capacity && h->keys[key] != NULL && strcmp(h->keys[key], word) != 0){
        if (h->method != LINEAR_P){
            key = (key +step) % h->capacity;
        } else {
            key++;
            if(key == h->capacity){
                key = 0;
            }
        }
        collisions++;
    }
    if(collisions == h->capacity){
        return 0;
    }else{
        return h->frequencies[key];
    }
}
 
/**
 *The method prints the entire hashtable listing.
 *the fequency of the words their position in the table
 *the amount of collisions and word name
 *@param h The hash table that is being printed
 *@param stream the stream that we're printing to
 */
void htable_print_entire_table(htable h, FILE *stream){
    int i;
    fprintf(stream, "  Pos  Freq  Stats  Word\n"
            "----------------------------------------\n");
    for (i=0; i < h->capacity; i++){
        fprintf(stream,"%5d %5d %5d   %s\n", i, h->frequencies[i], h->stats[i],
                NULL == h->keys[i] ? "" : h->keys[i]);
    }
}

/**
 *This method is mainly used for the stats of the associated hash table.
 *according to the state at which its a certain perrcentage full to it's
 *capacity. Method prints out the percentage full the hash table is
 *the maximum and average amount of collisions per key
 *the amount of keys that are in the table at a certain point
 *the percentage of the keys placed at "home"
 *@param h the hash table we are working with
 *@param stream the stream we are printing to
 *@param percent_full The state/percentage full of the hash table at
 *the time we are printing the stats out for it. Nothing is printed if
 *hash table is full.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full){
    int current_entries = h->capacity * percent_full/100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    int i=0;

    if (current_entries > 0 && current_entries <= h->num_keys){
        for (i=0; i< current_entries; i++){
            if (h->stats[i] == 0) {
                at_home++;
            }
            if (h->stats[i] > max_collisions){
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }
        fprintf(stream, "%4d %10d %10.1f %10.2f %11d\n", percent_full,
                current_entries, at_home * 100.0/ current_entries,
                average_collisions / current_entries, max_collisions);
    }
}

/**
 *This method is for printing the number of stats, as well as each stat representing
 *the state of the hash table at a particular percengtage full
 *@param h the hash table we are printing the stats for
 *@param stream the stream getting printed to
 *@param num_stats the number of states/snapshots we want to take
 *of the hash table at particular percentages.
 */
void htable_print_stats(htable h, FILE *stream, int num_stats){
    int i;

    fprintf(stream, "\n%s\n\n",
            h->method == LINEAR_P ? "Linear Probing" : "Double Hashing");
    fprintf(stream, "Percent   Current   Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries   At Home   Collisions   Collisions\n");
    fprintf(stream, "-----------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "-----------------------------------------------------\n\n");
}


