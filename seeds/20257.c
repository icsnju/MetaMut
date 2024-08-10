/* C99 6.5.2.2 Function calls.
   Test structure passing and return values involving decimal floating
   point types.  */

#ifdef __cplusplus
typedef float _Decimal32 __attribute__((mode(SD)));
typedef float _Decimal64 __attribute__((mode(DD)));
typedef float _Decimal128 __attribute__((mode(TD)));

#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

int failures;

#ifdef DBG
extern int printf (const char *, ...);
#define FAILURE { printf ("failed at line %d\n", __LINE__); failures++; }
#define FINISH if (failures != 0) __builtin_abort (); return 0;
#else
#define FAILURE __builtin_abort ();
#define FINISH return 0;
#endif

struct example
{
  _Decimal128 d128;
  char dummy1;
  _Decimal64 d64;
  char dummy2;
  _Decimal32 d32;
} nums = { 1.0dl, 'a', 2.0dd, 'b', 3.0df };

_Decimal32
d32_field (struct example s)
{
  return s.d32;
}

_Decimal64
d64_field (struct example s)
{
  return s.d64;
}

_Decimal128
d128_field (struct example s)
{
  return s.d128;
}

char
dummy1_field (struct example s)
{
  return s.dummy1;
}

char
dummy2_field (struct example s)
{
  return s.dummy2;
}

_Decimal32
ptr_d32_field (struct example *s)
{
  return s->d32;
}

_Decimal64
ptr_d64_field (struct example *s)
{
  return s->d64;
}

_Decimal128
ptr_d128_field (struct example *s)
{
  return s->d128;
}

char
ptr_dummy1_field (struct example *s)
{
  return s->dummy1;
}

char
ptr_dummy2_field (struct example *s)
{
  return s->dummy2;
}


int
main ()
{
  if (d32_field (nums) != 3.0df) FAILURE
  if (d64_field (nums) != 2.0dd) FAILURE
  if (d128_field (nums) != 1.0dl) FAILURE
  if (dummy1_field (nums) != 'a') FAILURE
  if (dummy2_field (nums) != 'b') FAILURE

  if (ptr_d32_field (&nums) != 3.0df) FAILURE
  if (ptr_d64_field (&nums) != 2.0dd) FAILURE
  if (ptr_d128_field (&nums) != 1.0dl) FAILURE
  if (ptr_dummy1_field (&nums) != 'a') FAILURE
  if (ptr_dummy2_field (&nums) != 'b') FAILURE

  FINISH
}
