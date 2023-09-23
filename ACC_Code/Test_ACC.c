#include "ACC_Code.h"
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

///////////////////////////////////////////////////////////////////////////////////
void test_speedSet_min(void) { CU_ASSERT_DOUBLE_EQUAL(40/3.6, speedSet(20), 0.1)}

void test_speedSet_any(void) { CU_ASSERT_DOUBLE_EQUAL(50/3.6, speedSet(50), 0.1) }

void test_speedSet_max(void) { CU_ASSERT_DOUBLE_EQUAL(120/3.6, speedSet(150), 0.1)}
///////////////////////////////////////////////////////////////////////////////////

void test_accelerationControl1(void) {
  // ACC_enabled, Ego_velo, Time_Gap, ACC_speed_set, Relative_distance_past, Relative_distance_pres, interval
  struct ACCcontrol i = accelerationControl(1, 19.44, 3, 27.78, 20.00, 20.00, 0.01);
  CU_ASSERT_DOUBLE_EQUAL(-0.96, i.Acceleration, 0.1);
  CU_ASSERT_DOUBLE_EQUAL(68.16, i.Safe_distance, 0.2);
}

void test_accelerationControl2(void) {
  // ACC_enabled, Ego_velo, Time_Gap, ACC_speed_set, Relative_distance_past, Relative_distance_pres, interval
  struct ACCcontrol i = accelerationControl(1, 19.71, 3, 27.77, 4.99722, 4.99457, 0.01);
  CU_ASSERT_DOUBLE_EQUAL(-1.29, i.Acceleration, 0.1);
  CU_ASSERT_DOUBLE_EQUAL(69.13, i.Safe_distance, 0.2);
}

void test_accelerationControl3(void) {
  // ACC_enabled, Ego_velo, Time_Gap, ACC_speed_set, Relative_distance_past, Relative_distance_pres, interval
  struct ACCcontrol i = accelerationControl(0, 16.6, 3, 19.4, 2.7, 60, 0.01);
  CU_ASSERT_DOUBLE_EQUAL(0.0, i.Acceleration, 0.1);
}

void test_accelerationControl4(void) {
  // ACC_enabled, Ego_velo, Time_Gap, ACC_speed_set, Relative_distance_past, Relative_distance_pres, interval
  struct ACCcontrol i = accelerationControl(1, 20.25, 3, 27.77, 89.13385, 89.12582, 0.01);
  printf("%f", i.Acceleration);
  CU_ASSERT_DOUBLE_EQUAL(0.34, i.Acceleration, 0.1);
  CU_ASSERT_DOUBLE_EQUAL(70.74, i.Safe_distance, 0.2);
}

void test_accelerationControl5(void) {
  // ACC_enabled, Ego_velo, Time_Gap, ACC_speed_set, Relative_distance_past, Relative_distance_pres, interval
  struct ACCcontrol i = accelerationControl(1, 11.18, 3, 27.77, 140.69298, 140.83113, 0.01);
  printf("%f", i.Acceleration);
  CU_ASSERT_DOUBLE_EQUAL(1.47, i.Acceleration, 0.1);
  CU_ASSERT_DOUBLE_EQUAL(43.55, i.Safe_distance, 0.2);
}


/////////////////////////////////////////////////////////////////////
///////////--Test cases of "aux" output MC/DC--//////////////////////

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

/////////////////////////////////////////////////////////////////////
////////--Test cases of "Acc_enabled" output MC/D-----///////////////

//----------Test ID 22 MC/DC------------------//
void test_logicBLockAccEnable8(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 11, 0,1); 
  CU_ASSERT(0 == i.ACC_enabled);
}

//----------Test ID 45 MC/DC------------------//
void test_logicBLockAccEnable9(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(1, 1, 0, 10, 1,0); 
  CU_ASSERT(0 == i.ACC_enabled);
}

//----------Test ID 54 MC/DC------------------//
void test_logicBLockAccEnable10(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 11, 0,0); 
  CU_ASSERT(1 == i.ACC_enabled);
}

