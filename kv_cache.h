#ifndef _KVCACHE
#define _KVCACHE

// base KV pair structure
typedef struct KVP {
    long key;
    int modified;
    unsigned int size;
    char *val;
    struct KVP *prev;
    struct KVP *next;
} KVPAIR;

// A hash bin
typedef struct {
    unsigned int num_keys;
    KVPAIR *list;
} KVCACHE;

// limit on number of KV pairs per hash table bin
#define MAX_BIN_ELEMENTS 4

// Global variables keeping track of the running total of hits, misses
// and writebacks.
// Should be incremented by lookup
unsigned int hits;
unsigned int misses;
unsigned int writebacks; 

// Lookup a value in the hash table.
KVPAIR *lookup(KVCACHE *kv_cache, long key, unsigned int bins);

// STORAGE interface: read_key and write_key
// Provided for use by lookup().

// Read a key value pair (with the specified key) from storage
// Returns a pointer to the KVPAIR, NULL if failure
KVPAIR *read_key(long key);

// Write a modified KVPAIR back to storage
// Returns a 1 on success 0 on failure
int write_key(KVPAIR *kv_pair);

// UTILITIES
void printlist(KVPAIR *list); // this is a useful function for you to use

// print cache, prints out the entire cache use with on large tests
void print_cache(KVCACHE *cache, unsigned int bins);

// Functions used by the test harness
void run_tests(unsigned int num_pairs, unsigned int bins, unsigned int test);

#endif
