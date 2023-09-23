#include "../ACC_Code/ACC_Code.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

void test_timeGap(void) { CU_ASSERT(3 == timeGap(false)); }

void test_timeGap_rain(void) { CU_ASSERT(6 == timeGap(true)); }

void test_speedSet_min(void) { CU_ASSERT(40 == speedSet(20)); }

void test_speedSet_any(void) { CU_ASSERT(50 == speedSet(50)); }

void test_speedSet_max(void) { CU_ASSERT(120 == speedSet(150)); }

void test_accelerationControl(void) {
  struct ACCcontrol i = accelerationControl(1, 16.6, 3, 19.4, 2.7, 60);
  CU_ASSERT_DOUBLE_EQUAL(0.11, i.Acceleration, 0.01);
  CU_ASSERT_DOUBLE_EQUAL(60.00, i.Safe_distance, 0.2);
}

//----------Test cases of "aux" output MC/DC----//
//----------Test ID 22 MC/DC------------------//
void test_logicBLockAccEnable(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 11, 0,1); 
  CU_ASSERT(0 == i.aux);
}

//----------Test ID 38 MC/DC------------------//
void test_logicBLockAccEnable2(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 11, 1,0); 
  CU_ASSERT(0 == i.aux);
}

//----------Test ID 50 MC/DC------------------//
void test_logicBLockAccEnable3(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 1, 11, 0,0); 
  CU_ASSERT(0 == i.aux);
}

//----------Test ID 53 MC/DC------------------//
void test_logicBLockAccEnable4(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(1, 1, 0, 11, 0,0); 
  CU_ASSERT(0 == i.aux);
}

//----------Test ID 54 MC/DC------------------//
void test_logicBLockAccEnable5(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 11, 0,0); 
  CU_ASSERT(1 == i.aux);
}

//----------Test ID 56 MC/DC------------------//
void test_logicBLockAccEnable6(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 0, 0, 11, 0,0); 
  CU_ASSERT(0 == i.aux);
}

//----------Test ID 62 MC/DC------------------//
void test_logicBLockAccEnable7(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 10, 0,0); 
  CU_ASSERT(0 == i.aux);
}
//--Finished test cases of "aux" output MC/DC---//

//----------Test cases of "Acc_enabled" output MC/DC----//

int main(void) {

  CU_pSuite pSuite1, pSuite2 = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  pSuite1 = CU_add_suite("Test Suite1", init_suite, clean_suite);
  if (NULL == pSuite1) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL ==
       CU_add_test(pSuite1, "TimeGap function Testing without rain", test_timeGap))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

    if ((NULL ==
       CU_add_test(pSuite1, "TimeGap function Testing with rain", test_timeGap_rain))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL ==
       CU_add_test(pSuite1, "SpeedSet function Testing for values below range", test_speedSet_min))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL ==
       CU_add_test(pSuite1, "SpeedSet function Testing for values within range", test_speedSet_any))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL ==
       CU_add_test(pSuite1, "SpeedSet function Testing for values above range", test_speedSet_max))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == 
    CU_add_test(pSuite1, "Acceleration Control function Testing",test_accelerationControl))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

   pSuite2 = CU_add_suite("Test Suite of Aux output from logic block", init_suite, clean_suite);
  if (NULL == pSuite2) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Testing 1",
                           test_logicBLockAccEnable))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Testing 2",
                           test_logicBLockAccEnable2))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Testing 3",
                           test_logicBLockAccEnable3))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Testing 4",
                           test_logicBLockAccEnable4))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Testing 5",
                           test_logicBLockAccEnable5))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Testing 6",
                           test_logicBLockAccEnable6))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Testing 7",
                           test_logicBLockAccEnable7))) {
    CU_cleanup_registry();
    return CU_get_error();
  }



  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  CU_cleanup_registry();
  return CU_get_error();
}
