# KVcache Project: Caching for a Key-Value Store

## Objectives

* Implement a writeback software cache with an LRU replacement policy.
* Work with a chained hash table with a doubly-linked list for each bucket (bin).
* Handle LRU list updates for hits and writeback eviction for misses.
---

## Overview

This project uses a common structure for a software cache.

Take a look at the header file `kv_cache.h`.   It defines a KVPAIR struct type for a key-value pair.
KVPAIR is similar to the KVproj project, but it has a few new fields.   It has a
prev link as well as a next link, for use with a doubly linked list.   It also has a 
*modified* field: if it is true (nonzero) that indicates that the corresponding
value has been modified in the cache.

The `kv_cache.h` header also defines a KVCACHE struct type to represent a
bucket (bin) of a hash table.   The hash table indexes all  KVPAIRs in the cache.
It is an array of KVCACHE buckets (bins), each with a doubly linked list of
KVPAIRs in the bin.

You are provided with a storage interface:

* `read_key()` creates/fetches a KVPAIR for a key and returns a pointer.
* `write_key()` writes a modified KVPAIR back to storage.

The hash table has a bounded "chain depth" of *MAX_BIN_ELEMENTS* (4).
That means there can be at most *MAX_BIN_ELEMENTS* in each bin.   If a bin
list has *MAX_BIN_ELEMENTS* KVPAIRs on it, then it is full: it is necessary to
evict one KVPAIR from the bin before adding another.   Your code maintains each bin list in LRU 
order to aid in selecting a victim KVPAIR to evict.

---

## What to do

Your objective is to write the code for the `lookup` function in `kv_cache.c`. 
Please do not modify any other files.

**lookup**

Query the hash table for the key.  The hash function
to select the bin is a simple modulo (%).   it should spread the keys across all of the bins.

Search the bin list to determine if the request is a hit or a miss.  On a hit, return the KVPAIR*. 
On a miss, call `read_key` to get the KVPAIR and add it to the bin, then return it.

Both hit and miss cases involve some extra steps to maintain the bin's LRU list in the proper
order.   Also maintain the count of keys in each KVCACHE bin.

If the bin is full on a miss, then `lookup` must select a victim and evict it by removing it
from the bin before adding
the new KVPAIR to the bin's list.   If the victim has the *modified*
flag set (nonzero), then call `write_key` to write the victim KVPAIR back before eviction.

Track hits, misses, and writebacks.   The `kv_cache.h` header declares 3 global int values `hits`, `misses`, and `writebacks`.
You should increment `hits` when lookup finds the key in the cache, increment `misses`
when the key is not the cache, and increment `writebacks` whenever a modified
key is replaced from the cache and written back with `write_key`.

---

**Testing**

There are eight different tests provided, numbered 1 through 8, with increasing
complexity. Test number 0 runs all the tests. Run the tests individually via the command line for debugging and to check valgrind output.

Typing `make` on the command line builds the program` kv_cache_test` which includes your code from `kv_cache.c`
along with test harness code that is not visible to you.

Run the program on the command line with
```
./kv_cache_test -b <bins> -n <keypairs> -t <test>
```
Note that tests 1-5 ignore the keypair parameter.

You can also run the local python tests using
```
python3 hwtest.py ALL
```
The results of these local tests should be the same as those on the autograder.

---

**Debugging**

`kvcache.h` declares two printing functions to output
the state of your cache.  `printlist` outputs the contents of
a specified bin list, and `print_cache` outputs the entire contents
of the cache. Pair this with command line arguments for `kv_cache_test`
to run your program with small inputs and understand the cache contents
after each lookup.

---

**Submission**

- Submit your project on gradescope via gitlab in the usual way.
- Don't forget to include a file `statement.txt` summarizing
the collaboration and any sources of assistance.
