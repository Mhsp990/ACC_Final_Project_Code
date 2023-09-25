//Doxygen is still being implemented
#include "ACC_Code.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/////////// Test Suite 1
//Test cases of "Time_Gap" output

// Case that rain sansor is false
void test_timeGap(void) { CU_ASSERT(3 == timeGap(false)); } 
// Case that rain sansor is true
void test_timeGap_rain(void) { CU_ASSERT(6 == timeGap(true)); } 
//////// Finished test Suite 1


/////////// Test Suite 2
//Test cases of "aux" output MC/DC 

//----------Test ID 22 MC/DC------------------//
void test_logicBLockAccEnable(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 11, 0,1); 
  CU_ASSERT(0 == i.aux);
}

//----------Test ID 38 MC/DC------------------//
void test_logicBLockAccEnable2(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 11, 1,0); 
  CU_ASSERT(0 == i.aux);
}

//----------Test ID 50 MC/DC------------------//
void test_logicBLockAccEnable3(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 1, 11, 0,0); 
  CU_ASSERT(0 == i.aux);
}

//----------Test ID 53 MC/DC------------------//
void test_logicBLockAccEnable4(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(1, 1, 0, 11, 0,0); 
  CU_ASSERT(1 == i.aux);
}

//----------Test ID 54 MC/DC------------------//
void test_logicBLockAccEnable5(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 11, 0,0); 
  CU_ASSERT(1 == i.aux);
}

//----------Test ID 56 MC/DC------------------//
void test_logicBLockAccEnable6(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 0, 0, 11, 0,0); 
  CU_ASSERT(0 == i.aux);
}

//----------Test ID 62 MC/DC------------------//
void test_logicBLockAccEnable7(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 10, 0,0); 
  CU_ASSERT(0 == i.aux);
}
//////// Finished test Suite 2


//////// Test Suite 3
//Test cases of "Acc_enabled" output MC/DC

//----------Test ID 22 MC/DC------------------//
void test_logicBLockAccEnable8(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 11, 0,1); 
  CU_ASSERT(0 == i.ACC_enabled);
}

//----------Test ID 45 MC/DC------------------//
void test_logicBLockAccEnable9(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(1, 1, 0, 10, 1,0); 
  CU_ASSERT(0 == i.ACC_enabled);
}

//----------Test ID 54 MC/DC------------------//
void test_logicBLockAccEnable10(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 11, 0,0); 
  CU_ASSERT(1 == i.ACC_enabled);
}

//----------Test ID 57 MC/DC------------------//
void test_logicBLockAccEnable11(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(1, 1, 1, 10, 0,0); 
  CU_ASSERT(0 == i.ACC_enabled);
}

//----------Test ID 61 MC/DC------------------//
void test_logicBLockAccEnable12(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(1, 1, 0, 10, 0,0); 
  CU_ASSERT(1 == i.ACC_enabled);
}

//----------Test ID 62 MC/DC------------------//
void test_logicBLockAccEnable13(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(0, 1, 0, 10, 0,0); 
  CU_ASSERT(0 == i.ACC_enabled);
}

//----------Test ID 63 MC/DC------------------//
void test_logicBLockAccEnable14(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(1, 0, 0, 10, 0,0); 
  CU_ASSERT(0 == i.ACC_enabled);
}

//----------Test ID 29 MC/DC------------------//
void test_logicBLockAccEnable15(void) {
  //params: aux, ACC_input, Fault_signal, Ego_velo, Gas_pedal, Brake_pedal
  struct ACCenable i = logicBlockAccEnable(1, 1, 0, 10, 0,1); 
  CU_ASSERT(0 == i.ACC_enabled);
}
//////// Finished test Suite 3


/////////// Test Suite 4
//Test cases of "ACC_speed_set" output

// Case that ACC_speed_set is lower than 40km/h. Expects 40km/h
void test_speedSet_min(void) { CU_ASSERT_DOUBLE_EQUAL(40/3.6, speedSet(20), 0.1)} 
// Case that ACC_speed_set is between 40 and 120 km/h. Expects input value.
void test_speedSet_any(void) { CU_ASSERT_DOUBLE_EQUAL(50/3.6, speedSet(50), 0.1) }
// Case that ACC_speed_set is higher than 120km/h. Expects 120km/h
void test_speedSet_max(void) { CU_ASSERT_DOUBLE_EQUAL(120/3.6, speedSet(150), 0.1)}
//////// Finished test Suite 4


//////// Test Suite 5
//Test cases of "Acceleration and Safe_distance" output

