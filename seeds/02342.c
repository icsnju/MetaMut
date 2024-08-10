/* Reduced from haproxy-2.7.1: src/tcpcheck.c.  */

/* { dg-additional-options "-Wno-analyzer-too-complex" } */

typedef __SIZE_TYPE__ size_t;

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


extern void *calloc(size_t __nmemb, size_t __size)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__malloc__))
    __attribute__((__alloc_size__(1, 2)));
extern char *strdup(const char *__s) __attribute__((__nothrow__, __leaf__))
__attribute__((__malloc__)) __attribute__((__nonnull__(1)));
extern char *strstr(const char *__haystack, const char *__needle)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__pure__))
    __attribute__((__nonnull__(1, 2)));
extern size_t strlen(const char *__s) __attribute__((__nothrow__, __leaf__))
__attribute__((__pure__)) __attribute__((__nonnull__(1)));
struct list {
  struct list *n;
  struct list *p;
};
struct buffer {
  size_t size;
  char *area;
  size_t data;
  size_t head;
};
struct proxy;
struct ist {
  char *ptr;
  size_t len;
};
static inline int isttest(const struct ist ist) { return ist.ptr != NULL; }

enum http_meth_t {
  HTTP_METH_OPTIONS,
  /* [...snip...] */
} __attribute__((packed));

struct http_meth {
  enum http_meth_t meth;
  struct buffer str;
};
enum tcpcheck_send_type {
  /* [...snip...] */
  TCPCHK_SEND_HTTP,
};
enum tcpcheck_rule_type {
  TCPCHK_ACT_SEND = 0,
  /* [...snip...] */
};
struct tcpcheck_http_hdr {
  struct ist name;
  struct list value;
  struct list list;
};
struct tcpcheck_send {
  enum tcpcheck_send_type type;
  union {
    /* [...snip...] */
    struct {
      unsigned int flags;
      struct http_meth meth;
      union {
        struct ist uri;
        /* [...snip...] */
      };
      struct ist vsn;
      struct list hdrs;
      /* [...snip...] */
    } http;
  };
};
struct tcpcheck_rule {
  /* [...snip...] */
  enum tcpcheck_rule_type action;
  /* [...snip...] */
  union {
    /* [...snip...] */
    struct tcpcheck_send send;
    /* [...snip...] */
  };
};
enum http_meth_t find_http_meth(const char *str, const int len);
void free_tcpcheck(struct tcpcheck_rule *rule, int in_pool);
void free_tcpcheck_http_hdr(struct tcpcheck_http_hdr *hdr);

#define ist(str) ({                                                    \
	char *__x = (char *) ((void *)(str));                                     \
	(struct ist){                                                  \
		.ptr = __x,                                            \
		.len = __builtin_constant_p(str) ?                     \
			((void *)str == (void *)0) ? 0 :               \
			__builtin_strlen(__x) :                        \
			({                                             \
				size_t __l = 0;                        \
				if (__x) for (__l--; __x[++__l]; ) ;   \
				__l;                                   \
			})                                             \
	};                                                             \
})

struct tcpcheck_rule *proxy_parse_httpchk_req(char **args, int cur_arg,
                                              struct proxy *px, char **errmsg) {
  struct tcpcheck_rule *chk = NULL;
  struct tcpcheck_http_hdr *hdr = NULL;
  char *meth = NULL, *uri = NULL, *vsn = NULL;
  char *hdrs, *body;

  hdrs = (*args[cur_arg + 2] ? strstr(args[cur_arg + 2], "\r\n") : NULL);
  body = (*args[cur_arg + 2] ? strstr(args[cur_arg + 2], "\r\n\r\n") : NULL);
  if (hdrs || body) {
    /* [...snip...] */
    goto error;
  }

  chk = (struct tcpcheck_rule *) calloc(1, sizeof(*chk));
  if (!chk) {
    /* [...snip...] */
    goto error;
  }
  chk->action = TCPCHK_ACT_SEND;
  chk->send.type = TCPCHK_SEND_HTTP;
  chk->send.http.flags |= 0x0004;
  chk->send.http.meth.meth = HTTP_METH_OPTIONS;
  ((&chk->send.http.hdrs)->n = (&chk->send.http.hdrs)->p =
       (&chk->send.http.hdrs));

  if (*args[cur_arg]) {
    if (!*args[cur_arg + 1])
      uri = args[cur_arg];
    else
      meth = args[cur_arg];
  }
  if (*args[cur_arg + 1])
    uri = args[cur_arg + 1];
  if (*args[cur_arg + 2])
    vsn = args[cur_arg + 2];

  if (meth) { /* { dg-bogus "check of 'meth' for NULL after already dereferencing it" } */
    chk->send.http.meth.meth = find_http_meth(meth, strlen(meth));
    chk->send.http.meth.str.area = strdup(meth);
    chk->send.http.meth.str.data = strlen(meth);
    if (!chk->send.http.meth.str.area) {
      /* [...snip...] */
      goto error;
    }
  }
  if (uri) {
    chk->send.http.uri = ist(strdup(uri));
    if (!isttest(chk->send.http.uri)) {
      /* [...snip...] */
      goto error;
    }
  }
  if (vsn) { /* { dg-bogus "check of 'vsn' for NULL after already dereferencing it" } */
    chk->send.http.vsn = ist(strdup(vsn));
    if (!isttest(chk->send.http.vsn)) {
      /* [...snip...] */
      goto error;
    }
  }
  return chk;

error:
  free_tcpcheck_http_hdr(hdr);
  free_tcpcheck(chk, 0);
  return NULL;
}
