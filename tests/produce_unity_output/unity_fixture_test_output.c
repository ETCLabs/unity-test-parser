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

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "unity_fixture.h"

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

TEST_GROUP(test_tests);

TEST_SETUP(test_tests) {}

TEST_TEAR_DOWN(test_tests) {}

TEST(test_tests, test_pass) { TEST_ASSERT_TRUE(true); }

// We want to exercise the failure messages from different assertion macros
// Unity always bails on the first failure, so each one needs to be in a
// different test

TEST(test_tests, basic_fail) { TEST_FAIL(); }
TEST(test_tests, ignore) { TEST_IGNORE(); }
TEST(test_tests, basic_assert_fail) { TEST_ASSERT(false); }
TEST(test_tests, boolean_fail) { TEST_ASSERT_TRUE(false); }
TEST(test_tests, equal_int) { TEST_ASSERT_EQUAL_INT(1, 2); }
TEST(test_tests, equal_uint8) { TEST_ASSERT_EQUAL_UINT8(128u, 255u); }
TEST(test_tests, less_than_int) { TEST_ASSERT_LESS_THAN_INT(1, 2); }
TEST(test_tests, equal_string) { TEST_ASSERT_EQUAL_STRING("yes", "no"); }
TEST(test_tests, equal_uint8_array)
{
  uint8_t arr_1[3] = {1, 2, 3};
  uint8_t arr_2[3] = {1, 2, 4};
  TEST_ASSERT_EQUAL_UINT8_ARRAY(arr_1, arr_2, 3);
}
TEST(test_tests, float_within) { TEST_ASSERT_FLOAT_WITHIN(0.5, 2.1, 3.0); }

TEST(test_tests, basic_fail_message) { TEST_FAIL_MESSAGE("AAAAA++++++"); }
TEST(test_tests, boolean_fail_message) { TEST_ASSERT_TRUE_MESSAGE(false, "A message with some spaces"); }
TEST(test_tests, less_than_int_fail_message) { TEST_ASSERT_GREATER_THAN_INT_MESSAGE(2, 1, "1 is not greater than 2, dummy"); }

TEST(test_tests, pass_with_message)
{
  TEST_MESSAGE("Diagnostic message...");
  TEST_PASS();
}
TEST(test_tests, fail_with_message)
{
  TEST_MESSAGE("Another diagnostic message...");
  TEST_MESSAGE("Yet another diagnostic message...");
  TEST_FAIL();
}

TEST_GROUP_RUNNER(test_tests)
{
  // Generate a bunch of test passes
  for (int i = 0; i < 50; ++i)
  {
    RUN_TEST_CASE(test_tests, test_pass);
  }
  RUN_TEST_CASE(test_tests, basic_fail);
  RUN_TEST_CASE(test_tests, ignore);
  RUN_TEST_CASE(test_tests, basic_assert_fail);
  RUN_TEST_CASE(test_tests, boolean_fail);
  RUN_TEST_CASE(test_tests, equal_int);
  RUN_TEST_CASE(test_tests, equal_uint8);
  RUN_TEST_CASE(test_tests, less_than_int);
  RUN_TEST_CASE(test_tests, equal_string);
  RUN_TEST_CASE(test_tests, equal_uint8_array);
  RUN_TEST_CASE(test_tests, float_within);

  RUN_TEST_CASE(test_tests, basic_fail_message);
  RUN_TEST_CASE(test_tests, boolean_fail_message);
  RUN_TEST_CASE(test_tests, less_than_int_fail_message);

  RUN_TEST_CASE(test_tests, pass_with_message);
  RUN_TEST_CASE(test_tests, fail_with_message);
}

void run_all_tests(void)
{
  RUN_TEST_GROUP(test_tests);
}

FILE* active_file;

int main(int argc, char* argv[])
{
  const char* file_name = "unity_fixture.txt";
  for (int i = 0; i < argc; ++i)
  {
    if (0 == strcmp(argv[i], "-v"))
    {
      file_name = "unity_fixture_verbose.txt";
      break;
    }
    if (0 == strcmp(argv[i], "-s"))
    {
      file_name = "unity_fixture_silent.txt";
      break;
    }
  }

  active_file = fopen(file_name, "w");
  assert(active_file);

  // Test printing some gak at the beginning and end of the test
  fprintf(active_file, "Starting tests...\n");
  fflush(active_file);

  UnityMain(argc, argv, run_all_tests);

  fprintf(active_file, "The test is done now!\n");
  fflush(active_file);
  fclose(active_file);
  return 0;
}
