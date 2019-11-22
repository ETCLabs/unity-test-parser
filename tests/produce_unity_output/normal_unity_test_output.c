/******************************************************************************
 * Copyright 2019 ETC Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************
 * This file is a part of RDMnet. For more information, go to:
 * https://github.com/ETCLabs/RDMnet
 *****************************************************************************/

// Test producing output from the base unity functionality

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "unity.h"

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

void setUp(void) {}

void tearDown(void) {}

void test_pass_1(void) { TEST_ASSERT_TRUE(true); }
void test_pass_2(void) { TEST_PASS(); }

// We want to exercise the failure messages from different assertion macros
// Unity always bails on the first failure, so each one needs to be in a
// different test

void basic_fail(void) { TEST_FAIL(); }
void ignore(void) { TEST_IGNORE(); }
void basic_assert_fail(void) { TEST_ASSERT(false); }
void boolean_fail(void) { TEST_ASSERT_TRUE(false); }
void equal_int(void) { TEST_ASSERT_EQUAL_INT(1, 2); }
void equal_uint8(void) { TEST_ASSERT_EQUAL_UINT8(128u, 255u); }
void less_than_int(void) { TEST_ASSERT_LESS_THAN_INT(1, 2); }
void equal_string(void) { TEST_ASSERT_EQUAL_STRING("yes", "no"); }
void equal_uint8_array(void)
{
  uint8_t arr_1[3] = {1, 2, 3};
  uint8_t arr_2[3] = {1, 2, 4};
  TEST_ASSERT_EQUAL_UINT8_ARRAY(arr_1, arr_2, 3);
}
void float_within(void) { TEST_ASSERT_FLOAT_WITHIN(0.5, 2.1, 3.0); }

void basic_fail_message(void) { TEST_FAIL_MESSAGE("AAAAA++++++"); }
void boolean_fail_message(void)
{
  TEST_ASSERT_TRUE_MESSAGE(false, "A message with some spaces");
}
void less_than_int_fail_message(void)
{
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(2, 1, "1 is not greater than 2, dummy");
}

void pass_with_message(void)
{
  TEST_MESSAGE("Diagnostic message...");
  TEST_PASS();
}
void fail_with_message(void)
{
  TEST_MESSAGE("Another diagnostic message...");
  TEST_FAIL();
}

FILE *active_file;

int main(void)
{
  active_file = fopen("unity_normal.txt", "w");
  assert(active_file);

  // Test printing some gak at the beginning and end of the test
  fprintf(active_file, "Starting tests...\n");
  fflush(active_file);

  UNITY_BEGIN();
  RUN_TEST(test_pass_1);
  RUN_TEST(test_pass_2);
  RUN_TEST(basic_fail);
  RUN_TEST(ignore);
  RUN_TEST(basic_assert_fail);
  RUN_TEST(boolean_fail);
  RUN_TEST(equal_int);
  RUN_TEST(equal_uint8);
  RUN_TEST(less_than_int);
  RUN_TEST(equal_string);
  RUN_TEST(equal_uint8_array);
  RUN_TEST(float_within);

  RUN_TEST(basic_fail_message);
  RUN_TEST(boolean_fail_message);
  RUN_TEST(less_than_int_fail_message);

  RUN_TEST(pass_with_message);
  RUN_TEST(fail_with_message);

  UNITY_END();

  fprintf(active_file, "The test is done now!\n");
  fflush(active_file);

  fclose(active_file);
  return 0;
}
