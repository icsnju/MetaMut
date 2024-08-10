/* { dg-lto-do link } */
struct foo;
extern struct foo *FOO_PTR_ARR[1];


int main() { 
  // just to prevent symbol removal
  FOO_PTR_ARR[1] = 0;
  return 0;
}
