from pathlib import Path
import hashlib
import sys


proj_dir_path = Path(__file__).parent.parent

starter_file_hashes = {
    "Makefile": "a8acfd1391a16d08115e322bc9d6bdcf",
    "src/asserts.c": "de6b1c338a4155f4af5badf68f6885a3",
    "src/asserts.h": "c0dd70b272b965272daf0d274126f9af",
    "src/snake_utils.c": "141228e5a6828da7b34b2a53797bab9d",
    "src/snake_utils.h": "afd91a278c87e5df3f15dc2baffb3abf",
    "src/state.h": "694e33309de6e0b26596074643b62170",
    "src/unit_tests.c": "1c0e425cc80a2956206aee37328c7492",
    "tests/01-simple-in.snk": "9300d4767e059a3056a9bf4bd5455b20",
    "tests/01-simple-ref.snk": "7d1aa30890d6d7f38b4c8fe864680532",
    "tests/02-direction-in.snk": "6d3d008aae31e41adf17c715564a5c5f",
    "tests/02-direction-ref.snk": "08aee417af99bb6136def17a2bd0c914",
    "tests/03-tail-in.snk": "8be00656f21d4671ee451ccb5df7df41",
    "tests/03-tail-ref.snk": "39693c6555691e6500ff8646d7ec7086",
    "tests/04-food-in.snk": "b236cd1818fdb6b049061f7748e87bc4",
    "tests/04-food-ref.snk": "556ea29cc495ca2b8edb05502975ccc7",
    "tests/05-wall-in.snk": "ae995ac0b27e6937fd3d9448a06c1aa0",
    "tests/05-wall-ref.snk": "8cae377fa2345b77f623de82f2f06fbd",
    "tests/06-small-in.snk": "98c8a9f69382dd2158cacf5ead11aa87",
    "tests/06-small-ref.snk": "2fc8189234af65fbda486597f7dd767b",
    "tests/07-medium-in.snk": "ed4fef20e8fe1182d43dbea0bbda7cf2",
    "tests/07-medium-ref.snk": "49cee1475c3d6b5d01278da0d7f12471",
    "tests/08-multisnake-in.snk": "65e25ff843b119c753dedae6833a9b94",
    "tests/08-multisnake-ref.snk": "4b90bdb046d4df04d0f1daf4fb5ca35a",
    "tests/09-everything-in.snk": "0a83f0b008bd5d21c3aaa51ee5ccc7e2",
    "tests/09-everything-ref.snk": "bea3e40f65c36efb26f40640e64fb977",
    "tests/10-filled-in.snk": "5e653dc1f54af62de43a2e09a2e10e92",
    "tests/10-filled-ref.snk": "d3ab39e78d19db2598ebbd48679fa683",
    "tests/11-manyclose-in.snk": "4849186f4b6c58f4f258328bfec1d08a",
    "tests/11-manyclose-ref.snk": "8e840cea5f73ba8beca0c3ce1f583ce9",
    "tests/12-corner-in.snk": "21bb8673e2b7e1133ca242921ea29958",
    "tests/12-corner-ref.snk": "74e2673c3c39cefaf67dc337e243edac",
    "tests/13-sus-in.snk": "9911f05dad20f1e9192a5bd6e8d24ad7",
    "tests/13-sus-ref.snk": "9911f05dad20f1e9192a5bd6e8d24ad7",
    "tests/14-orochi-in.snk": "5a089a599db6f98ef1f5c6dc1017bca2",
    "tests/14-orochi-ref.snk": "8c0593b325d315e96628214441b9db14",
    "tests/15-hydra-in.snk": "665783b4d161546f8351cd42db89fd4e",
    "tests/15-hydra-ref.snk": "764211dbba8c731ba258b5d0918a6454",
    "tests/16-huge-in.snk": "b86a77d7348b0d33ac3144b42dd3c52e",
    "tests/16-huge-ref.snk": "413f750dd777172aad6b6e88c06fa015",
    "tests/17-wide-in.snk": "5e0a8abd91dd106c79a706ba115f457e",
    "tests/17-wide-ref.snk": "ee78b7b2b6e040762f3d8717678d7b90",
    "tests/18-tall-in.snk": "21e33117457ea25568e26b6a793bddb8",
    "tests/18-tall-ref.snk": "1b4f849831c4623595471c3f6bb93a7a",
    "tests/19-101-127-in.snk": "f5d73f7dcc8c323376dbd71416fe1693",
    "tests/19-101-127-ref.snk": "f5d73f7dcc8c323376dbd71416fe1693",
    "tests/20-long-line-in.snk": "2c39d3df540b9ecbabca82191ade9222",
    "tests/20-long-line-ref.snk": "2c39d3df540b9ecbabca82191ade9222",
    "tests/21-bigL-in.snk": "8184091120d1c5460f11197e65ce0ddf",
    "tests/21-bigL-ref.snk": "ee4cba65687f2563ef7e90181239ddf2",
    "tools/test_nonexistent_input_file.sh": "1caf6fb696559a8ade6823ee447b51fa",
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

passed_all = True
for rel_path_str in starter_file_hashes.keys():
    passed, reason = check_hash(rel_path_str)
    if not passed:
        passed_all = False
        print(f"Error: {reason}", file=sys.stderr)
if not passed_all:
    sys.exit(1)
