/* { dg-lto-do link } */

typedef struct {
  int i;
  int ints[];
} struct_t;

extern struct_t my_struct;

int main() {
 return my_struct.ints[1];
}

