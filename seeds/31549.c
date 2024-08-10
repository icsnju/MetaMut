/* { dg-options "nonpic-addr.o pic-receive-fn-addr.o pic-addr-call.o nonpic-receive-fn-addr.o nonpic-nothing.o pic-nothing.o" } */

extern int hit_pic_addr;
extern int hit_pic_addr_call;
extern int hit_pic_nothing;
extern int hit_pic_call;

extern int hit_nonpic_addr;
extern int hit_nonpic_addr_call;
extern int hit_nonpic_call;
extern int hit_nonpic_nothing;

extern void nonpic_addr (void);
extern void nonpic_call (void);
extern void nonpic_addr_call (void);
extern void nonpic_nothing (void);
extern void nonpic_receive_fn_addr (void *);

extern void pic_addr (void);
extern void pic_call (void);
extern void pic_addr_call (void);
extern void pic_receive_fn_addr (void *);
extern void pic_nothing (void);

extern void abort (void);
extern void exit (int);

int
main ()
{
  nonpic_addr ();
  pic_addr_call ();

  if (hit_nonpic_addr != 1)
    abort ();

  if (hit_pic_addr_call != 1)
    abort ();

  exit (0);
} 
