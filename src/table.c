#include "table.h"

#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void initTable(Table* table) {
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void freeTable(Table* table) {
    FREE_ARRAY(Entry, table->entries, table->capacity);
    initTable(table);
}

// Finds the corresponding entry for the key-value pair that `key` corresponds
// to. If the bucket is taken, then linear probing is used to find the next
// available bucket.
static Entry* findEntry(Entry* entries, int capacity, ObjString* key) {
    uint32_t index = key->hash % capacity;
    Entry* tombstone = NULL;

    for (;;) {
        Entry* entry = &entries[index];
        if (entry->key == NULL) {
            if (IS_NIL(entry->value)) {  // Entry is not a tombstone.
                // If a tombstone was previously found, return it. Otherwise,
                // return the empty entry.
                return tombstone != NULL ? tombstone : entry;
            } else {  // Entry is a tombstone.
                if (tombstone == NULL) tombstone = entry;
            }
        } else if (entry->key == key) {
            return entry;
        }

        // Use linear probing to find the next bucket.
        index = (index + 1) % capacity;
    }
}

// Modifies the table to a new capacity, shifting entries to their new buckets.
static void adjustCapacity(Table* table, int capacity) {
    Entry* entries = ALLOCATE(Entry, capacity);
    for (int i = 0; i < capacity; i++) {
        entries[i].key = NULL;
        entries[i].value = NIL_VAL;
    }

    table->count = 0;
    for (int i = 0; i < table->capacity; i++) {
        Entry* entry = &table->entries[i];
        if (entry->key == NULL) continue;
        Entry* destination = findEntry(entries, capacity, entry->key);
        destination->key = entry->key;
        destination->value = entry->value;
        table->count++;
    }

    FREE_ARRAY(Entry, table->entries, table->capacity);
    table->entries = entries;
    table->capacity = capacity;
}

bool tableGet(Table* table, ObjString* key, Value* value) {
    if (table->count == 0) return false;  // Table is empty.

    Entry* entry = findEntry(table->entries, table->capacity, key);
    if (entry->key == NULL) return false;  // Table does not contain this key.

    *value = entry->value;  // Set the output parameter.
    return true;
}

bool tableSet(Table* table, ObjString* key, Value value) {
    // Grow (or allocate) memory for the table.
    if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(table->capacity);
        adjustCapacity(table, capacity);
    }

    // Find the original key-value pair, or finds where the new key-value pair
    // should be stored.
    Entry* entry = findEntry(table->entries, table->capacity, key);
    bool isNewKey = entry->key == NULL;
    // Increment count if entry is not a tombstone.
    if (isNewKey && IS_NIL(entry->value)) table->count++;

    // Set new key-value pair.
    entry->key = key;
    entry->value = value;
    return isNewKey;
}

bool tableDelete(Table* table, ObjString* key) {
    if (table->count == 0) return false;  // Cannot delete from empty table.

    Entry* entry = findEntry(table->entries, table->capacity, key);
    if (entry->key == NULL) return false;  // Key-value pair does not exit.

    // Replace the entry with a tombstone (NULL, true).
    entry->key = NULL;
    entry->value = BOOL_VAL(true);
    return true;
}

void tableAddAll(Table* from, Table* to) {
    for (int i = 0; i < from->capacity; i++) {
        Entry* entry = &from->entries[i];
        if (entry->key != NULL) {
            tableSet(to, entry->key, entry->value);
        }
    }
}