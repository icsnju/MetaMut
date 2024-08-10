// PR c++/90590
// { dg-options -Wswitch }
#pragma GCC system_header
enum E { _A, _B, _C };

void
g ()
{
  enum E e = _A;
  switch (e) // { dg-bogus "enumeration value '_C' not handled in switch" }
    {
    case _A:
    case _B:
      break;
    }
}
