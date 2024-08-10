/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "-O3 --param riscv-autovec-preference=fixed-vlmax -Wno-psabi" } */

/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfhmin -mabi=lp64d -O3 --param riscv-autovec-preference=fixed-vlmax -Wno-psabi" } */

#include <stdint-gcc.h>

typedef int8_t vnx2qi __attribute__((vector_size (2)));
typedef uint8_t vnx2uqi __attribute__((vector_size (2)));

#define MASK_2		0, 3

void __attribute__ ((noipa))
merge0 (vnx2qi x, vnx2qi y, vnx2qi *out)
{
  vnx2qi v = __builtin_shufflevector ((vnx2qi) x, (vnx2qi) y, MASK_2);
  *(vnx2qi *)out = v;
}

void __attribute__ ((noipa))
merge1 (vnx2uqi x, vnx2uqi y, vnx2uqi *out)
{
  vnx2uqi v = __builtin_shufflevector ((vnx2uqi) x, (vnx2uqi) y, MASK_2);
  *(vnx2uqi *)out = v;
}

/* { dg-final { scan-assembler-times {\tvmerge.vvm} 2 } } */

int main(void)
{
    vnx2qi vnx2qi_x= {1,2};
    vnx2qi vnx2qi_y= {101,102};
    vnx2qi vnx2qi_expect= {1,102};
    vnx2qi vnx2qi_real;
    merge0(vnx2qi_x,vnx2qi_y,&vnx2qi_real);
    for(int i=0; i<2; i++)
        if(vnx2qi_real[i]!=vnx2qi_expect[i]) {
            __builtin_abort();
        }

    vnx2uqi vnx2uqi_x= {1,2};
    vnx2uqi vnx2uqi_y= {101,102};
    vnx2uqi vnx2uqi_expect= {1,102};
    vnx2uqi vnx2uqi_real;
    merge1(vnx2uqi_x,vnx2uqi_y,&vnx2uqi_real);
    for(int i=0; i<2; i++)
        if(vnx2uqi_real[i]!=vnx2uqi_expect[i]) {
            __builtin_abort();
        }

    return 0;
}
