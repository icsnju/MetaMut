#include <stdio.h>

#define info(fmt, ...)                                   \
  printf("%s:%s:%d: " fmt, __FILE__, __func__, __LINE__, \
      ##__VA_ARGS__)

int link_foo(int x) {
  if (x > 0) {
    info("Positive %d\n", x);
    return x;
  } else {
    info("Non-positive %d\n", x);
    return -x;
  }
}

int link_bar(int x) {
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

void link_baz(int x) {
  switch (x) {
  case 1: info("One\n"); break;
  case 2: info("Two\n"); break;
  default: info("Other\n"); break;
  }
}
