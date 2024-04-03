#include "Unity/src/unity.h"
#include "Unity/src/unity.c"
#include "math_functions.h"
void setUp(void) {
    // Set up things before each test (if needed)
}
void tearDown(void) {
// Clean up after each test (if needed)
}
void test_add_function(void) {
    TEST_ASSERT_EQUAL(5, add(2, 3));
    TEST_ASSERT_EQUAL(-1, add(-3, 2));
    TEST_ASSERT_EQUAL(0, add(0, 0));
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_add_function);
    return UNITY_END();
}
