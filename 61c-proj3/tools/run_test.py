from io import StringIO
from contextlib import redirect_stdout
from pathlib import Path
import argparse
import csv
import hashlib
import os
import re
import subprocess
import sys
import textwrap
import time
import traceback

from diff_output import diff_output
from fetch_encoding import update_imm_circ


CIRC_IMPORT_REGEX = re.compile(r"desc=\"file#([^\"]+?\.circ)\"")

proj_dir_path = Path(__file__).parent.parent
tests_dir_path = proj_dir_path / "tests"
logisim_path = proj_dir_path / "tools" / "logisim-evolution.jar"

banned_component_names = [
    "Pull Resistor",
    "Transistor",
    "Transmission Gate",
    "Power",
    "POR",
    "Ground",
    "Divider",
    "Random",
    "PLA",
    "RAM",
    "Random Generator",
    "Clock",
]
known_imports_dict = {
    "cpu/cpu.circ": [
        "cpu/alu.circ",
        "cpu/branch-comp.circ",
        "cpu/control-logic.circ",
        "cpu/imm-gen.circ",
        "cpu/regfile.circ",
        "cpu/partial-load.circ",
        "cpu/partial-store.circ",
    ],
    "harnesses/alu-harness.circ": [
        "cpu/alu.circ",
    ],
    "harnesses/imm-gen-harness.circ": [
        "cpu/imm-gen.circ",
    ],
    "harnesses/branch-comp-harness.circ": [
        "cpu/branch-comp.circ",
    ],
    "harnesses/cpu-harness.circ": [
        "cpu/cpu.circ",
        "cpu/mem.circ",
    ],
    "harnesses/regfile-harness.circ": [
        "cpu/regfile.circ",
    ],
    "harnesses/run.circ": [
        "harnesses/cpu-harness.circ",
    ],
    "tests/unit-alu/*.circ": [
        "cpu/alu.circ",
    ],
    "tests/unit-regfile/*.circ": [
        "cpu/regfile.circ",
    ],
    "tests/unit-partial-load/*.circ": [
        "cpu/partial-load.circ",
    ],
    "tests/unit-partial-store/*.circ": [
        "cpu/partial-store.circ",
    ],
    "tests/integration-*/*.circ": [
        "harnesses/cpu-harness.circ",
    ],
}


def find_banned(circ_path):
    if circ_path.name in ["mem.circ"]:
        return False
    with circ_path.open("r") as f:
        contents = f.read()
    found = False
    for component_name in banned_component_names:
        if re.search(rf'<comp.*\bname="{component_name}"', contents):
            print(
                f"ERROR: found banned element ({component_name}) in {circ_path.as_posix()}"
            )
            found = True
    return found


