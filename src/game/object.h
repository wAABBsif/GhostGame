#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef uint16_t object_id;

#define MAX_OBJECT_COUNT	1024

typedef uint16_t object_type;

#define OBJECT_TYPE_NONE	0	//Used for deletion
#define OBJECT_TYPE_TEST	1
#define OBJECT_TYPE_COUNT	2

#define OBJECT_TYPE_DELETION_MASK	INT16_MAX
#define OBJECT_TYPE_DELETION_BIT	(INT16_MAX + 1)

typedef struct object_base
{
	object_id	id;
	object_type type;
} object_base;

typedef void (*object_start_func)(void *generic_object);
typedef void (*object_update_func)(void *generic_object);
typedef void (*object_end_func)(void *generic_object);

typedef struct object_func_entry
{
	object_start_func start;
	object_update_func update;
	object_end_func end;
} object_func_entry;

#define JOIN(a, b) a##b
#define FUNC_ENTRY_FROM_NAME(func) {JOIN(func, _start), JOIN(func, _update), JOIN(func, _end)}

void object_init(void);
void object_update(void);
void object_terminate(void);

//do not store this pointer for more than a single frame
void *object_get_ptr(object_id object);
object_id object_get_id(const void *ptr);

object_id object_add(const size_t size, const object_type type);
//object is queued for destruction and is not destroyed immediately
bool object_destroy(object_id object);