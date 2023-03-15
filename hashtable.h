// Simple hash table implemented in C.

#ifndef _hashtable_H
#define _hashtable_H

#include <stdbool.h>
#include <stddef.h>

// Hash table structure: create with hashtable_create, free with hashtable_destroy.
typedef struct hashtable hashtable;

// Create hash table and return pointer to it, or NULL if out of memory.
hashtable* hashtable_create(void);

// Free memory allocated for hash table, including allocated keys.
void hashtable_destroy(hashtable* table);

// Get item with given key (NUL-terminated) from hash table. Return
// value (which was set with hashtable_set), or NULL if key not found.
void* hashtable_get(hashtable* table, const char* key);

// Set item with given key (NUL-terminated) to value (which must not
// be NULL). If not already present in table, key is copied to newly
// allocated memory (keys are freed automatically when hashtable_destroy is
// called). Return address of copied key, or NULL if out of memory.
const char* hashtable_set(hashtable* table, const char* key, void* value);

// Return number of items in hash table.
size_t hashtable_length(hashtable* table);

// Hash table iterator: create with hashtable_iterator, iterate with hashtable_next.
typedef struct {
    const char* key;  // current key
    void* value;      // current value

    // Don't use these fields directly.
    hashtable* _table;       // reference to hash table being iterated
    size_t _index;    // current index into hashtable._entries
} hashtablei;

// Return new hash table iterator (for use with hashtable_next).
hashtablei hashtable_iterator(hashtable* table);

// Move iterator to next item in hash table, update iterator's key
// and value to current item, and return true. If there are no more
// items, return false. Don't call hashtable_set during iteration.
bool hashtable_next(hashtablei* it);

#endif // _hashtable_H