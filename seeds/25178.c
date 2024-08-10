/* Check offloaded function's attributes and classification for OpenACC
   kernels.  */

/* { dg-additional-options "--param openacc-kernels=decompose" } */

/* { dg-additional-options "-O2" }
   { dg-additional-options "-fopt-info-all-omp" }
   { dg-additional-options "-fdump-tree-ompexp" }
   { dg-additional-options "-fdump-tree-parloops1-all" }
   { dg-additional-options "-fdump-tree-oaccloops" } */

/* { dg-additional-options "--param=openacc-privatization=noisy" }
   Prune a few: uninteresting:
   { dg-prune-output {note: variable 'D\.[0-9]+' declared in block isn't candidate for adjusting OpenACC privatization level: not addressable} } */

/* { dg-additional-options "-Wopenacc-parallelism" } for testing/documenting
   aspects of that functionality.  */

#define N 1024

extern unsigned int *__restrict a;
extern unsigned int *__restrict b;
extern unsigned int *__restrict c;

void KERNELS ()
{
#pragma acc kernels copyin (a[0:N], b[0:N]) copyout (c[0:N]) /* { dg-line l_compute1 } */
  /* { dg-note {OpenACC 'kernels' decomposition: variable 'i' declared in block requested to be made addressable} {} { target *-*-* } l_compute1 } */
  /* { dg-note {variable 'i' made addressable} {} { target *-*-* } l_compute1 } */
  /* { dg-note {variable 'i' declared in block is candidate for adjusting OpenACC privatization level} {} { target *-*-* } l_compute1 } */
  /* { dg-note {variable '[abc]\.[0-9]+' declared in block isn't candidate for adjusting OpenACC privatization level: not addressable} {} { target *-*-* } l_compute1 } */
  /* { dg-optimized {assigned OpenACC gang loop parallelism} {} { target *-*-* } l_compute1 } */
  /* { dg-note {beginning 'parloops' part in OpenACC 'kernels' region} {} { target *-*-* } .+1 } */
  for (unsigned int i = 0; i < N; i++)
    c[i] = a[i] + b[i];
}

/* Check the offloaded function's attributes.
   { dg-final { scan-tree-dump-times "(?n)__attribute__\\(\\(oacc kernels, omp target entrypoint, noclone\\)\\)" 1 "ompexp" } } */

/* Check that exactly one OpenACC kernels construct is analyzed, and that it
   can be parallelized.
   { dg-final { scan-tree-dump-times "SUCCESS: may be parallelized" 1 "parloops1" } }
   { dg-final { scan-tree-dump-times "(?n)__attribute__\\(\\(oacc kernels parallelized, oacc function \\(, , \\), oacc kernels, omp target entrypoint, noclone\\)\\)" 1 "parloops1" } }
   { dg-final { scan-tree-dump-not "FAILED:" "parloops1" } } */

/* Check the offloaded function's classification and compute dimensions (will
   always be 1 x 1 x 1 for non-offloading compilation).
   { dg-final { scan-tree-dump-times "(?n)Function is parallelized OpenACC kernels offload" 1 "oaccloops" } }
   { dg-final { scan-tree-dump-times "(?n)Compute dimensions \\\[1, 1, 1\\\]" 1 "oaccloops" } }
   { dg-final { scan-tree-dump-times "(?n)__attribute__\\(\\(oacc function \\(1, 1, 1\\), oacc kernels parallelized, oacc function \\(, , \\), oacc kernels, omp target entrypoint, noclone\\)\\)" 1 "oaccloops" } } */
