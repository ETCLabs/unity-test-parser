"""
Exercise the to_junit() function of the TestResults class using ApprovalTests to maintain golden
master XML.
"""
import io
import junit_xml
import os
import unittest
import unity_test_parser

from approvaltests.approvals import verify
from approvaltests.reporters.generic_diff_reporter_factory import GenericDiffReporterFactory
    
THIS_SCRIPT_DIRECTORY = os.path.dirname(os.path.realpath(__file__))
    
class TestJUnitOutput(unittest.TestCase):
    def setUp(self):
        self.reporter = GenericDiffReporterFactory().get_first_working()

    def test_junit_from_unity_normal(self):
        """Verify the JUnit created from the unity_normal.txt output against a golden master."""
        with open(os.path.join(THIS_SCRIPT_DIRECTORY, "produce_unity_output", "unity_normal.txt"), "r") as output_file:
            results = unity_test_parser.TestResults(output_file.read(), unity_test_parser.UNITY_BASIC)
        
        out_string = io.StringIO()
        junit_xml.TestSuite.to_file(out_string, [results.to_junit()])
        verify(out_string.getvalue(), self.reporter)

    def test_junit_from_unity_fixture(self):
        """Verify the JUnit created from the unity_fixture_verbose.txt output against a golden master."""
        with open(os.path.join(THIS_SCRIPT_DIRECTORY, "produce_unity_output", "unity_fixture_verbose.txt"), "r") as output_file:
            results = unity_test_parser.TestResults(output_file.read(), unity_test_parser.UNITY_FIXTURE_VERBOSE)
        
        out_string = io.StringIO()
        junit_xml.TestSuite.to_file(out_string, [results.to_junit()])
        verify(out_string.getvalue(), self.reporter)
