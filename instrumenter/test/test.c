#include <stdio.h>

#define info(fmt, ...)                                   \
  printf("%s:%s:%d: " fmt, __FILE__, __func__, __LINE__, \
      ##__VA_ARGS__)

extern int link_foo(int);
extern int link_bar(int);
extern int link_baz(int);

int foo(int x) {
  if (x > 0) {
    info("Positive %d\n", x);
    return x;
  } else {
    info("Non-positive %d\n", x);
    return -x;
  }
}

int bar(int x) {
  if (x == 0) {
    info("Zero\n");
    return 0;
  } else if (x > 0) {
    info("Positive\n");
    return x;
  } else {
    info("Negative\n");
    return -x;
  }
}

void baz(int x) {
  switch (x) {
  case 1: info("One\n"); break;
  case 2: info("Two\n"); break;
  default: info("Other\n"); break;
  }
}

int main() {
  int a = foo(5);
  int b = bar(-3);
  baz(a + b);
  a = link_foo(-2);
  b = link_bar(3);
  link_baz(a + b);
  return 0;
}
