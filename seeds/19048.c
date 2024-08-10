// RUN: %clang_cc1 -Wrestrict-expansion %s -fsyntax-only -verify
// expected-error@+1{{expected (}}
#pragma clang restrict_expansion

// expected-error@+1{{expected identifier}}
#pragma clang restrict_expansion(4

// expected-error@+1{{no macro named 'foo'}}
#pragma clang restrict_expansion(foo)


#define UNSAFE_MACRO 1
// expected-note@+8{{macro marked 'restrict_expansion' here}}
// expected-note@+7{{macro marked 'restrict_expansion' here}}
// expected-note@+6{{macro marked 'restrict_expansion' here}}
// expected-note@+5{{macro marked 'restrict_expansion' here}}
// expected-note@+4{{macro marked 'restrict_expansion' here}}
// expected-note@+3{{macro marked 'restrict_expansion' here}}
// expected-note@+2{{macro marked 'restrict_expansion' here}}
// expected-note@+1{{macro marked 'restrict_expansion' here}} 
#pragma clang restrict_expansion(UNSAFE_MACRO, "Don't use this!")

#define UNSAFE_MACRO_2 2
// expected-note@+1{{macro marked 'restrict_expansion' here}}
#pragma clang restrict_expansion(UNSAFE_MACRO_2)

// expected-error@+1{{expected )}}
#pragma clang deprecated(UNSAFE_MACRO
// expected-warning@+1{{macro 'UNSAFE_MACRO' has been marked as unsafe for use in headers: Don't use this!}}
#if UNSAFE_MACRO
#endif

// expected-warning@+1{{macro 'UNSAFE_MACRO' has been marked as unsafe for use in headers: Don't use this!}}
#if defined(UNSAFE_MACRO)
#endif

// expected-warning@+1{{macro 'UNSAFE_MACRO' has been marked as unsafe for use in headers: Don't use this!}}
#ifdef UNSAFE_MACRO
#endif

// expected-warning@+1{{macro 'UNSAFE_MACRO' has been marked as unsafe for use in headers: Don't use this!}}
#ifndef UNSAFE_MACRO
#endif

// expected-warning@+1{{macro 'UNSAFE_MACRO' has been marked as unsafe for use in headers: Don't use this!}}
const int x = UNSAFE_MACRO;

// expected-warning@+1{{macro 'UNSAFE_MACRO_2' has been marked as unsafe for use in headers}}
const int y = UNSAFE_MACRO_2;

// not-expected-warning@+1{{macro 'UNSAFE_MACRO_2' has been marked as unsafe for use in headers}}
#undef UNSAFE_MACRO_2
// not-expected-warning@+1{{macro 'UNSAFE_MACRO_2' has been marked as unsafe for use in headers}}
#define UNSAFE_MACRO_2 2

// not-expected-warning@+1{{macro 'UNSAFE_MACRO_2' has been marked as unsafe for use in headers}}
const int z = UNSAFE_MACRO_2;


// Test that we diagnose on #elif.
#if 0
#elif UNSAFE_MACRO
// expected-warning@-1{{macro 'UNSAFE_MACRO' has been marked as unsafe for use in headers: Don't use this!}}
#endif


// Test that we diagnose on #elifdef.
#ifdef baz
#elifdef UNSAFE_MACRO
// expected-warning@-1{{macro 'UNSAFE_MACRO' has been marked as unsafe for use in headers: Don't use this!}}
// expected-warning@-2{{use of a '#elifdef' directive is a C23 extension}}
#endif

// Test that we diagnose on #elifndef.
#ifdef baz
#elifndef UNSAFE_MACRO
#endif
// expected-warning@-2{{macro 'UNSAFE_MACRO' has been marked as unsafe for use in headers: Don't use this!}}
// expected-warning@-3{{use of a '#elifndef' directive is a C23 extension}}

// FIXME: These cases are currently not handled because clang doesn't expand
// conditions on skipped #elif* blocks. See the FIXME notes in
// Preprocessor::SkipExcludedConditionalBlock.

#define frobble

#ifdef frobble
// not-expected-warning@+2{{macro 'UNSAFE_MACRO' has been marked as unsafe for use in headers: Don't use this!}}
// expected-warning@+1{{use of a '#elifndef' directive is a C23 extension}}
#elifndef UNSAFE_MACRO
#endif

#ifdef frobble
// not-expected-warning@+2{{macro 'UNSAFE_MACRO' has been marked as unsafe for use in headers: Don't use this!}}
// expected-warning@+1{{use of a '#elifdef' directive is a C23 extension}}
#elifdef UNSAFE_MACRO
#endif

#if 1
// not-expected-warning@+1{{macro 'UNSAFE_MACRO' has been marked as unsafe for use in headers: Don't use this!}}
#elif UNSAFE_MACRO
#endif

// not-expected-warning@+1{{macro 'UNSAFE_MACRO' has been marked as unsafe for use in headers: Don't use this!}}
#if UNSAFE_MACRO
#endif
