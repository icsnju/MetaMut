/* { dg-additional-options "-Wno-analyzer-too-complex" } */

/* Shared header for testing memory leak false positives seen in OpenBLAS,
   e.g. in lapacke_cgbbrd_work.c.

   The code is of the form:

   if( LAPACKE_lsame( vect, 'b' ) || LAPACKE_lsame( vect, 'p' ) ) {
            pt_t = (lapack_complex_float*)
                LAPACKE_malloc( sizeof(lapack_complex_float) *
                                ldpt_t * MAX(1,n) );
      ...snip...
   }

   [...snip lots of code...]

   if( LAPACKE_lsame( vect, 'b' ) || LAPACKE_lsame( vect, 'p' ) ) {
            LAPACKE_free( pt_t );
   }

   where LAPACKE_lsame is a case-insensitive comparison, implemented in its
   own source file.  Without __attribute__((const)) on LAPACKE_lsame, the
   analyzer considers the execution paths where the malloc is called, and
   then the free is not called.  With __attribute__((const)), the analyzer
   ought to rule out such paths.  */

#define NULL ((void *)0)
typedef __SIZE_TYPE__ size_t;

extern void *malloc (size_t __size)
  __attribute__ ((__nothrow__ , __leaf__))
  __attribute__ ((__malloc__))
  __attribute__ ((__alloc_size__ (1)))
  __attribute__ ((__warn_unused_result__));
extern void free (void *__ptr)
  __attribute__ ((__nothrow__ , __leaf__));

/* Header adapted/reduced from
     https://github.com/xianyi/OpenBLAS/
   which has this license text.  */

/*****************************************************************************
  Copyright (c) 2014, Intel Corp.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#define lapack_int     int
#define lapack_logical lapack_int

#define LAPACKE_malloc( size ) malloc( size )
#define LAPACKE_free( p )      free( p )

#define LAPACK_ROW_MAJOR               101
#define LAPACK_COL_MAJOR               102

#define LAPACK_TRANSPOSE_MEMORY_ERROR  -1011

#define MAX(x,y) (((x) > (y)) ? (x) : (y))

#define LAPACK_GLOBAL(lcname,UCNAME)  lcname##_

typedef float _Complex lapack_complex_float;

void LAPACKE_xerbla( const char *name, int info );

void LAPACKE_cgb_trans( int matrix_layout, int m, int n,
                        int kl, int ku,
                        const lapack_complex_float *in, int ldin,
                        lapack_complex_float *out, int ldout );
void LAPACKE_cge_trans( int matrix_layout, int m, int n,
                        const lapack_complex_float* in, int ldin,
                        lapack_complex_float* out, int ldout );

#define LAPACK_cgbbrd LAPACK_GLOBAL(cgbbrd,CGBBRD)
void LAPACK_cgbbrd(
    char const* vect,
    lapack_int const* m, lapack_int const* n, lapack_int const* ncc, lapack_int const* kl, lapack_int const* ku,
    lapack_complex_float* AB, lapack_int const* ldab,
    float* D,
    float* E,
    lapack_complex_float* Q, lapack_int const* ldq,
    lapack_complex_float* PT, lapack_int const* ldpt,
    lapack_complex_float* C, lapack_int const* ldc,
    lapack_complex_float* work,
    float* rwork,
    lapack_int* info );

/* Declare LAPACKE_lsame with __attribute__((const)).  */
int LAPACKE_lsame( char ca, char cb ) __attribute__((const));

/* Testcase adapted/reduced from
     https://github.com/xianyi/OpenBLAS/blob/c5f280a7f0e875d83833d895b2b8b0e341efabf4/lapack-netlib/LAPACKE/src/lapacke_cgbbrd_work.c
   which has this license text.  */

/*****************************************************************************
  Copyright (c) 2014, Intel Corp.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************
* Contents: Native middle-level C interface to LAPACK function cgbbrd
* Author: Intel Corporation
* Generated November 2015
*****************************************************************************/

