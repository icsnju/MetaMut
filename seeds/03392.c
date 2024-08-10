struct dummy { };
struct foo
{
  int i1;
  int i2;
  int i3;
  int i4;
  int i5;
};
void fun(struct dummy d, struct foo f)
{
  if (f.i1 == -1)
    __builtin_abort();
}
