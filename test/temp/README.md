Unity is a powerful unit testing framework for C, designed to be simple to use
yet flexible enough for most projects. Here's a basic example to demonstrate
how to write and run a test using Unity.

### Step 1: Install Unity

First, you need to have Unity available in your project. You can clone it from
its GitHub repository or include it as a submodule if you're using Git. For
this example, let's assume you've cloned Unity into a directory named `Unity`
in your project.

### Step 2: Write a Function to Test

Let's say you have a simple C function that you want to test. Create a file
named `math_functions.c`:

```c
// math_functions.c
#include "math_functions.h"
int add(int a, int b) {
    return a + b;
}
```

And its header file `math_functions.h`:

```c
// math_functions.h
#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H
int add(int a, int b);
#endif // MATH_FUNCTIONS_H
```

### Step 3: Write a Test File

Create a test file for this function. Let's name it `test_math_functions.c`:

```c
#include "unity.h"
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
```

### Step 4: Compile and Run the Test

To compile and run the test, you need to include Unity source files and specify
the test file and the file under test. Here's how you might compile it using
`gcc`:

```sh
gcc -o test_runner test_math_functions.c math_functions.c Unity/src/unity.c -IUnity/src
```

This command compiles the test runner executable named `test_runner` by
including the Unity source file (`unity.c`), the test file
(`test_math_functions.c`), and the implementation file (`math_functions.c`).
The `-IUnity/src` flag tells the compiler where to find Unity's header files.

### Step 5: Execute the Test Runner

Run the compiled test runner:

```sh
./test_runner
```

If everything is set up correctly, Unity will run the test(s) you've defined
and output the results, indicating whether each test passed or failed.

### Conclusion

This example demonstrates the basics of using Unity to test a simple C
function. Unity is capable of much more, including testing more complex
behaviors, mocking, and handling different types of assertions. For more
advanced features and usage, refer to the [Unity
documentation](https://github.com/ThrowTheSwitch/Unity).