//----------Test ID 57 MC/DC------------------//
void test_logicBLockAccEnable11(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(1, 1, 1, 10, 0,0); 
  CU_ASSERT(0 == i.ACC_enabled);
}

//----------Test ID 61 MC/DC------------------//
void test_logicBLockAccEnable12(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(1, 1, 0, 10, 0,0); 
  CU_ASSERT(1 == i.ACC_enabled);
}

//----------Test ID 62 MC/DC------------------//
void test_logicBLockAccEnable13(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 10, 0,0); 
  CU_ASSERT(0 == i.ACC_enabled);
}

//----------Test ID 63 MC/DC------------------//
void test_logicBLockAccEnable14(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(1, 0, 0, 10, 0,0); 
  CU_ASSERT(0 == i.ACC_enabled);
}

//----------Test ID 29 MC/DC------------------//
void test_logicBLockAccEnable15(void) {
  // aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(1, 1, 0, 10, 0,1); 
  CU_ASSERT(0 == i.ACC_enabled);
}

//--Finished test cases of "Acc_enabled" output MC/DC---//


int main(void) {
  CU_pSuite pSuite1, pSuite2, pSuite3 = NULL; //Suite declaration

  if (CUE_SUCCESS != CU_initialize_registry()) //Connection check
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


///////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////////////////////
  if ((NULL == 
    CU_add_test(pSuite1, "Acceleration Control function Test 1",test_accelerationControl1))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

   if ((NULL == 
    CU_add_test(pSuite1, "Acceleration Control function Test 2",test_accelerationControl2))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

if ((NULL == 
    CU_add_test(pSuite1, "Acceleration Control function Test 3",test_accelerationControl3))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == 
    CU_add_test(pSuite1, "Acceleration Control function Test 4",test_accelerationControl4))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == 
    CU_add_test(pSuite1, "Acceleration Control function Test 5",test_accelerationControl5))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

///////////// - Suite 2 - ///////////////////
   pSuite2 = CU_add_suite("Test Suite of Aux output from logic block", init_suite, clean_suite);
  if (NULL == pSuite2) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Test 1",
                           test_logicBLockAccEnable))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Test 2",
                           test_logicBLockAccEnable2))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Test 3",
                           test_logicBLockAccEnable3))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Test 4",
                           test_logicBLockAccEnable4))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Test 5",
                           test_logicBLockAccEnable5))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Test 6",
                           test_logicBLockAccEnable6))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite2, "Aux Control function Test 7",
                           test_logicBLockAccEnable7))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

///////////// - Suite 3 - ///////////////////
 pSuite3 = CU_add_suite("Test Suite of Acc_enabled output from logic block", init_suite, clean_suite);
  if (NULL == pSuite3) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite3, "Acc_enabled Control function Test 1",
                           test_logicBLockAccEnable8))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

   if ((NULL == CU_add_test(pSuite3, "Acc_enabled Control function Test 2",
                           test_logicBLockAccEnable9))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

    if ((NULL == CU_add_test(pSuite3, "Acc_enabled Control function Test 3",
                           test_logicBLockAccEnable10))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

   if ((NULL == CU_add_test(pSuite3, "Acc_enabled Control function Test 4",
                           test_logicBLockAccEnable11))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

   if ((NULL == CU_add_test(pSuite3, "Acc_enabled Control function Test 5",
                           test_logicBLockAccEnable12))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

   if ((NULL == CU_add_test(pSuite3, "Acc_enabled Control function Test 6",
                           test_logicBLockAccEnable13))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

   if ((NULL == CU_add_test(pSuite3, "Acc_enabled Control function Test 7",
                           test_logicBLockAccEnable14))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

if ((NULL == CU_add_test(pSuite3, "Acc_enabled Control function Test 8",
                           test_logicBLockAccEnable15))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  CU_cleanup_registry();
  return CU_get_error();
}
