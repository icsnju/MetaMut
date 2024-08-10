/* { dg-do compile } */
/* { dg-options "-O0 -dA -gbtf -mco-re" } */

struct my_struct {
 int a;
};

union my_union {
 int b;
 int c;
};

typedef struct my_struct named_struct_tdef;
typedef int (*func_tdef)(void *);

typedef struct {
 int d, e;
} *tdef_struct_ptr;

enum my_enum {
 MY_ENUM_VAL1 = 1,
 MY_ENUM_VAL2 = 2,
};
typedef enum { TE1, TE2, TE3 } enum_tdef;

typedef int int_tdef;
typedef void *void_ptr_tdef;
typedef int *restrict restrict_ptr_tdef;
typedef int (*func_tdef)(void *);
typedef char array_tdef[10];

struct my_complex_struct {
 int a;
 int b;
 union {
  int *a;
  struct my_struct *s;
 } c;
};

typedef struct {
  int a;
} anon_struct_tdef;

typedef struct my_struct *struct_ptr_tdef;

enum named_ue64 {
 UE64_VAL1 = 0x1ffffffffULL,
 UE64_VAL2 = 0x01fffffffULL,
 UE64_VAL3 = 0x001ffffffULL,
};

enum named_se64 {
 SE64_VAL1 = 0x1ffffffffLL,
 SE64_VAL2 = 0x01fffffffLL,
 SE64_VAL3 = -1,
};

enum named_ue {
 UE_VAL1 = 0x1ffffff,
 UE_VAL2 = 0x01fffff,
 UE_VAL3 = 0x001ffff,
};

enum named_se {
 SE_VAL1 = 0x1fffffff,
 SE_VAL2 = 0x01ffffff,
 SE_VAL3 = -1,
};


/* BPF specific code */

enum bpf_type_id_kind {
 BPF_TYPE_ID_LOCAL = 0,
 BPF_TYPE_ID_TARGET = 1,
};

enum bpf_type_info_kind {
 BPF_TYPE_EXISTS = 0,
 BPF_TYPE_SIZE = 1,
 BPF_TYPE_MATCHES = 2,
};

enum bpf_enum_value_kind {
 BPF_ENUMVAL_EXISTS = 0,
 BPF_ENUMVAL_VALUE = 1,
};

#define COMPOSE_VAR(t,s) t##s
#define bpf_type1(type, NR) ({ \
	extern  typeof(type) *COMPOSE_VAR(bpf_type_tmp_, NR); \
	COMPOSE_VAR(bpf_type_tmp_, NR); \
})
#define bpf_type(type) bpf_type1(type, __COUNTER__)

#define bpf_core_type_id_local(type)					    \
	__builtin_btf_type_id(*bpf_type(type), BPF_TYPE_ID_LOCAL)
#define bpf_core_type_id_target(type)					    \
	__builtin_btf_type_id(*bpf_type(type), BPF_TYPE_ID_TARGET)

#define bpf_core_type_exists(type)					    \
	__builtin_preserve_type_info(*bpf_type(type), BPF_TYPE_EXISTS)
#define bpf_core_type_matches(type)					    \
	__builtin_preserve_type_info(*bpf_type(type), BPF_TYPE_MATCHES)
#define bpf_core_type_size(type)					    \
	__builtin_preserve_type_info(*bpf_type(type), BPF_TYPE_SIZE)

#define bpf_core_enum_value_exists(enum_type, enum_value)		    \
	__builtin_preserve_enum_value(bpf_type(enum_type), enum_value, BPF_ENUMVAL_EXISTS)
#define bpf_core_enum_value(enum_type, enum_value)			    \
	__builtin_preserve_enum_value(bpf_type(enum_type), enum_value, BPF_ENUMVAL_VALUE)

extern int *v;

int foo(void *data)
{
  int i = 0;

  v[i++] = bpf_core_type_exists (struct my_struct);
  v[i++] = bpf_core_type_exists (struct my_complex_struct);
  v[i++] = bpf_core_type_exists (union my_union);
  v[i++] = bpf_core_type_exists (enum my_enum);
  v[i++] = bpf_core_type_exists (named_struct_tdef);
  v[i++] = bpf_core_type_exists (anon_struct_tdef);
  v[i++] = bpf_core_type_exists (struct_ptr_tdef);
  v[i++] = bpf_core_type_exists (int_tdef);
  v[i++] = bpf_core_type_exists (enum_tdef);
  v[i++] = bpf_core_type_exists (void_ptr_tdef);
  v[i++] = bpf_core_type_exists (restrict_ptr_tdef);
  v[i++] = bpf_core_type_exists (func_tdef);
  v[i++] = bpf_core_type_exists (array_tdef);
 
  v[i++] = bpf_core_type_matches (struct my_struct);
  v[i++] = bpf_core_type_matches (struct my_complex_struct);
  v[i++] = bpf_core_type_matches (union my_union);
  v[i++] = bpf_core_type_matches (enum my_enum);
  v[i++] = bpf_core_type_matches (named_struct_tdef);
  v[i++] = bpf_core_type_matches (anon_struct_tdef);
  v[i++] = bpf_core_type_matches (struct_ptr_tdef);
  v[i++] = bpf_core_type_matches (int_tdef);
  v[i++] = bpf_core_type_matches (enum_tdef);
  v[i++] = bpf_core_type_matches (void_ptr_tdef);
  v[i++] = bpf_core_type_matches (restrict_ptr_tdef);
  v[i++] = bpf_core_type_matches (func_tdef);
  v[i++] = bpf_core_type_matches (array_tdef);
 
  v[i++] = bpf_core_type_size (struct my_struct);
  v[i++] = bpf_core_type_size (union my_union);
  v[i++] = bpf_core_type_size (enum my_enum);
  v[i++] = bpf_core_type_size (named_struct_tdef);
  v[i++] = bpf_core_type_size (anon_struct_tdef);
  v[i++] = bpf_core_type_size (struct_ptr_tdef);
  v[i++] = bpf_core_type_size (int_tdef);
  v[i++] = bpf_core_type_size (enum_tdef);
  v[i++] = bpf_core_type_size (void_ptr_tdef);
  v[i++] = bpf_core_type_size (func_tdef);
  v[i++] = bpf_core_type_size (array_tdef);

  return 0;
}

/* { dg-final { scan-assembler-times "\t.4byte\t0x0\t; bpfcr_type" 0 } } */
/* { dg-final { scan-assembler-times "\t.4byte\t0x8\t; bpfcr_kind" 13 } } BPF_TYPE_EXISTS */
/* { dg-final { scan-assembler-times "\t.4byte\t0x9\t; bpfcr_kind" 11 } } BPF_TYPE_SIZE */
/* { dg-final { scan-assembler-times "\t.4byte\t0xc\t; bpfcr_kind" 13 } } BPF_TYPE_MATCHES */
