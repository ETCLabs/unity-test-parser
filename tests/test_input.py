"""
Exercise the TestResults object with different types of test output.
"""
import os
import unittest
import unity_test_parser

TEST_INPUT_VALID_1 = """
"""

THIS_SCRIPT_DIRECTORY = os.path.dirname(os.path.realpath(__file__))


class TestValidInput(unittest.TestCase):
    """Test passing valid input to the TestResults class."""

    def test_normal_valid_input(self):
        results = unity_test_parser.TestResults(
            """
path/to/first_file.c:100:sample_works:PASS
path/to/second_file.c:101:sample_doesnt_work:FAIL: Expected 1 was 0

-----------------------
2 Tests 1 Failures 0 Ignored
FAIL
""",
            unity_test_parser.UNITY_BASIC,
        )
        self.assertEqual(results.num_tests(), 2)
        self.assertEqual(results.num_passed(), 1)
        self.assertEqual(results.num_failed(), 1)

        first_test = results.tests()[0]
        self.assertEqual(first_test.name(), "sample_works")
        self.assertEqual(first_test.result(), "PASS")
        self.assertEqual(first_test.message(), "")
        self.assertEqual(
            first_test.full_line(), "path/to/first_file.c:100:sample_works:PASS"
        )

        second_test = results.tests()[1]
        self.assertEqual(second_test.file(), "path/to/second_file.c")
        self.assertEqual(second_test.line(), 101)
        self.assertEqual(second_test.name(), "sample_doesnt_work")
        self.assertEqual(second_test.result(), "FAIL")
        self.assertEqual(second_test.message(), "Expected 1 was 0")
        self.assertEqual(
            second_test.full_line(),
            "path/to/second_file.c:101:sample_doesnt_work:FAIL: Expected 1 was 0",
        )

    def test_fixture_verbose_valid_input(self):
        results = unity_test_parser.TestResults(
            """
TEST(sample, sample_works) PASS
TEST(sample, sample_doesnt_work)path/to/file.c:100::FAIL: Expected 1 was 0

------------------------
2 Tests 1 Failures 0 Ignored
FAIL
""",
            unity_test_parser.UNITY_FIXTURE_VERBOSE,
        )
        self.assertEqual(results.num_tests(), 2)
        self.assertEqual(results.num_passed(), 1)
        self.assertEqual(results.num_failed(), 1)

        first_test = results.tests()[0]
        self.assertEqual(first_test.name(), "sample_works")
        self.assertEqual(first_test.result(), "PASS")
        self.assertEqual(first_test.group(), "sample")
        self.assertEqual(first_test.message(), "")
        self.assertEqual(first_test.full_line(), "TEST(sample, sample_works) PASS")

        second_test = results.tests()[1]
        self.assertEqual(second_test.file(), "path/to/file.c")
        self.assertEqual(second_test.line(), 100)
        self.assertEqual(second_test.name(), "sample_doesnt_work")
        self.assertEqual(second_test.result(), "FAIL")
        self.assertEqual(second_test.group(), "sample")
        self.assertEqual(second_test.message(), "Expected 1 was 0")
        self.assertEqual(
            second_test.full_line(),
            "TEST(sample, sample_doesnt_work)path/to/file.c:100::FAIL: Expected 1 was 0",
        )

    def test_actual_normal_input(self):
        with open(
            os.path.join(
                THIS_SCRIPT_DIRECTORY, "produce_unity_output", "unity_normal.txt"
            ),
            "r",
        ) as output_file:
            _ = unity_test_parser.TestResults(
                output_file.read(), unity_test_parser.UNITY_BASIC
            )

    def test_actual_fixture_verbose_input(self):
        with open(
            os.path.join(
                THIS_SCRIPT_DIRECTORY,
                "produce_unity_output",
                "unity_fixture_verbose.txt",
            ),
            "r",
        ) as output_file:
            _ = unity_test_parser.TestResults(
                output_file.read(), unity_test_parser.UNITY_FIXTURE_VERBOSE
            )


class TestInvalidInput(unittest.TestCase):
    """Test passing invalid input to the TestResults class."""

    def test_empty_string(self):
        """Empty output should fail."""
        with self.assertRaises(ValueError):
            _ = unity_test_parser.TestResults("")

    def test_no_summary_block(self):
        """Test output with no test summary block should fail."""
        with self.assertRaises(ValueError):
            _ = unity_test_parser.TestResults(
                """
path/to/first_file.c:100:sample_works:PASS
path/to/second_file.c:101:sample_doesnt_work:FAIL: Expected 1 was 0
"""
            )

    def test_no_tests(self):
        """Test output with no test lines but a valid summary block should fail."""
        with self.assertRaises(ValueError):
            _ = unity_test_parser.TestResults(
                """

-----------------------
0 Tests 0 Failures 0 Ignored
PASS
"""
            )

    def test_basic_in_fixture(self):
        """TestResults should not accept basic Unity output in UNITY_FIXTURE mode."""
        with self.assertRaises(ValueError):
            _ = unity_test_parser.TestResults(
                """
path/to/first_file.c:100:sample_works:PASS
path/to/second_file.c:101:sample_doesnt_work:FAIL: Expected 1 was 0

-----------------------
2 Tests 1 Failures 0 Ignored
FAIL
""",
                unity_test_parser.UNITY_FIXTURE_VERBOSE,
            )

    def test_fixture_in_basic(self):
        """TestResults should not accept Unity Fixture output in UNITY_BASIC mode."""
        with self.assertRaises(ValueError):
            _ = unity_test_parser.TestResults(
                """
TEST(sample, sample_works) PASS
TEST(sample, sample_doesnt_work)path/to/file.c:100::FAIL: Expected 1 was 0

------------------------
2 Tests 1 Failures 0 Ignored
FAIL
""",
                unity_test_parser.UNITY_BASIC,
            )
