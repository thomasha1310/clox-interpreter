#ifndef CLOX_TABLE_H
#define CLOX_TABLE_H

#include "common.h"
#include "value.h"

typedef struct {
    ObjString* key;
    Value value;
} Entry;

typedef struct {
    int count;
    int capacity;
    Entry* entries;
} Table;

// Reset `table` to have a count and capacity of 0 with `NULL` entries.
void initTable(Table* table);

// Frees `table->entries` and calls `initTable`.
void freeTable(Table* table);

// Sets the specified key-value pair in `table`, creating a new entry if one
// does not exist.
bool tableSet(Table* table, ObjString* key, Value value);

// Adds all entries from `from` to `to`.
void tableAddAll(Table* from, Table* to);

#endif