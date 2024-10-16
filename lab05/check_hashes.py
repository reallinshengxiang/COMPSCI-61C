from pathlib import Path
import hashlib
import sys


lab_dir_path = Path(__file__).parent

starter_file_hashes = {
    "tests/ex2-test.circ": "c8132dd333192b2f21d972b95d0081ba",
    "tests/ex3-test.circ": "0ad02b354c5af8e3ef9095402cc0569b",
    "tests/ex4-test.circ": "367a76692971ee0f76d853f595ccc0eb",
    "tests/out/ex2-test.ref": "79a531baaf1d6f2bc8ea15326cf3b4b7",
    "tests/out/ex3-test.ref": "a672e4541824677d50216f3d36724f7f",
    "tests/out/ex4-test.ref": "d8e069b3b4d479c2cb9967206dfd9f06",
}


def check_hash(rel_path_str):
    if rel_path_str not in starter_file_hashes:
        return (True, f"Starter does not have hash for {rel_path_str}")
    path = lab_dir_path / rel_path_str
    with path.open("rb") as f:
        contents = f.read()
    contents = contents.replace(b"\r\n", b"\n")
    hashed_val = hashlib.md5(contents).hexdigest()
    if hashed_val != starter_file_hashes[rel_path_str]:
        return (False, f"{rel_path_str} was changed from starter")
    return (True, f"{rel_path_str} matches starter file")


def check_hashes():
    passed_all = True
    for rel_path_str in starter_file_hashes.keys():
        passed, reason = check_hash(rel_path_str)
        if not passed:
            passed_all = False
            print(f"Error: {reason}", file=sys.stderr)
    if not passed_all:
        sys.exit(1)
