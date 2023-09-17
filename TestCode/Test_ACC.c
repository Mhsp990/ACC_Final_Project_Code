#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "ACC_Code.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>


int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

void test_timeGap(void){
    CU_ASSERT(6==timeGap(true));
}


void test_speedSet(void){
    CU_ASSERT(40==speedSet(40));
}

void test_accelerationControl(void){
    struct ACCcontrol i = accelerationControl(1, 16.6, 3, 19.4, 2.7, 60);
    CU_ASSERT(0.11 == i.Acceleration);
}


int main (void)
{

CU_pSuite pSuite1 = NULL;

if (CUE_SUCCESS != CU_initialize_registry())
return CU_get_error();

pSuite1 = CU_add_suite("Test Suite1", init_suite, clean_suite);
if (NULL == pSuite1) {
CU_cleanup_registry();
return CU_get_error();
}

if ((NULL == CU_add_test(pSuite1, "TimeGap function Testing", test_timeGap)))
{
CU_cleanup_registry();
return CU_get_error();
}

if ((NULL == CU_add_test(pSuite1, "SpeedSet function Testing", test_speedSet)))
{
CU_cleanup_registry();
return CU_get_error();
}

if ((NULL == CU_add_test(pSuite1, "Acceleration Control function Testing", test_accelerationControl)))
{
CU_cleanup_registry();
return CU_get_error();
}

CU_basic_set_mode(CU_BRM_VERBOSE);
CU_basic_run_tests();

CU_cleanup_registry();
return CU_get_error();
}