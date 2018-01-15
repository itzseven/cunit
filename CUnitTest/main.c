//
//  main.c
//  CUnitTest
//
//  Created by itzseven on 28/11/2017.
//  Copyright © 2017 itzseven. All rights reserved.
//

#include "cunit.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#pragma mark - Test assertions

ctest_return_t testFail(ctest_t *test, void *arg) {
    puts("testFail execution\n");
    CTFail(test, "This library should be more comprehensive :(\n")
}

ctest_return_t testEquality(ctest_t *test, void *arg) {
    puts("testEquality execution\n");
    int a = 5, b = 9;
    CTAssertEqual(test, a, b, "testEquality failed: %d and %d are not equal\n", a, b)
}

ctest_return_t testNonEquality(ctest_t *test, void *arg) {
    puts("testNonEquality execution\n");
    int a = 5, b = 9;
    CTAssertNotEqual(test, a, b, "testNonEquality failed: %d and %d are equal\n", a, b)
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
    const char *string = (const char *)malloc(5 * sizeof(char));
    CTAssertNotNull(test, string, "testNotNull failed: string is NULL\n")
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
    int i = 0;
    
    FILE* debug = fopen("/dev/null", "w");
    
    for (i = 0; i < 10000; i++) {
        fprintf(debug, "%d.", i);
    }
    
    puts("testPerformanceNotTooLong finished\n");
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
    int i = 0;
    
    FILE* debug = fopen("/dev/null", "w");
    
    for (i = 0; i < 10000; i++) {
        fprintf(debug, "%d.", i);
    }
    
    puts("testPerformanceTooLong finished\n");
}

void *testThread(void *arg) {
    sleep(3);
    fulfill((ctexpect_t *)arg);
    
    pthread_exit(NULL);
}

ctest_return_t testExpectation(ctest_t *test, void *arg) {
    
    ctexpect_t *expect = ctexpectwdesc(test, "Test expectation");
    
    pthread_t thread;
    
    if (pthread_create(&thread, NULL, testThread, (void *)expect) != 0) {
        puts("Couldn't create thread (main)\n");
    }
    
    ctexpectwait(test, 5);
}

int main(int argc, const char * argv[]) {
    
//    ctsuite_t *suite = ctsuite("Test assertions1");
//
//    ctcase_t *tcase0 = ctcase("Failing");
//
//    ctestadd(tcase0, ctest("testFail", testFail, NULL));
//
//    ctcase_t *tcase1 = ctcase("Equality");
//
//    ctestadd(tcase1, ctest("testEquality", testEquality, NULL));
//    ctestadd(tcase1, ctest("testNonEquality", testNonEquality, NULL));
//
//    ctcase_t *tcase2 = ctcase("Boolean");
//
//    ctestadd(tcase2, ctest("testTrue", testTrue, NULL));
//    ctestadd(tcase2, ctest("testFalse", testFalse, NULL));
//
//    ctcase_t *tcase3 = ctcase("Nullability");
//
//    ctestadd(tcase3, ctest("testNull", testNull, NULL));
//    ctestadd(tcase3, ctest("testNotNull", testNotNull, NULL));
//
//    ctcase_t *tcase4 = ctcase("Comparison");
//
//    ctestadd(tcase4, ctest("testGreaterThan", testGreaterThan, NULL));
//    ctestadd(tcase4, ctest("testGreaterOrEqual", testGreaterOrEqual, NULL));
//    ctestadd(tcase4, ctest("testLessThan", testLessThan, NULL));
//    ctestadd(tcase4, ctest("testLessOrEqual", testLessOrEqual, NULL));
//
//    ctcase_t *tcase5 = ctcase("Setup and teardown");
//    unsigned int arg = 5;
//    ctest_t *test = ctest("testWithOptionalFunctions", testWithOptionalFunctions, (void *)&arg);
//    test->setup = testCaseSetup;
//    test->tdown = testCaseTeardown;
//
//    ctestadd(tcase5, test);
//
//    ctsaddtc(suite, tcase0);
//    ctsaddtc(suite, tcase1);
//    ctsaddtc(suite, tcase2);
//    ctsaddtc(suite, tcase3);
//    ctsaddtc(suite, tcase4);
//    ctsaddtc(suite, tcase5);
//
//    ctsuite_t *suite2 = ctsuite("Test assertions2");
//
//    ctcase_t *tcase = ctcase("Performance");
//
//    ctestperfadd(tcase, ctest("testPerformanceNotTooLong", testPerformanceNotTooLong, NULL), 0.005);
//    ctestperfadd(tcase, ctest("testPerformanceTooLong", testPerformanceTooLong, NULL), 0.0005);
//
//    ctsaddtc(suite2, tcase);
//
//    ctsrun(suite);
//    ctsrun(suite2);
    
    ctsuite_t *suite = ctsuite("Test expect");
    
    ctcase_t *tcase0 = ctcase("Failing");
    
    ctestadd(tcase0, ctest("testExpectation", testExpectation, NULL));
    
    ctsaddtc(suite, tcase0);
    
    ctsrun(suite);
    
    return 0;
}
