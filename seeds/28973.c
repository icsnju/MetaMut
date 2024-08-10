/* { dg-additional-options "-fno-analyzer-call-summaries" } */
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

/* data structures */

struct screen_s {
	size_t rows;
	size_t cols;
	char **data;
};

struct context_s {
	struct screen_s *scr;
};

/* global context variable */

static struct context_s *ctx;

/* prototypes */

struct screen_s *screen_create(size_t cols, size_t rows);
void screen_destroy(struct screen_s *scr);
void resize_screen(size_t cols, size_t rows);

/* functions */

struct screen_s *screen_create(size_t cols, size_t rows)
{
	struct screen_s *result = NULL;

	result = (struct screen_s *) __builtin_calloc(1, sizeof(*result));
	if (!result)
		return NULL;

	result->cols = cols;
	result->rows = rows;

	/* make one allocation which will be accessed like a 2D array */
	result->data = (char **) __builtin_calloc(rows, sizeof(result->data) + sizeof(*result->data) * cols);
	if (!result->data) {
		__builtin_free(result);
		return NULL;
	}

	/* obtain pointer to start of data area */
	char *ptr = (char *)(result->data + rows);

	/* setup pointers for each row of data to allow 2D array access */
	for (size_t row = 0; row < rows; row++)
		result->data[row] = (ptr + row * cols);
	/* array can now be accessed like data[row][col] */

	return result;
}

void screen_destroy(struct screen_s *scr)
{
	if (!scr)
		return;

	__builtin_free(scr->data);

	scr->data = NULL;
	scr->rows = 0;
	scr->cols = 0;

	__builtin_free(scr); /* { dg-bogus "leak" } */
}

void resize_screen(size_t cols, size_t rows)
{
	/* create a new screen */
	struct screen_s *new_scr = NULL;
	new_scr = screen_create(cols, rows); /* { dg-bogus "leak" } */
	if (!new_scr) {
		return;
	}

	/* swap the old screen with the new one */
	struct screen_s *old_scr = ctx->scr;
	ctx->scr = new_scr;

	/* omitted: copy the old screen contents to the new screen */

	/* free the old screen */
	screen_destroy(old_scr);
}

int main(void)
{
	ctx = (struct context_s *) __builtin_calloc(1, sizeof(*ctx));
	if (!ctx)
		__builtin_abort();

	ctx->scr = screen_create(80, 25); /* { dg-bogus "leak" } */
	resize_screen(100, 20);

	/* tidy up and quit */
	screen_destroy(ctx->scr);
	__builtin_free(ctx);
	ctx = NULL;
	return 0;
}
