
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "kv_cache.h"

/*
Take a look at the header file kv_cache.h.   It defines a KVPAIR struct type for a key-value pair.
KVPAIR is similar to the KVproj project, but it has a few new fields.   It has a
prev link as well as a next link, for use with a doubly linked list.   It also has a
modified field: if it is true (nonzero) that indicates that the corresponding
value has been modified in the cache.
The kv_cache.h header also defines a KVCACHE struct type to represent a
bucket (bin) of a hash table.   The hash table indexes all  KVPAIRs in the cache.
It is an array of KVCACHE buckets (bins), each with a doubly linked list of
KVPAIRs in the bin.
You are provided with a storage interface:


read_key() creates/fetches a KVPAIR for a key and returns a pointer.

write_key() writes a modified KVPAIR back to storage.

The hash table has a bounded "chain depth" of MAX_BIN_ELEMENTS (4).
That means there can be at most MAX_BIN_ELEMENTS in each bin.   If a bin
list has MAX_BIN_ELEMENTS KVPAIRs on it, then it is full: it is necessary to
evict one KVPAIR from the bin before adding another.   Your code maintains each bin list in LRU
order to aid in selecting a victim KVPAIR to evict.
*/

/*

Lookup a key in the KV cache: search the list for its bin.
Return: pointer to KVPAIR with the desired key

Hit: update LRU order for the bin list to put the key's KVPAIR at the front.   

Miss: call read_key() to get a KVPAIR* for the key (e.g., fetch value
from storage).  Add the KVPAIR to the bin list.  If the bin is already full
(MAX_BIN_ELEMENTS), then select a victim KVPAIR from the bin using the LRU
replacement policy.  If the victim KVPAIR has been modified (dirty),
then call write_key() to write the changes back to storage.   Evict victim
by removing it from the bin.

*/

KVPAIR *lookup(KVCACHE *kv_cache, long key, unsigned int bins) {
    long binIndex = key % bins;
   // KVCACHE *a = &kv_cache[binIndex];
    
    KVPAIR *curr = kv_cache[binIndex].list;
    KVPAIR *prev = NULL;
    //try to find the key
    while (curr != NULL) {
       if(curr->key == key){
            hits++;
            if(prev != NULL){
                prev->next = curr->next;
                if (curr->next != NULL) {
                    curr->next->prev = prev;
                }
                curr->next = kv_cache[binIndex].list;
                if (kv_cache[binIndex].list != NULL) {
                    kv_cache[binIndex].list->prev = curr;
                }
                kv_cache[binIndex].list = curr;
                curr->prev = NULL;
            }
            return curr;
       }
       prev = curr;
       curr = curr->next;
    }
    misses++;
   //if we are replacing
    if (kv_cache[binIndex].num_keys >= MAX_BIN_ELEMENTS){
        KVPAIR *new = read_key(key);
        KVPAIR *victim = kv_cache[binIndex].list;
        while (victim->next != NULL) {
            victim = victim->next;
        }
        if (victim->modified != 0){
            write_key(victim);
            writebacks++;
        }
        if (victim->prev != NULL) {
            victim->prev->next = NULL;
        } else {
            kv_cache[binIndex].list = NULL;
        }
        kv_cache[binIndex].num_keys--;
        /*
        ong key;
    int modified;
    unsigned int size;
    char *val;
    struct KVP *prev;
    struct KVP *next;
        */
        //free(victim->next);
        //free(victim->prev);
        free(victim->val);
        free(victim);
        new->next = kv_cache[binIndex].list;
        if (kv_cache[binIndex].list != NULL) {
            kv_cache[binIndex].list->prev = new;
        }
        kv_cache[binIndex].list = new;
        new->prev = NULL;
        kv_cache[binIndex].num_keys++;
        return new;
    }
    else { //add new key without replacing
        kv_cache[binIndex].num_keys++;
        KVPAIR *new = read_key(key);
        new->prev = NULL;
        new->next = kv_cache[binIndex].list;
        if (kv_cache[binIndex].list != NULL) {
            kv_cache[binIndex].list->prev = new;
        }
        kv_cache[binIndex].list = new;
        return new;
    }
    return NULL;
}
