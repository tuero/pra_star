# run_algorithms.py
# Run all the search algorithms

import os
import subprocess
from multiprocessing import Pool, cpu_count

import tqdm

SCRIPT_PATH = os.path.dirname(os.path.abspath(__file__))
ROOT_PATH = os.path.dirname(SCRIPT_PATH)
EXE_PATH = os.path.join(ROOT_PATH, "build", "Release", "src", "run_multi")
LOG_PATH = os.path.join(ROOT_PATH, "logs", "run_algorithms")
EXPERIMENT_PATH = os.path.join(ROOT_PATH, "experiments")
MAP_BASE_PATH = os.path.join(ROOT_PATH, "scenarios")

SCENARIO_FILE_EXTENSION = ".map.scen"
ALGORITHMS = ["astar", "pra"]
K_PARAMS = [0, 2, 4, 8, 16]


def run_scenario(scenario_path: str, algorithm: str, k: int):
    file_name = os.path.relpath(scenario_path, MAP_BASE_PATH)[
        : -len(SCENARIO_FILE_EXTENSION)
    ].replace("/", "_")
    log_full_path = os.path.join(LOG_PATH, algorithm, file_name) + "_{}.txt".format(k)
    experiment_full_path = os.path.join(
        EXPERIMENT_PATH, algorithm, file_name
    ) + "_{}.csv".format(k)

    result = subprocess.run(
        [
            "/usr/bin/time",
            "--verbose",
            EXE_PATH,
            "--scenario_path",
            scenario_path,
            "--export_path",
            experiment_full_path,
            "--algorithm",
            algorithm,
            "--k",
            str(k),
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )

    stdout = result.stdout.decode("utf-8")
    time_output = result.stderr.decode("utf-8")
    with open(log_full_path, "w") as output_file:
        output_file.write(stdout)
        output_file.write(time_output)


def run_all_scenarios(scenario_path: str):
    # Run A*
    run_scenario(scenario_path, "astar", 0)

    # Run PRA*
    for k in K_PARAMS:
        run_scenario(scenario_path, "pra", k)


def main():
    # Get all map scenario file paths
    scenario_files = []
    for dirpath, _, filenames in os.walk(MAP_BASE_PATH):
        for filename in filenames:
            if filename[-len(SCENARIO_FILE_EXTENSION) :] == SCENARIO_FILE_EXTENSION:
                scenario_files.append(os.path.join(dirpath, filename))

    if not os.path.exists(LOG_PATH):
        os.makedirs(LOG_PATH)

    for algorithm in ALGORITHMS:
        if not os.path.exists(os.path.join(LOG_PATH, algorithm)):
            os.makedirs(os.path.join(LOG_PATH, algorithm))

    # Create maps in thread pool
    num_threads = cpu_count()
    with Pool(num_threads) as p:
        for _ in tqdm.tqdm(
            p.imap_unordered(run_all_scenarios, scenario_files),
            total=len(scenario_files),
        ):
            pass


if __name__ == "__main__":
    main()
