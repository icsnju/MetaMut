/* { dg-do link } */
/* { dg-options "-O2 -fdump-tree-vrp1" } */

extern void link_error(void);

#define RANGE(name, min, max) \
  if (name < min || name > max) \
    return;
#define ANTI_RANGE(name, min, max) \
  if (name >= min && name <= max) \
    return;
#define MERGE(cond, name1, name2) \
  if (cond) \
    name1 = name2;
#define CHECK_RANGE(expr, min, max) \
  do { \
     __typeof__ (expr) v = (expr); \
     if (v < min) link_error(); \
     if (v > max) link_error(); \
     if (v < min || v > max) link_error (); \
  } while (0) 
#define CHECK_ANTI_RANGE(expr, min, max) \
  do { \
    __typeof__ (expr) v = (expr); \
    if (v >= min) \
      if (v <= max) \
        link_error(); \
    if (v >= min && v <= max) \
      link_error(); \
  } while (0)

void test1 (int i, int j, int b)
{
  RANGE(i, 2, 6);
  ANTI_RANGE(j, 1, 7);
  MERGE(b, i, j);
  CHECK_ANTI_RANGE(i, 1, 1);
}
int main() { }

/* VRP will arbitrarily choose ~[1, 1] when merging [2, 6] with ~[1, 7].  */

/* { dg-final { scan-tree-dump-times "link_error" 0 "vrp1" } } */
