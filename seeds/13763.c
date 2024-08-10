#define FOO 1
#pragma push_macro ("FOO")
#undef FOO
#define FOO 2


#if FOO != 2
#error FOO != 2
#endif
#pragma pop_macro("FOO")

#if FOO != 1
#error FOR != 1
#endif

