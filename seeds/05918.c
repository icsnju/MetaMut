/* Reduced from SoftEtherVPN's src/Cedar/WebUI.c.   */
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
typedef int (COMPARE)(void *p1, void *p2);
typedef unsigned int UINT;
typedef unsigned long int UINT64;
typedef struct LIST LIST;
typedef struct STRMAP_ENTRY STRMAP_ENTRY;
typedef struct WEBUI
{
	/* [...snip...] */
	LIST *Contexts;
} WEBUI;

typedef struct WU_CONTEXT
{
	/* [...snip...] */
	UINT64 ExpireDate;
} WU_CONTEXT;

struct LIST
{
	/* [...snip...] */
	UINT num_item, num_reserved;
	void **p;
	/* [...snip...] */
};

#define	LIST_DATA(o, i)		(((o) != NULL) ? ((o)->p[(i)]) : NULL)
#define	LIST_NUM(o)			(((o) != NULL) ? (o)->num_item : 0)
#ifdef __cplusplus
#ifndef _Bool
typedef bool _Bool;
#endif
#endif

struct STRMAP_ENTRY
{
	char *Name;
	void *Value;
};

void Free(void *addr);
void Add(LIST *o, void *p);
_Bool Delete(LIST *o, void *p);
void LockList(LIST *o);
void UnlockList(LIST *o);
void ReleaseList(LIST *o);
LIST *NewList(COMPARE *cmp);
UINT64 Tick64();
void WuFreeContext(WU_CONTEXT *context);

void WuExpireSessionKey(WEBUI *wu)
{
	LIST *Expired = NewList(NULL);
	UINT i;

	LockList(wu->Contexts);

	for(i=0; i<LIST_NUM(wu->Contexts); i++)
	{
		STRMAP_ENTRY *entry = (STRMAP_ENTRY*)LIST_DATA(wu->Contexts, i);
		WU_CONTEXT *context = (WU_CONTEXT*)entry->Value; /* { dg-bogus "dereference of NULL 'entry'" "PR analyzer/108400" { xfail *-*-* } } */
		if(context->ExpireDate < Tick64())
		{
			Add(Expired, entry);
		}
	}

	for(i=0; i<LIST_NUM(Expired); i++)
	{
		STRMAP_ENTRY *entry = (STRMAP_ENTRY*)LIST_DATA(Expired, i);
		Delete(wu->Contexts, entry);
		Free(entry->Name);
		WuFreeContext((WU_CONTEXT*)entry->Value);
		Free(entry);
	}
	ReleaseList(Expired);

	UnlockList(wu->Contexts);
}