// Case that slowdown is the minimum value = -5m/s²
void test_accelerationControl1(void) { 
  //params: ACC_enabled, Ego_velo, Time_Gap, ACC_speed_set, Relative_distance_past, Relative_distance_pres, interval
  struct ACCcontrol i = accelerationControl(1, 11, 3, 27, 20.00, 20.00, 0.01);
  CU_ASSERT_DOUBLE_EQUAL(-5, i.Acceleration, 0.1);
  CU_ASSERT_DOUBLE_EQUAL(43.00, i.Safe_distance, 0.2);
}
// Case that acceleration is maximum value = 1.47m/s²
void test_accelerationControl2(void) {
  //params: ACC_enabled, Ego_velo, Time_Gap, ACC_speed_set, Relative_distance_past, Relative_distance_pres, interval
  struct ACCcontrol i = accelerationControl(1, 15, 3, 27, 70, 70, 0.01);
  CU_ASSERT_DOUBLE_EQUAL(1.47, i.Acceleration, 0.1);
  CU_ASSERT_DOUBLE_EQUAL(55.00, i.Safe_distance, 0.2);
}
// Case that acceleration is zero = 0m/s²
void test_accelerationControl3(void) { 
  // ACC_enabled, Ego_velo, Time_Gap, ACC_speed_set, Relative_distance_past, Relative_distance_pres, interval
  struct ACCcontrol i = accelerationControl(0, 16.6, 3, 19.4, 2.7, 60, 0.01);
  CU_ASSERT_DOUBLE_EQUAL(0.0, i.Acceleration, 0.1);
}
// Case of slowdown
void test_accelerationControl4(void) {
  //params: ACC_enabled, Ego_velo, Time_Gap, ACC_speed_set, Relative_distance_past, Relative_distance_pres, interval
  struct ACCcontrol i = accelerationControl(1, 20, 3, 27, 65, 65, 0.01);
  CU_ASSERT_DOUBLE_EQUAL(-2.5, i.Acceleration, 0.1);
  CU_ASSERT_DOUBLE_EQUAL(70, i.Safe_distance, 0.2);
}

void test_accelerationControl5(void) {
  //params: ACC_enabled, Ego_velo, Time_Gap, ACC_speed_set, Relative_distance_past, Relative_distance_pres, interval
  struct ACCcontrol i = accelerationControl(1, 15.80302, 3, 27.778, 60.48257, 60.46676, 0.001);
  CU_ASSERT_DOUBLE_EQUAL(1.47, i.Acceleration, 0.1);
  CU_ASSERT_DOUBLE_EQUAL(57.41, i.Safe_distance, 0.2);
}
//////// Finished test Suite 5








int main(void) {
  CU_pSuite pSuite1, pSuite2, pSuite3, pSuite4, pSuite5 = NULL; //Suite declaration

  if (CUE_SUCCESS != CU_initialize_registry()) //Connection check
    return CU_get_error();


///////////// - Suite 1 - ///////////////////
  pSuite1 = CU_add_suite("Test Suite of Time_Gap from timegap() ", init_suite, clean_suite);

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


///////////// - Suite 2 - ///////////////////
   pSuite2 = CU_add_suite("Test Suite of Aux output from logicBlockEnable()", init_suite, clean_suite);
  
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
 pSuite3 = CU_add_suite("Test Suite of Acc_enabled output from logicBlockEnable()", init_suite, clean_suite);
  
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

///////////// - Suite 4 - ///////////////////
   pSuite4 = CU_add_suite("Test Suite of ACC_speed_set from speedSet()", init_suite, clean_suite);
  
  if (NULL == pSuite4) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL ==
       CU_add_test(pSuite4, "SpeedSet function Testing for values below range", test_speedSet_min))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL ==
       CU_add_test(pSuite4, "SpeedSet function Testing for values within range", test_speedSet_any))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL ==
       CU_add_test(pSuite4, "SpeedSet function Testing for values above range", test_speedSet_max))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

///////////// - Suite 5 - ///////////////////
 pSuite5 = CU_add_suite("Test cases of Acceleration and Safe_distance output", init_suite, clean_suite);
   if (NULL == pSuite5) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  if ((NULL == 
    CU_add_test(pSuite5, "Acceleration Control function Test 1",test_accelerationControl1))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

   if ((NULL == 
    CU_add_test(pSuite5, "Acceleration Control function Test 2",test_accelerationControl2))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

if ((NULL == 
    CU_add_test(pSuite5, "Acceleration Control function Test 3",test_accelerationControl3))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == 
    CU_add_test(pSuite5, "Acceleration Control function Test 4",test_accelerationControl4))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == 
    CU_add_test(pSuite5, "Acceleration Control function Test 5",test_accelerationControl5))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  CU_cleanup_registry();
  return CU_get_error();
}
