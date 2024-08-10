/* { dg-options {-O0} } */

void e(void);
void f(void);
void a(void *, void *);
void c(int);

void a(void * a1, void * a2) { e(); }

int main(){}
