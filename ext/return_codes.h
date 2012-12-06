#include <ruby.h>
#include <libmemcached/memcached.h>
#include <libmemcached-1.0/return.h>

extern VALUE return_codes[MEMCACHED_MAXIMUM_RETURN+1];
void Init_return_codes();
