/* { dg-require-effective-target int128 } */
/* { dg-require-effective-target sync_char_short } */
/* { dg-require-effective-target sync_int_long_stack } */
/* { dg-require-effective-target sync_int_long } */

#include <stdint.h>

#ifndef ANALYZER_DECLS_H
#define ANALYZER_DECLS_H

#ifndef NULL
#ifdef __cplusplus
#if __cplusplus >= 201103L
#define NULL nullptr
#else
#define NULL 0
#endif
#else
#define NULL ((void *)0)
#endif
#endif

/* Function decls with special meaning to the analyzer.
   None of these are actually implemented.  */

/* Trigger a breakpoint in the analyzer when reached.  */
extern void __analyzer_break (void);

/* Emit a warning describing the 2nd argument (which can be of any
   type), at the given verbosity level.  This is for use when
   debugging, and may be of use in DejaGnu tests.  */
extern void __analyzer_describe (int verbosity, ...);

/* Dump copious information about the analyzer’s state when reached.  */
extern void __analyzer_dump (void);

/* Emit a warning describing the size of the base region of (*ptr).  */
extern void __analyzer_dump_capacity (const void *ptr);

/* Dump information about what decls have escaped at this point on the path.  */
extern void __analyzer_dump_escaped (void);

/* Dump information after analysis on all of the exploded nodes at this
   program point.

   __analyzer_dump_exploded_nodes (0);
   will dump just the number of nodes, and their IDs.

   __analyzer_dump_exploded_nodes (1);
   will also dump all of the states within those nodes.  */
extern void __analyzer_dump_exploded_nodes (int);

/* Emit a warning describing what is known about the value of NAME.  */
extern void __analyzer_dump_named_constant (const char *name);

/* Emit a placeholder "note" diagnostic with a path to this call site,
   if the analyzer finds a feasible path to it.  */
extern void __analyzer_dump_path (void);

/* Dump the region_model's state to stderr.  */
extern void __analyzer_dump_region_model (void);

/* Emit a warning describing the state of the 2nd argument
   (which can be of any type) with respect to NAME.
   This is for use when debugging, and may be of use in DejaGnu tests.  */
extern void __analyzer_dump_state (const char *name, ...);

/* Emit a warning with text "TRUE", FALSE" or "UNKNOWN" based on the
   truthfulness of the argument.  */
extern void __analyzer_eval (int);

/* Obtain an "unknown" void *.  */
extern void *__analyzer_get_unknown_ptr (void);

/* Complain if PTR doesn't point to a null-terminated string.
   TODO: eventually get the strlen of the buffer (without the
   optimizer touching it).  */
extern __SIZE_TYPE__ __analyzer_get_strlen (const char *ptr);

#endif /* #ifndef ANALYZER_DECLS_H.  */

/* __atomic_exchange.  */

