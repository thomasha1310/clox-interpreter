#ifndef CLOX_OBJECT_H
#define CLOX_OBJECT_H

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type);

// Checks if a Value is an `Obj` of type `OBJ_STRING`.
#define IS_STRING(value) isObjType(value, OBJ_STRING)

// Casts a string object Value to an `ObjString` pointer.
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
// Returns the raw C string (`char*`) from a string object Value.
#define AS_CSTRING(value) (AS_STRING(value)->chars)


typedef enum {
    OBJ_STRING,
} ObjType;

struct Obj {
    ObjType type;
};

struct ObjString {
    Obj obj;
    int length;
    char* chars;
};

ObjString* copyString(const char* chars, int length);

// Returns whether `value` is an `Obj` of type `type`.
static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif