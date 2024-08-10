/* PR c/102989 */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=c2x -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __FLT_MANT_DIG__ == 24
#if __BITINT_MAXWIDTH__ >= 135
__attribute__((noipa)) _BitInt(135)
testflt_135 (float d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(135)
testfltu_135 (float d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) _BitInt(192)
testflt_192 (float d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(192)
testfltu_192 (float d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) _BitInt(575)
testflt_575 (float d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(575)
testfltu_575 (float d)
{
  return d;
}
#endif
#endif

#if __DBL_MANT_DIG__ == 53
#if __BITINT_MAXWIDTH__ >= 135
__attribute__((noipa)) _BitInt(135)
testdbl_135 (double d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(135)
testdblu_135 (double d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) _BitInt(192)
testdbl_192 (double d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(192)
testdblu_192 (double d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) _BitInt(575)
testdbl_575 (double d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(575)
testdblu_575 (double d)
{
  return d;
}
#endif
#endif

int
main ()
{
#if __FLT_MANT_DIG__ == 24
#if __BITINT_MAXWIDTH__ >= 135
  static float flt_135[] = {
    -1.0f, -0.96875f, -0.5f, -0.0f, 0.25f, 0.0f, 1.0f, -42.0f, 16777215.25f,
    -50855501824.5f, 39580962846540824576.0f,
    -122147759335346835551823516808712814592.0f,
    -85070591730234615865843651857942052864.0f
  };
  static _BitInt(135) fltr_135[] = {
    -1wb, 0wb, 0wb, 0wb, 0wb, 0wb, 1wb, -42wb, 16777215wb,
    -50855501824wb, 39580962846540824576wb,
    -122147759335346835551823516808712814592wb,
    -85070591730234615865843651857942052864wb
  };
  for (int i = 0; i < 13; ++i)
    if (testflt_135 (flt_135[i]) != fltr_135[i])
      __builtin_abort ();
  static float fltu_135[] = {
    -0.9375f, -0.25f, -0.0f, 0.125f, 0.0f, 1.0f, 42.59375f, 16777215.75f,
    397530272.5f, 1557145973265213792232550652968960.0f,
    69722182498815269047577928656018735104.0f
  };
  static unsigned _BitInt(135) fltur_135[] = {
    0uwb, 0uwb, 0uwb, 0uwb, 0uwb, 1uwb, 42uwb, 16777216uwb,
    397530272uwb, 1557145973265213792232550652968960uwb,
    69722182498815269047577928656018735104uwb
  };
  for (int i = 0; i < 11; ++i)
    if (testfltu_135 (fltu_135[i]) != fltur_135[i])
      __builtin_abort ();
#endif
#if __BITINT_MAXWIDTH__ >= 192
  static float flt_192[] = {
    -0.984375f, -0.75f, -0.0f, 0.625f, 0.0f, 1.0f, -42.0f, 16777215.25f,
    -166724322197504.5f, 7557890186859550768856301568.0f,
    6614377725485260848635133905887821824.0f,
    -85070591730234615865843651857942052864.0f
  };
  static _BitInt(192) fltr_192[] = {
    0wb, 0wb, 0wb, 0wb, 0wb, 1wb, -42wb, 16777215wb,
    -166724322197504wb, 7557890186859550768856301568wb,
    6614377725485260848635133905887821824wb,
    -85070591730234615865843651857942052864wb
  };
  for (int i = 0; i < 12; ++i)
    if (testflt_192 (flt_192[i]) != fltr_192[i])
      __builtin_abort ();
  static float fltu_192[] = {
    -0.9921875f, -0.125f, -0.0f, __FLT_MIN__, 0.0f, 1.0f, 42.125f,
    16777216.75f, 2197600337920.75f, 4591683053408742366929307227985543168.0f,
    89668786387864080590151097276117811200.0f
  };
  static unsigned _BitInt(192) fltur_192[] = {
    0uwb, 0uwb, 0uwb, 0uwb, 0uwb, 1uwb, 42uwb, 16777216uwb,
    2197600337920uwb, 4591683053408742366929307227985543168uwb,
    89668786387864080590151097276117811200uwb
  };
  for (int i = 0; i < 11; ++i)
    if (testfltu_192 (fltu_192[i]) != fltur_192[i])
      __builtin_abort ();
#endif
#if __BITINT_MAXWIDTH__ >= 575
  static float flt_575[] = {
    -0.99609375f, -0.0625f, -0.0f, 0.0001f, 0.0f, 1.0f, -28.0f, 16777214.75f,
    -1321998852554752.5f, 3447217350995641328695097279119360.0f,
    -153684583933125564944797950598195445760.0f,
    -6614377725485260848635133905887821824.0f
  };
  static _BitInt(575) fltr_575[] = {
    0wb, 0wb, 0wb, 0wb, 0wb, 1wb, -28wb, 16777215wb,
    -1321998852554752wb, 3447217350995641328695097279119360wb,
    -153684583933125564944797950598195445760wb,
    -6614377725485260848635133905887821824wb
  };
  for (int i = 0; i < 12; ++i)
    if (testflt_575 (flt_575[i]) != fltr_575[i])
      __builtin_abort ();
  static float fltu_575[] = {
    -0.9921875f, -0.125f, -0.0f, 4.0f * __FLT_MIN__, 0.0f, 1.0f, 38.125f,
    16777210.75f, 450604433408.75f, 49420393052787309452540595771398946816.0f,
    144076816645080570490809349637635309568.0f
  };
  static unsigned _BitInt(575) fltur_575[] = {
    0uwb, 0uwb, 0uwb, 0uwb, 0uwb, 1uwb, 38uwb, 16777211uwb,
    450604433408uwb, 49420393052787309452540595771398946816uwb,
    144076816645080570490809349637635309568uwb
  };
  for (int i = 0; i < 11; ++i)
    if (testfltu_575 (fltu_575[i]) != fltur_575[i])
      __builtin_abort ();
#endif
#endif
#if __DBL_MANT_DIG__ == 53
#if __BITINT_MAXWIDTH__ >= 135
  static double dbl_135[] = {
    -0.998046875, -0.5, -0.0, 0.25, 0.0, 1.0, -42.0, 4294967295.25,
    -600381955574889.5, 31830517255028909053694705664.0,
    -18738990071541038357046558935673124945920.0,
    -21778071482940061661655974875633165533184.0
  };
  static _BitInt(135) dblr_135[] = {
    0wb, 0wb, 0wb, 0wb, 0wb, 1wb, -42wb, 4294967295wb,
    -600381955574889wb, 31830517255028909053694705664wb,
    -18738990071541038357046558935673124945920wb,
    -21778071482940061661655974875633165533183wb - 1wb
  };
  for (int i = 0; i < 12; ++i)
    if (testdbl_135 (dbl_135[i]) != dblr_135[i])
      __builtin_abort ();
  static double dblu_135[] = {
    -0.9990234375, -0.25, -0.0, 0.125, 0.0, 1.0, 42.59375, 4294967295.75,
    1379919377114330.5, 1887176628826898072984354816.0,
    28122698032432725461429202074509590396928.0
  };
  static unsigned _BitInt(135) dblur_135[] = {
    0uwb, 0uwb, 0uwb, 0uwb, 0uwb, 1uwb, 42uwb, 4294967295uwb,
    1379919377114330uwb, 1887176628826898072984354816uwb,
    28122698032432725461429202074509590396928uwb
  };
  for (int i = 0; i < 11; ++i)
    if (testdblu_135 (dblu_135[i]) != dblur_135[i])
      __builtin_abort ();
#endif
#if __BITINT_MAXWIDTH__ >= 192
  static double dbl_192[] = {
    -0.99951171875, -0.75, -0.0, 0.625, 0.0, 1.0, -42.0, 4294967295.25,
    -334692618508390720.5, 7241713824636372052614321786924022698463985664.0,
    1051470826442514757151114043324296190023320100185399361536.0,
    -3138550867693340381917894711603833208051177722232017256448.0
  };
  static _BitInt(192) dblr_192[] = {
    0wb, 0wb, 0wb, 0wb, 0wb, 1wb, -42wb, 4294967295wb,
    -334692618508390720wb, 7241713824636372052614321786924022698463985664wb,
    1051470826442514757151114043324296190023320100185399361536wb,
    -3138550867693340381917894711603833208051177722232017256447wb - 1wb
  };
  for (int i = 0; i < 12; ++i)
    if (testdbl_192 (dbl_192[i]) != dblr_192[i])
      __builtin_abort ();
  static double dblu_192[] = {
    -0.999755859375, -0.25, -0.0, __DBL_MIN__, 0.0, 1.0, 42.125, 4294967296.75,
    85543274084479312.75, 1452517026416706854711122198528.0,
    4878159368183578289135952951143197426988727705359645409280.0
  };
  static unsigned _BitInt(192) dblur_192[] = {
    0uwb, 0uwb, 0uwb, 0uwb, 0uwb, 1uwb, 42uwb, 4294967296uwb,
    85543274084479312uwb, 1452517026416706854711122198528uwb,
    4878159368183578289135952951143197426988727705359645409280uwb
  };
  for (int i = 0; i < 11; ++i)
    if (testdblu_192 (dblu_192[i]) != dblur_192[i])
      __builtin_abort ();
#endif
#if __BITINT_MAXWIDTH__ >= 575
  static double dbl_575[] = {
    -1.0, -0.9998779296875, -0.125, -0.0, 0.0001, 0.0, 1.0, -28.0, 4294967294.75,
    -37518052491018912.5, 103740344328578397848785318842252129922803939603530145015162908477052192630505472.0,
    -60264112735379285992549557294906950572505306720710356591257742566415478531569283626542380634430345616295459560427283714375540825055746407845009719778430303250931745441185792.0,
    -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174784.0
  };
  static _BitInt(575) dblr_575[] = {
    -1wb, 0wb, 0wb, 0wb, 0wb, 0wb, 1wb, -28wb, 4294967294wb,
    -37518052491018912wb, 103740344328578397848785318842252129922803939603530145015162908477052192630505472wb,
    -60264112735379285992549557294906950572505306720710356591257742566415478531569283626542380634430345616295459560427283714375540825055746407845009719778430303250931745441185792wb,
    -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1wb
  };
  for (int i = 0; i < 13; ++i)
    if (testdbl_575 (dbl_575[i]) != dblr_575[i])
      __builtin_abort ();
  static double dblu_575[] = {
    -0.99993896484375, -0.0625, -0.0, 4.0 * __DBL_MIN__, 0.0, 1.0, 38.125, 4294967297.75,
    138240328005104064.75, 1588129215384992270224118343146332410113351482118715170651778867930903010675691494429363974899842924150784.0,
    106390366882040443685518677989907085524608028099417390750037507495093586319031581000179209552004867195248003952212636389496136628539779816025838629984567658036669448510767104.0
  };
  static unsigned _BitInt(575) dblur_575[] = {
    0uwb, 0uwb, 0uwb, 0uwb, 0uwb, 1uwb, 38uwb, 4294967297uwb,
    138240328005104064uwb, 1588129215384992270224118343146332410113351482118715170651778867930903010675691494429363974899842924150784uwb,
    106390366882040443685518677989907085524608028099417390750037507495093586319031581000179209552004867195248003952212636389496136628539779816025838629984567658036669448510767104uwb
  };
  for (int i = 0; i < 11; ++i)
    if (testdblu_575 (dblu_575[i]) != dblur_575[i])
      __builtin_abort ();
#endif
#endif
}
