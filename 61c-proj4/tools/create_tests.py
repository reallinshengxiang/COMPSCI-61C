import argparse
import fnmatch
from pathlib import Path

from framework import all_tests, set_tests_dir

# Do not touch!
if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="create_tests",
        description="Procedurally generates tests for CS 61C Project 4",
    )
    parser.add_argument(
        "test_name",
        nargs="?",
        default="*",
        help="Name or pattern of tests to generate (default: *)",
    )
    parser.add_argument(
        "-o",
        "--output-dir",
        type=Path,
        default=None,
        help="Sets the output directory (default: tests)",
    )

    args = parser.parse_args()

    # process args
    if args.output_dir:
        set_tests_dir(args.output_dir)

    # import tests
    import custom_tests
    import staff_tests

    # run tests
    tests = all_tests.keys()
    matched_tests = fnmatch.filter(tests, args.test_name)

    for test in matched_tests:
        print(f"Generating {test}")
        all_tests[test].generate()
