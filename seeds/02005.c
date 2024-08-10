/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "-O3 --param riscv-autovec-preference=fixed-vlmax -Wno-psabi" } */

/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfhmin -mabi=lp64d -O3 --param riscv-autovec-preference=fixed-vlmax -Wno-psabi" } */

#include <stdint-gcc.h>

typedef int8_t vnx16qi __attribute__((vector_size (16)));
typedef int16_t vnx8hi __attribute__((vector_size (16)));
typedef int32_t vnx4si __attribute__((vector_size (16)));
typedef int64_t vnx2di __attribute__((vector_size (16)));
typedef uint8_t vnx16uqi __attribute__((vector_size (16)));
typedef uint16_t vnx8uhi __attribute__((vector_size (16)));
typedef uint32_t vnx4usi __attribute__((vector_size (16)));
typedef uint64_t vnx2udi __attribute__((vector_size (16)));

typedef _Float16 vnx8hf __attribute__((vector_size (16)));
typedef float vnx4sf __attribute__((vector_size (16)));
typedef double vnx2df __attribute__((vector_size (16)));

#define MASK_16		0, 17, 2, 19, 4, 21, 6, 23, 8, 25, 10, 27, 12, 29, 14, 31
#define MASK_8		0, 9, 2, 11, 4, 13, 6, 15
#define MASK_4		0, 5, 2, 7
#define MASK_2		0, 3

void __attribute__ ((noipa))
merge0 (vnx16qi x, vnx16qi y, vnx16qi *out)
{
  vnx16qi v = __builtin_shufflevector ((vnx16qi) x, (vnx16qi) y, MASK_16);
  *(vnx16qi*)out = v;
}

void __attribute__ ((noipa))
merge1 (vnx16uqi x, vnx16uqi y, vnx16uqi *out)
{
  vnx16uqi v = __builtin_shufflevector ((vnx16uqi) x, (vnx16uqi) y, MASK_16);
  *(vnx16uqi*)out = v;
}

void __attribute__ ((noipa))
merge2 (vnx8hi x, vnx8hi y, vnx8hi *out)
{
  vnx8hi v = __builtin_shufflevector ((vnx8hi) x, (vnx8hi) y, MASK_8);
  *(vnx8hi*)out = v;
}

void __attribute__ ((noipa))
merge3 (vnx8uhi x, vnx8uhi y, vnx8uhi *out)
{
  vnx8uhi v = __builtin_shufflevector ((vnx8uhi) x, (vnx8uhi) y, MASK_8);
  *(vnx8uhi*)out = v;
}

void __attribute__ ((noipa))
merge4 (vnx4si x, vnx4si y, vnx4si *out)
{
  vnx4si v = __builtin_shufflevector ((vnx4si) x, (vnx4si) y, MASK_4);
  *(vnx4si*)out = v;
}

void __attribute__ ((noipa))
merge5 (vnx4usi x, vnx4usi y, vnx4usi *out)
{
  vnx4usi v = __builtin_shufflevector ((vnx4usi) x, (vnx4usi) y, MASK_4);
  *(vnx4usi*)out = v;
}

void __attribute__ ((noipa))
merge6 (vnx2di x, vnx2di y, vnx2di *out)
{
  vnx2di v = __builtin_shufflevector ((vnx2di) x, (vnx2di) y, MASK_2);
  *(vnx2di*)out = v;
}

void __attribute__ ((noipa))
merge7 (vnx2udi x, vnx2udi y, vnx2udi *out)
{
  vnx2udi v = __builtin_shufflevector ((vnx2udi) x, (vnx2udi) y, MASK_2);
  *(vnx2udi*)out = v;
}

void __attribute__ ((noipa))
merge8 (vnx8hf x, vnx8hf y, vnx8hf *out)
{
  vnx8hf v = __builtin_shufflevector ((vnx8hf) x, (vnx8hf) y, MASK_8);
  *(vnx8hf*)out = v;
}

void __attribute__ ((noipa))
merge9 (vnx4sf x, vnx4sf y, vnx4sf *out)
{
  vnx4sf v = __builtin_shufflevector ((vnx4sf) x, (vnx4sf) y, MASK_4);
  *(vnx4sf*)out = v;
}

void __attribute__ ((noipa))
merge10 (vnx2df x, vnx2df y, vnx2df *out)
{
  vnx2df v = __builtin_shufflevector ((vnx2df) x, (vnx2df) y, MASK_2);
  *(vnx2df*)out = v;
}

/* { dg-final { scan-assembler-times {\tvmerge.vvm} 11 } } */

