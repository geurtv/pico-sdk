
#include <stdlib.h>
#include <string.h>

#include "pico/mutex.h"

//------------------------------------------------------------------------------

// ripped from gcc-arm-none-eabi-10-2020-q4-major

typedef unsigned int word __attribute__((mode(word)));
typedef unsigned int pointer __attribute__((mode(pointer)));

struct __emutls_object {
	word size;
	word align;
	union {
		pointer offset;
		void *ptr;
	} loc;
	void *templ;
};

//------------------------------------------------------------------------------

struct emutls_array {
	void* data[NUM_CORES];
};

//------------------------------------------------------------------------------

auto_init_mutex(emutls_mutex);

//------------------------------------------------------------------------------

static void* emutls_alloc(const struct __emutls_object* obj)
{
	// FIXME: will pointer returned by malloc() always be suitably aligned?
	// FIXME: figure out what obj->align actually means
	void* ptr = malloc(obj->size /*+ obj->align - 1*/);
	// FIXME: panic when ptr == NULL?
	if (obj->templ) {
		memcpy(ptr, obj->templ, obj->size);
	}
	else {
		memset(ptr, 0, obj->size);
	}
	return ptr;
}

void* __wrap___emutls_get_address(struct __emutls_object* obj)
{
	// lock mutex: we don't know which core gets here first
	mutex_enter_blocking(&emutls_mutex);
	if (__builtin_expect(obj->loc.ptr == NULL, 0)) {
		obj->loc.ptr = calloc(1, sizeof(struct emutls_array));
		// FIXME: panic when obj->loc.ptr == NULL?
	}
	mutex_exit(&emutls_mutex);

	uint core = get_core_num();
	struct emutls_array* array = obj->loc.ptr;
	if (__builtin_expect(array->data[core] == NULL, 0)) {
		array->data[core] = emutls_alloc(obj);
	}
	return array->data[core];
}

//------------------------------------------------------------------------------
