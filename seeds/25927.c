/* Reduced from apr-1.7.0/tables/apr_hash.c: 'apr_hash_merge' */

/* { dg-additional-options "-Wno-analyzer-too-complex" } */

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

typedef __SIZE_TYPE__ size_t;

extern void*
memset(void* __s, int __c, size_t __n)
  __attribute__((__nothrow__, __leaf__, __nonnull__(1)));

typedef struct apr_pool_t apr_pool_t;

void*
apr_palloc(apr_pool_t* p, size_t size)
  __attribute__((alloc_size(2), nonnull(1)));

typedef struct apr_hash_t apr_hash_t;
typedef struct apr_hash_index_t apr_hash_index_t;
typedef unsigned int (*apr_hashfunc_t)(const char* key, size_t* klen);
typedef struct apr_hash_entry_t apr_hash_entry_t;

struct apr_hash_entry_t
{
  apr_hash_entry_t* next;
  unsigned int hash;
  const void* key;
  size_t klen;
  const void* val;
};

struct apr_hash_t
{
  apr_pool_t* pool;
  apr_hash_entry_t** array;
  /* [...snip.../ */
  unsigned int count, max, seed;
  apr_hashfunc_t hash_func;
  apr_hash_entry_t* free;
};

static apr_hash_entry_t**
alloc_array(apr_hash_t* ht, unsigned int max)
{
  return (apr_hash_entry_t **) memset(apr_palloc(ht->pool, sizeof(*ht->array) * (max + 1)),
                0,
                sizeof(*ht->array) * (max + 1));
}

apr_hash_t*
apr_hash_merge(apr_pool_t* p,
               const apr_hash_t* overlay,
               const apr_hash_t* base)
{
  apr_hash_t* res;
  apr_hash_entry_t* new_vals = NULL;
  apr_hash_entry_t* iter;
  unsigned int i, j, k;
  res = (apr_hash_t *) apr_palloc(p, sizeof(apr_hash_t));
  res->pool = p;
  res->free = NULL;
  res->hash_func = base->hash_func;
  res->count = base->count;
  res->max = (overlay->max > base->max) ? overlay->max : base->max;
  if (base->count + overlay->count > res->max) {
    res->max = res->max * 2 + 1;
  }
  res->seed = base->seed;
  res->array = alloc_array(res, res->max);
  if (base->count + overlay->count) {
    new_vals = (apr_hash_entry_t *)
      apr_palloc(p, sizeof(apr_hash_entry_t) * (base->count + overlay->count));
  }
  j = 0;
  for (k = 0; k <= base->max; k++) {
    for (iter = base->array[k]; iter; iter = iter->next) {
      i = iter->hash & res->max;
      /* We should only warn for the first of these
	 (it's actually a false positive, but we don't have the
	 invariante to know that).  */
      new_vals[j].klen = iter->klen;   /* { dg-warning "dereference of NULL 'new_vals'" } */
      /* ...but not for subsequent ones: */
      new_vals[j].key = iter->key;      /* { dg-bogus "dereference of NULL 'new_vals'" "PR analyzer/108830" } */
      new_vals[j].val = iter->val;      /* { dg-bogus "dereference of NULL 'new_vals'" "PR analyzer/108830" } */
      new_vals[j].hash = iter->hash;    /* { dg-bogus "dereference of NULL 'new_vals'" "PR analyzer/108830" } */
      new_vals[j].next = res->array[i]; /* { dg-bogus "dereference of NULL 'new_vals'" "PR analyzer/108830" } */
      res->array[i] = &new_vals[j];     /* { dg-bogus "dereference of NULL 'new_vals'" "PR analyzer/108830" } */
      j++;
    }
  }
  /* [...snip...] */
  return res;
}
