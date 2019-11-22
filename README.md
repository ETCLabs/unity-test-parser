# Unity Test Parser

[![Build Status](https://dev.azure.com/ETCLabs/unity-test-parser/_apis/build/status/ETCLabs.unity-test-parser?branchName=develop)](https://dev.azure.com/ETCLabs/unity-test-parser/_build/latest?definitionId=7&branchName=develop)

A Python 3 module for parsing the console output of the
[Unity Test Framework](https://github.com/ThrowTheSwitch/Unity). Unity (not to
be confused with the far more popular graphics framework) is a unit testing
framework optimized for embedded C. This module is intended to solve a couple
of problems related to using Unity and to unit testing embedded apps in
general:

* Unity does not have a built-in method for generating XML or HTML output which
  can be parsed by CI systems.
* Unity tests are often run in a simulator or directly on target hardware,
  where the exit code of the test process cannot be captured. In these cases,
  parsing the output is often the only possible method for determining whether
  the tests passed or failed.

This module can parse the output of basic Unity tests, of the form:
```
path/to/file.c:100:test_name:PASS
path/to/file.c:101:other_test_name:FAIL: Expected 1 Was 2
```

It can also parse the output of the Unity Fixture add-on, which groups tests
into test groups or "fixtures":
```
TEST(group, test_name)path/to/file.c:100::FAIL: Expected 1 Was 2
```

## Installation

unity-test-parser can be installed with `pip`:

```
pip install unity-test-parser
```

## Usage

### Getting Started

Use the `UnityTestResults` object to do the initial parsing of Unity test
output. Assuming your test output is in a file, you can do:

```python
import unity_test_parser

with open("results_file.txt", "r") as results_file:
    # This will raise ValueError if the results are improperly formatted
    results = unity_test_parser.TestResults(results_file.read())
```

You can inspect the individual tests, each represented by a UnityTest object:

```python
for test in results.test_iter():
    print("Test: {} Result was: {}".format(test.name(), test.result()))
```

To convert the tests to JUnit XML:
```python
import junit_xml

with open("test_results.xml", "w") as out_file:
    junit_xml.TestSuite.to_file(out_file, [results.to_junit()])
```

### Test Results Format

Currently two types of Unity output are supported for parsing: The output of
Unity used on its own (`UNITY_BASIC`) and the output of the unity_fixture extra
invoked with the `-v` flag (`UNITY_FIXTURE_VERBOSE`). Use the second `__init__`
argument to the `TestResults` object to specify which format is being parsed.
`UNITY_BASIC` is the default. If the results are in an invalid format,
ValueError is raised.

```python
results = unity_test_parser.TestResults(basic_results_file.read(), unity_test_parser.UNITY_BASIC)

fixture_results = unity_test_parser.TestResults(fixture_results_file.read(),
                                                unity_test_parser.UNITY_FIXTURE_VERBOSE)
```
