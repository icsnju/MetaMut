/* PR debug/45882 */
/* { dg-do run } */
/* { dg-options "-g" } */

/* Copyright (C) 2018 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef PREVENT_OPTIMIZATION_H
#define PREVENT_OPTIMIZATION_H

#ifdef PREVENT_OPTIMIZATION
#define ATTRIBUTE_USED __attribute__((used))
#else
#define ATTRIBUTE_USED
#endif

#endif

extern void abort (void);
int a[1024] ATTRIBUTE_USED;
volatile short int v;

__attribute__((noinline,noclone,used)) int
foo (int i, int j)
{
  int b = i;		/* { dg-final { gdb-test .+4 "b" "7" } } */
  int c = i + 4;	/* { dg-final { gdb-test .+3 "c" "11" } } */
  int d = a[i];		/* { dg-final { gdb-test .+2 "d" "112" } } */
  int e = a[i + 6];	/* { dg-final { gdb-test .+1 "e" "142" } } */
  ++v;
  return ++j;
}

int
main (void)
{
  int l;
  asm ("" : "=r" (l) : "0" (7));
  a[7] = 112;
  a[7 + 6] = 142;
  if (foo (l, 7) != 8)
    abort ();
  return l - 7;
}
