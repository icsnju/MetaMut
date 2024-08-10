/* PR c/84293 unexpected warning from system header.  */
/* PR c/84293 unexpected warning from system header expansion.  */
#pragma GCC system_header
struct typeobject { unsigned refs; };
typedef struct object { unsigned refs; } Object;

#define INCREF_TDEF(op) (((Object*)(op))->refs++)
#define INCREF_STAG(op) (((struct object*)(op))->refs++)
struct typeobject thing;

#pragma GCC diagnostic warning "-Wstrict-aliasing"
void __attribute__ ((optimize (2))) init ()
{
  INCREF_TDEF (&thing);
  INCREF_STAG (&thing);
}