starter_file_hashes = {
    "harnesses/alu-harness.circ": "d3ad2ad2394ced94a380b077f0027c2c",
    "harnesses/branch-comp-harness.circ": "3d3720b05a99e8dbe3b54d00c53dffec",
    "harnesses/cpu-harness.circ": "3a9ad93fafdc727bf78833261eb9c1b3",
    "harnesses/imm-gen-harness.circ": "91b37d8150a1e1741fdeec3d3735e8bb",
    "harnesses/partial-load-harness.circ": "85e2f5ba8c0eeffbd50a3a2340b8fb40",
    "harnesses/partial-store-harness.circ": "5f092637e013aae70e7edb5399b0e81f",
    "harnesses/regfile-harness.circ": "572742a1318cce0afdd884f1af6b6a85",
    "harnesses/run.circ": "f07829beebb21f5c0511d69f1e964c1f",
    "tests/integration-addi/addi-basic.circ": "102826832b8d2ce80ceac8f69abea11a",
    "tests/integration-addi/addi-negative.circ": "15836720e2c055032c05d642a9e362c9",
    "tests/integration-addi/addi-positive.circ": "fad24964e7c18958a84a3a841362e196",
    "tests/integration-addi/out/addi-basic.piperef": "5adef575722b93ed04c82a37618ef2eb",
    "tests/integration-addi/out/addi-basic.ref": "4e1cfb0543418d95d3c75b49224c394d",
    "tests/integration-addi/out/addi-negative.piperef": "880daf267a095c2ab6012186be29aea6",
    "tests/integration-addi/out/addi-negative.ref": "8c4f4354e017d347a246b549c0a7c019",
    "tests/integration-addi/out/addi-positive.piperef": "554b05ad4952e65e3ac8844f36c0394e",
    "tests/integration-addi/out/addi-positive.ref": "079f17180d45eb7104220427f6360412",
    "tests/integration-all-regs/all-regs-basic.circ": "c4340e3f3d4f604de33260dcc2e454c2",
    "tests/integration-all-regs/out/all-regs-basic.piperef": "1a294bd256f2900d58a05a30f8cb19a0",
    "tests/integration-all-regs/out/all-regs-basic.ref": "5859036aac504f27418af1dddfb6128e",
    "tests/integration-branch/branch-basic.circ": "cf7b5dbc22049097ba5a3c936d342da8",
    "tests/integration-branch/out/branch-basic.piperef": "26eeaf22f577024bf2c501f27f032ce9",
    "tests/integration-branch/out/branch-basic.ref": "5673c41c4955ffecf1b6815c6ba244cc",
    "tests/integration-immediates/immediates-basic.circ": "3ba1f373d4e4d77bb9b8252afa1be559",
    "tests/integration-immediates/out/immediates-basic.piperef": "0d97830b0c73851d9a8b55487e2650ee",
    "tests/integration-immediates/out/immediates-basic.ref": "6da63f3c1b24f300abca821c7b7b7eec",
    "tests/integration-jump/jump-basic.circ": "c327f3effdabfcde85ce8cdbae037631",
    "tests/integration-jump/jump-empty.circ": "9059db50ff47068ca9d3595d5276f7b7",
    "tests/integration-jump/jump-far.circ": "fa6c3ab9831f90706651a0f87539c5a8",
    "tests/integration-jump/out/jump-basic.piperef": "23a000a99fd7da605c7d0956bec7421e",
    "tests/integration-jump/out/jump-basic.ref": "7596edead8499ee707032f40a36d1195",
    "tests/integration-jump/out/jump-empty.piperef": "cfbe380fd5e9e0ea6558e6e58e7ecb8a",
    "tests/integration-jump/out/jump-empty.ref": "fe6d0e0373a367879e4ed244c00dc71d",
    "tests/integration-jump/out/jump-far.piperef": "30565d9c925b08c32f3994f59e0f7189",
    "tests/integration-jump/out/jump-far.ref": "ed18fd0e1ccc3a05c22d60d001a50e15",
    "tests/integration-lui/lui-basic.circ": "f0099535c87ff204cd592da28b1618fd",
    "tests/integration-lui/out/lui-basic.piperef": "89bc4cda38d269d7174babfd4706d43d",
    "tests/integration-lui/out/lui-basic.ref": "dd77a9439d6e8f1809e2e8d2ec4610ea",
    "tests/integration-mem/mem-load-basic.circ": "fa1e4d77b6adb560262935c599825c13",
    "tests/integration-mem/mem-sign-extend.circ": "f1f8e7c3c64c79c1de2e27a2b84f280d",
    "tests/integration-mem/mem-store-basic.circ": "99d9ba74f7227db499a11c6d2a4ecacf",
    "tests/integration-mem/out/mem-load-basic.piperef": "b605abe6bd1179ca65e75ee44a2b4d9f",
    "tests/integration-mem/out/mem-load-basic.ref": "ec15cb3d0bf51001f5afc5926cd25786",
    "tests/integration-mem/out/mem-sign-extend.piperef": "d158338ecfcb249a589deed5a8c8ee99",
    "tests/integration-mem/out/mem-sign-extend.ref": "b7f514f6a8df4f716deff9039f1be3c5",
    "tests/integration-mem/out/mem-store-basic.piperef": "7dc25fcd6505835a734e88226524344d",
    "tests/integration-mem/out/mem-store-basic.ref": "b0bd3f8d5c28aa44a5908b9ce40f6c6f",
    "tests/integration-programs/out/programs-fib.piperef": "b2723e49f6e2bf889c9c5a26f8a034ec",
    "tests/integration-programs/out/programs-fib.ref": "98b7fd0d0545cd66c747dbf30d0eb114",
    "tests/integration-programs/out/programs-power.piperef": "02108bb7b96a90ef93b2a87afa60ff7b",
    "tests/integration-programs/out/programs-power.ref": "a727b44291c2efc8ed1c319ac5929b99",
    "tests/integration-programs/programs-fib.circ": "51aaaf5f4803d177d2af7016242d1e82",
    "tests/integration-programs/programs-power.circ": "e7014bc982d1e1dca099d62c1f26cb03",
    "tests/unit-alu/alu-add.circ": "70f7fe578c9a3ccfd60519442ede7792",
    "tests/unit-alu/alu-all.circ": "dc30bd98aa84dd6424dea3a8e48e7c75",
    "tests/unit-alu/alu-logic.circ": "16410c9c778e2c7bd4a54369a78b072d",
    "tests/unit-alu/alu-mult.circ": "c76180bf1b7a8c8b82f8329bc844c697",
    "tests/unit-alu/alu-shift.circ": "16410c9c778e2c7bd4a54369a78b072d",
    "tests/unit-alu/alu-slt-sub-bsel.circ": "92ab9e099e8f042c389be0332eaff57e",
    "tests/unit-alu/out/alu-add.ref": "dcc38dabf240b9643322caab0a09fe1f",
    "tests/unit-alu/out/alu-all.ref": "97b8eaac983e2774b9b47bff46d41748",
    "tests/unit-alu/out/alu-logic.ref": "09a61bb272d6563dcfa35d76840765ec",
    "tests/unit-alu/out/alu-mult.ref": "786c1000eb6780b8df85d2528a17f972",
    "tests/unit-alu/out/alu-shift.ref": "09a61bb272d6563dcfa35d76840765ec",
    "tests/unit-alu/out/alu-slt-sub-bsel.ref": "a27b828a9e4d093aaff92b7183cd9fb0",
    "tests/unit-branch-comp/branch-comp-all.circ": "97116f37b6b68592ddb95e1117be67f7",
    "tests/unit-branch-comp/branch-comp-signed.circ": "efd82df0d8a7d13c816f94c12c6f4c05",
    "tests/unit-branch-comp/branch-comp-unsigned.circ": "ef677ae0b9e87cdae6cf916094f3f583",
    "tests/unit-branch-comp/out/branch-comp-all.ref": "d30994303c8ceb1905f12af93e6c14da",
    "tests/unit-branch-comp/out/branch-comp-signed.ref": "132dc72e35324509f4e85d1931bc1787",
    "tests/unit-branch-comp/out/branch-comp-unsigned.ref": "6ac191bd3ba2c49afbbd4ecdde82f9d9",
    "tests/unit-partial-load/out/partial-load-byte.ref": "b06a9f39d91399ada2076dbabda0b044",
    "tests/unit-partial-load/out/partial-load-half.ref": "29eef73a529a567327be8348123dee12",
    "tests/unit-partial-load/out/partial-load-word.ref": "4d30a508d7c07f7eabc67a8dcfe0bffc",
    "tests/unit-partial-load/partial-load-byte.circ": "c78b8ff5f4e6df66b9ff04610c3b09f8",
    "tests/unit-partial-load/partial-load-half.circ": "f8c9347b40921011a2e93bb9f460e818",
    "tests/unit-partial-load/partial-load-word.circ": "3d5e071087b84b049a27817743ba5bdc",
    "tests/unit-partial-store/out/partial-store-byte.ref": "9c2d7230b0b57be5842564fa267e44e2",
    "tests/unit-partial-store/out/partial-store-half.ref": "7439a9715528ff3ac5d19e431ccd7e37",
    "tests/unit-partial-store/out/partial-store-non-store.ref": "f1cded4242cb0030a4eaabd90e44c51a",
    "tests/unit-partial-store/out/partial-store-word.ref": "03c331ff93622577e349dec9aed01cf2",
    "tests/unit-partial-store/partial-store-byte.circ": "97f71935cef44e9a03be7210eda46abb",
    "tests/unit-partial-store/partial-store-half.circ": "ef277d3b541c5abccadc0a2631f7d694",
    "tests/unit-partial-store/partial-store-non-store.circ": "cc7d39a2d26d397c2114ba6e0e1d272c",
    "tests/unit-partial-store/partial-store-word.circ": "3f6c24cc6bee94b8320e6c67e37e22bd",
    "tests/unit-regfile/out/regfile-more-regs.ref": "c89f3ec0f21d3a3e337e8f77c2030fe7",
    "tests/unit-regfile/out/regfile-read-only.ref": "1d94f63dea83b3a29bf3248ef9d2ea98",
    "tests/unit-regfile/out/regfile-read-write.ref": "fc633018f091dc1595a157afeb6cb1ba",
    "tests/unit-regfile/out/regfile-x0.ref": "8c9b3e2281cbaabf69f8d4c3a7f4b963",
    "tests/unit-regfile/regfile-more-regs.circ": "9e4edb294fd8b13e212bf5be1aeac0ef",
    "tests/unit-regfile/regfile-read-only.circ": "ed4691e5868ecd42343bd717684f4aa1",
    "tests/unit-regfile/regfile-read-write.circ": "746b02df6cbd1a092974914778dcfeef",
    "tests/unit-regfile/regfile-x0.circ": "ed4be7ee94d9a8f9f137f639ea670bc2",
}


