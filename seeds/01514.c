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

void test1(int i, int j)
{
  RANGE(i, 1, 5);
  RANGE(j, 7, 10);
  CHECK_RANGE(i + j, 8, 15);
}

#define UINT_MAX 2*(unsigned)__INT_MAX__ + 1
void test2(unsigned int i)
{
  RANGE(i, UINT_MAX - 0x4, UINT_MAX - 0x1);
  CHECK_ANTI_RANGE(i + 0x2, 0x1, UINT_MAX - 0x3);
}
void test3(unsigned int i)
{
  RANGE(i, UINT_MAX - 0x4, UINT_MAX - 0x1);
  CHECK_RANGE(i + 0x5, 0x0, 0x3);
}
void test4(unsigned int i)
{
  RANGE(i, 2, 4);
  CHECK_ANTI_RANGE(i - 4, 1, UINT_MAX - 2);
}
void test5(unsigned int i)
{
  RANGE(i, 2, 4);
  CHECK_RANGE(i - 8, UINT_MAX - 5, UINT_MAX - 3);
}

int main() {}

/* { dg-final { scan-tree-dump-times "link_error" 0 "vrp1" } } */
