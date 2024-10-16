#!/usr/bin/env python

import os
import re
import signal
import subprocess
import sys
import time
import traceback

from check_hashes import check_hashes

check_hashes()

lab_dir_path = os.getcwd()
tests_dir_path = os.path.join(os.getcwd(), "tests")
logisim_path = os.path.join(os.getcwd(), "..", "tools", "logisim-evolution.jar")


class TestCase:
    """
    Runs specified circuit file and compares output against the provided reference trace file.
    """

    def __init__(self, id, name):
        self.id = id
        self.name = name

    def fix_circ(self):
        old_data = None
        data = None
        with open(self.get_circ_path(), "rt") as test_circ:
            old_data = test_circ.read()
        import_regex = re.compile(rf"desc=\"(file#[^\"]*\b{self.id}\.circ)\"")
        correct_desc = f'desc="file#../{self.id}.circ"'
        match = re.search(import_regex, old_data)
        if not match or match.group(0) == correct_desc:
            return
        print(f"Fixing bad import in {self.id}-test.circ")
        data = re.sub(import_regex, correct_desc, old_data)
        with open(self.get_circ_path(), "wt") as test_circ:
            test_circ.write(data)

    def get_circ_path(self):
        return os.path.join(tests_dir_path, f"{self.id}-test.circ")

    def get_expected_table_path(self):
        return os.path.join(tests_dir_path, "out", f"{self.id}-test.ref")

    def get_actual_table_path(self):
        return os.path.join(tests_dir_path, "out", f"{self.id}-test.out")

    def run(self):
        passed = False
        proc = None
        try:
            self.fix_circ()

            proc = subprocess.Popen(
                [
                    "java",
                    "-jar",
                    logisim_path,
                    "-tty",
                    "table,binary,csv",
                    self.get_circ_path(),
                ],
                stdout=subprocess.PIPE,
            )

            with open(self.get_expected_table_path(), "r") as reference:
                passed = self.check_output(proc.stdout, reference)
                kill_proc(proc)
                if passed:
                    return (True, "Matched expected output")
                else:
                    return (False, "Did not match expected output")
        except KeyboardInterrupt:
            sys.exit(1)
        except SystemExit:
            raise
        except:
            traceback.print_exc()
            if proc:
                kill_proc(proc)
        return (False, "Errored while running test")

    def check_output(self, student, reference):
        passed = True
        student_lines = []
        while True:
            student_line = student.readline().decode("ascii", errors="ignore").strip()
            reference_line = reference.readline().strip()
            if reference_line == "":
                break
            student_lines.append(student_line)
            if student_line != reference_line:
                passed = False
        output_path = self.get_actual_table_path()
        os.makedirs(os.path.dirname(output_path), mode=0o755, exist_ok=True)
        with open(output_path, "w") as f:
            for line in student_lines:
                f.write(f"{line}\n")
        return passed


def kill_proc(proc):
    if proc.poll() == None:
        if sys.platform == "win32":
            os.kill(proc.pid, signal.CTRL_C_EVENT)
            for _ in range(10):
                if proc.poll() != None:
                    break
                time.sleep(0.1)
    if proc.poll() == None:
        proc.kill()


tests = [
    TestCase("ex2", "Exercise 2: Sub-Circuits"),
    TestCase("ex3", "Exercise 3: Add Machine"),
    TestCase("ex4", "Exercise 4: Rotate"),
]


def run_tests(tests):
    print("Testing files...")
    tests_failed = 0
    tests_passed = 0

    for test in tests:
        did_pass, reason = test.run()
        if did_pass:
            print(f"PASSED test: {test.name}")
            tests_passed += 1
        else:
            print(f"FAILED test: {test.name} ({reason})")
            tests_failed += 1

    print(f"Passed {tests_passed}/{tests_failed + tests_passed} tests")


if __name__ == "__main__":
    run_tests(tests)