class TestCase:
    def __init__(self, circ_path, name=None):
        self.circ_path = Path(circ_path)
        self.id = str(self.circ_path)
        self.name = name or self.circ_path.stem

    def can_pipeline(self):
        if self.circ_path.match("unit-*/*.circ"):
            return False
        return True

    def fix_circ(self):
        fix_circ(self.circ_path)

    def check_hashes(self, pipelined=False):
        passed, reason = check_hash(self.circ_path)
        if not passed:
            return passed, reason

        passed, reason = check_hash(self.get_expected_table_path(pipelined=pipelined))
        if not passed:
            return passed, reason

        passed, reason = check_hash(proj_dir_path / "harnesses" / "cpu-harness.circ")
        if not passed:
            return passed, reason

        passed, reason = check_hash(proj_dir_path / "harnesses" / "run.circ")
        if not passed:
            return passed, reason

        return (True, "Circuit data matches starter code")

    def get_actual_table_path(self):
        return self.circ_path.parent / "out" / f"{self.name}.out"

    def get_expected_table_path(self, pipelined=False):
        path = self.circ_path.parent / "out" / f"{self.name}.ref"
        if pipelined:
            path = path.with_name(f"{self.name}.piperef")
        return path

    def run(self, pipelined=False):
        self.fix_circ()
        passed, reason = self.check_hashes(pipelined=pipelined)
        if not passed:
            return passed, reason, None

        if pipelined and not self.can_pipeline():
            pipelined = False
        passed = False
        proc = None

        output_path = self.get_actual_table_path()
        output_path.parent.mkdir(parents=True, exist_ok=True)
        try:
            with output_path.open("w") as output_file:
                proc = subprocess.Popen(
                    [
                        "java",
                        "-jar",
                        str(logisim_path),
                        "-tty",
                        "table,binary,csv",
                        str(self.circ_path),
                    ],
                    stdout=output_file,
                    encoding="utf-8",
                    errors="ignore",
                )
                proc.wait()

            with redirect_stdout(StringIO()) as s:
                passed = diff_output(self.circ_path, pipelined)
                s.flush()
                s.seek(0)
                diff = s.read().strip("\n")

            if not passed:
                return False, "Did not match expected output", diff
            return True, "Matched expected output", None
        except KeyboardInterrupt:
            kill_proc(proc)
            sys.exit(1)
        except:
            traceback.print_exc()
            kill_proc(proc)
        return False, "Errored while running test", None