void test__atomic_exchange_on_int8 (int8_t i, int8_t j)
{
  int8_t orig_i = i;
  int8_t orig_j = j;
  int8_t ret;
  __atomic_exchange (&i, &j, &ret, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

void test__atomic_exchange_on_int16 (int16_t i, int16_t j)
{
  int16_t orig_i = i;
  int16_t orig_j = j;
  int16_t ret;
  __atomic_exchange (&i, &j, &ret, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

void test__atomic_exchange_on_int32 (int32_t i, int32_t j)
{
  int32_t orig_i = i;
  int32_t orig_j = j;
  int32_t ret;
  __atomic_exchange (&i, &j, &ret, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

void test__atomic_exchange_on_int64 (int64_t i, int64_t j)
{
  int64_t orig_i = i;
  int64_t orig_j = j;
  int64_t ret;
  __atomic_exchange (&i, &j, &ret, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

void test__atomic_exchange_on_int128 (__int128 i, __int128 j)
{
  __int128 orig_i = i;
  __int128 orig_j = j;
  __int128 ret;
  __atomic_exchange (&i, &j, &ret, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

/* __atomic_exchange_n.  */

void test__atomic_exchange_n_on_int8 (int8_t i, int8_t j)
{
  int8_t orig_i = i;
  int8_t orig_j = j;
  int8_t ret;
  ret = __atomic_exchange_n (&i, j, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

void test__atomic_exchange_n_on_int16 (int16_t i, int16_t j)
{
  int16_t orig_i = i;
  int16_t orig_j = j;
  int16_t ret;
  ret = __atomic_exchange_n (&i, j, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

void test__atomic_exchange_n_on_int32 (int32_t i, int32_t j)
{
  int32_t orig_i = i;
  int32_t orig_j = j;
  int32_t ret;
  ret = __atomic_exchange_n (&i, j, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

void test__atomic_exchange_n_on_int64 (int64_t i, int64_t j)
{
  int64_t orig_i = i;
  int64_t orig_j = j;
  int64_t ret;
  ret = __atomic_exchange_n (&i, j, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

void test__atomic_exchange_n_on_int128 (__int128 i, __int128 j)
{
  __int128 orig_i = i;
  __int128 orig_j = j;
  __int128 ret;
  ret = __atomic_exchange_n (&i, j, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

/* __atomic_exchange_1 through __atomic_exchange_16.  */

void test__atomic_exchange_1 (int8_t i, int8_t j)
{
  int8_t orig_i = i;
  int8_t orig_j = j;
  int8_t ret;
  ret = __atomic_exchange_1 (&i, j, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

void test__atomic_exchange_2 (int16_t i, int16_t j)
{
  int16_t orig_i = i;
  int16_t orig_j = j;
  int16_t ret;
  ret = __atomic_exchange_2 (&i, j, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

void test__atomic_exchange_4 (int32_t i, int32_t j)
{
  int32_t orig_i = i;
  int32_t orig_j = j;
  int32_t ret;
  ret = __atomic_exchange_4 (&i, j, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

void test__atomic_exchange_8 (int64_t i, int64_t j)
{
  int64_t orig_i = i;
  int64_t orig_j = j;
  int64_t ret;
  ret = __atomic_exchange_8 (&i, j, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

void test__atomic_exchange_16 (__int128 i, __int128 j)
{
  __int128 orig_i = i;
  __int128 orig_j = j;
  __int128 ret;
  ret = __atomic_exchange_16 (&i, j, 0);
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (i == orig_j); /* { dg-warning "TRUE" } */
}

/* __atomic_load.  */

void test__atomic_load_from_int8 (int8_t i)
{
  int8_t orig_i = i;
  int8_t ret;
  __atomic_load (&i, &ret, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_load_from_int16 (int16_t i)
{
  int16_t orig_i = i;
  int16_t ret;
  __atomic_load (&i, &ret, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_load_from_int32 (int32_t i)
{
  int32_t orig_i = i;
  int32_t ret;
  __atomic_load (&i, &ret, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_load_from_int64 (int64_t i)
{
  int64_t orig_i = i;
  int64_t ret;
  __atomic_load (&i, &ret, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_load_from_int1288 (__int128 i)
{
  __int128 orig_i = i;
  __int128 ret;
  __atomic_load (&i, &ret, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

/* __atomic_load_n.  */

void test__atomic_load_n_from_int8 (int8_t i)
{
  int8_t orig_i = i;
  int8_t ret;
  ret = __atomic_load_n (&i, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_load_n_from_int16 (int16_t i)
{
  int16_t orig_i = i;
  int16_t ret;
  ret = __atomic_load_n (&i, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_load_n_from_int32 (int32_t i)
{
  int32_t orig_i = i;
  int32_t ret;
  ret = __atomic_load_n (&i, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_load_n_from_int64 (int64_t i)
{
  int64_t orig_i = i;
  int64_t ret;
  ret = __atomic_load_n (&i, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_load_n_from_int128 (__int128 i)
{
  __int128 orig_i = i;
  __int128 ret;
  ret = __atomic_load_n (&i, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

/* __atomic_load_1 through __atomic_load_16.  */

void test__atomic_load_1 (int8_t i)
{
  int8_t orig_i = i;
  int8_t ret;
  ret = __atomic_load_1 (&i, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_load_2 (int16_t i)
{
  int16_t orig_i = i;
  int16_t ret;
  ret = __atomic_load_2 (&i, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_load_4 (int32_t i)
{
  int32_t orig_i = i;
  int32_t ret;
  ret = __atomic_load_4 (&i, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_load_8 (int64_t i)
{
  int64_t orig_i = i;
  int64_t ret;
  ret = __atomic_load_8 (&i, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_load_16 (__int128 i)
{
  __int128 orig_i = i;
  __int128 ret;
  ret = __atomic_load_16 (&i, 0);
  __analyzer_eval (i == orig_i); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_store_n_on_uint8 (uint8_t i)
{
  uint8_t tmp;
  __atomic_store_n (&tmp, i, 0);
  __analyzer_eval (tmp == i); /* { dg-warning "TRUE" } */  
}

void test__atomic_store_n_on_uint16 (uint16_t i)
{
  uint16_t tmp;
  __atomic_store_n (&tmp, i, 0);
  __analyzer_eval (tmp == i); /* { dg-warning "TRUE" } */  
}

void test__atomic_store_n_on_uint32 (uint32_t i)
{
  uint32_t tmp;
  __atomic_store_n (&tmp, i, 0);
  __analyzer_eval (tmp == i); /* { dg-warning "TRUE" } */  
}

void test__atomic_store_n_on_uint64 (uint64_t i)
{
  uint64_t tmp;
  __atomic_store_n (&tmp, i, 0);
  __analyzer_eval (tmp == i); /* { dg-warning "TRUE" } */  
}

void test__atomic_store_n_on_int128 (__int128 i)
{
  __int128 tmp;
  __atomic_store_n (&tmp, i, 0);
  __analyzer_eval (tmp == i); /* { dg-warning "TRUE" } */  
}

/* __atomic_OP_fetch built-ins.  */

/* __atomic_add_fetch.  */

void test__atomic_add_fetch_on_uint32_t (uint32_t i, uint32_t j)
{
  uint32_t orig_i = i;
  uint32_t ret;
  ret = __atomic_add_fetch (&i, j, 0);
  __analyzer_eval (i == (orig_i + j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == i); /* { dg-warning "TRUE" } */
}

void test__atomic_add_fetch_on_uint64_t (uint64_t i, uint64_t j)
{
  uint64_t orig_i = i;
  uint64_t ret;
  ret = __atomic_add_fetch (&i, j, 0);
  __analyzer_eval (i == (orig_i + j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == i); /* { dg-warning "TRUE" } */
}

/* __atomic_sub_fetch.  */

void test__atomic_sub_fetch_on_uint32_t (uint32_t i, uint32_t j)
{
  uint32_t orig_i = i;
  uint32_t ret;
  ret = __atomic_sub_fetch (&i, j, 0);
  __analyzer_eval (i == (orig_i - j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == i); /* { dg-warning "TRUE" } */
}

void test__atomic_sub_fetch_on_uint64_t (uint64_t i, uint64_t j)
{
  uint64_t orig_i = i;
  uint64_t ret;
  ret = __atomic_sub_fetch (&i, j, 0);
  __analyzer_eval (i == (orig_i - j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == i); /* { dg-warning "TRUE" } */
}

/* __atomic_and_fetch.  */

void test__atomic_and_fetch_on_uint32_t (uint32_t i, uint32_t j)
{
  uint32_t orig_i = i;
  uint32_t ret;
  ret = __atomic_and_fetch (&i, j, 0);
  __analyzer_eval (i == (orig_i & j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == i); /* { dg-warning "TRUE" } */
}

void test__atomic_and_fetch_on_uint64_t (uint64_t i, uint64_t j)
{
  uint64_t orig_i = i;
  uint64_t ret;
  ret = __atomic_and_fetch (&i, j, 0);
  __analyzer_eval (i == (orig_i & j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == i); /* { dg-warning "TRUE" } */
}

/* __atomic_xor_fetch.  */

void test__atomic_xor_fetch_on_uint32_t (uint32_t i, uint32_t j)
{
  uint32_t orig_i = i;
  uint32_t ret;
  ret = __atomic_xor_fetch (&i, j, 0);
  __analyzer_eval (i == (orig_i ^ j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == i); /* { dg-warning "TRUE" } */
}

void test__atomic_xor_fetch_on_uint64_t (uint64_t i, uint64_t j)
{
  uint64_t orig_i = i;
  uint64_t ret;
  ret = __atomic_xor_fetch (&i, j, 0);
  __analyzer_eval (i == (orig_i ^ j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == i); /* { dg-warning "TRUE" } */
}

/* __atomic_or_fetch.  */

void test__atomic_or_fetch_on_uint32_t (uint32_t i, uint32_t j)
{
  uint32_t orig_i = i;
  uint32_t ret;
  ret = __atomic_or_fetch (&i, j, 0);
  __analyzer_eval (i == (orig_i | j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == i); /* { dg-warning "TRUE" } */
}

void test__atomic_or_fetch_on_uint64_t (uint64_t i, uint64_t j)
{
  uint64_t orig_i = i;
  uint64_t ret;
  ret = __atomic_or_fetch (&i, j, 0);
  __analyzer_eval (i == (orig_i | j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == i); /* { dg-warning "TRUE" } */
}

/* __atomic_fetch_OP built-ins.  */

/* __atomic_fetch_add.  */

void test__atomic_fetch_add_on_uint32_t (uint32_t i, uint32_t j)
{
  uint32_t orig_i = i;
  uint32_t ret;
  ret = __atomic_fetch_add (&i, j, 0);
  __analyzer_eval (i == (orig_i + j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_fetch_add_on_uint64_t (uint64_t i, uint64_t j)
{
  uint64_t orig_i = i;
  uint64_t ret;
  ret = __atomic_fetch_add (&i, j, 0);
  __analyzer_eval (i == (orig_i + j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

/* __atomic_fetch_sub.  */

void test__atomic_fetch_sub_on_uint32_t (uint32_t i, uint32_t j)
{
  uint32_t orig_i = i;
  uint32_t ret;
  ret = __atomic_fetch_sub (&i, j, 0);
  __analyzer_eval (i == (orig_i - j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_fetch_sub_on_uint64_t (uint64_t i, uint64_t j)
{
  uint64_t orig_i = i;
  uint64_t ret;
  ret = __atomic_fetch_sub (&i, j, 0);
  __analyzer_eval (i == (orig_i - j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

/* __atomic_fetch_and.  */

void test__atomic_fetch_and_on_uint32_t (uint32_t i, uint32_t j)
{
  uint32_t orig_i = i;
  uint32_t ret;
  ret = __atomic_fetch_and (&i, j, 0);
  __analyzer_eval (i == (orig_i & j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_fetch_and_on_uint64_t (uint64_t i, uint64_t j)
{
  uint64_t orig_i = i;
  uint64_t ret;
  ret = __atomic_fetch_and (&i, j, 0);
  __analyzer_eval (i == (orig_i & j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

/* __atomic_fetch_xor.  */

void test__atomic_fetch_xor_on_uint32_t (uint32_t i, uint32_t j)
{
  uint32_t orig_i = i;
  uint32_t ret;
  ret = __atomic_fetch_xor (&i, j, 0);
  __analyzer_eval (i == (orig_i ^ j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_fetch_xor_on_uint64_t (uint64_t i, uint64_t j)
{
  uint64_t orig_i = i;
  uint64_t ret;
  ret = __atomic_fetch_xor (&i, j, 0);
  __analyzer_eval (i == (orig_i ^ j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

/* __atomic_fetch_or.  */

void test__atomic_fetch_or_on_uint32_t (uint32_t i, uint32_t j)
{
  uint32_t orig_i = i;
  uint32_t ret;
  ret = __atomic_fetch_or (&i, j, 0);
  __analyzer_eval (i == (orig_i | j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}

void test__atomic_fetch_or_on_uint64_t (uint64_t i, uint64_t j)
{
  uint64_t orig_i = i;
  uint64_t ret;
  ret = __atomic_fetch_or (&i, j, 0);
  __analyzer_eval (i == (orig_i | j)); /* { dg-warning "TRUE" } */
  __analyzer_eval (ret == orig_i); /* { dg-warning "TRUE" } */
}
