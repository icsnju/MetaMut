/* { dg-lto-do link } */
/* { dg-require-effective-target fpic } */
/* { dg-lto-options {{-fPIC -r -nostdlib -O2 -flto} {-fPIC -r -nostdlib -O2 -flto -flto-partition=1to1}} } */
/* { dg-extra-ld-options "-flinker-output=nolto-rel" } */

typedef struct _IO_FILE FILE;
extern struct _IO_FILE *stderr;
void diagnostic_initialize (FILE **stream) { *stream = stderr; }
