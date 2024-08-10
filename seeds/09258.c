/* { dg-additional-options "-fdiagnostics-text-art-charset=unicode" } */
/* { dg-skip-if "" { powerpc-ibm-aix* } } */

#include <string.h>

void
test_non_ascii ()
{
  char buf[5];
  strcpy (buf, "文字化け"); /* { dg-warning "stack-based buffer overflow" } */
  /* { dg-warning "'__builtin_memcpy' writing 13 bytes into a region of size 5 overflows the destination" "" { target *-*-* } .-1 } */
}

/* With unicode support, we should show the printable unicode chars.  */

/* { dg-begin-multiline-output "" }

  ┌─────┬─────┬─────┬────┬────┐┌────┬────┬────┬────┬────┬────┬────┬──────┐
  │ [0] │ [1] │ [2] │[3] │[4] ││[5] │[6] │[7] │[8] │[9] │[10]│[11]│ [12] │
  ├─────┼─────┼─────┼────┼────┤├────┼────┼────┼────┼────┼────┼────┼──────┤
  │0xe6 │0x96 │0x87 │0xe5│0xad││0x97│0xe5│0x8c│0x96│0xe3│0x81│0x91│ 0x00 │
  ├─────┴─────┴─────┼────┴────┴┴────┼────┴────┴────┼────┴────┴────┼──────┤
  │     U+6587      │    U+5b57     │    U+5316    │    U+3051    │U+0000│
  ├─────────────────┼───────────────┼──────────────┼──────────────┼──────┤
  │       文        │      字       │      化      │      け      │ NUL  │
  ├─────────────────┴───────────────┴──────────────┴──────────────┴──────┤
  │                  string literal (type: 'char[13]')                   │
  └──────────────────────────────────────────────────────────────────────┘
     │     │     │    │    │     │    │    │    │    │    │    │     │
     │     │     │    │    │     │    │    │    │    │    │    │     │
     v     v     v    v    v     v    v    v    v    v    v    v     v
  ┌─────┬────────────────┬────┐┌─────────────────────────────────────────┐
  │ [0] │      ...       │[4] ││                                         │
  ├─────┴────────────────┴────┤│            after valid range            │
  │  'buf' (type: 'char[5]')  ││                                         │
  └───────────────────────────┘└─────────────────────────────────────────┘
  ├─────────────┬─────────────┤├────────────────────┬────────────────────┤
                │                                   │
       ╭────────┴────────╮                ╭─────────┴─────────╮
       │capacity: 5 bytes│                │overflow of 8 bytes│
       ╰─────────────────╯                ╰───────────────────╯

   { dg-end-multiline-output "" } */