int main(void)
{
    vnx16qi vnx16qi_x= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    vnx16qi vnx16qi_y= {201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216};
    vnx16qi vnx16qi_expect= {1,202,3,204,5,206,7,208,9,210,11,212,13,214,15,216};
    vnx16qi vnx16qi_real;
    merge0(vnx16qi_x,vnx16qi_y, &vnx16qi_real);
    for(int i=0; i<16; i++)
        if(vnx16qi_real[i]!=vnx16qi_expect[i]) {
            __builtin_abort();
        }

    vnx16uqi vnx16uqi_x= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    vnx16uqi vnx16uqi_y= {101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116};
    vnx16uqi vnx16uqi_expect= {1,102,3,104,5,106,7,108,9,110,11,112,13,114,15,116};
    vnx16uqi vnx16uqi_real;
    merge1(vnx16uqi_x,vnx16uqi_y, &vnx16uqi_real);
    for(int i=0; i<16; i++)
        if(vnx16uqi_real[i]!=vnx16uqi_expect[i]) {
            __builtin_abort();
        }

    vnx8hi vnx8hi_x= {1,2,3,4,5,6,7,8};
    vnx8hi vnx8hi_y= {101,102,103,104,105,106,107,108};
    vnx8hi vnx8hi_expect= {1,102,3,104,5,106,7,108};
    vnx8hi vnx8hi_real;
    merge2(vnx8hi_x,vnx8hi_y, &vnx8hi_real);
    for(int i=0; i<8; i++)
        if(vnx8hi_real[i]!=vnx8hi_expect[i]) {
            __builtin_abort();
        }

    vnx8uhi vnx8uhi_x= {1,2,3,4,5,6,7,8};
    vnx8uhi vnx8uhi_y= {101,102,103,104,105,106,107,108};
    vnx8uhi vnx8uhi_expect= {1,102,3,104,5,106,7,108};
    vnx8uhi vnx8uhi_real;
    merge3(vnx8uhi_x,vnx8uhi_y, &vnx8uhi_real);
    for(int i=0; i<8; i++)
        if(vnx8uhi_real[i]!=vnx8uhi_expect[i]) {
            __builtin_abort();
        }

    vnx4si vnx4si_x= {1,2,3,4};
    vnx4si vnx4si_y= {101,102,103,104};
    vnx4si vnx4si_expect= {1,102,3,104};
    vnx4si vnx4si_real;
    merge4(vnx4si_x,vnx4si_y,&vnx4si_real);
    for(int i=0; i<4; i++)
        if(vnx4si_real[i]!=vnx4si_expect[i]) {
            __builtin_abort();
        }

    vnx4usi vnx4usi_x= {1,2,3,4};
    vnx4usi vnx4usi_y= {101,102,103,104};
    vnx4usi vnx4usi_expect= {1,102,3,104};
    vnx4usi vnx4usi_real;
    merge5(vnx4usi_x,vnx4usi_y,&vnx4usi_real);
    for(int i=0; i<4; i++)
        if(vnx4usi_real[i]!=vnx4usi_expect[i]) {
            __builtin_abort();
        }

    vnx2di vnx2di_x= {1,2};
    vnx2di vnx2di_y= {101,102};
    vnx2di vnx2di_expect= {1,102};
    vnx2di vnx2di_real;
    merge6(vnx2di_x,vnx2di_y,&vnx2di_real);
    for(int i=0; i<2; i++)
        if(vnx2di_real[i]!=vnx2di_expect[i]) {
            __builtin_abort();
        }

    vnx2udi vnx2udi_x= {1,2};
    vnx2udi vnx2udi_y= {101,102};
    vnx2udi vnx2udi_expect= {1,102};
    vnx2udi vnx2udi_real;
    merge7(vnx2udi_x,vnx2udi_y,&vnx2udi_real);
    for(int i=0; i<2; i++)
        if(vnx2udi_real[i]!=vnx2udi_expect[i]) {
            __builtin_abort();
        }

    vnx8hf vnx8hf_x= {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0};
    vnx8hf vnx8hf_y= {1.1,2.1,3.1,4.1,5.1,6.1,7.1,8.1};
    vnx8hf vnx8hf_expect= {1.0,2.1,3.0,4.1,5.0,6.1,7.0,8.1};
    vnx8hf vnx8hf_real;
    merge8(vnx8hf_x,vnx8hf_y,&vnx8hf_real);
    for(int i=0; i<8; i++)
        if(vnx8hf_real[i]!=vnx8hf_expect[i]) {
            __builtin_abort();
        }

    vnx4sf vnx4sf_x= {1.0,2.0,3.0,4.0};
    vnx4sf vnx4sf_y= {1.1,2.1,3.1,4.1};
    vnx4sf vnx4sf_expect= {1.0,2.1,3.0,4.1};
    vnx4sf vnx4sf_real;
    merge9(vnx4sf_x,vnx4sf_y,&vnx4sf_real);
    for(int i=0; i<4; i++)
        if(vnx4sf_real[i]!=vnx4sf_expect[i]) {
            __builtin_abort();
        }

    vnx2df vnx2df_x= {1.0,2.0};
    vnx2df vnx2df_y= {1.1,2.1};
    vnx2df vnx2df_expect= {1.0,2.1};
    vnx2df vnx2df_real;
    merge10(vnx2df_x,vnx2df_y,&vnx2df_real);
    for(int i=0; i<2; i++)
        if(vnx2df_real[i]!=vnx2df_expect[i]) {
            __builtin_abort();
        }

    return 0;
}
