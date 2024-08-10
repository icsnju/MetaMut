// { dg-do compile }
// { dg-options "-O2 -w -fnon-call-exceptions -fno-delete-dead-exceptions -fdump-tree-optimized" }

int func_10_ptr_12;

void func_10(long li_8) 
{
  long *ptr_9 = &li_8;
  li_8 &= *ptr_9 / 0 ?: li_8;
  for (;;)
    func_10_ptr_12 &= 4 ? *ptr_9 : 4;
}

void func_9_s_8() 
{ 
  func_10(func_9_s_8); 
}

// { dg-final { scan-tree-dump " / 0" "optimized" } }
