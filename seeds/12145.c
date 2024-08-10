/* { dg-lto-do link } */
/* { dg-require-effective-target fpic } */
/* { dg-lto-options {{-flto -flto-partition=1to1 -r -nostdlib -fPIC}} } */
/* { dg-extra-ld-options "-flinker-output=nolto-rel" } */
typedef struct foo {
 int x;
} foo_t;

extern struct foo x;

void f(void) {
  x.x = 0;
}
