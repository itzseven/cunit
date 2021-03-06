//
//  main.c
//  CUnitExample
//
//  Created by Romain on 18/01/2018.
//  Copyright © 2018 itzseven. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <cunit.h>

#pragma mark - Test assertions

ctest_return_t testFail(ctest_t *test, void *arg) {
    puts("testFail execution\n");
    CTFail(test, "This library should be more comprehensive :(\n")
}

ctest_return_t testEquality(ctest_t *test, void *arg) {
    puts("testEquality execution\n");
    int a = 5, b = 9;
    CTAssertEqual(test, a, b - 4, "testEquality failed: %d and %d are not equal\n", a, b)
    double c = 9.56, d = 9.560;
    CTAssertDecimalEqual(test, c, d, 0.001)
}

ctest_return_t testNonEquality(ctest_t *test, void *arg) {
    puts("testNonEquality execution\n");
    int a = 5, b = 9;
    CTAssertNotEqual(test, a, b, "testNonEquality failed: %d and %d are equal\n", a, b)
    float c = 9.56, d = 9.561;
    CTAssertDecimalEqual(test, c, d, 0.001)
}

ctest_return_t testStringEquality(ctest_t *test, void *arg) {
    puts("testStringEquality execution\n");
    char string1[10] = "Hello!", string2[10] = "Goodbye!";
    CTAssertStringEqual(test, string1, string2, "testStringEquality failed: %s and %s are not equal\n", string1, string2)
}

ctest_return_t testStringNonEquality(ctest_t *test, void *arg) {
    puts("testStringNonEquality execution\n");
    char string1[10] = "Hello!", string2[10] = "Goodbye!";
    CTAssertStringNotEqual(test, string1, string2, "testStringNonEquality failed: %s and %s are equal\n", string1, string2)
}

int stringEquality(const void *str1, const void *str2) {
    return strcmp(str1, str2);
}

ctest_return_t testArrayEquality(ctest_t *test, void *arg) {
    const char *array1[] = {"hello", "how are you", "goodbye"}, *array2[] = {"hello", "how are you", "goodbye"};
    
    CTAssertArrayEqual(test, array1, array2, 3, sizeof(char *), stringEquality)
}

ctest_return_t testArrayNonEquality(ctest_t *test, void *arg) {
    const char *array1[] = {"hello", "how are you", "goodbye"}, *array2[] = {"how are you", "hello", "goodbye"};
    
    CTAssertArrayNotEqual(test, array1, array2, 3, sizeof(char *), stringEquality)
}

ctest_return_t testTrue(ctest_t *test, void *arg) {
    puts("testTrue execution\n");
    int a = 5, b = 9, c = 14;
    CTAssertTrue(test, (a + b) == c)
}

ctest_return_t testFalse(ctest_t *test, void *arg) {
    puts("testFalse execution\n");
    int a = 5, b = 9, c = 14;
    CTAssertFalse(test, (a + b) == c, "testFalse failed: (%d + %d) is equal to %d\n", a, b, c)
}

ctest_return_t testNull(ctest_t *test, void *arg) {
    puts("testNull execution\n");
    const char *string = NULL;
    CTAssertNull(test, string, "testNull failed: string is not NULL\n")
}

ctest_return_t testNotNull(ctest_t *test, void *arg) {
    puts("testNotNull execution\n");
    char *string = (char *)malloc(5 * sizeof(char));
    CTAssertNotNull(test, string, "testNotNull failed: string is NULL\n")
    free(string);
}

ctest_return_t testGreaterThan(ctest_t *test, void *arg) {
    puts("testGreaterThan execution\n");
    int a = 2, b = 10;
    CTAssertGreaterThan(test, a, b, "testGreaterThan failed: %d is not greater than %d\n", a, b)
}

ctest_return_t testGreaterOrEqual(ctest_t *test, void *arg) {
    puts("testGreaterOrEqual execution\n");
    int a = 9, b = 10;
    CTAssertGreaterThanOrEqual(test, a, b, "testGreaterOrEqual failed: %d is not greater or equal to %d\n", a, b)
}

ctest_return_t testLessThan(ctest_t *test, void *arg) {
    puts("testLessThan execution\n");
    int a = 2, b = 10;
    CTAssertLessThan(test, a, b, "testLessThan failed: %d is not less than %d\n", a, b)
}

ctest_return_t testLessOrEqual(ctest_t *test, void *arg) {
    puts("testLessOrEqual execution\n");
    int a = 9, b = 10;
    CTAssertLessThanOrEqual(test, a, b, "testLessOrEqual failed: %d is not less or equal to %d\n", a, b)
}