def fix_circ(circ_path):
    circ_path = circ_path.resolve()

    for glob, known_imports in known_imports_dict.items():
        if circ_path.match(glob):
            old_data = None
            data = None
            is_modified = False
            with circ_path.open("r", encoding="utf-8") as test_circ:
                old_data = test_circ.read()
                data = old_data
            for match in re.finditer(CIRC_IMPORT_REGEX, old_data):
                import_path_str = match.group(1)
                import_path = (circ_path.parent / Path(import_path_str)).resolve()
                for known_import in known_imports:
                    if import_path.match(known_import):
                        known_import_path = (proj_dir_path / known_import).resolve()
                        expected_import_path = Path(
                            os.path.relpath(known_import_path, circ_path.parent)
                        )
                        if import_path_str != expected_import_path.as_posix():
                            print(
                                f"Fixing bad import {import_path_str} in {circ_path.as_posix()} (should be {expected_import_path.as_posix()})"
                            )
                            data = data.replace(
                                import_path_str, expected_import_path.as_posix()
                            )
                            is_modified = True
                        break
                else:
                    expected_import_path = Path(
                        os.path.relpath(import_path, circ_path.parent)
                    )
                    if import_path_str != expected_import_path.as_posix():
                        print(
                            f"Fixing probably bad import {import_path_str} in {circ_path.as_posix()} (should be {expected_import_path.as_posix()})"
                        )
                        data = data.replace(
                            import_path_str, expected_import_path.as_posix()
                        )
                        is_modified = True
            if is_modified:
                with circ_path.open("w", encoding="utf-8") as test_circ:
                    test_circ.write(data)
            break


