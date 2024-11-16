from pathlib import Path
from typing import Callable, Dict, List, Optional
import hashlib
import json
import numpy as np
import os
import random
import struct
import subprocess

from PIL import Image
from PIL import ImageOps

project_dir = Path(__file__).parent.resolve().parent
tests_dir = project_dir / "tests"
oracle_path = Path("/home/ff/cs61c/fa24/proj4/convolve_oracle")

all_tests: Dict[str, "TestSpec"] = {}

FILTER_MULTIPLIER = 1e6


def set_tests_dir(path: Path):
    global tests_dir
    tests_dir = path


def run_oracle(a_path: Path, b_path: Path, out_path: Path):
    if not oracle_path.exists():
        raise RuntimeError(
            "Oracle does not exist, please run on the hive machines")
    subprocess.run([oracle_path, a_path, b_path, out_path])


def randint(lower, upper, **kwargs):
    return np.random.randint(lower, upper + 1, **kwargs)


def md5sum(path: Path) -> str:
    with path.open("rb") as f:
        contents = f.read()
    return hashlib.md5(contents).hexdigest()


def gif_to_frames(gif_path: str) -> List["GIFFrame"]:
    gif = Image.open(gif_path)
    gif_frames = []
    for frame in range(gif.n_frames):
        gif.seek(frame)
        matrix = Matrix(gif.size[1], gif.size[0], list(
            ImageOps.grayscale(gif.copy()).getdata()))
        gif_frames.append(GIFFrame(gif.info["duration"] or 10, matrix))
    gif.close()

    return gif_frames


def frames_to_gif(frames: List["GIFFrame"], gif_path: str):
    images = [Image.fromarray((np.array(f.matrix.data).flatten().reshape(
        (f.matrix.rows, f.matrix.cols), ) / FILTER_MULTIPLIER).astype(np.uint8), mode="L") for f in frames]
    durations = [f.duration for f in frames]
    images[0].save(gif_path, save_all=True,
                   append_images=images[1:], loop=0, duration=durations)


class GIFFrame:
    def __init__(self, duration: int, matrix: "Matrix"):
        self.duration = duration
        self.matrix = matrix


class Matrix:
    @staticmethod
    def random(rows: int, cols: int, min_value=-1000, max_value=1000) -> "Matrix":
        values = list(randint(min_value, max_value,
                      size=rows * cols) & 0xFFFFFFFF)
        return Matrix(rows, cols, values)

    @staticmethod
    def from_path(path: Path):
        try:
            with path.open("rb") as f:
                input_bin_contents = f.read()
            rows = struct.unpack("I", input_bin_contents[0:4])[0]
            cols = struct.unpack("I", input_bin_contents[4:8])[0]
            data = struct.unpack("I" * rows * cols, input_bin_contents[8:])

            return Matrix(rows, cols, list(data))
        except Exception as e:
            print("Unexpected error while reading matrix")
            print(e)
            exit(1)

    def __init__(self, rows: int, cols: int, data: List[int]):
        self.rows = rows
        self.cols = cols
        self.data = data

    def generate(self, path: Path):
        with path.open("wb") as f:
            # Write row and column counts
            f.write(struct.pack("I", self.rows))
            f.write(struct.pack("I", self.cols))

            # Write matrix elements as bytes
            f.write(struct.pack("I" * self.rows * self.cols, *self.data))


class Task:
    def __init__(self, a_matrix: Matrix, b_matrix: Matrix):
        self.a_matrix = a_matrix
        self.b_matrix = b_matrix

        assert a_matrix.rows >= b_matrix.rows, "Rows of matrix A must be greater than or equal to the rows in matrix B"
        assert a_matrix.cols >= b_matrix.cols, "Columns of matrix A must be greater than or equal to the columns in matrix B"

    def generate(self, path: Path):
        path.mkdir(exist_ok=True)
        self.a_matrix.generate(path / "a.bin")
        self.b_matrix.generate(path / "b.bin")
        a_md5 = md5sum(path / "a.bin")
        b_md5 = md5sum(path / "b.bin")

        try:
            with (path / ".hashes.json").open("r") as f:
                hashes_json = json.load(f)
                if hashes_json["a.bin"] == a_md5 and hashes_json["b.bin"] == b_md5:
                    return
        except Exception:
            pass

        with (path / ".hashes.json").open("w") as f:
            f.write(json.dumps({"a.bin": a_md5, "b.bin": b_md5}))
        run_oracle(path / "a.bin", path / "b.bin", path / "ref.bin")


class TestSpec:
    def __init__(self, name: str, func: Callable[["TestSpec"], None]):
        self.path = tests_dir / name
        self.func = func
        self._tasks: List[Task] = []
        self._gifs: List[tuple[str, Matrix]] = []

    def add_task(self, task: Task):
        self._tasks.append(task)

    def add_gif(self, file: str, filter: Matrix):
        self._gifs.append((file, filter))

    def generate(self):
        self.func(self)

        tasks = self._tasks[:]
        gifs: List[List[tuple[str, int]]] = []
        for (gif_path, gif_filter) in self._gifs:
            gif_frames = gif_to_frames(gif_path)
            gif_tasks: List[tuple[str, int]] = []
            for frame in gif_frames:
                gif_tasks.append((f"task{len(tasks)}", frame.duration))
                tasks.append(Task(frame.matrix, gif_filter))
            gifs.append(gif_tasks)

        self.path.mkdir(exist_ok=True, parents=True)
        with (self.path / "input.txt").open("w") as f:
            f.write(str(len(tasks)) + "\n")
            for i, task in enumerate(tasks):
                task_path = self.path / f"task{i}"
                task.generate(task_path)
                rel_task_dir = os.path.relpath(task_path, self.path)
                f.write(f"./{rel_task_dir}\n")

        if len(gifs) > 0:
            with (self.path / "gifs.json").open("w") as f:
                json.dump(gifs, f)


def Test(name: Optional[str] = None, seed: int = 42):
    def decorator(func):
        nonlocal name
        name = name or str(func.__name__)

        def _func(test: TestSpec):
            random.seed(seed)
            np.random.seed(seed)
            func(test)

        test = TestSpec(name, _func)
        all_tests[name] = test

    return decorator
