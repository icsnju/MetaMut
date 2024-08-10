/* { dg-do run } */
/* { dg-options "-Os -fno-tree-switch-conversion -ffunction-sections -fdata-sections" } */

/* Make sure jumptables work properly if placed below 64 KB i.e. 2 byte
   flash address for loading jump table entry, 2 byte entry, after
   removing the special section placement hook. */

#define SECTION_NAME ".foo"

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
