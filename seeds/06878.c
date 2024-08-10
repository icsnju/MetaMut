#include <stdlib.h>
#include <stddef.h>

extern void *wrapped_malloc (size_t size);
extern void wrapped_free (void *ptr);

typedef struct boxed_int
{
  int i;
} boxed_int;

extern boxed_int *make_boxed_int (int i);
extern void free_boxed_int (boxed_int *bi);

boxed_int *
make_boxed_int (int i)
{
  boxed_int *result = (boxed_int *)wrapped_malloc (sizeof (boxed_int));
  if (!result)
    abort ();
  result->i = i;
  return result;
}

void
free_boxed_int (boxed_int *bi)
{
  wrapped_free (bi);
}
