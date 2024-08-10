/* { dg-do run } */
/* { dg-options "-O2 -fwrapv" } */
signed char sc;
__INT16_TYPE__ ss;

void testsc() {
  unsigned int g = 0;
  unsigned int *p1 = &g;
  signed char *p2 = &sc;

  do {
    (*p1)++;
    (*p2)--;
  } while (sc);

  if (g != 256)
    __builtin_abort();
}

void testss() {
  __UINT32_TYPE__ g = 0;
  __UINT32_TYPE__ *p1 = &g;
  __INT16_TYPE__ *p2 = &ss;

  do {
    (*p1)++;
    (*p2)--;
  } while (ss);

  if (g != 65536)
    __builtin_abort();
}

int main() {
  testsc();
  testss();
  return 0;
}