def run_tests(search_paths, pipelined=False):
    circ_paths = []
    for search_path in search_paths:
        if search_path.is_file() and search_path.suffix == ".circ":
            circ_paths.append(search_path)
            continue
        for circ_path in search_path.rglob("*.circ"):
            circ_paths.append(circ_path)
    circ_paths = sorted(circ_paths)

    has_banned_circuit = False
    for circ_path in proj_dir_path.rglob("cpu/*.circ"):
        fix_circ(circ_path)
        if find_banned(circ_path):
            has_banned_circuit = True
    for circ_path in proj_dir_path.rglob("harnesses/*.circ"):
        fix_circ(circ_path)
    if has_banned_circuit:
        return

    failed_tests = []
    passed_tests = []
    for circ_path in circ_paths:
        if "imm" in str(circ_path):
            update_imm_circ()

        test = TestCase(circ_path)
        did_pass, reason, extra = False, "Unknown test error", None
        try:
            did_pass, reason, extra = test.run(pipelined=pipelined)
        except KeyboardInterrupt:
            raise
        except SystemExit:
            raise
        except:
            traceback.print_exc()
        if did_pass:
            print(f"PASS: {test.id}", flush=True)
            passed_tests.append(test)
        else:
            print(f"FAIL: {test.id} ({reason})", flush=True)
            failed_tests.append(test)
        if extra:
            print(textwrap.indent(extra, "  "), flush=True)

    print(
        f"Passed {len(passed_tests)}/{len(failed_tests) + len(passed_tests)} tests",
        flush=True,
    )


def check_hash(path):
    rel_path = path.resolve().relative_to(proj_dir_path.resolve())
    rel_path_str = rel_path.as_posix()
    if rel_path_str not in starter_file_hashes:
        return (True, f"Starter does not have hash for {rel_path_str}")
    with path.open("rb") as f:
        contents = f.read()
    contents = contents.replace(b"\r\n", b"\n")
    hashed_val = hashlib.md5(contents).hexdigest()
    if hashed_val != starter_file_hashes[rel_path_str]:
        return (False, f"{rel_path_str} was changed from starter")
    return (True, f"{rel_path_str} matches starter file")


def kill_proc(proc):
    if proc.poll() is None:
        proc.terminate()
        for _ in range(10):
            if proc.poll() is not None:
                return
            time.sleep(0.1)
    if proc.poll() is None:
        proc.kill()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run Logisim tests")
    parser.add_argument(
        "test_path",
        help="Path to a test circuit, or a directory containing test circuits",
        type=Path,
        nargs="+",
    )
    parser.add_argument(
        "-p",
        "--pipelined",
        help="Check against reference output for 2-stage pipeline (when applicable)",
        action="store_true",
        default=False,
    )
    args = parser.parse_args()

    run_tests(args.test_path, args.pipelined)
