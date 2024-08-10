/* { dg-lto-do link } */
/* { dg-require-effective-target fpic } */
/* { dg-lto-options {{-fPIC -r -nostdlib -O2 -flto}} } */
/* { dg-extra-ld-options "-flinker-output=nolto-rel" } */

typedef struct VEC_constructor_elt_gc { } VEC_constructor_elt_gc;
typedef union tree_node *tree;
union tree_node {
    tree * use;
    VEC_constructor_elt_gc *elts;
};

struct stmt_tree_s {
    tree x_cur_stmt_list;
};
void *add_stmt (struct stmt_tree_s *x)
{
  return &x->x_cur_stmt_list;
}

