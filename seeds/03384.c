/* PR c/102989 */
/* { dg-do run } */
/* { dg-require-effective-target fenv_exceptions } */
/* { dg-options "-std=c2x" } */
/* { dg-add-options ieee } */

#include <fenv.h>

#if __FLT_MANT_DIG__ == 24
#if __BITINT_MAXWIDTH__ >= 135
__attribute__((noipa)) float
testflt_135 (_BitInt(135) b)
{
  return b;
}

__attribute__((noipa)) float
testfltu_135 (unsigned _BitInt(135) b)
{
  return b;
}
#endif

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) float
testflt_192 (_BitInt(192) b)
{
  return b;
}

__attribute__((noipa)) float
testfltu_192 (unsigned _BitInt(192) b)
{
  return b;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) float
testflt_575 (_BitInt(575) b)
{
  return b;
}

__attribute__((noipa)) float
testfltu_575 (unsigned _BitInt(575) b)
{
  return b;
}
#endif
#endif

#if __DBL_MANT_DIG__ == 53
#if __BITINT_MAXWIDTH__ >= 135
__attribute__((noipa)) double
testdbl_135 (_BitInt(135) b)
{
  return b;
}

__attribute__((noipa)) double
testdblu_135 (unsigned _BitInt(135) b)
{
  return b;
}
#endif

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) double
testdbl_192 (_BitInt(192) b)
{
  return b;
}

__attribute__((noipa)) double
testdblu_192 (unsigned _BitInt(192) b)
{
  return b;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) double
testdbl_575 (_BitInt(575) b)
{
  return b;
}

__attribute__((noipa)) double
testdblu_575 (unsigned _BitInt(575) b)
{
  return b;
}
#endif
#endif

#if __LDBL_MANT_DIG__ == 64
#if __BITINT_MAXWIDTH__ >= 135
__attribute__((noipa)) long double
testldbl_135 (_BitInt(135) b)
{
  return b;
}

__attribute__((noipa)) long double
testldblu_135 (unsigned _BitInt(135) b)
{
  return b;
}
#endif

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) long double
testldbl_192 (_BitInt(192) b)
{
  return b;
}

__attribute__((noipa)) long double
testldblu_192 (unsigned _BitInt(192) b)
{
  return b;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) long double
testldbl_575 (_BitInt(575) b)
{
  return b;
}

__attribute__((noipa)) long double
testldblu_575 (unsigned _BitInt(575) b)
{
  return b;
}
#endif
#endif

#if __FLT128_MANT_DIG__ == 113
#if __BITINT_MAXWIDTH__ >= 135
__attribute__((noipa)) _Float128
testflt128_135 (_BitInt(135) b)
{
  return b;
}

__attribute__((noipa)) _Float128
testflt128u_135 (unsigned _BitInt(135) b)
{
  return b;
}
#endif

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) _Float128
testflt128_192 (_BitInt(192) b)
{
  return b;
}

__attribute__((noipa)) _Float128
testflt128u_192 (unsigned _BitInt(192) b)
{
  return b;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) _Float128
testflt128_575 (_BitInt(575) b)
{
  return b;
}

__attribute__((noipa)) _Float128
testflt128u_575 (unsigned _BitInt(575) b)
{
  return b;
}
#endif
#endif

__attribute__((noipa)) void
check_except (int test, int inex, int ovf)
{
  if (!test)
    __builtin_abort ();
  if ((!fetestexcept (FE_INEXACT)) != (!inex))
    __builtin_abort ();
  if ((!fetestexcept (FE_OVERFLOW)) != (!ovf))
    __builtin_abort ();
  feclearexcept (FE_INEXACT | FE_OVERFLOW);
}

