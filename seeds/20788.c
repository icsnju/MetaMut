/* PR c/102989 */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=c2x -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) _BitInt(192)
test192 (int a, int b, int c, int d, int e, int f, int g)
{
  _BitInt(192) r;
  if (a)
    r = 0wb;
  else if (b)
    r = -1wb;
  else if (c)
    r = 8509079622807245684wb;
  else if (d)
    r = -906140470171472463wb;
  else if (e)
    r = 16213049720239626860wb;
  else if (f)
    r = 147892183939833533694801609136018843670wb;
  else if (g)
    r = -65591640145960897840899726907249505215wb;
  else
    r = 2430673202376479588648422478850704999441011825542092986793wb;
  return r;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) _BitInt(575)
test575 (int a, int b, int c, int d, int e, int f, int g)
{
  _BitInt(575) r;
  if (a)
    r = 0wb;
  else if (b)
    r = -1wb;
  else if (c)
    r = 8509079622807245684wb;
  else if (d)
    r = -906140470171472463wb;
  else if (e)
    r = 16213049720239626860wb;
  else if (f)
    r = 147892183939833533694801609136018843670wb;
  else if (g)
    r = -65591640145960897840899726907249505215wb;
  else
    r = -61793624342683909782972416440098036406501521208177932544452266579025268523357080574330290352054553626825262073339234752683755828499698570848704076099511077751520231100121930wb;
  return r;
}
#endif

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 192
  if (test192 (1, 0, 0, 0, 0, 0, 0) != 0wb
      || test192 (0, 1, 0, 0, 0, 0, 0) != -1wb
      || test192 (0, 0, 1, 0, 0, 0, 0) != 8509079622807245684wb
      || test192 (0, 0, 0, 1, 0, 0, 0) != -906140470171472463wb
      || test192 (0, 0, 0, 0, 1, 0, 0) != 16213049720239626860wb
      || test192 (0, 0, 0, 0, 0, 1, 0) != 147892183939833533694801609136018843670wb
      || test192 (0, 0, 0, 0, 0, 0, 1) != -65591640145960897840899726907249505215wb
      || test192 (0, 0, 0, 0, 0, 0, 0) != 2430673202376479588648422478850704999441011825542092986793wb)
    __builtin_abort ();
#endif
#if __BITINT_MAXWIDTH__ >= 575
  if (test575 (1, 0, 0, 0, 0, 0, 0) != 0wb
      || test575 (0, 1, 0, 0, 0, 0, 0) != -1wb
      || test575 (0, 0, 1, 0, 0, 0, 0) != 8509079622807245684wb
      || test575 (0, 0, 0, 1, 0, 0, 0) != -906140470171472463wb
      || test575 (0, 0, 0, 0, 1, 0, 0) != 16213049720239626860wb
      || test575 (0, 0, 0, 0, 0, 1, 0) != 147892183939833533694801609136018843670wb
      || test575 (0, 0, 0, 0, 0, 0, 1) != -65591640145960897840899726907249505215wb
      || test575 (0, 0, 0, 0, 0, 0, 0) != -61793624342683909782972416440098036406501521208177932544452266579025268523357080574330290352054553626825262073339234752683755828499698570848704076099511077751520231100121930wb)
    __builtin_abort ();
#endif
}
