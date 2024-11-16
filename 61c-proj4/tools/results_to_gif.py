import argparse
import json

import framework


def results_to_gif(test_name: str):
    test_dir = framework.project_dir / test_name

    gifs_json_path = test_dir / "gifs.json"
    if not gifs_json_path.exists():
        print("Unable to find gifs.json in test directory")
        return

    try:
        with gifs_json_path.open("r") as f:
            gifs_json = json.load(f)
    except Exception:
        print("Unable to read gifs.json in test directory")
        return

    for (i, gif_tasks) in enumerate(gifs_json):
        frames = []

        # Read output matrices
        for (task, duration) in gif_tasks:
            output_file = test_dir / task / "out.bin"
            if not output_file.exists():
                raise FileNotFoundError(f"No output file for {task}")
            frames.append(framework.GIFFrame(
                duration, framework.Matrix.from_path(output_file)))

        # Generate output gif
        output_file = test_dir / f"out{i}.gif"
        framework.frames_to_gif(frames, str(output_file))
        print(f"Output gif saved to {output_file}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog='results_to_gif',
        description='Converting result bins to gifs')
    parser.add_argument('test_path', type=str, help='test folder path')
    results_to_gif(parser.parse_args().test_path)
