/* { dg-do preprocess } */
/* PR 4713 */

/* dg.exp doesn't read the header files for magic comments. */
/* { dg-error "unterminated comment" "" { target *-*-* } 4 } */
/* { dg-error "unterminated comment" "header error" { target *-*-* } 8 } */

/* comment start in comment error
/* in a .h file */

#if 0
#endif /* comment start in comment error
/* after a cpp directive */

/* comment start in comment error


/* { dg-message "" "In file included from:" { target *-*-* } 0 } */
/* spanning a .h file */

#if 0
#endif /* comment start in comment error
/* spanning a .h file */


/* comment start in comment error
/* in a .c file */

int main ()
{
    return 0;
}

