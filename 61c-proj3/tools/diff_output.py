from pathlib import Path
import argparse
from contextlib import redirect_stdout
from io import StringIO

from format_output import format_output


def diff_output(circ_path, pipelined):
    if not circ_path.is_file() or circ_path.suffix != ".circ":
        print(f"Error: {str(circ_path)} is not a Logisim circuit file, skipping")
        return
    circ_slug = circ_path.stem
    circ_dir = circ_path.parent

    ref_dir = circ_dir / "out"
    if pipelined:
        ref_file = ref_dir / f"{circ_slug}.piperef"
    else:
        ref_file = ref_dir / f"{circ_slug}.ref"
    if not ref_dir.exists() or not ref_file.exists():
        print(f"Error: cannot find the reference output for circuit")
        return

    student_dir = circ_dir / "out"
    student_file = student_dir / f"{circ_slug}.out"
    if not student_dir.exists() or not student_file.exists():
        print(f"Error: cannot find the student output for circuit")
        return

    s = StringIO()
    with redirect_stdout(s):
        format_output(ref_file)
    s.flush()
    s.seek(0)
    ref_output = s.readlines()

    s = StringIO()
    with redirect_stdout(s):
        format_output(student_file)
    s.flush()
    s.seek(0)
    student_output = s.readlines()

    if ref_output[0] != student_output[0]:
        print(f"Error: files have different headers")
        print(f"Reference: {ref_output[0].strip()}")
        print(f"Student:   {student_output[0].strip()}")

    # Print header with space before
    header_str = "           " + ref_output[0].strip()

    ref_output.pop(0)
    student_output.pop(0)

    output = []

    while ref_output and student_output:
        ref_line = ref_output.pop(0).strip()
        student_line = student_output.pop(0).strip()

        if ref_line == student_line:
            continue
        output.append(f"Reference: {ref_line}")
        output.append(f"Student:   {student_line}")
        output.append("---")
    if output:
        output.insert(0, header_str)

    if ref_output:
        output.append("The following lines are in reference output only")
        output.append(header_str)
        for line in ref_output:
            output.append(f"Reference: {line.strip()}")
        output.append("---")
    # elif student_output:
    #     output.append("The following lines are in student output only")
    #     output.append(header_str)
    #     for line in student_output:
    #         output.append(f"Student:   {line.strip()}")
    #     output.append("---")

    if not output:
        print("The student file is identical to the reference file")
        return True
    else:
        print("\n".join(output))
        return False


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Diff Logisim test output using circuit file"
    )
    parser.add_argument("circ_path", help="Path to a circuit file", type=Path)
    parser.add_argument(
        "-p",
        "--pipelined",
        help="Check against reference output for 2-stage pipeline (when applicable)",
        action="store_true",
        default=False,
    )
    args = parser.parse_args()

    diff_output(args.circ_path, args.pipelined)
