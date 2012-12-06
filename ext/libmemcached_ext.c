#include <ruby.h>
#include <libmemcached/memcached.h>
#include <libmemcached-1.0/return.h>
#include "return_codes.h"

VALUE rb_mLibmemcached;
VALUE rb_cNativeClient;
VALUE rb_eConfigurationError;
ID id_struct;

VALUE
lm_initialize(VALUE self, VALUE config)
{
  memcached_return_t ret;
  char errors[2048];

  Check_Type(config, T_STRING);
  ret = libmemcached_check_configuration(RSTRING_PTR(config), RSTRING_LEN(config), errors, sizeof(errors));
  if (!memcached_success(ret))
    rb_raise(rb_eConfigurationError, "%s", errors);

  memcached_st *memc = memcached(RSTRING_PTR(config), RSTRING_LEN(config));
  rb_ivar_set(self, id_struct, Data_Wrap_Struct(rb_cObject, NULL, memcached_free, memc));

  return self;
}

/* Delete a key.
 *
 *   key - String key to delete
 *
 * Returns true when key is deleted
 * Returns false when key does not exist
 * Returns a Symbol error code otherwise
 */
VALUE
lm_delete(VALUE self, VALUE key)
{
  memcached_st *memc;
  Data_Get_Struct(rb_ivar_get(self, id_struct), memcached_st, memc);

  memcached_return_t ret;

  ret = memcached_delete(memc, RSTRING_PTR(key), RSTRING_LEN(key), 0);
  if (memcached_success(ret))
    return Qtrue;
  else if (ret == MEMCACHED_NOTFOUND)
    return Qfalse;
  else
    return return_codes[ret];
}

/* Get a key
 *
 *   key - String key to retrieve
 *
 * Returns an Array on success
 *   value - String value for key
 *   flags - Fixnum flags associated with value
 * Returns nil when key is not found
 * Returns a Symbol error code otherwise
 */
VALUE
lm_get(VALUE self, VALUE key)
{
  memcached_st *memc;
  Data_Get_Struct(rb_ivar_get(self, id_struct), memcached_st, memc);

  memcached_return_t ret;
  uint32_t flags = 0;
  size_t value_length = 0;
  char *value;

  value = memcached_get(memc, RSTRING_PTR(key), RSTRING_LEN(key), &value_length, &flags, &ret);
  if (memcached_success(ret))
    return rb_ary_new3(2, rb_str_new(value, value_length), LONG2FIX(flags));
  else if (ret == MEMCACHED_NOTFOUND)
    return Qnil;
  else
    return return_codes[ret];
}

/* Set a key
 *
 *   key   - String key to set
 *   value - String value to set
 *   ttl   - Optional Fixnum time to expire key
 *   flags - Optional Fixnum flags to set
 *
 * Returns true on success
 * Returns false when key is not stored
 * Returns a Symbol error code otherwise
 */
VALUE
lm_set(int argc, VALUE *argv, VALUE self)
{
  memcached_st *memc;
  Data_Get_Struct(rb_ivar_get(self, id_struct), memcached_st, memc);

  VALUE key, val, ttl, flags;
  switch (rb_scan_args(argc, argv, "22", &key, &val, &ttl, &flags)) {
    case 2:
      ttl = INT2FIX(0);
    case 3:
      flags = INT2FIX(0);
  }

  memcached_return_t ret;
  ret = memcached_set(memc, RSTRING_PTR(key), RSTRING_LEN(key), RSTRING_PTR(val), RSTRING_LEN(val), FIX2ULONG(ttl), FIX2UINT(flags));
  if (memcached_success(ret))
    return Qtrue;
  else if (ret == MEMCACHED_NOTSTORED)
    return Qfalse;
  else
    return return_codes[ret];
}

/* Retrieve multiple keys
 *
 *   keys - Array of String keys
 *
 * Returns a tuple
 *   error_code - :success or Symbol error code if mget was interrupted
 *   values     - Hash of key => [value, flags] pairs retrieved
 */
VALUE
lm_mget(VALUE self, VALUE arg)
{
  memcached_st *memc;
  Data_Get_Struct(rb_ivar_get(self, id_struct), memcached_st, memc);

  Check_Type(arg, T_ARRAY);
  size_t n;
  size_t num_keys = RARRAY_LEN(arg);
  char  *keys[num_keys];
  size_t lens[num_keys];
  for (n=0; n<num_keys; n++) {
    keys[n] = RSTRING_PTR(RARRAY_PTR(arg)[n]);
    lens[n] = RSTRING_LEN(RARRAY_PTR(arg)[n]);
  }

  memcached_return_t ret;
  ret = memcached_mget(memc, (const char * const *)keys, (const size_t *)lens, num_keys);
  if (!memcached_success(ret))
    return Qfalse;

  VALUE values = rb_hash_new();
  memcached_result_st results_obj;
  memcached_result_st *results = memcached_result_create(memc, &results_obj);
  while ((results = memcached_fetch_result(memc, results, &ret)) != NULL) {
    if (ret == MEMCACHED_END)
      break;
    else if (memcached_continue(ret))
      continue;
    else if (ret == MEMCACHED_SUCCESS) {
      const char *key = memcached_result_key_value(results);
      size_t key_length = memcached_result_key_length(results);
      const char *value = memcached_result_value(results);
      size_t value_length = memcached_result_length(results);
      uint32_t flags = memcached_result_flags(results);

      /* rb_hash_aset(values, rb_str_new(key, key_length), rb_str_new(value, value_length));*/
      rb_hash_aset(values, rb_str_new(key, key_length), rb_ary_new3(2, rb_str_new(value, value_length), LONG2FIX(flags)));
    } else
      break;
  }
  memcached_result_free(&results_obj);

  return rb_ary_new3(2, return_codes[ret], values);
}

void
Init_libmemcached_ext()
{
  Init_return_codes();

  rb_mLibmemcached = rb_define_module("Libmemcached");
  rb_cNativeClient = rb_define_class_under(rb_mLibmemcached, "NativeClient", rb_cObject);
  rb_eConfigurationError = rb_define_class_under(rb_mLibmemcached, "ConfigurationError", rb_eStandardError);

  rb_define_method(rb_cNativeClient, "initialize", lm_initialize, 1);
  rb_define_method(rb_cNativeClient, "delete", lm_delete, 1);
  rb_define_method(rb_cNativeClient, "get", lm_get, 1);
  rb_define_method(rb_cNativeClient, "set", lm_set, -1);
  rb_define_method(rb_cNativeClient, "mget", lm_mget, 1);

  id_struct = rb_intern("struct");
}