#include "return_codes.h"

VALUE return_codes[MEMCACHED_MAXIMUM_RETURN+1];
void
Init_return_codes()
{
  return_codes[MEMCACHED_SUCCESS] = ID2SYM(rb_intern("success"));
  return_codes[MEMCACHED_FAILURE] = ID2SYM(rb_intern("failure"));
  return_codes[MEMCACHED_HOST_LOOKUP_FAILURE] = ID2SYM(rb_intern("host_lookup_failure"));
  return_codes[MEMCACHED_CONNECTION_FAILURE] = ID2SYM(rb_intern("connection_failure"));
  return_codes[MEMCACHED_CONNECTION_BIND_FAILURE] = ID2SYM(rb_intern("connection_bind_failure"));
  return_codes[MEMCACHED_READ_FAILURE] = ID2SYM(rb_intern("read_failure"));
  return_codes[MEMCACHED_UNKNOWN_READ_FAILURE] = ID2SYM(rb_intern("unknown_read_failure"));
  return_codes[MEMCACHED_PROTOCOL_ERROR] = ID2SYM(rb_intern("protocol_error"));
  return_codes[MEMCACHED_CLIENT_ERROR] = ID2SYM(rb_intern("client_error"));
  return_codes[MEMCACHED_SERVER_ERROR] = ID2SYM(rb_intern("server_error"));
  return_codes[MEMCACHED_WRITE_FAILURE] = ID2SYM(rb_intern("write_failure"));
  return_codes[MEMCACHED_ERROR] = ID2SYM(rb_intern("error"));
  return_codes[MEMCACHED_DATA_EXISTS] = ID2SYM(rb_intern("data_exists"));
  return_codes[MEMCACHED_DATA_DOES_NOT_EXIST] = ID2SYM(rb_intern("data_does_not_exist"));
  return_codes[MEMCACHED_NOTSTORED] = ID2SYM(rb_intern("notstored"));
  return_codes[MEMCACHED_STORED] = ID2SYM(rb_intern("stored"));
  return_codes[MEMCACHED_NOTFOUND] = ID2SYM(rb_intern("notfound"));
  return_codes[MEMCACHED_MEMORY_ALLOCATION_FAILURE] = ID2SYM(rb_intern("memory_allocation_failure"));
  return_codes[MEMCACHED_PARTIAL_READ] = ID2SYM(rb_intern("partial_read"));
  return_codes[MEMCACHED_SOME_ERRORS] = ID2SYM(rb_intern("some_errors"));
  return_codes[MEMCACHED_NO_SERVERS] = ID2SYM(rb_intern("no_servers"));
  return_codes[MEMCACHED_END] = ID2SYM(rb_intern("end"));
  return_codes[MEMCACHED_DELETED] = ID2SYM(rb_intern("deleted"));
  return_codes[MEMCACHED_VALUE] = ID2SYM(rb_intern("value"));
  return_codes[MEMCACHED_STAT] = ID2SYM(rb_intern("stat"));
  return_codes[MEMCACHED_ITEM] = ID2SYM(rb_intern("item"));
  return_codes[MEMCACHED_ERRNO] = ID2SYM(rb_intern("errno"));
  return_codes[MEMCACHED_FAIL_UNIX_SOCKET] = ID2SYM(rb_intern("fail_unix_socket"));
  return_codes[MEMCACHED_NOT_SUPPORTED] = ID2SYM(rb_intern("not_supported"));
  return_codes[MEMCACHED_FETCH_NOTFINISHED] = ID2SYM(rb_intern("fetch_notfinished"));
  return_codes[MEMCACHED_NO_KEY_PROVIDED] = ID2SYM(rb_intern("no_key_provided"));
  return_codes[MEMCACHED_BUFFERED] = ID2SYM(rb_intern("buffered"));
  return_codes[MEMCACHED_TIMEOUT] = ID2SYM(rb_intern("timeout"));
  return_codes[MEMCACHED_BAD_KEY_PROVIDED] = ID2SYM(rb_intern("bad_key_provided"));
  return_codes[MEMCACHED_INVALID_HOST_PROTOCOL] = ID2SYM(rb_intern("invalid_host_protocol"));
  return_codes[MEMCACHED_SERVER_MARKED_DEAD] = ID2SYM(rb_intern("server_marked_dead"));
  return_codes[MEMCACHED_UNKNOWN_STAT_KEY] = ID2SYM(rb_intern("unknown_stat_key"));
  return_codes[MEMCACHED_E2BIG] = ID2SYM(rb_intern("e2big"));
  return_codes[MEMCACHED_INVALID_ARGUMENTS] = ID2SYM(rb_intern("invalid_arguments"));
  return_codes[MEMCACHED_KEY_TOO_BIG] = ID2SYM(rb_intern("key_too_big"));
  return_codes[MEMCACHED_AUTH_PROBLEM] = ID2SYM(rb_intern("auth_problem"));
  return_codes[MEMCACHED_AUTH_FAILURE] = ID2SYM(rb_intern("auth_failure"));
  return_codes[MEMCACHED_AUTH_CONTINUE] = ID2SYM(rb_intern("auth_continue"));
  return_codes[MEMCACHED_PARSE_ERROR] = ID2SYM(rb_intern("parse_error"));
  return_codes[MEMCACHED_PARSE_USER_ERROR] = ID2SYM(rb_intern("parse_user_error"));
  return_codes[MEMCACHED_DEPRECATED] = ID2SYM(rb_intern("deprecated"));
  return_codes[MEMCACHED_IN_PROGRESS] = ID2SYM(rb_intern("in_progress"));
  return_codes[MEMCACHED_SERVER_TEMPORARILY_DISABLED] = ID2SYM(rb_intern("server_temporarily_disabled"));
  return_codes[MEMCACHED_SERVER_MEMORY_ALLOCATION_FAILURE] = ID2SYM(rb_intern("server_memory_allocation_failure"));
  return_codes[MEMCACHED_MAXIMUM_RETURN] = ID2SYM(rb_intern("maximum_return"));
}
