extern void called_by_test_type_mismatch_1 (int placeholder, ...);

void test_type_mismatch_1 (void)
{
  called_by_test_type_mismatch_1 (42, 1066);
}
