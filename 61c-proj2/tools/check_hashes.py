from pathlib import Path
import hashlib
import sys


proj_dir_path = Path(__file__).parent.parent

starter_file_hashes = {
    "convert.py": "a38bce8aa5e6297812426f843e23023f",
    "framework.py": "9ac4d66cd94b4c2c4b6a147fc4fc430e",
    "src/utils.s": "dde84da8d1f0cd38a5aee55f81049f84",
    "tests/chain-1/batch0-input.bin": "ce7224a6338c437d7a7a76ced7558f4d",
    "tests/chain-1/batch0-m0.bin": "718df00fe5047410dd1c5f40405c1286",
    "tests/chain-1/batch0-m1.bin": "7b5ed6b448c1dfd08383cde788f8c29f",
    "tests/chain-1/batch0-reference.bin": "4b81c535f801a54bbfd9502d204a540e",
    "tests/chain-1/batch1-input.bin": "d9ab23909fbb3229409c5dd9e865e838",
    "tests/chain-1/batch1-m0.bin": "7dcc6e37756fa1cd5ed13a13660fc122",
    "tests/chain-1/batch1-m1.bin": "71ddfbcbf589f68e9361d2da78b3a927",
    "tests/chain-1/batch1-reference.bin": "e14d1e5cb561ea93adb551072b385ab0",
    "tests/chain-1/chain.s": "d0a1d801588c416f2ea8f161ca917bb2",
    "tests/classify-1/input.bin": "37a2283f5c69c342946cad8073b58fca",
    "tests/classify-1/m0.bin": "718df00fe5047410dd1c5f40405c1286",
    "tests/classify-1/m1.bin": "7b5ed6b448c1dfd08383cde788f8c29f",
    "tests/classify-1/reference.bin": "58793807e443612b003668708f92fdbd",
    "tests/classify-2/input.bin": "37a2283f5c69c342946cad8073b58fca",
    "tests/classify-2/m0.bin": "718df00fe5047410dd1c5f40405c1286",
    "tests/classify-2/m1.bin": "7b5ed6b448c1dfd08383cde788f8c29f",
    "tests/classify-2/reference.bin": "58793807e443612b003668708f92fdbd",
    "tests/classify-3/input.bin": "893dc01bc4629189cd161563442c3d99",
    "tests/classify-3/m0.bin": "d863e92ae209896813122cc37b868c2e",
    "tests/classify-3/m1.bin": "ed1972db0da36cc7610f84d4544edc8f",
    "tests/classify-3/reference.bin": "0033be7f707097087fdac5e262bff95c",
    "tests/read-matrix-1/input.bin": "718df00fe5047410dd1c5f40405c1286",
    "tests/read-matrix-2/input.bin": "ce7224a6338c437d7a7a76ced7558f4d",
    "tests/read-matrix-3/input.bin": "d9ab23909fbb3229409c5dd9e865e838",
    "tests/write-matrix-1/reference.bin": "718df00fe5047410dd1c5f40405c1286",
}


def check_hash(rel_path_str):
    if rel_path_str not in starter_file_hashes:
        return (True, f"Starter does not have hash for {rel_path_str}")
    path = proj_dir_path / rel_path_str
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