ctest_return_t testPerformanceNotTooLong(ctest_t *test, void *arg) {
    usleep(70);
}

ctest_return_t testWithOptionalFunctions() {
    puts("testWithOptionalFunctions execution\n");
}

ctopt_return_t testCaseSetup(void *arg) {
    puts("testCaseSetup execution\n");
    (*((int *)arg)) += 10;
}

ctopt_return_t testCaseTeardown(void *arg) {
    puts("testCaseTeardown execution\n");
}

ctopt_return_t testPerformanceSetup(void *arg) {
    puts("testPerformanceSetup execution\n");
}

ctopt_return_t testPerformanceTeardown(void *arg) {
    puts("testPerformanceTeardown execution\n");
}

ctest_return_t testPerformanceTooLong(ctest_t *test, void *arg) {
    usleep(800);
}

void *testThread(void *arg) {
    sleep(3);
    fulfill((ctexpect_t *)arg);

    pthread_exit(NULL);
}

ctest_return_t testExpectation(ctest_t *test, void *arg) {

    ctexpect_t *expect = ctexpect(test, "Test expectation");

    pthread_t thread;

    if (pthread_create(&thread, NULL, testThread, (void *)expect) != 0) {
        puts("Couldn't create thread (main)\n");
    }

    ctexpectwait(test, 5);
    
    if (pthread_join(thread, NULL) != 0) {
        puts("Couldn't join thread\n");
    }
}

int main(int argc, const char * argv[]) {
    ctsuite_t *suite = ctsuite("Test assertions1");

    ctcase_t *tcase0 = ctcase("Failing");

    ctctestadd(tcase0, ctest("testFail", testFail, NULL));

    ctcase_t *tcase1 = ctcase("Equality");

    ctctestadd(tcase1, ctest("testEquality", testEquality, NULL));
    ctctestadd(tcase1, ctest("testNonEquality", testNonEquality, NULL));
    ctctestadd(tcase1, ctest("testStringEquality", testStringEquality, NULL));
    ctctestadd(tcase1, ctest("testStringNonEquality", testStringNonEquality, NULL));
    ctctestadd(tcase1, ctest("testArrayEquality", testArrayEquality, NULL));
    ctctestadd(tcase1, ctest("testArrayNonEquality", testArrayNonEquality, NULL));

    ctcase_t *tcase2 = ctcase("Boolean");

    ctctestadd(tcase2, ctest("testTrue", testTrue, NULL));
    ctctestadd(tcase2, ctest("testFalse", testFalse, NULL));

    ctcase_t *tcase3 = ctcase("Nullability");

    ctctestadd(tcase3, ctest("testNull", testNull, NULL));
    ctctestadd(tcase3, ctest("testNotNull", testNotNull, NULL));

    ctcase_t *tcase4 = ctcase("Comparison");

    ctctestadd(tcase4, ctest("testGreaterThan", testGreaterThan, NULL));
    ctctestadd(tcase4, ctest("testGreaterOrEqual", testGreaterOrEqual, NULL));
    ctctestadd(tcase4, ctest("testLessThan", testLessThan, NULL));
    ctctestadd(tcase4, ctest("testLessOrEqual", testLessOrEqual, NULL));

    ctcase_t *tcase5 = ctcase("Setup and teardown");
    unsigned int arg = 5;
    ctest_t *test = ctest("testWithOptionalFunctions", testWithOptionalFunctions, (void *)&arg);
    test->setup = testCaseSetup;
    test->tdown = testCaseTeardown;

    ctctestadd(tcase5, test);

    ctscaseadd(suite, tcase0);
    ctscaseadd(suite, tcase1);
    ctscaseadd(suite, tcase2);
    ctscaseadd(suite, tcase3);
    ctscaseadd(suite, tcase4);
    ctscaseadd(suite, tcase5);

    

    ctcase_t *tcase6 = ctcase("Performance");
    ctcperfadd(tcase6, ctest("testPerformanceNotTooLong", testPerformanceNotTooLong, NULL), 0.005);
    ctcperfadd(tcase6, ctest("testPerformanceTooLong", testPerformanceTooLong, NULL), 0.0005);

    ctcase_t *tcase7 = ctcase("Failing");
    ctctestadd(tcase7, ctest("testExpectation", testExpectation, NULL));
    
    ctsuite_t *suite2 = ctsuite("Test assertions2");
    ctscaseadd(suite2, tcase6);
    ctscaseadd(suite2, tcase7);

    ctsrun(suite);
    ctsrun(suite2);
    
    ctsfree(suite);
    ctsfree(suite2);
    
    return 0;
}
