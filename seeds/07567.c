/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized -ftree-vectorize" } */

#define MODIFY
#define USE
#define M 1024
unsigned int arr1[M];
unsigned int arr2[M];
volatile unsigned int sink;

unsigned int
test (void)
{
  unsigned int sum = 0;
  for (int i = 0; i < M; i++)
    {
#ifdef MODIFY
      /* Modify the loop accumulator using a chain of operations - this should
         not affect its rank biasing.  */
      sum |= 1;
      sum ^= 2;
#endif
#ifdef STORE
      /* Save the loop accumulator into a global variable - this should not
         affect its rank biasing.  */
      sink = sum;
#endif
#ifdef USE
      /* Add a tricky use of the loop accumulator - this should prevent its
         rank biasing.  */
      i = (i + sum) % M;
#endif
      /* Use addends with different ranks.  */
      sum += arr1[i];
      sum += arr2[((i ^ 1) + 1) % M];
    }
  return sum;
}

/* Check that if the loop accumulator has multiple uses inside the loop, it's
   not forced to the end of the reassociation chain.  */
/* { dg-final { scan-tree-dump-times {(?:vect_)?sum_[\d._]+ = (?:(?:vect_)?_[\d._]+ \+ (?:vect_)?sum_[\d._]+|(?:vect_)?sum_[\d._]+ \+ (?:vect_)?_[\d._]+)} 2 "optimized" } } */
