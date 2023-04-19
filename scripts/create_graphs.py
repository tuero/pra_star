# create_graphs.py
# Create and serialize the graphs for later use

import os
import subprocess
from multiprocessing import Pool, cpu_count

import tqdm

SCRIPT_PATH = os.path.dirname(os.path.abspath(__file__))
ROOT_PATH = os.path.dirname(SCRIPT_PATH)
EXE_PATH = os.path.join(ROOT_PATH, "build", "Release", "src", "create_graphs")
LOG_PATH = os.path.join(ROOT_PATH, "logs", "create_maps")
MAP_BASE_PATH = os.path.join(ROOT_PATH, "scenarios")


def create_graph(map_path: str):
    log_file_name = (
        os.path.relpath(map_path, MAP_BASE_PATH)[:-4].replace("/", "_") + ".txt"
    )
    log_full_path = os.path.join(LOG_PATH, log_file_name)
    result = subprocess.run(
        [
            "/usr/bin/time",
            "--verbose",
            EXE_PATH,
            "--map_path",
            map_path,
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )

    stdout = result.stdout.decode("utf-8")
    time_output = result.stderr.decode("utf-8")
    with open(log_full_path, "w") as output_file:
        output_file.write(stdout)
        output_file.write(time_output)


def main():
    # Get all map file paths
    maps = []
    for dirpath, _, filenames in os.walk(MAP_BASE_PATH):
        for filename in filenames:
            if filename[-4:] == ".map":
                maps.append(os.path.join(dirpath, filename))

    if not os.path.exists(LOG_PATH):
        os.makedirs(LOG_PATH)

    # Create maps in thread pool
    num_threads = cpu_count()
    with Pool(num_threads) as p:
        for _ in tqdm.tqdm(p.imap_unordered(create_graph, maps), total=len(maps)):
            pass


if __name__ == "__main__":
    main()