int
main ()
{
#if __FLT_MANT_DIG__ == 24
#if __BITINT_MAXWIDTH__ >= 135
  check_except (testflt_135 (151115713941029764726783wb) == 0xfffffep+53f, 1, 0);
  check_except (testflt_135 (151115713941029764726784wb) == 0xfffffep+53f, 1, 0);
  check_except (testflt_135 (151115713941029764726785wb) == 0xffffffp+53f, 1, 0);
  check_except (testflt_135 (151115718444629392097280wb) == 0xffffffp+53f, 0, 0);
  check_except (testflt_135 (151115722948229019467775wb) == 0xffffffp+53f, 1, 0);
  check_except (testflt_135 (151115722948229019467776wb) == 0x1000000p+53f, 1, 0);
  check_except (testflt_135 (151115722948229019467777wb) == 0x1000000p+53f, 1, 0);
  check_except (testflt_135 (-340282346638528859811704183484516925440wb) == -0xffffffp+104f, 0, 0);
  check_except (testflt_135 (-340282356779733661637539395458142568447wb) == -0xffffffp+104f, 1, 0);
  check_except (testflt_135 (-340282356779733661637539395458142568448wb) == -__builtin_inff (), 1, 1);
  check_except (testflt_135 (-21778071482940061661655974875633165533183wb - 1) == -__builtin_inff (), 1, 1);
  check_except (testfltu_135 (151115713941029764726783uwb) == 0xfffffep+53f, 1, 0);
  check_except (testfltu_135 (151115713941029764726784uwb) == 0xfffffep+53f, 1, 0);
  check_except (testfltu_135 (151115713941029764726785uwb) == 0xffffffp+53f, 1, 0);
  check_except (testfltu_135 (151115718444629392097280uwb) == 0xffffffp+53f, 0, 0);
  check_except (testfltu_135 (151115722948229019467775uwb) == 0xffffffp+53f, 1, 0);
  check_except (testfltu_135 (151115722948229019467776uwb) == 0x1000000p+53f, 1, 0);
  check_except (testfltu_135 (151115722948229019467777uwb) == 0x1000000p+53f, 1, 0);
  check_except (testfltu_135 (340282346638528859811704183484516925440uwb) == 0xffffffp+104f, 0, 0);
  check_except (testfltu_135 (340282356779733661637539395458142568447uwb) == 0xffffffp+104f, 1, 0);
  check_except (testfltu_135 (340282356779733661637539395458142568448uwb) == __builtin_inff (), 1, 1);
  check_except (testfltu_135 (43556142965880123323311949751266331066367uwb) == __builtin_inff (), 1, 1);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  check_except (testflt_192 (-10141203895131470501001744613375wb) == -0xfffffep+79f, 1, 0);
  check_except (testflt_192 (-10141203895131470501001744613376wb) == -0xfffffep+79f, 1, 0);
  check_except (testflt_192 (-10141203895131470501001744613377wb) == -0xffffffp+79f, 1, 0);
  check_except (testflt_192 (-10141204197362925404659038289920wb) == -0xffffffp+79f, 0, 0);
  check_except (testflt_192 (-10141204499594380308316331966463wb) == -0xffffffp+79f, 1, 0);
  check_except (testflt_192 (-10141204499594380308316331966464wb) == -0x1000000p+79f, 1, 0);
  check_except (testflt_192 (-10141204499594380308316331966465wb) == -0x1000000p+79f, 1, 0);
  check_except (testflt_192 (340282346638528859811704183484516925440wb) == 0xffffffp+104f, 0, 0);
  check_except (testflt_192 (340282356779733661637539395458142568447wb) == 0xffffffp+104f, 1, 0);
  check_except (testflt_192 (340282356779733661637539395458142568448wb) == __builtin_inff (), 1, 1);
  check_except (testflt_192 (3138550867693340381917894711603833208051177722232017256447wb) == __builtin_inff (), 1, 1);
  check_except (testfltu_192 (10141203895131470501001744613375uwb) == 0xfffffep+79f, 1, 0);
  check_except (testfltu_192 (10141203895131470501001744613376uwb) == 0xfffffep+79f, 1, 0);
  check_except (testfltu_192 (10141203895131470501001744613377uwb) == 0xffffffp+79f, 1, 0);
  check_except (testfltu_192 (10141204197362925404659038289920uwb) == 0xffffffp+79f, 0, 0);
  check_except (testfltu_192 (10141204499594380308316331966463uwb) == 0xffffffp+79f, 1, 0);
  check_except (testfltu_192 (10141204499594380308316331966464uwb) == 0x1000000p+79f, 1, 0);
  check_except (testfltu_192 (10141204499594380308316331966465uwb) == 0x1000000p+79f, 1, 0);
  check_except (testfltu_192 (340282346638528859811704183484516925440uwb) == 0xffffffp+104f, 0, 0);
  check_except (testfltu_192 (340282356779733661637539395458142568447uwb) == 0xffffffp+104f, 1, 0);
  check_except (testfltu_192 (340282356779733661637539395458142568448uwb) == __builtin_inff (), 1, 1);
  check_except (testfltu_192 (6277101735386680763835789423207666416102355444464034512895uwb) == __builtin_inff (), 1, 1);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  check_except (testflt_575 (10633823015541376812058405359715352575wb) == 0xfffffep+99f, 1, 0);
  check_except (testflt_575 (10633823015541376812058405359715352576wb) == 0xfffffep+99f, 1, 0);
  check_except (testflt_575 (10633823015541376812058405359715352577wb) == 0xffffffp+99f, 1, 0);
  check_except (testflt_575 (10633823332454026869115755733891153920wb) == 0xffffffp+99f, 0, 0);
  check_except (testflt_575 (10633823649366676926173106108066955263wb) == 0xffffffp+99f, 1, 0);
  check_except (testflt_575 (10633823649366676926173106108066955264wb) == 0x1000000p+99f, 1, 0);
  check_except (testflt_575 (10633823649366676926173106108066955265wb) == 0x1000000p+99f, 1, 0);
  check_except (testflt_575 (-340282346638528859811704183484516925440wb) == -0xffffffp+104f, 0, 0);
  check_except (testflt_575 (-340282356779733661637539395458142568447wb) == -0xffffffp+104f, 1, 0);
  check_except (testflt_575 (-340282356779733661637539395458142568448wb) == -__builtin_inff (), 1, 1);
  check_except (testflt_575 (-61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1) == -__builtin_inff (), 1, 1);
  check_except (testfltu_575 (10633823015541376812058405359715352575uwb) == 0xfffffep+99f, 1, 0);
  check_except (testfltu_575 (10633823015541376812058405359715352576uwb) == 0xfffffep+99f, 1, 0);
  check_except (testfltu_575 (10633823015541376812058405359715352577uwb) == 0xffffffp+99f, 1, 0);
  check_except (testfltu_575 (10633823332454026869115755733891153920uwb) == 0xffffffp+99f, 0, 0);
  check_except (testfltu_575 (10633823649366676926173106108066955263uwb) == 0xffffffp+99f, 1, 0);
  check_except (testfltu_575 (10633823649366676926173106108066955264uwb) == 0x1000000p+99f, 1, 0);
  check_except (testfltu_575 (10633823649366676926173106108066955265uwb) == 0x1000000p+99f, 1, 0);
  check_except (testfltu_575 (340282346638528859811704183484516925440uwb) == 0xffffffp+104f, 0, 0);
  check_except (testfltu_575 (340282356779733661637539395458142568447uwb) == 0xffffffp+104f, 1, 0);
  check_except (testfltu_575 (340282356779733661637539395458142568448uwb) == __builtin_inff (), 1, 1);
  check_except (testfltu_575 (123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb) == __builtin_inff (), 1, 1);
#endif
#endif
#if __DBL_MANT_DIG__ == 53
#if __BITINT_MAXWIDTH__ >= 135
  check_except (testdbl_135 (-21267647932558650424686050812251602943wb) == -0x1ffffffffffffep+71, 1, 0);
  check_except (testdbl_135 (-21267647932558650424686050812251602944wb) == -0x1ffffffffffffep+71, 1, 0);
  check_except (testdbl_135 (-21267647932558650424686050812251602945wb) == -0x1fffffffffffffp+71, 1, 0);
  check_except (testdbl_135 (-21267647932558651605277671529662906368wb) == -0x1fffffffffffffp+71, 0, 0);
  check_except (testdbl_135 (-21267647932558652785869292247074209791wb) == -0x1fffffffffffffp+71, 1, 0);
  check_except (testdbl_135 (-21267647932558652785869292247074209792wb) == -0x20000000000000p+71, 1, 0);
  check_except (testdbl_135 (-21267647932558652785869292247074209793wb) == -0x20000000000000p+71, 1, 0);
  check_except (testdbl_135 (21778071482940059243804335646374816120832wb) == 0x1fffffffffffffp+81, 0, 0);
  check_except (testdbl_135 (21778071482940060452730155261003990827007wb) == 0x1fffffffffffffp+81, 1, 0);
  check_except (testdbl_135 (21778071482940060452730155261003990827008wb) == 0x20000000000000p+81, 1, 0);
  check_except (testdbl_135 (21778071482940060452730155261003990827009wb) == 0x20000000000000p+81, 1, 0);
  check_except (testdbl_135 (21778071482940061661655974875633165533183wb) == 0x20000000000000p+81, 1, 0);
  check_except (testdblu_135 (21267647932558650424686050812251602943uwb) == 0x1ffffffffffffep+71, 1, 0);
  check_except (testdblu_135 (21267647932558650424686050812251602944uwb) == 0x1ffffffffffffep+71, 1, 0);
  check_except (testdblu_135 (21267647932558650424686050812251602945uwb) == 0x1fffffffffffffp+71, 1, 0);
  check_except (testdblu_135 (21267647932558651605277671529662906368uwb) == 0x1fffffffffffffp+71, 0, 0);
  check_except (testdblu_135 (21267647932558652785869292247074209791uwb) == 0x1fffffffffffffp+71, 1, 0);
  check_except (testdblu_135 (21267647932558652785869292247074209792uwb) == 0x20000000000000p+71, 1, 0);
  check_except (testdblu_135 (21267647932558652785869292247074209793uwb) == 0x20000000000000p+71, 1, 0);
  check_except (testdblu_135 (43556142965880118487608671292749632241664uwb) == 0x1fffffffffffffp+82, 0, 0);
  check_except (testdblu_135 (43556142965880120905460310522007981654015uwb) == 0x1fffffffffffffp+82, 1, 0);
  check_except (testdblu_135 (43556142965880120905460310522007981654016uwb) == 0x20000000000000p+82, 1, 0);
  check_except (testdblu_135 (43556142965880120905460310522007981654017uwb) == 0x20000000000000p+82, 1, 0);
  check_except (testdblu_135 (43556142965880123323311949751266331066367uwb) == 0x20000000000000p+82, 1, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  check_except (testdbl_192 (89202980794122477710862401666030147234430975wb) == 0x1ffffffffffffep+93, 1, 0);
  check_except (testdbl_192 (89202980794122477710862401666030147234430976wb) == 0x1ffffffffffffep+93, 1, 0);
  check_except (testdbl_192 (89202980794122477710862401666030147234430977wb) == 0x1fffffffffffffp+93, 1, 0);
  check_except (testdbl_192 (89202980794122482662622558807551246830927872wb) == 0x1fffffffffffffp+93, 0, 0);
  check_except (testdbl_192 (89202980794122487614382715949072346427424767wb) == 0x1fffffffffffffp+93, 1, 0);
  check_except (testdbl_192 (89202980794122487614382715949072346427424768wb) == 0x20000000000000p+93, 1, 0);
  check_except (testdbl_192 (89202980794122487614382715949072346427424769wb) == 0x20000000000000p+93, 1, 0);
  check_except (testdbl_192 (-3138550867693340033468750984562846621555579712101368725504wb) == -0x1fffffffffffffp+138, 0, 0);
  check_except (testdbl_192 (-3138550867693340207693322848083339914803378717166692990975wb) == -0x1fffffffffffffp+138, 1, 0);
  check_except (testdbl_192 (-3138550867693340207693322848083339914803378717166692990976wb) == -0x20000000000000p+138, 1, 0);
  check_except (testdbl_192 (-3138550867693340207693322848083339914803378717166692990977wb) == -0x20000000000000p+138, 1, 0);
  check_except (testdbl_192 (-3138550867693340381917894711603833208051177722232017256447wb - 1) == -0x20000000000000p+138, 0, 0);
  check_except (testdblu_192 (89202980794122477710862401666030147234430975uwb) == 0x1ffffffffffffep+93, 1, 0);
  check_except (testdblu_192 (89202980794122477710862401666030147234430976uwb) == 0x1ffffffffffffep+93, 1, 0);
  check_except (testdblu_192 (89202980794122477710862401666030147234430977uwb) == 0x1fffffffffffffp+93, 1, 0);
  check_except (testdblu_192 (89202980794122482662622558807551246830927872uwb) == 0x1fffffffffffffp+93, 0, 0);
  check_except (testdblu_192 (89202980794122487614382715949072346427424767uwb) == 0x1fffffffffffffp+93, 1, 0);
  check_except (testdblu_192 (89202980794122487614382715949072346427424768uwb) == 0x20000000000000p+93, 1, 0);
  check_except (testdblu_192 (89202980794122487614382715949072346427424769uwb) == 0x20000000000000p+93, 1, 0);
  check_except (testdblu_192 (6277101735386680066937501969125693243111159424202737451008uwb) == 0x1fffffffffffffp+139, 0, 0);
  check_except (testdblu_192 (6277101735386680415386645696166679829606757434333385981951uwb) == 0x1fffffffffffffp+139, 1, 0);
  check_except (testdblu_192 (6277101735386680415386645696166679829606757434333385981952uwb) == 0x20000000000000p+139, 1, 0);
  check_except (testdblu_192 (6277101735386680415386645696166679829606757434333385981953uwb) == 0x20000000000000p+139, 1, 0);
  check_except (testdblu_192 (6277101735386680763835789423207666416102355444464034512895uwb) == 0x20000000000000p+139, 1, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  check_except (testdbl_575 (-615656346818663635164482277361060010743329029962521103256875011322006445221646740336801072761830405785423389392895wb) == -0x1ffffffffffffep+325, 1, 0);
  check_except (testdbl_575 (-615656346818663635164482277361060010743329029962521103256875011322006445221646740336801072761830405785423389392896wb) == -0x1ffffffffffffep+325, 1, 0);
  check_except (testdbl_575 (-615656346818663635164482277361060010743329029962521103256875011322006445221646740336801072761830405785423389392897wb) == -0x1fffffffffffffp+325, 1, 0);
  check_except (testdbl_575 (-615656346818663669340274852095621329063676328675354936900147369028450764374312465492316685252079206152816780378112wb) == -0x1fffffffffffffp+325, 0, 0);
  check_except (testdbl_575 (-615656346818663703516067426830182647384023627388188770543419726734895083526978190647832297742328006520210171363327wb) == -0x1fffffffffffffp+325, 1, 0);
  check_except (testdbl_575 (-615656346818663703516067426830182647384023627388188770543419726734895083526978190647832297742328006520210171363328wb) == -0x20000000000000p+325, 1, 0);
  check_except (testdbl_575 (-615656346818663703516067426830182647384023627388188770543419726734895083526978190647832297742328006520210171363329wb) == -0x20000000000000p+325, 1, 0);
  check_except (testdbl_575 (61832600368276126650327970124302082526882038193909742709080463879918896882169507607035916867654709124839777195049479857541529867095829765369898539058829479405123401922117632wb) == 0x1fffffffffffffp+521, 0, 0);
  check_except (testdbl_575 (61832600368276130082726800189606940017832437734606351343798113951571601579401237199807508566482735186119597525776757346189685562836258783930892538917151385692137547479646207wb) == 0x1fffffffffffffp+521, 1, 0);
  check_except (testdbl_575 (61832600368276130082726800189606940017832437734606351343798113951571601579401237199807508566482735186119597525776757346189685562836258783930892538917151385692137547479646208wb) == 0x20000000000000p+521, 1, 0);
  check_except (testdbl_575 (61832600368276130082726800189606940017832437734606351343798113951571601579401237199807508566482735186119597525776757346189685562836258783930892538917151385692137547479646209wb) == 0x20000000000000p+521, 1, 0);
  check_except (testdbl_575 (61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb) == 0x20000000000000p+521, 1, 0);
  check_except (testdblu_575 (615656346818663635164482277361060010743329029962521103256875011322006445221646740336801072761830405785423389392895uwb) == 0x1ffffffffffffep+325, 1, 0);
  check_except (testdblu_575 (615656346818663635164482277361060010743329029962521103256875011322006445221646740336801072761830405785423389392896uwb) == 0x1ffffffffffffep+325, 1, 0);
  check_except (testdblu_575 (615656346818663635164482277361060010743329029962521103256875011322006445221646740336801072761830405785423389392897uwb) == 0x1fffffffffffffp+325, 1, 0);
  check_except (testdblu_575 (615656346818663669340274852095621329063676328675354936900147369028450764374312465492316685252079206152816780378112uwb) == 0x1fffffffffffffp+325, 0, 0);
  check_except (testdblu_575 (615656346818663703516067426830182647384023627388188770543419726734895083526978190647832297742328006520210171363327uwb) == 0x1fffffffffffffp+325, 1, 0);
  check_except (testdblu_575 (615656346818663703516067426830182647384023627388188770543419726734895083526978190647832297742328006520210171363328uwb) == 0x20000000000000p+325, 1, 0);
  check_except (testdblu_575 (615656346818663703516067426830182647384023627388188770543419726734895083526978190647832297742328006520210171363329uwb) == 0x20000000000000p+325, 1, 0);
  check_except (testdblu_575 (123665200736552253300655940248604165053764076387819485418160927759837793764339015214071833735309418249679554390098959715083059734191659530739797078117658958810246803844235264uwb) == 0x1fffffffffffffp+522, 0, 0);
  check_except (testdblu_575 (123665200736552260165453600379213880035664875469212702687596227903143203158802474399615017132965470372239195051553514692379371125672517567861785077834302771384275094959292415uwb) == 0x1fffffffffffffp+522, 1, 0);
  check_except (testdblu_575 (123665200736552260165453600379213880035664875469212702687596227903143203158802474399615017132965470372239195051553514692379371125672517567861785077834302771384275094959292416uwb) == 0x20000000000000p+522, 1, 0);
  check_except (testdblu_575 (123665200736552260165453600379213880035664875469212702687596227903143203158802474399615017132965470372239195051553514692379371125672517567861785077834302771384275094959292417uwb) == 0x20000000000000p+522, 1, 0);
  check_except (testdblu_575 (123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb) == 0x20000000000000p+522, 1, 0);
#endif
#endif
#if __LDBL_MANT_DIG__ == 64
#if __BITINT_MAXWIDTH__ >= 135
  check_except (testldbl_135 (-27577662721237071616947187835994111wb) == -0xa9f5e144d113e1c4p+51L, 1, 0);
  check_except (testldbl_135 (-27577662721237071616947187835994112wb) == -0xa9f5e144d113e1c4p+51L, 1, 0);
  check_except (testldbl_135 (-27577662721237071616947187835994113wb) == -0xa9f5e144d113e1c5p+51L, 1, 0);
  check_except (testldbl_135 (-27577662721237071618073087742836736wb) == -0xa9f5e144d113e1c5p+51L, 0, 0);
  check_except (testldbl_135 (-27577662721237071619198987649679359wb) == -0xa9f5e144d113e1c5p+51L, 1, 0);
  check_except (testldbl_135 (-27577662721237071619198987649679360wb) == -0xa9f5e144d113e1c6p+51L, 1, 0);
  check_except (testldbl_135 (-27577662721237071619198987649679361wb) == -0xa9f5e144d113e1c6p+51L, 1, 0);
  check_except (testldbl_135 (-21778071482940061660475383254915754229760wb) == -0xffffffffffffffffp+70L, 0, 0);
  check_except (testldbl_135 (-21778071482940061661065679065274459881471wb) == -0xffffffffffffffffp+70L, 1, 0);
  check_except (testldbl_135 (-21778071482940061661065679065274459881472wb) == -0x10000000000000000p+70L, 1, 0);
  check_except (testldbl_135 (-21778071482940061661065679065274459881473wb) == -0x10000000000000000p+70L, 1, 0);
  check_except (testldbl_135 (-21778071482940061661655974875633165533183wb - 1) == -0x10000000000000000p+70L, 0, 0);
  check_except (testldblu_135 (27577662721237071616947187835994111uwb) == 0xa9f5e144d113e1c4p+51L, 1, 0);
  check_except (testldblu_135 (27577662721237071616947187835994112uwb) == 0xa9f5e144d113e1c4p+51L, 1, 0);
  check_except (testldblu_135 (27577662721237071616947187835994113uwb) == 0xa9f5e144d113e1c5p+51L, 1, 0);
  check_except (testldblu_135 (27577662721237071618073087742836736uwb) == 0xa9f5e144d113e1c5p+51L, 0, 0);
  check_except (testldblu_135 (27577662721237071619198987649679359uwb) == 0xa9f5e144d113e1c5p+51L, 1, 0);
  check_except (testldblu_135 (27577662721237071619198987649679360uwb) == 0xa9f5e144d113e1c6p+51L, 1, 0);
  check_except (testldblu_135 (27577662721237071619198987649679361uwb) == 0xa9f5e144d113e1c6p+51L, 1, 0);
  check_except (testldblu_135 (43556142965880123320950766509831508459520uwb) == 0xffffffffffffffffp+71L, 0, 0);
  check_except (testldblu_135 (43556142965880123322131358130548919762943uwb) == 0xffffffffffffffffp+71L, 1, 0);
  check_except (testldblu_135 (43556142965880123322131358130548919762944uwb) == 0x10000000000000000p+71L, 1, 0);
  check_except (testldblu_135 (43556142965880123322131358130548919762945uwb) == 0x10000000000000000p+71L, 1, 0);
  check_except (testldblu_135 (43556142965880123323311949751266331066367uwb) == 0x10000000000000000p+71L, 1, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  check_except (testldbl_192 (96388802158769743653878219701497927252918090596351wb) == 0x83e75ebf94ce024ep+103L, 1, 0);
  check_except (testldbl_192 (96388802158769743653878219701497927252918090596352wb) == 0x83e75ebf94ce024ep+103L, 1, 0);
  check_except (testldbl_192 (96388802158769743653878219701497927252918090596353wb) == 0x83e75ebf94ce024fp+103L, 1, 0);
  check_except (testldbl_192 (96388802158769743658948822102410844858904903417856wb) == 0x83e75ebf94ce024fp+103L, 0, 0);
  check_except (testldbl_192 (96388802158769743664019424503323762464891716239359wb) == 0x83e75ebf94ce024fp+103L, 1, 0);
  check_except (testldbl_192 (96388802158769743664019424503323762464891716239360wb) == 0x83e75ebf94ce0250p+103L, 1, 0);
  check_except (testldbl_192 (96388802158769743664019424503323762464891716239361wb) == 0x83e75ebf94ce0250p+103L, 1, 0);
  check_except (testldbl_192 (-3138550867693340381747753528143363976319490418516133150720wb) == -0xffffffffffffffffp+127L, 0, 0);
  check_except (testldbl_192 (-3138550867693340381832824119873598592185334070374075203583wb) == -0xffffffffffffffffp+127L, 1, 0);
  check_except (testldbl_192 (-3138550867693340381832824119873598592185334070374075203584wb) == -0x10000000000000000p+127L, 1, 0);
  check_except (testldbl_192 (-3138550867693340381832824119873598592185334070374075203585wb) == -0x10000000000000000p+127L, 1, 0);
  check_except (testldbl_192 (-3138550867693340381917894711603833208051177722232017256447wb - 1wb) == -0x10000000000000000p+127L, 0, 0);
  check_except (testldblu_192 (96388802158769743653878219701497927252918090596351uwb) == 0x83e75ebf94ce024ep+103L, 1, 0);
  check_except (testldblu_192 (96388802158769743653878219701497927252918090596352uwb) == 0x83e75ebf94ce024ep+103L, 1, 0);
  check_except (testldblu_192 (96388802158769743653878219701497927252918090596353uwb) == 0x83e75ebf94ce024fp+103L, 1, 0);
  check_except (testldblu_192 (96388802158769743658948822102410844858904903417856uwb) == 0x83e75ebf94ce024fp+103L, 0, 0);
  check_except (testldblu_192 (96388802158769743664019424503323762464891716239359uwb) == 0x83e75ebf94ce024fp+103L, 1, 0);
  check_except (testldblu_192 (96388802158769743664019424503323762464891716239360uwb) == 0x83e75ebf94ce0250p+103L, 1, 0);
  check_except (testldblu_192 (96388802158769743664019424503323762464891716239361uwb) == 0x83e75ebf94ce0250p+103L, 1, 0);
  check_except (testldblu_192 (6277101735386680763495507056286727952638980837032266301440uwb) == 0xffffffffffffffffp+128L, 0, 0);
  check_except (testldblu_192 (6277101735386680763665648239747197184370668140748150407167uwb) == 0xffffffffffffffffp+128L, 1, 0);
  check_except (testldblu_192 (6277101735386680763665648239747197184370668140748150407168uwb) == 0x10000000000000000p+128L, 1, 0);
  check_except (testldblu_192 (6277101735386680763665648239747197184370668140748150407169uwb) == 0x10000000000000000p+128L, 1, 0);
  check_except (testldblu_192 (6277101735386680763835789423207666416102355444464034512895uwb) == 0x10000000000000000p+128L, 1, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  check_except (testldbl_575 (-61832600368276133511773678272426148233889331025751498446645922568076207932202076431648659257792374503198949281962308977915333294030066289778448068072486649492543280785653760wb) == -0xffffffffffffffffp+510L, 0, 0);
  check_except (testldbl_575 (-61832600368276133513449654263668972871336084150527229212580843295650257104417521612113879761551567875299183569233171906376587276303377046135167303423979970735847486911414271wb) == -0xffffffffffffffffp+510L, 1, 0);
  check_except (testldbl_575 (-61832600368276133513449654263668972871336084150527229212580843295650257104417521612113879761551567875299183569233171906376587276303377046135167303423979970735847486911414272wb) == -0x10000000000000000p+510L, 1, 0);
  check_except (testldbl_575 (-61832600368276133513449654263668972871336084150527229212580843295650257104417521612113879761551567875299183569233171906376587276303377046135167303423979970735847486911414273wb) == -0x10000000000000000p+510L, 1, 0);
  check_except (testldbl_575 (-61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1) == -0x10000000000000000p+510L, 0, 0);
  check_except (testldblu_575 (123665200736552267023547356544852296467778662051502996893291845136152415864404152863297318515584749006397898563924617955830666588060132579556896136144973298985086561571307520uwb) == 0xffffffffffffffffp+511L, 0, 0);
  check_except (testldblu_575 (123665200736552267026899308527337945742672168301054458425161686591300514208835043224227759523103135750598367138466343812753174552606754092270334606847959941471694973822828543uwb) == 0xffffffffffffffffp+511L, 1, 0);
  check_except (testldblu_575 (123665200736552267026899308527337945742672168301054458425161686591300514208835043224227759523103135750598367138466343812753174552606754092270334606847959941471694973822828544uwb) == 0x10000000000000000p+511L, 1, 0);
  check_except (testldblu_575 (123665200736552267026899308527337945742672168301054458425161686591300514208835043224227759523103135750598367138466343812753174552606754092270334606847959941471694973822828545uwb) == 0x10000000000000000p+511L, 1, 0);
  check_except (testldblu_575 (123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb) == 0x10000000000000000p+511L, 1, 0);
#endif
#endif
#if __FLT128_MANT_DIG__ == 113
#if __BITINT_MAXWIDTH__ >= 135
  check_except (testflt128_135 (-21646332438261169091754659013488783917055wb) == -0x1fce71fdcfb1797b42dede66ac9ecp+21F128, 1, 0);
  check_except (testflt128_135 (-21646332438261169091754659013488783917056wb) == -0x1fce71fdcfb1797b42dede66ac9ecp+21F128, 1, 0);
  check_except (testflt128_135 (-21646332438261169091754659013488783917057wb) == -0x1fce71fdcfb1797b42dede66ac9edp+21F128, 1, 0);
  check_except (testflt128_135 (-21646332438261169091754659013488784965632wb) == -0x1fce71fdcfb1797b42dede66ac9edp+21F128, 0, 0);
  check_except (testflt128_135 (-21646332438261169091754659013488786014207wb) == -0x1fce71fdcfb1797b42dede66ac9edp+21F128, 1, 0);
  check_except (testflt128_135 (-21646332438261169091754659013488786014208wb) == -0x1fce71fdcfb1797b42dede66ac9eep+21F128, 1, 0);
  check_except (testflt128_135 (-21646332438261169091754659013488786014209wb) == -0x1fce71fdcfb1797b42dede66ac9eep+21F128, 1, 0);
  check_except (testflt128u_135 (21646332438261169091754659013488783917055uwb) == 0x1fce71fdcfb1797b42dede66ac9ecp+21F128, 1, 0);
  check_except (testflt128u_135 (21646332438261169091754659013488783917056uwb) == 0x1fce71fdcfb1797b42dede66ac9ecp+21F128, 1, 0);
  check_except (testflt128u_135 (21646332438261169091754659013488783917057uwb) == 0x1fce71fdcfb1797b42dede66ac9edp+21F128, 1, 0);
  check_except (testflt128u_135 (21646332438261169091754659013488784965632uwb) == 0x1fce71fdcfb1797b42dede66ac9edp+21F128, 0, 0);
  check_except (testflt128u_135 (21646332438261169091754659013488786014207uwb) == 0x1fce71fdcfb1797b42dede66ac9edp+21F128, 1, 0);
  check_except (testflt128u_135 (21646332438261169091754659013488786014208uwb) == 0x1fce71fdcfb1797b42dede66ac9eep+21F128, 1, 0);
  check_except (testflt128u_135 (21646332438261169091754659013488786014209uwb) == 0x1fce71fdcfb1797b42dede66ac9eep+21F128, 1, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 192
  check_except (testflt128_192 (3138550867693340381917894711603832905819722818574723579904wb) == 0x1ffffffffffffffffffffffffffffp+78F128, 0, 0);
  check_except (testflt128_192 (3138550867693340381917894711603833056935450270403370418175wb) == 0x1ffffffffffffffffffffffffffffp+78F128, 1, 0);
  check_except (testflt128_192 (3138550867693340381917894711603833056935450270403370418176wb) == 0x20000000000000000000000000000p+78F128, 1, 0);
  check_except (testflt128_192 (3138550867693340381917894711603833056935450270403370418177wb) == 0x20000000000000000000000000000p+78F128, 1, 0);
  check_except (testflt128_192 (3138550867693340381917894711603833208051177722232017256447wb) == 0x20000000000000000000000000000p+78F128, 1, 0);
  check_except (testflt128u_192 (6277101735386680763835789423207665811639445637149447159808uwb) == 0x1ffffffffffffffffffffffffffffp+79F128, 0, 0);
  check_except (testflt128u_192 (6277101735386680763835789423207666113870900540806740836351uwb) == 0x1ffffffffffffffffffffffffffffp+79F128, 1, 0);
  check_except (testflt128u_192 (6277101735386680763835789423207666113870900540806740836352uwb) == 0x20000000000000000000000000000p+79F128, 1, 0);
  check_except (testflt128u_192 (6277101735386680763835789423207666113870900540806740836353uwb) == 0x20000000000000000000000000000p+79F128, 1, 0);
  check_except (testflt128u_192 (6277101735386680763835789423207666416102355444464034512895uwb) == 0x20000000000000000000000000000p+79F128, 1, 0);
#endif
#if __BITINT_MAXWIDTH__ >= 575
  check_except (testflt128_575 (-39695651458311907436978914487787846289740055435765388813682045155135192382154626611682813571487190641804615256990296246545713518740501887218789991403746059512699763279527936wb) == -0x148b25ce53790ddc343a80e5af6bap+461F128, 0, 0);
  check_except (testflt128_575 (-39695651458311907436978914487787849266871470150571212503712362264401765094669639986937588484471046485703139369468190190624257242316066424102078490670010875270428034085158911wb) == -0x148b25ce53790ddc343a80e5af6bap+461F128, 1, 0);
  check_except (testflt128_575 (-39695651458311907436978914487787849266871470150571212503712362264401765094669639986937588484471046485703139369468190190624257242316066424102078490670010875270428034085158912wb) == -0x148b25ce53790ddc343a80e5af6bap+461F128, 1, 0);
  check_except (testflt128_575 (-39695651458311907436978914487787849266871470150571212503712362264401765094669639986937588484471046485703139369468190190624257242316066424102078490670010875270428034085158913wb) == -0x148b25ce53790ddc343a80e5af6bbp+461F128, 1, 0);
  check_except (testflt128_575 (-39695651458311907436978914487787852244002884865377036193742679373668337807184653362192363397454902329601663481946084134702800965891630960985366989936275691028156304890789888wb) == -0x148b25ce53790ddc343a80e5af6bbp+461F128, 0, 0);
  check_except (testflt128_575 (-39695651458311907436978914487787855221134299580182859883772996482934910519699666737447138310438758173500187594423978078781344689467195497868655489202540506785884575696420863wb) == -0x148b25ce53790ddc343a80e5af6bbp+461F128, 1, 0);
  check_except (testflt128_575 (-39695651458311907436978914487787855221134299580182859883772996482934910519699666737447138310438758173500187594423978078781344689467195497868655489202540506785884575696420864wb) == -0x148b25ce53790ddc343a80e5af6bcp+461F128, 1, 0);
  check_except (testflt128_575 (-39695651458311907436978914487787855221134299580182859883772996482934910519699666737447138310438758173500187594423978078781344689467195497868655489202540506785884575696420865wb) == -0x148b25ce53790ddc343a80e5af6bcp+461F128, 1, 0);
  check_except (testflt128_575 (-61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1) == -0x20000000000000000000000000000p+461F128, 0, 0);
  check_except (testflt128u_575 (79391302916623814873957828975575692579480110871530777627364090310270384764309253223365627142974381283609230513980592493091427037481003774437579982807492119025399526559055872uwb) == 0x148b25ce53790ddc343a80e5af6bap+462F128, 0, 0);
  check_except (testflt128u_575 (79391302916623814873957828975575698533742940301142425007424724528803530189339279973875176968942092971406278738936380381248514484632132848204156981340021750540856068170317823uwb) == 0x148b25ce53790ddc343a80e5af6bap+462F128, 1, 0);
  check_except (testflt128u_575 (79391302916623814873957828975575698533742940301142425007424724528803530189339279973875176968942092971406278738936380381248514484632132848204156981340021750540856068170317824uwb) == 0x148b25ce53790ddc343a80e5af6bap+462F128, 1, 0);
  check_except (testflt128u_575 (79391302916623814873957828975575698533742940301142425007424724528803530189339279973875176968942092971406278738936380381248514484632132848204156981340021750540856068170317825uwb) == 0x148b25ce53790ddc343a80e5af6bbp+462F128, 1, 0);
  check_except (testflt128u_575 (79391302916623814873957828975575704488005769730754072387485358747336675614369306724384726794909804659203326963892168269405601931783261921970733979872551382056312609781579776uwb) == 0x148b25ce53790ddc343a80e5af6bbp+462F128, 0, 0);
  check_except (testflt128u_575 (79391302916623814873957828975575710442268599160365719767545992965869821039399333474894276620877516347000375188847956157562689378934390995737310978405081013571769151392841727uwb) == 0x148b25ce53790ddc343a80e5af6bbp+462F128, 1, 0);
  check_except (testflt128u_575 (79391302916623814873957828975575710442268599160365719767545992965869821039399333474894276620877516347000375188847956157562689378934390995737310978405081013571769151392841728uwb) == 0x148b25ce53790ddc343a80e5af6bcp+462F128, 1, 0);
  check_except (testflt128u_575 (79391302916623814873957828975575710442268599160365719767545992965869821039399333474894276620877516347000375188847956157562689378934390995737310978405081013571769151392841729uwb) == 0x148b25ce53790ddc343a80e5af6bcp+462F128, 1, 0);
  check_except (testflt128u_575 (123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb) == 0x20000000000000000000000000000p+462F128, 1, 0);
#endif
#endif
}
