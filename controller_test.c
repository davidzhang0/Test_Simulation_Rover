#include <CUnit.h>
#include <Basic.h>
#include "controller.h"



int init_suite(void) { 
  return 0;
}

int clean_suite(void) {
  return 0;
}



void test1() {
  CU_ASSERT(1 == 1);
}

void test2() {
  CU_ASSERT(1 == 1);
}



int main() {
  CU_pSuite pSuite = NULL;
  CU_pTest pTest = NULL;
  if (CUE_SUCCESS != CU_initialize_registry()) {
    return CU_get_error();
  }
  pSuite = CU_add_suite( "suite", init_suite, clean_suite );
  if (NULL == pSuite) {
     CU_cleanup_registry();
     return CU_get_error();
  }

  pTest = CU_add_test(pSuite, "test1", test1);
  if (NULL == pTest) {
     CU_cleanup_registry();
     return CU_get_error();
  }
  pTest = CU_add_test(pSuite, "test2", test2);
  if (NULL == pTest) {
     CU_cleanup_registry();
     return CU_get_error();
  }

  CU_automated_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}


/*
gcc -Wall -LC:/CUnit-2.1-3/CUnit/Sources/.libs -IC:/CUnit-2.1-3/CUnit/Headers -o controller_test controller_test.c controller.o -lcunit


gcc -Wall -c controleur_rover.c -o controleur_rover.o
gcc -Wall -LC:/CUnit-2.1-3/CUnit/Sources/.libs -IC:/CUnit-2.1-3/CUnit/Headers -o controleur_rover_test controleur_rover_test.c controleur_rover.o -lcunit

*/