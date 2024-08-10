/* Contributed by Dodji Seketeli <dodji@redhat.com> */
/* Origin: PR preprocessor/7263 */
/* { dg-options "-pedantic -std=c89 -ftrack-macro-expansion=1" } */
/* { dg-do compile } */

/* This tests the proprer suppression of warning coming from macro
   defined in system headers and expanded in a non-system header
   location.  */
#pragma GCC system_header

#define _Complex __complex__
#define _Complex_I 1.0iF

#define U_LL unsigned long long
#define ONE_ULL 1ULL

static _Complex float c = _Complex_I + _Complex_I; /* These macros are defined in
						    system header so we should
						    have no warning here.  */
U_LL u = ONE_ULL; /* Likewise here.  */

unsigned long long v = 1ULL; /* { dg-warning "long long" } */
