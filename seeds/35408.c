#if defined (__mips) && !defined(SKIP_ATTRIBUTE)
#define SDATA_SECTION __attribute__((__section__(".sdata")))
#else
#define SDATA_SECTION
#endif

extern void abort (void);

struct s { int x; int y[4]; };
struct s small_struct SDATA_SECTION;
