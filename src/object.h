#ifndef CLOX_OBJECT_H
#define CLOX_OBJECT_H

#include "chunk.h"
#include "common.h"
#include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define IS_STRING(value) isObjType(value, OBJ_STRING)

// Casts a function object Value to an `ObjFunction` pointer.
#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))
// Casts a Lox native function object Value to its C function pointer.
#define AS_NATIVE(value) (((ObjNative*)AS_OBJ(value))->function)
// Casts a string object Value to an `ObjString` pointer.
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
// Returns the raw C string (`char*`) from a string object Value.
#define AS_CSTRING(value) (AS_STRING(value)->chars)

typedef enum {
    OBJ_FUNCTION,
    OBJ_NATIVE,
    OBJ_STRING,
} ObjType;

struct Obj {
    ObjType type;
    struct Obj* next;
};

typedef struct {
    Obj obj;
    int arity;
    Chunk chunk;
    ObjString* name;
} ObjFunction;

typedef Value (*NativeFn)(int argCount, Value* args);

typedef struct {
    Obj obj;
    NativeFn function;
} ObjNative;

struct ObjString {
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};

// Creates a new Lox function.
ObjFunction* newFunction();
// Creates a new native function.
ObjNative* newNative(NativeFn function);
// Creates and returns a pointer to a new `ObjString`, assuming that the
// function can take ownership of `chars`.
ObjString* takeString(char* chars, int length);
// Creates and returns a pointer to a new `ObjString` by copying a character
// array into newly allocated heap memory.
ObjString* copyString(const char* chars, int length);

void printObject(Value value);

// Returns whether `value` is an `Obj` of type `type`.
static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif