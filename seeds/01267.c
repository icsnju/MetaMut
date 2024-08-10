/* Minimized/hacked up from openvswitch lib/conntrack.c, which had this license
   header:  */
/*
 * Copyright (c) 2015-2019 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

#ifndef __cplusplus
#define false 0
#endif

#define OBJECT_OFFSETOF(OBJECT, MEMBER)\
    __builtin_offsetof(struct zone_limit, MEMBER)

#define OBJECT_CONTAINING(POINTER, OBJECT, MEMBER)                      \
    ((struct zone_limit *) (void *)                                      \
     ((char *) (POINTER) - OBJECT_OFFSETOF(OBJECT, MEMBER)))

#define ASSIGN_CONTAINER(OBJECT, POINTER, MEMBER)			\
    ((OBJECT) = OBJECT_CONTAINING(POINTER, OBJECT, MEMBER), (void) 0)

#define INIT_CONTAINER(OBJECT, POINTER, MEMBER)				\
    ((OBJECT) = NULL, ASSIGN_CONTAINER(OBJECT, POINTER, MEMBER))

#define HMAP_FOR_EACH_POP(NODE, MEMBER, HMAP)				\
    for (size_t bucket__ = 0;                                               \
         INIT_CONTAINER(NODE, hmap_pop_helper__(HMAP, &bucket__), MEMBER),  \
         (NODE != OBJECT_CONTAINING(NULL, NODE, MEMBER))                    \
         || ((NODE = NULL), false);)

struct hmap {
    struct hmap_node **buckets;
    struct hmap_node *one;
    size_t mask;
    size_t n;
};

struct hmap_node {
    size_t hash;
    struct hmap_node *next;
};

static inline void hmap_remove(struct hmap *, struct hmap_node *);

struct hmap_node *
hmap_pop_helper__(struct hmap *hmap, size_t *bucket) {

    for (; *bucket <= hmap->mask; (*bucket)++) {
        struct hmap_node *node = hmap->buckets[*bucket];

        if (node) {
            hmap_remove(hmap, node);
            return node;
        }
    }

    return NULL;
}

static inline void
hmap_remove(struct hmap *hmap, struct hmap_node *node)
{
    struct hmap_node **bucket = &hmap->buckets[node->hash & hmap->mask];
    while (*bucket != node) {
        bucket = &(*bucket)->next;
    }
    *bucket = node->next;
    hmap->n--;
}

struct conntrack {
    struct hmap zone_limits;
};

struct zone_limit {
    struct hmap_node node;
};

void
conntrack_destroy(struct conntrack *ct)
{
    struct zone_limit *zl;
    HMAP_FOR_EACH_POP (zl, node, &ct->zone_limits) {
      __builtin_free(zl);
    }
}
