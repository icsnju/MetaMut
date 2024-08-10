/* Reduced from qemu-7.2.0's qobject/json-parser.c, which
   is licensed under LGPLv2.1 or later.  */

/* { dg-additional-options "-fno-analyzer-call-summaries -Wno-analyzer-too-complex" } */

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


typedef __builtin_va_list va_list;

typedef struct _GQueue GQueue;
typedef struct Error Error;
typedef struct QList QList;
typedef struct QObject QObject;

struct QObjectBase_ {
  /* [...snip...] */
};


struct QObject {
  struct QObjectBase_ base;
};

#define QOBJECT(obj) ((QObject *)obj)
#define qobject_unref(OBJ) /* [...snip...] */

typedef struct QTailQLink {
  void *tql_next;
  struct QTailQLink *tql_prev;
} QTailQLink;

struct QList {
  struct QObjectBase_ base;
  union {
    struct QListEntry *tqh_first;
    QTailQLink tqh_circ;
  } head;
};
QList *qlist_new(void);
void qlist_append_obj(QList *qlist, QObject *obj);

typedef enum json_token_type {
  /* [...snip...] */			      
  JSON_LSQUARE,
  JSON_RSQUARE,
  /* [...snip...] */			      
  JSON_COMMA,
  /* [...snip...] */			      
  JSON_KEYWORD,
  /* [...snip...] */			      
} JSONTokenType;
typedef struct JSONToken JSONToken;

struct JSONToken {
  JSONTokenType type;
  int x;
  int y;
  char str[];
};

typedef struct JSONParserContext {
  Error *err;
  JSONToken *current;
  GQueue *buf;
  va_list *ap;
} JSONParserContext;
static QObject *parse_value(JSONParserContext *ctxt);

JSONToken *parser_context_pop_token(JSONParserContext *ctxt);
JSONToken *parser_context_peek_token(JSONParserContext *ctxt);

static QObject *parse_array(JSONParserContext *ctxt) {
  QList *list = NULL;
  JSONToken *token, *peek;

  token = parser_context_pop_token(ctxt);

  list = qlist_new();

  peek = parser_context_peek_token(ctxt);
  if (peek == NULL) {
    goto out;
  }

  if (peek->type != JSON_RSQUARE) {
    QObject *obj;

    obj = parse_value(ctxt); /* { dg-bogus "infinite recursion" } */
    if (obj == NULL) {
      goto out;
    }

    qlist_append_obj(list, obj);

    token = parser_context_pop_token(ctxt);
    if (token == NULL) {
      goto out;
    }

    while (token->type != JSON_RSQUARE) {
      if (token->type != JSON_COMMA) {
        goto out;
      }

      obj = parse_value(ctxt);
      if (obj == NULL) {
        goto out;
      }

      qlist_append_obj(list, obj);

      token = parser_context_pop_token(ctxt);
      if (token == NULL) {
        goto out;
      }
    }
  } else {
    (void)parser_context_pop_token(ctxt);
  }

  return QOBJECT(list);

out:
  qobject_unref(list);
  return NULL;
}

QObject *parse_keyword(JSONParserContext *ctxt);

QObject *parse_value(JSONParserContext *ctxt) {
  JSONToken *token;

  token = parser_context_peek_token(ctxt);
  if (token == NULL) {
    return NULL;
  }

  switch (token->type) {
  case JSON_LSQUARE:
    return parse_array(ctxt); /* { dg-bogus "infinite recursion" } */
  case JSON_KEYWORD:
    return parse_keyword(ctxt);
  default:
    return NULL;
  }
}
