/* { dg-lto-do link } */

void e(void);
void f(void);
void a(void *, void *);
void c(int);

static void * b;
void c(int d) {
  f();
  a(b, b);
}

void e(void) { c(0); }
