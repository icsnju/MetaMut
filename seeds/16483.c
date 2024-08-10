/* { dg-do preprocess } */
/* Create a fake include for pr61474-2.h and exercise looking it up.  */
/* Use #pragma once to check also that the fake-include entry in the file
   cache does not cause a problem in libcpp/files.cc:has_unique_contents().  */
#pragma once
# 1 "pr61474-2.h" 1
# 2 "pr61474-2.h" 1
/* Make sure that the file can be included for real, after it was
   fake-included by the linemarker directives in pr61474.h.  */
#pragma once
