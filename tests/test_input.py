"""
Test the unity input
"""
import unittest
from unity_test_parser import UnityTestResults

TEST_INPUT_VALID_1 = """
TEST(sample, sample_works) PASS
TEST(sample, sample_doesnt_work)path/to/file.c:100::FAIL: Expected 1 to be equal to 0

------------------------
2 Tests 1 Failures 0 Ignored
FAIL
"""


class TestValidInput(unittest.TestCase):
    """Test passing valid input to the UnityTestResults class."""

    def test_valid_input_1(self):
        _ = UnityTestResults(TEST_INPUT_VALID_1)
