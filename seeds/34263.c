/* { dg-do run } */
/* { dg-options "-Os -fno-tree-switch-conversion -ffunction-sections -fdata-sections -mno-relax -Wl,--section-start=.foo=0x20000" } */

#ifdef __AVR_3_BYTE_PC__
/* Make sure jumptables work properly if placed above 128 KB, i.e. 3 byte
   flash address for loading jump table entry and a jump table entry
   that is a stub, with relaxation disabled, after removing the special
   section placement hook. */
#define SECTION_NAME ".foo"
#else
/* No special jump table placement so that avrtest won't abort
   for, e.g. ATmega128.  */
#define SECTION_NAME ".text.foo"
#endif

#ifdef __cplusplus
extern "C" {
#endif
  extern void exit (int);
  extern void abort (void);
#ifdef __cplusplus
}
#endif
volatile char y;
volatile char g;

__attribute__((section(SECTION_NAME)))
void foo(char x) 
{
	switch (x)
	{
		case 0:
			y = 67; break;
		case 1:
			y = 20; break;
		case 2:
			y = 109; break;
		case 3:
			y = 33; break;
		case 4:
			y = 44; break;
		case 5:
			y = 37; break;
		case 6:
			y = 10; break;
		case 7:
			y = 98; break;
	}
	y = y + g;
}

int main()
{
	foo(5);
	if (y != 37)
		abort();

	foo(0);
	if (y != 67)
		abort();

	foo(7);
	if (y != 98)
		abort();
}
