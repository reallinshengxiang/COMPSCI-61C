##############################################################
# Do not modify! (But feel free to use the functions provided)
##############################################################
import os, subprocess, sys, unittest, tempfile
from collections import defaultdict
from pathlib import Path
from typing import List, Optional, Set

a_regs = {f"a{i}" for i in range(8)}

# find venus jar
_root_dir = Path(os.path.dirname(__file__)).resolve()
_source_dir = (_root_dir / "src").resolve()
test_asm_dir = (_root_dir / "test-src").resolve()
_test_suffix = ".s"
_venus_path = (_root_dir / "tools" / "venus.jar").resolve()
assert _venus_path.is_file(), f"Could not find venus at {_venus_path}"

# --immutableText: immutable text, ensures that code cannot be modified
# --maxsteps -1: no upper bound on the number of cycles
_venus_default_args = ["--immutableText", "--maxsteps", "-1"]

__unittest = True
sys.tracebacklimit = 0


def run_raw_venus(
    check_calling_convention: bool = True,
    extra_flags: Optional[List[str]] = None,
    args: Optional[List[str]] = None,
    verbose: bool = False,
):
    cmd = ["java", "-jar", str(_venus_path)] + _venus_default_args
    if check_calling_convention:
        cmd += ["--callingConvention"]
    if extra_flags:
        cmd += extra_flags
    if args is not None:
        cmd += args
    # Windows + Python <3.8 bug: https://bugs.python.org/issue33617
    cmd = [str(arg) for arg in cmd]
    if verbose:
        print("Executing: " + " ".join(cmd))
    # print(" ".join((str(c) for c in cmd)))
    test_asm_dir.mkdir(parents=True, exist_ok=True)
    r = subprocess.run(
        cmd, cwd=test_asm_dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )
    return r


def run_venus(
    filename: str,
    check_calling_convention: bool = True,
    extra_flags: Optional[List[str]] = None,
    args: Optional[List[str]] = None,
    verbose: bool = False,
):
    assert os.path.isfile(filename), f"{filename} not found, cannot run venus"
    # print(filename)

    with tempfile.TemporaryDirectory() as tmp_dir:
        coverage_file = (Path(tmp_dir) / "coverage").resolve()
        final_flags = ["--coverageFile", str(coverage_file.absolute())]
        if extra_flags is not None:
            final_flags += extra_flags
        final_args = [str(filename)]
        if args is not None:
            final_args += args
        r = run_raw_venus(
            check_calling_convention=check_calling_convention,
            extra_flags=final_flags,
            args=final_args,
            verbose=verbose,
        )
        try:
            with open(coverage_file) as c:
                coverage = c.read()
        except FileNotFoundError:
            if verbose:
                print(f"Could not find the coverage file `{coverage_file}`!")
            coverage = ""
    return r, coverage


# global coverage dictionary
# maps filename -> line -> count
_global_coverage = defaultdict(lambda: defaultdict(lambda: 0))


def _process_coverage(coverage: str, file_filter: str):
    for line in coverage.split("\n"):
        line = line.strip()
        if len(line) == 0:
            continue
        # space-separated output but paths can have spaces...
        addr, rem = line.split(" ", 1)
        path, cov_value = rem.rsplit(" ", 1)
        import_path, line_num = path.rsplit(":", 1)
        filename = os.path.basename(import_path)
        if filename != file_filter and "../coverage-src/" + filename != file_filter:
            continue
        _global_coverage[filename][int(line_num)] += int(cov_value)


def print_coverage(filename: str, verbose: bool = True):
    if filename not in _global_coverage:
        print(f"No coverage numbers found for `{filename}`")
    else:
        cov = _global_coverage[filename]
        line_count = len(cov)
        covered_count = sum(c > 0 for c in cov.values())
        print()
        print(f"Coverage for `{filename}`: {covered_count}/{line_count}")
        if verbose:
            for line, count in cov.items():
                print(f"{filename}:{line}\t{count}")


