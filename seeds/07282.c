typedef struct {
  int i;
  int ints[];
} struct_t;

struct_t my_struct = {
 20,
 { 1, 2 }
};
