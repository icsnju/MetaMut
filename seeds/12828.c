/* Contributed by Devang Patel <dpatel@apple.com> */
/* { dg-do compile } */
/* { dg-options "-gdwarf" } */

/* Contributed by Devang Patel <dpatel@apple.com> */

struct point
{
  int x;
  int y;
};


int main()
{
  struct point p;
  p.x = 0;
  p.y = 0;
}