def save_assembly(name: str, src: str, verbose: bool) -> str:
    # create test directory if it does not already exist
    if not test_asm_dir.is_dir():
        os.mkdir(test_asm_dir)
    filename = (test_asm_dir / (name + _test_suffix)).resolve()
    with open(filename, "w") as f:
        f.write(src)
    if verbose:
        print(f"Wrote test to file: {filename}")
    return filename


def _indent(lines: List[str]) -> List[str]:
    return [f"    {l}" if len(l.strip()) > 0 else l for l in lines]


def _read_lines(filename: str) -> List[str]:
    with open(filename) as _f:
        _res = _f.read().split("\n")
    return _res


class ArrayData:
    # represents an array in the data section
    def __init__(self, name: str, init: List[int]):
        self.name = name
        self.init = init

    def __len__(self):
        return len(self.init)


class FileName:
    # represents an input or output filename, relative to the unittests directory
    def __init__(self, name: str, is_input: bool):
        self.name = name
        self.is_input = is_input


def _test_id_to_name(test: unittest.TestCase) -> str:
    parts = test.id().split(".")
    assert len(parts) == 3, f"Unexpected test id: {test.id()}"
    return parts[2]


class AssemblyTest:
    """represents a single assembly test"""

    def __init__(
        self,
        test: unittest.TestCase,
        assembly: str,
        check_calling_convention: bool = True,
        no_utils: bool = False,
    ):
        self.name = _test_id_to_name(test)
        self._test = test
        self._has_executed = False
        self.data: List[str] = []
        self._checks: List[str] = []
        self._args: List[str] = []
        self._call: Optional[str] = None
        self._imports: List[str] = []
        self._array_count: int = 0
        self._msg_count: int = 0
        self._labels: Set[str] = set()
        self._output_regs: Set[int] = set()
        self._arrays: dict = {}
        self._assembly = assembly
        self._program_executed = False
        self._write_files: Set[str] = set()
        self._std_out: Optional[str] = None
        self.check_calling_convention = check_calling_convention

        if not no_utils:
            self.include("utils.s")
        self.include(assembly)

    def include(self, name: str):
        filename = (_source_dir / name).resolve()
        assert filename.is_file(), f"{filename} does not exist"
        self._imports.append(name)

    def call(self, function: str):
        """Specifies which function to call. Remember to provide any input with the `input` method."""
        assert (
            self._call is None
        ), f"Can only call one function per test! Already called {self._call}"
        self._call = function

    # This function puts the arguments into the unittest which is nice because then you can just
    # copy the test to venus, however we recommend students use the optional `args` argument to
    # the `execute` method instead.
    def _input_args(self, args: List[str]):
        """Provides command line arguments through the a0 (argc) and a1 (argv) registers."""
        assert (
            self._call is None
        ), f"You need to specify all inputs before calling `{self._call}`"
        assert isinstance(
            args, list
        ), f"{args} is a {type(args)}, expected a list of strings!"
        assert len(args) > 0, f"Expected a non-empty argument list!"
        assert all(
            isinstance(a, str) for a in args
        ), f"Expected a list of strings, not {[type(a) for a in args]}!"
        # all arguments could potentially be filenames that we write to, so let's just add them
        self._write_files |= set(args)
        # add dummy argument zero
        args = [""] + args
        # allocate args in memory
        arg_strings = [self._str(a, "arg") for a in args]
        # allocate a pointer array for argv
        self.data += [f"argv: .word " + " ".join("0" for _ in range(len(args)))]
        # load argc and argv
        self._args += ["", "# argument count in a0", f"li a0, {len(args)}"]
        self._args += [
            "",
            "# load pointers to argument strings into argv",
            f"la a0, argv",
        ]
        for ii, aa in enumerate(arg_strings):
            self._args += [f"la t1, {aa}", f"sw t1, {ii * 4}(a1)"]

    def input_scalar(self, register: str, value: int):
        """Provides a scalar input through an "a" register"""
        assert (
            self._call is None
        ), f"You need to specify all inputs before calling `{self._call}`"
        assert (
            register in a_regs
        ), f"Register {register} must be one of the a registers!"
        assert isinstance(value, int), f"{value} is a {type(value)}, expected an int!"
        self._args += ["", f"# load {value} into {register}", f"li {register} {value}"]

    def input_array(self, register: str, value: ArrayData):
        """Provides an array input through an "a" register"""
        assert (
            self._call is None
        ), f"You need to specify all inputs before calling `{self._call}`"
        assert (
            register in a_regs
        ), f"Register {register} must be one of the a registers!"
        assert isinstance(
            value, ArrayData
        ), f"{value} is a {type(value)}, expected an array (created with the array([..]) method!"
        name = self._lookup_array(value)
        self._args += [
            "",
            f"# load address to array {name} into {register}",
            f"la {register} {name}",
        ]

    def input_read_filename(self, register: str, filename: str):
        """Provides a filename string input through an "a" register"""
        full_path = (test_asm_dir / filename).resolve()
        if not full_path.is_file():
            print(f"WARN: Input file {full_path} does not exist.")
        self._input_filename(register, filename)

    def input_write_filename(self, register: str, filename: str):
        """Provides a filename string input through an "a" register"""
        dir_path = (test_asm_dir / filename).resolve().parent
        if not dir_path.is_dir():
            print(f"Creating directory: {dir_path}")
            dir_path.mkdir(parents=True, exist_ok=True)
        self._write_files.add(filename)
        self._input_filename(register, filename)

    def _input_filename(self, register: str, filename: str):
        assert (
            self._call is None
        ), f"You need to specify all inputs before calling `{self._call}`"
        assert (
            register in a_regs
        ), f"Register {register} must be one of the a registers!"
        path = self._str(filename)
        self._args += [
            "",
            f"# load filename {filename} into {register}",
            f"la {register} {path}",
        ]

    def check_scalar(self, register: str, value: int):
        """checks the the value of register"""
        assert (
            self._call is not None
        ), f"You must first call a function before checking its return values!"
        assert isinstance(value, int), f"{value} is a {type(value)}, expected an int!"
        """ Checks that when this function is called, we have not already assembled and run the test. """
        assert not self._has_executed, f"Test has already been assembled and run!"
        exit_code = 8
        saved_register = self._parse_register(register)
        lbl = self._make_lbl(f"{register}_eq_{value}")
        msg = f"msg{self._msg_count}"
        self._msg_count += 1
        self.data += [f'{msg}: .asciiz "Expected {register} to be {value} not: "']
        self._checks += [
            "",
            f"# check that {register} == {value}",
            f"li t0 {value}",
            f"beq {saved_register} t0 {lbl}",
            "# print error and exit",
            f"la a0, {msg}",
            "jal print_str",
            f"mv a0 {saved_register}",
            "jal print_int",
            "# Print newline",
            "li a0 '\\n'",
            "jal ra print_char",
            f"# exit with code {exit_code} to indicate failure",
            f"li a0 {exit_code}",
            "jal exit",
            f"{lbl}:",
            "",
        ]

    def check_array(self, array: ArrayData, value: List[int]):
        """checks the the value of an array in memory"""
        assert (
            self._call is not None
        ), f"You must first call a function before checking its return values!"
        """ Checks that when this function is called, we have not already assembled and run the test. """
        assert not self._has_executed, f"Test has already been assembled and run!"
        assert (
            len(value) > 0
        ), "Array to compare against has to contain at least one element."
        assert isinstance(
            array, ArrayData
        ), f"Input ({array}) was of the wrong type. Expected a t.array() return value"
        assert len(value) <= len(
            array
        ), "Array to compare against must contain a smaller or equal amount of elements."
        expected = self.array(value).name
        actual = "la a2, " + self._lookup_array(array)
        self._compare_int_array(array.name, actual, expected, value, exit_code=2)

    def check_array_pointer(self, register: str, value: List[int]):
        """check the memory region pointed to by the register content"""
        assert (
            self._call is not None
        ), f"You must first call a function before checking its return values!"
        """ Checks that when this function is called, we have not already assembled and run the test. """
        assert not self._has_executed, f"Test has already been assembled and run!"
        assert (
            len(value) > 0
        ), "Array to compare against has to contain at least one element."
        saved_register = self._parse_register(register)
        array_name = f"array pointed to by {register}"
        expected = self.array(value).name
        actual = f"mv a2 {saved_register}"
        self._compare_int_array(array_name, actual, expected, value, exit_code=2)

    def check_file_output(self, actual: str, expected: str):
        """compares the actual file to the expected file"""
        assert (
            self._program_executed
        ), f"You first need to `execute` the program before checking its outputs!"
        assert (
            actual in self._write_files
        ), f"Unknown output file {actual}. Did you forget to provide it to the program by calling input_write_filename?"
        full_expected = (test_asm_dir / expected).resolve()
        assert (
            full_expected.is_file()
        ), f"Reference file {full_expected} does not exist!"
        # check to make sure the output file exists
        full_actual = (test_asm_dir / actual).resolve()
        if not full_actual.is_file():
            self._test.fail(
                f"It seems like the program never created the output file {full_actual}",
            )
        # open and compare the files
        with open(full_actual, "rb") as a:
            actual_bin = a.read()
        with open(full_expected, "rb") as e:
            expected_bin = e.read()
        if actual_bin != expected_bin:
            self._test.fail(f"Bytes of {actual} and {expected} did not match!")

    def check_stdout(self, expected: str):
        """compares the output of the program"""
        assert (
            self._std_out is not None
        ), f"You first need to `execute` the program before checking stdout!"
        line = "-" * 35
        if self._std_out.strip() != expected.strip():
            assert_msg = f"\n{line}\nExpected stdout:\n{expected.strip()}\n{line}\nActual stdout:\n{self._std_out.strip()}\n{line}"
            self._test.fail(assert_msg)

    def _parse_register(self, register: str) -> str:
        assert register in a_regs, "Only a registers can be checked"
        register_index = int(register[1:])
        assert (
            register_index not in self._output_regs
        ), f"Register {register} was already checked!"
        self._output_regs.add(register_index)
        return f"s{register_index}"

    def _compare_int_array(
        self,
        array_name: str,
        actual: str,
        expected: str,
        value: List[int],
        exit_code: int,
    ):
        value_str = " ".join(str(v) for v in value)
        msg = self._str(
            f"Expected {array_name} to be:\\n{value_str}\\nInstead it is:\\n"
        )
        self._checks += [
            "",
            "##################################",
            f"# check that {array_name} == {value}",
            "##################################",
            "# a0: exit code",
            f"li a0, {exit_code}",
            "# a1: expected data",
            f"la a1, {expected}",
            "# a2: actual data",
            actual,
            "# a3: length",
            f"li a3, {len(value)}",
            "# a4: error message",
            f"la a4, {msg}",
            "jal compare_int_array",
        ]

    _can_fail = {"fopen", "fclose", "fread", "fwrite", "malloc", ""}

    def execute(
        self,
        code: int = 0,
        args: Optional[List[str]] = None,
        fail: str = "",
        verbose: bool = False,
        always_print_stdout: bool = False,
    ):
        if "-mcv" in _venus_default_args:
            always_print_stdout = True

        """Assembles the test and runs it through the venus simulator."""
        assert (
            fail in AssemblyTest._can_fail
        ), f"Invalid fail={fail}. Can only fail: {list(AssemblyTest._can_fail)}"

        """ As soon as this function is called, the AssemblyTest is considered "executed" for the duration of the life cycle of this test and should be treated as such. """
        self._has_executed = True

        # turn function to fail into a define
        if len(fail) == 0:
            defines = []
        else:
            ret = 0 if fail == "malloc" else -1
            defines = ["--def", f"#{fail.upper()}_RETURN_HOOK=li a0 {ret}"]

        # check arguments
        if args is not None:
            # TODO: check to see if any args clash with venus arguments
            assert len(args) > 0, "use None if you don't want to pass any arguments"
            for a in args:
                assert not a.startswith(
                    "-"
                ), f"argument '{a}' starting with '-' is not allowed"
            # all arguments could potentially be filenames that we write to, so let's just add them
            self._write_files |= set(args)
        else:
            # ensure that args is always a list
            args = []

        lines = []

        lines += [f".import ../src/{i}" for i in self._imports]
        lines += ["", ".data"] + self.data
        lines += [
            "",
            ".globl main_test",
            ".text",
            "# main_test function for testing",
            "main_test:",
        ]

        # prologue
        if len(self._output_regs) > 0:
            assert (
                len(self._output_regs) < 13
            ), f"Too many output registers: {len(self._output_regs)}!"
            p = [
                "# Prologue",
                f"addi sp, sp, -{4 * (len(self._output_regs) + 1)}",
                "sw ra, 0(sp)",
            ]
            p += [f"sw s{i}, {(i+1) * 4}(sp)" for i in range(len(self._output_regs))]
            lines += _indent(p + [""])

        lines += _indent(self._args)

        assert self._call is not None, "No function was called!"
        foo_call = ["", f"# call {self._call} function", f"jal ra {self._call}"]
        lines += _indent(foo_call)

        if len(self._output_regs) > 0:
            lines += _indent(["", "# save all return values in the save registers"])
            lines += _indent([f"mv s{i} a{i}" for i in self._output_regs] + [""])

        lines += _indent(self._checks)
        if code != 0:
            lines += _indent(
                [f"# we expect {self._call} to exit early with code {code}"]
            )

        lines += _indent(["", "# exit normally"])
        # epilogue
        if len(self._output_regs) > 0:
            p = ["# Epilogue", "lw ra, 0(sp)"]
            p += [f"lw s{i}, {(i + 1) * 4}(sp)" for i in range(len(self._output_regs))]
            p += [f"addi sp, sp, {4 * (len(self._output_regs) + 1)}"]
            lines += _indent(p + [""])
        # lines += _indent(["mv a0, zero", "ret"])
        lines += _indent([f"li a0 0", "jal exit"])
        lines += [""]

        if verbose:
            print()
        filename = save_assembly(self.name, "\n".join(lines), verbose=verbose)
        r, coverage = run_venus(
            filename, self.check_calling_convention, defines, args, verbose=verbose
        )
        _process_coverage(coverage, self._assembly)
        self._program_executed = True
        self._std_out = r.stdout.decode("UTF-8")
        venus_stderr_clean = (
            r.stderr.decode("UTF-8").replace("Found 0 warnings!", "").strip()
        )
        if r.returncode != code or venus_stderr_clean != "":
            self._print_failure(r, code)
        elif always_print_stdout:
            print(
                "stdout:\n"
                + r.stdout.decode("UTF-8")
                + "\n\nstderr:\n"
                + r.stderr.decode("UTF-8")
            )

    def _print_failure(self, r, expected_code):
        venus_out = (
            "stdout:\n"
            + r.stdout.decode("UTF-8")
            + "\n\nstderr:\n"
            + r.stderr.decode("UTF-8")
        )
        if expected_code != r.returncode:
            self._test.fail(
                f"Venus returned exit code {r.returncode} not {expected_code}.\n{venus_out}"
            )
        else:
            self._test.fail(
                f"Unexpected results from venus (exited with {r.returncode}).\n{venus_out}"
            )

    def _make_lbl(self, prefix: str) -> str:
        name = prefix
        ii = 0
        while name in self._labels:
            name = f"{prefix}_{ii}"
            ii += 1
        self._labels.add(name)
        return name

    def _lookup_array(self, a: ArrayData) -> str:
        assert (
            a.name in self._arrays
        ), f"Unknown array {a.name}. Did you declare it for this test?"
        assert (
            self._arrays[a.name] is a
        ), f"Array {a.name} was declared with a different test!"
        return a.name

    def array(self, data: List[int]) -> ArrayData:
        name = f"m{self._array_count}"
        self._array_count += 1
        self.data += [".align 4", f"{name}: .word " + " ".join((str(v) for v in data))]
        a = ArrayData(name, data)
        self._arrays[a.name] = a
        return a

    def _str(self, data: str, prefix: str = "msg") -> str:
        name = f"{prefix}{self._msg_count}"
        self._msg_count += 1
        self.data += [f'{name}: .asciiz "{data}"']
        return name
