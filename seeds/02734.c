#include <stdlib.h>

extern void calls_free (void *ptr);

extern void calls_free (void *ptr)
{
  free (ptr); 
}
