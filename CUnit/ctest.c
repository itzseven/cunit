//
//  ctest.c
//  cunit
//
//  Created by itzseven on 13/01/2018.
//  Copyright © 2018 itzseven. All rights reserved.
//

#include "ctest.h"
#include "_ctest.h"
#include "_ctexpect.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

ctexpectlist_t *ctexpectlist(ctexpect_t *expectation) {
    ctexpectlist_t *list = malloc(sizeof(ctexpectlist_t));
    list->expect = expectation;
    list->next = NULL;
    
    return list;
}

ctest_int_t *ctest_int() {
    ctest_int_t *testInternal = malloc(sizeof(ctest_int_t));
    testInternal->failures = 0;
    testInternal->unfulfilledExpectations = 0;
    testInternal->expectations = NULL;
    testInternal->expectCount = 0;
    
    return testInternal;
}

ctest_t *ctest(const char *name, ctinv_ptr_t inv, void *arg) {
    ctest_t *test = malloc(sizeof(ctest_t));
    test->name = name;
    test->inv = inv;
    test->arg = arg;
    test->setup = NULL;
    test->tdown = NULL;
    test->_internal = ctest_int();
    
    return test;
}

void ctfree(ctest_t *test) {
    ctest_int_t *testInternal = (ctest_int_t *)test->_internal;
    
    ctexpectlist_t *expectList = testInternal->expectations;
    
    while (expectList != NULL) {
        ctefree(expectList->expect);
        ctexpectlist_t *nextExpect = expectList->next;
        free(expectList);
        expectList = nextExpect;
    }
    
    free(testInternal);
    free(test);
}

ctexpect_t *ctexpect(ctest_t *test, const char *desc) {
    ctest_int_t *testInternal = (ctest_int_t *)test->_internal;
    ctexpect_t *expect = _ctexpect(desc);
    
    if (testInternal->expectations == NULL) {
        testInternal->expectations = ctexpectlist(expect);
    } else {
        ctexpectlist_t *tcurrentList = testInternal->expectations;
        
        while (tcurrentList->next != NULL) {
            tcurrentList = tcurrentList->next;
        }
        
        tcurrentList->next = ctexpectlist(expect);
    }
    
    testInternal->expectCount++;
    
    return expect;
}

void *ctthreadwait(void *arg) {
    unsigned int timeout = *((int *)arg);
    sleep(timeout);
    pthread_exit(NULL);
}

void ctexpectwait(ctest_t *test, unsigned int timeout) {
    pthread_t thread;
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    if (pthread_create(&thread, NULL, ctthreadwait, (void *)&timeout) != 0) {
        puts("Couldn't create thread\n");
    }
    
    pthread_attr_destroy(&attr);
    
    if (pthread_join(thread, NULL) != 0) {
        puts("Couldn't join thread\n");
    }
    
    ctexpectlist_t *list = ((ctest_int_t *)test->_internal)->expectations;
    
    while (list != NULL) {
        ctexpect_t *exp = list->expect;
        ctest_int_t *testInternal = (ctest_int_t *)test->_internal;
        ctexpect_int_t *expectInternal = (ctexpect_int_t *)exp->_internal;
        
        if (!expectInternal->fulfilled) {
            printf("Expectation %s is unfulfilled\n", exp->desc);
            testInternal->unfulfilledExpectations++;
        }
        
        list = list->next;
    }
}

void ctestfail(ctest_t *test) {
    ((ctest_int_t *)test->_internal)->failures++;
}