lapack_int LAPACKE_cgbbrd_work( int matrix_layout, char vect, lapack_int m,
                                lapack_int n, lapack_int ncc, lapack_int kl,
                                lapack_int ku, lapack_complex_float* ab,
                                lapack_int ldab, float* d, float* e,
                                lapack_complex_float* q, lapack_int ldq,
                                lapack_complex_float* pt, lapack_int ldpt,
                                lapack_complex_float* c, lapack_int ldc,
                                lapack_complex_float* work, float* rwork )
{
    lapack_int info = 0;
    if( matrix_layout == LAPACK_COL_MAJOR ) {
        /* Call LAPACK function and adjust info */
        LAPACK_cgbbrd( &vect, &m, &n, &ncc, &kl, &ku, ab, &ldab, d, e, q, &ldq,
                       pt, &ldpt, c, &ldc, work, rwork, &info );
        if( info < 0 ) {
            info = info - 1;
        }
    } else if( matrix_layout == LAPACK_ROW_MAJOR ) {
        lapack_int ldab_t = MAX(1,kl+ku+1);
        lapack_int ldc_t = MAX(1,m);
        lapack_int ldpt_t = MAX(1,n);
        lapack_int ldq_t = MAX(1,m);
        lapack_complex_float* ab_t = NULL;
        lapack_complex_float* q_t = NULL;
        lapack_complex_float* pt_t = NULL;
        lapack_complex_float* c_t = NULL;
        /* Check leading dimension(s) */
        if( ldab < n ) {
            info = -9;
            LAPACKE_xerbla( "LAPACKE_cgbbrd_work", info );
            return info;
        }
        if( ldc < ncc ) {
            info = -17;
            LAPACKE_xerbla( "LAPACKE_cgbbrd_work", info );
            return info;
        }
        if( ldpt < n ) {
            info = -15;
            LAPACKE_xerbla( "LAPACKE_cgbbrd_work", info );
            return info;
        }
        if( ldq < m ) {
            info = -13;
            LAPACKE_xerbla( "LAPACKE_cgbbrd_work", info );
            return info;
        }
        /* Allocate memory for temporary array(s) */
        ab_t = (lapack_complex_float*)
            LAPACKE_malloc( sizeof(lapack_complex_float) * ldab_t * MAX(1,n) );
        if( ab_t == NULL ) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        if( LAPACKE_lsame( vect, 'b' ) || LAPACKE_lsame( vect, 'q' ) ) {
            q_t = (lapack_complex_float*)
                LAPACKE_malloc( sizeof(lapack_complex_float) *
                                ldq_t * MAX(1,m) );
            if( q_t == NULL ) {
                info = LAPACK_TRANSPOSE_MEMORY_ERROR;
                goto exit_level_1;
            }
        }
        if( LAPACKE_lsame( vect, 'b' ) || LAPACKE_lsame( vect, 'p' ) ) {
            pt_t = (lapack_complex_float*)
                LAPACKE_malloc( sizeof(lapack_complex_float) *
                                ldpt_t * MAX(1,n) );
            if( pt_t == NULL ) {
                info = LAPACK_TRANSPOSE_MEMORY_ERROR;
                goto exit_level_2;
            }
        }
        if( ncc != 0 ) {
            c_t = (lapack_complex_float*)
                LAPACKE_malloc( sizeof(lapack_complex_float) *
                                ldc_t * MAX(1,ncc) );
            if( c_t == NULL ) {
                info = LAPACK_TRANSPOSE_MEMORY_ERROR;
                goto exit_level_3;
            }
        }
        /* Transpose input matrices */
        LAPACKE_cgb_trans( matrix_layout, m, n, kl, ku, ab, ldab, ab_t, ldab_t );
        if( ncc != 0 ) {
            LAPACKE_cge_trans( matrix_layout, m, ncc, c, ldc, c_t, ldc_t );
        }
        /* Call LAPACK function and adjust info */
        LAPACK_cgbbrd( &vect, &m, &n, &ncc, &kl, &ku, ab_t, &ldab_t, d, e, q_t,
                       &ldq_t, pt_t, &ldpt_t, c_t, &ldc_t, work, rwork, &info );
        if( info < 0 ) {
            info = info - 1;
        }
        /* Transpose output matrices */
        LAPACKE_cgb_trans( LAPACK_COL_MAJOR, m, n, kl, ku, ab_t, ldab_t, ab,
                           ldab );
        if( LAPACKE_lsame( vect, 'b' ) || LAPACKE_lsame( vect, 'q' ) ) {
            LAPACKE_cge_trans( LAPACK_COL_MAJOR, m, m, q_t, ldq_t, q, ldq );
        }
        if( LAPACKE_lsame( vect, 'b' ) || LAPACKE_lsame( vect, 'p' ) ) {
            LAPACKE_cge_trans( LAPACK_COL_MAJOR, n, n, pt_t, ldpt_t, pt, ldpt );
        }
        if( ncc != 0 ) {
            LAPACKE_cge_trans( LAPACK_COL_MAJOR, m, ncc, c_t, ldc_t, c, ldc );
        }
        /* Release memory and exit */
        if( ncc != 0 ) {
            LAPACKE_free( c_t );
        }
exit_level_3:
        if( LAPACKE_lsame( vect, 'b' ) || LAPACKE_lsame( vect, 'p' ) ) {
            LAPACKE_free( pt_t );
        }
exit_level_2:
        if( LAPACKE_lsame( vect, 'b' ) || LAPACKE_lsame( vect, 'q' ) ) {
            LAPACKE_free( q_t );
        }
exit_level_1:
        LAPACKE_free( ab_t );
exit_level_0:
        if( info == LAPACK_TRANSPOSE_MEMORY_ERROR ) {
            LAPACKE_xerbla( "LAPACKE_cgbbrd_work", info );
        }
    } else {
        info = -1;
        LAPACKE_xerbla( "LAPACKE_cgbbrd_work", info );
    }
    return info; /* { dg-bogus "leak of 'q_t'" "leak of q_t" } */
    /* { dg-bogus "leak of 'pt_t'" "leak of pt_t" { target *-*-* } .-1 } */
}
