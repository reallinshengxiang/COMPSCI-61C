import csv
import json
from pathlib import Path
import re

IMM_SEL_ROM_REGEX = re.compile(r"addr/data: 8 3\n(.*)?\n")
proj_dir_path = Path(__file__).parent.parent


def get_encoding_path():
    return proj_dir_path / "tests" / "unit-imm-gen" / "imm-gen-encoding.csv"


def get_imm_test_path(file_name):
    return proj_dir_path / "tests" / "unit-imm-gen" / file_name


def get_imm_ref_path(file_name):
    return proj_dir_path / "tests" / "unit-imm-gen" / "out" / file_name


def decimal_to_binary(n):
    # return bin(int(n)).replace("0b", "")
    return format(int(n), "03b")


def update_imm_circ():
    student_encoding = {}
    imm_sel_tests = {}

    with get_encoding_path().open(
        "r", encoding="utf-8", errors="ignore"
    ) as encoding_file:
        student_encoding = {
            row["Format"]: row["Code"] for row in csv.DictReader(encoding_file)
        }

    with get_imm_test_path("imm-sel-tests.json").open(
        "r", encoding="utf-8", errors="ignore"
    ) as imm_sel_file:
        imm_sel_tests = json.load(imm_sel_file)

    for test_file in imm_sel_tests:
        encoded_test = [student_encoding[fmt] for fmt in imm_sel_tests[test_file]]

        # update circ file
        with get_imm_test_path(f"{test_file}.circ").open("r") as f:
            circ_contents = f.read()

        rom_match = IMM_SEL_ROM_REGEX.search(circ_contents)
        # weird thing because Logisim does not save trailing 0s in ROM
        rom_content = re.sub(r"( 0)+$", "", " ".join(encoded_test))
        circ_contents = (
            circ_contents[: rom_match.start(1)]
            + rom_content
            + circ_contents[rom_match.end(1) :]
        )

        with get_imm_test_path(f"{test_file}.circ").open("w") as f:
            f.write(circ_contents)

        # update reference file
        new_records = []
        with get_imm_ref_path(f"{test_file}.ref").open(
            "r", encoding="utf-8", errors="ignore"
        ) as imm_ref_file:
            encoded_binary_test = [
                decimal_to_binary(fmt_num) for fmt_num in encoded_test
            ]
            imm_ref_reader = csv.DictReader(imm_ref_file)
            i = 0
            for record in imm_ref_reader:
                if i >= len(encoded_binary_test):
                    break
                record["ImmSel"] = encoded_binary_test[i]
                new_records.append([tpl[1] for tpl in record.items()])
                i += 1

        with get_imm_ref_path(f"{test_file}.ref").open(
            "w", encoding="utf-8", errors="ignore"
        ) as imm_ref_file:
            fields = ["Test", "Immediate", "Instruction", "ImmSel"]
            imm_ref_file.write(",".join(fields) + "\n")
            for record in new_records:
                imm_ref_file.write(",".join(record) + "\n")
