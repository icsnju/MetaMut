/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "-O3 --param riscv-autovec-preference=fixed-vlmax -Wno-psabi" } */

/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfhmin -mabi=lp64d -O3 --param riscv-autovec-preference=fixed-vlmax -Wno-psabi" } */

#include <stdint-gcc.h>

typedef int8_t vnx4qi __attribute__((vector_size (4)));
typedef int16_t vnx2hi __attribute__((vector_size (4)));
typedef uint8_t vnx4uqi __attribute__((vector_size (4)));
typedef uint16_t vnx2uhi __attribute__((vector_size (4)));

typedef _Float16 vnx2hf __attribute__((vector_size (4)));

#define MASK_4		0, 5, 2, 7
#define MASK_2		0, 3

void __attribute__ ((noipa))
merge0 (vnx4qi x, vnx4qi y, vnx4qi *out)
{
  vnx4qi v = __builtin_shufflevector ((vnx4qi) x, (vnx4qi) y, MASK_4);
  *(vnx4qi*)out = v;
}

void __attribute__ ((noipa))
merge1 (vnx4uqi x, vnx4uqi y, vnx4uqi *out)
{
  vnx4uqi v = __builtin_shufflevector ((vnx4uqi) x, (vnx4uqi) y, MASK_4);
  *(vnx4uqi*)out = v;
}

void __attribute__ ((noipa))
merge2 (vnx2hi x, vnx2hi y, vnx2hi *out)
{
  vnx2hi v = __builtin_shufflevector ((vnx2hi) x, (vnx2hi) y, MASK_2);
  *(vnx2hi*)out = v;
}

void __attribute__ ((noipa))
merge3 (vnx2uhi x, vnx2uhi y, vnx2uhi *out)
{
  vnx2uhi v = __builtin_shufflevector ((vnx2uhi) x, (vnx2uhi) y, MASK_2);
  *(vnx2uhi*)out = v;
}

void __attribute__ ((noipa))
merge6 (vnx2hf x, vnx2hf y, vnx2hf *out)
{
  vnx2hf v = __builtin_shufflevector ((vnx2hf) x, (vnx2hf) y, MASK_2);
  *(vnx2hf*)out = v;
}

/* { dg-final { scan-assembler-times {\tvmerge.vvm} 5 } } */

int main(void)
{
    vnx4qi vnx4qi_x= {1,2,3,4};
    vnx4qi vnx4qi_y= {101,102,103,104};
    vnx4qi vnx4qi_expect= {1,102,3,104};
    vnx4qi vnx4qi_real;
    merge0(vnx4qi_x,vnx4qi_y,&vnx4qi_real);
    for(int i=0; i<4; i++)
        if(vnx4qi_real[i]!=vnx4qi_expect[i]) {
            __builtin_abort();
        }

    vnx4uqi vnx4uqi_x= {1,2,3,4};
    vnx4uqi vnx4uqi_y= {101,102,103,104};
    vnx4uqi vnx4uqi_expect= {1,102,3,104};
    vnx4uqi vnx4uqi_real;
    merge1(vnx4uqi_x,vnx4uqi_y,&vnx4uqi_real);
    for(int i=0; i<4; i++)
        if(vnx4uqi_real[i]!=vnx4uqi_expect[i]) {
            __builtin_abort();
        }

    vnx2hi vnx2hi_x= {1,2};
    vnx2hi vnx2hi_y= {101,102};
    vnx2hi vnx2hi_expect= {1,102};
    vnx2hi vnx2hi_real;
    merge2(vnx2hi_x,vnx2hi_y,&vnx2hi_real);
    for(int i=0; i<2; i++)
        if(vnx2hi_real[i]!=vnx2hi_expect[i]) {
            __builtin_abort();
        }

    vnx2uhi vnx2uhi_x= {1,2};
    vnx2uhi vnx2uhi_y= {101,102};
    vnx2uhi vnx2uhi_expect= {1,102};
    vnx2uhi vnx2uhi_real;
    merge3(vnx2uhi_x,vnx2uhi_y,&vnx2uhi_real);
    for(int i=0; i<2; i++)
        if(vnx2uhi_real[i]!=vnx2uhi_expect[i]) {
            __builtin_abort();
        }

    vnx2hf vnx2hf_x= {1.0,2.0};
    vnx2hf vnx2hf_y= {1.1,2.1};
    vnx2hf vnx2hf_expect= {1.0,2.1};
    vnx2hf vnx2hf_real;
    merge6(vnx2hf_x,vnx2hf_y,&vnx2hf_real);
    for(int i=0; i<2; i++)
        if(vnx2hf_real[i]!=vnx2hf_expect[i]) {
            __builtin_abort();
        }

    return 0;
}
