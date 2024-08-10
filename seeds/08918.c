// PR c++/90590
// { dg-options -Wswitch }

#pragma GCC system_header
struct {
  enum { _A } b;
} c;

void
fn ()
{
  switch (c.b) // { dg-bogus "enumeration value" }
    ;
}
