from pathlib import Path
import argparse
import csv
import math


def masked_mem_data_formatter(val, row):
    write_mask = row["MemWriteMask"]
    mask = write_mask[0] * 8 + write_mask[1] * 8 + write_mask[2] * 8 + write_mask[3] * 8
    masked_val = "".join(["-" if mask[i] == "0" else val[i] for i in range(len(val))])
    return bin2hex_formatter(masked_val, row)


def masked_imm_gen_shift_formatter(val, row):
    masked_val = "".join(["0"] * 27) + val[-5:]
    return bin2hex_formatter(masked_val, row)


def bin2hex_formatter(val, _):
    return bin2hex(val).rjust(math.ceil(len(val) / 4), "0")


known_formats = {
    "tests/unit-alu/out/*": [
        ["Test", "Time"],
        ["ALUSel", "ALUSel"],
        ["InputA", "A"],
        ["InputB", "B"],
        ["ALUResult", "ALUResult"],
    ],
    "tests/unit-regfile/out/*": [
        ["Test", "Time"],
        ["WriteIndex", "WriteIdx"],
        ["ReadIndex1", "ReadIdx1"],
        ["ReadIndex2", "ReadIdx2"],
        ["RegWEn", "RegWEn"],
        ["WriteData", "WriteData"],
        ["ra", "ra (x1)"],
        ["sp", "sp (x2)"],
        ["t0", "t0 (x5)"],
        ["t1", "t1 (x6)"],
        ["t2", "t2 (x7)"],
        ["s0", "s0 (x8)"],
        ["s1", "s1 (x9)"],
        ["a0", "a0 (x10)"],
        ["ReadData1", "ReadData1"],
        ["ReadData2", "ReadData2"],
    ],
    "tests/unit-branch-comp/out/*": [
        ["Test", "Time"],
        ["BrData1", "BrData1"],
        ["BrData2", "BrData2"],
        ["BrUn", "BrUn"],
        ["BrEq", "BrEq"],
        ["BrLt", "BrLt"],
    ],
    "tests/unit-imm-gen/out/imm-gen-i-type-shift*": [
        ["Test", "Time"],
        ["Instruction", "Instruc."],
        ["ImmSel", "ImmSel"],
        ["Immediate", "Immediate", masked_imm_gen_shift_formatter],
    ],
    "tests/unit-imm-gen/out/*": [
        ["Test", "Time"],
        ["Instruction", "Instruc."],
        ["ImmSel", "ImmSel"],
        ["Immediate", "Immediate"],
    ],
    "tests/unit-partial-load/out/*": [
        ["Test", "Time"],
        ["Instruction", "Instruc."],
        ["MemAddress", "MemAddress"],
        ["DataFromMem", "DataFromMem"],
        ["DataToReg", "DataToReg"],
    ],
    "tests/unit-partial-store/out/*": [
        ["Test", "Time"],
        ["Instruction", "Instruc."],
        ["MemAddress", "MemAddress"],
        ["MemWEn", "MemWEn"],
        ["DataFromReg", "DataFromReg"],
        ["DataToMem", "DataToMem", masked_mem_data_formatter],
        ["MemWriteMask", "MemWriteMask"],
    ],
    "tests/integration-*/out/*": [
        ["TimeStep", "Time"],
        ["RequestedAddress", "PC"],
        ["RequestedInstruction", "Instruc."],
        ["ra", "ra (x1)"],
        ["sp", "sp (x2)"],
        ["t0", "t0 (x5)"],
        ["t1", "t1 (x6)"],
        ["t2", "t2 (x7)"],
        ["s0", "s0 (x8)"],
        ["s1", "s1 (x9)"],
        ["a0", "a0 (x10)"],
    ],
}


def bin2hex(bin_str):
    hex_str = ""
    num_groups = math.ceil(len(bin_str) / 4)
    bin_str = ("0" * (num_groups * 4 - len(bin_str))) + bin_str
    for i in range(num_groups):
        group = bin_str[i * 4 : (i + 1) * 4]
        if "E" in group:
            hex_str += "X"
        elif "U" in group:
            hex_str += "U"
        elif "-" in group:
            hex_str += "-"
        else:
            hex_str += format(int(group, 2), "x")
    return hex_str


def print_line(arr, lengths, remap=None):
    arr_len = len(arr)
    if remap is not None and len(remap) < arr_len:
        arr_len = len(remap)
    for i in range(arr_len):
        suffix = "\n" if i == arr_len - 1 else " "
        if remap:
            i = remap[i]
        cell = arr[i].ljust(lengths[i])
        print(f"{cell}", end=suffix)


def format_line(arr, formatters=None, header_line=None):
    arr_map = None
    if header_line:
        arr_map = dict(zip(header_line, arr))
    return [
        (formatters and formatters[i] or bin2hex_formatter)(bin_str, arr_map)
        for i, bin_str in enumerate(arr)
    ]


def format_output(output_path):
    with output_path.open("r", encoding="utf-8", errors="ignore") as output_file:
        output_csv = csv.reader(output_file)
        header_line = next(output_csv, None)
        if header_line is None:
            print(f"{str(output_path)} is empty")
            return
        output_line = next(output_csv, None)
        if output_line is None:
            print(f"{str(output_path)} has no rows")
            return
        column_remap = list(range(len(header_line)))
        formatters = [None] * len(header_line)
        for glob, column_order in known_formats.items():
            if output_path.match(glob):
                column_remap = [
                    header_line.index(cur_name) for cur_name, *_ in column_order
                ]
                for (_, new_name, *extra), i in zip(column_order, column_remap):
                    header_line[i] = new_name
                    if extra:
                        formatters[i] = extra[0]
                break
        column_lengths = [
            max(len(h), math.ceil(len(o) / 4)) for h, o in zip(header_line, output_line)
        ]
        print_line(header_line, lengths=column_lengths, remap=column_remap)
        print_line(
            format_line(output_line, formatters=formatters, header_line=header_line),
            lengths=column_lengths,
            remap=column_remap,
        )
        for output_line in output_csv:
            print_line(
                format_line(
                    output_line, formatters=formatters, header_line=header_line
                ),
                lengths=column_lengths,
                remap=column_remap,
            )


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Format Logisim test output")
    parser.add_argument("output_path", help="Path to a test output file", type=Path)
    args = parser.parse_args()

    format_output(args.output_path)
