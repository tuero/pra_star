# generate_results_figure.py
# Generate the tables/figures

import argparse
import os

import matplotlib.pyplot as plt
import matplotlib.style as style
import numpy as np
import pandas as pd
import seaborn as sns

style.use("seaborn-poster")  # sets the size of the charts
style.use("ggplot")
sns.set_context(
    "paper",
    rc={
        "axes.labelsize": 24,
        "xtick.labelsize": 20,
        "ytick.labelsize": 20,
        "legend.fontsize": 20,
    },
)

SCRIPT_PATH = os.path.dirname(os.path.abspath(__file__))
ROOT_PATH = os.path.dirname(SCRIPT_PATH)
EXPERIMENT_PATH = os.path.join(ROOT_PATH, "experiments")
OUTPUT_PATH = os.path.join(ROOT_PATH, "latex", "assets")


def load_dfs() -> pd.DataFrame:
    dfs = []
    astar_bucket_times = dict()
    astar_bucket_expanded = dict()

    # Load A* results
    path = os.path.join(EXPERIMENT_PATH, "astar")
    for filename in os.listdir(path):
        df = pd.read_csv(os.path.join(path, filename))
        df["algorithm"] = "A*"
        df["k"] = 0
        df["cost_ratio"] = df.apply(
            lambda row: row.solution_cost / row.optimal_cost
            if row.optimal_cost > 0
            else 1,
            axis=1,
        )
        df["bucket"] = df.apply(lambda row: int(row.optimal_cost / 4), axis=1)
        df["duration_ratio"] = 1.0
        df["expanded_ratio"] = 1.0
        df["first_move_duration_ratio"] = 1.0
        df = df.drop(df[df.bucket >= 128].index)
        dfs.append(df)
        for bucket, duration, expanded in zip(
            df["bucket"].to_list(), df["duration"].to_list(), df["expanded"].to_list()
        ):
            if bucket not in astar_bucket_times:
                astar_bucket_times[bucket] = []
                astar_bucket_expanded[bucket] = []
            astar_bucket_times[bucket].append(duration)
            astar_bucket_expanded[bucket].append(expanded)

    path = os.path.join(EXPERIMENT_PATH, "pra")
    for filename in os.listdir(path):
        k = int(filename.split("_")[-1][:-4])
        df = pd.read_csv(os.path.join(path, filename))
        df["algorithm"] = "PRA*({})".format(k if k > 0 else "inf")
        df["k"] = k
        df["cost_ratio"] = df.apply(
            lambda row: row.solution_cost / row.optimal_cost
            if row.optimal_cost > 0
            else 1,
            axis=1,
        )
        df["bucket"] = df.apply(lambda row: int(row.optimal_cost / 4), axis=1)
        df = df.drop(df[df.bucket >= 128].index)
        df["duration_ratio"] = df.apply(
            lambda row: np.mean(astar_bucket_times[row.bucket]) / row.duration, axis=1
        )
        df["expanded_ratio"] = df.apply(
            lambda row: np.mean(astar_bucket_expanded[row.bucket]) / row.expanded,
            axis=1,
        )
        df["first_move_duration_ratio"] = df.apply(
            lambda row: np.mean(astar_bucket_times[row.bucket])
            / row.first_move_duration,
            axis=1,
        )
        dfs.append(df)

    # Concat into a single table
    df = pd.concat(dfs)
    return df


def create_figures_all(df: pd.DataFrame):
    df = df.rename(
        columns={
            "bucket": "A* path length / 4",
            "duration": "Time (s)",
            "cost_ratio": "PRA*(k) / A* path length",
            "optimal_cost": "A* path length",
            "duration_ratio": "PRA*(k) speed-up over A*",
            "first_move_duration": "Time (s) to issue first move",
            "first_move_duration_ratio": "PRA*(k) speed-up over A* first move",
            "expanded": "Expanded",
            "expanded_ratio": "PRA*(k) speed-up over A* expansions",
        }
    )
    df_pra = df[df["algorithm"] != "A*"]

    # PRA*(k) path cost ratio
    plt.clf()
    sns_plot_cost_ratio = sns.lineplot(
        data=df_pra,
        x="A* path length / 4",
        y="PRA*(k) / A* path length",
        hue="algorithm",
        n_boot=10,
        linewidth=2,
    )
    handles, labels = plt.gca().get_legend_handles_labels()
    order = [1, 2, 4, 0, 3]
    plt.legend(
        [handles[idx] for idx in order], [labels[idx] for idx in order], title=None
    )
    sns_plot_cost_ratio.figure.savefig(os.path.join(OUTPUT_PATH, "plot_cost_ratio.png"))

    # PRA*(k) time
    plt.clf()
    sns_plot_duration = sns.lineplot(
        data=df_pra,
        x="A* path length / 4",
        y="Time (s)",
        hue="algorithm",
        n_boot=10,
        linewidth=2,
    )
    handles, labels = plt.gca().get_legend_handles_labels()
    order = [1, 2, 4, 0, 3]
    plt.legend(
        [handles[idx] for idx in order], [labels[idx] for idx in order], title=None
    )
    sns_plot_duration.figure.savefig(os.path.join(OUTPUT_PATH, "plot_duration.png"))

    # PRA*(k) first move time
    plt.clf()
    sns_plot_first_move_duration = sns.lineplot(
        data=df_pra,
        x="A* path length / 4",
        y="Time (s) to issue first move",
        hue="algorithm",
        n_boot=10,
        linewidth=2,
    )
    handles, labels = plt.gca().get_legend_handles_labels()
    order = [1, 2, 4, 0, 3]
    plt.legend(
        [handles[idx] for idx in order], [labels[idx] for idx in order], title=None
    )
    sns_plot_first_move_duration.figure.savefig(
        os.path.join(OUTPUT_PATH, "plot_first_move_duration.png")
    )

    # PRA*(k) duration ratio to A*
    plt.clf()
    sns_plot_duration_ratio = sns.lineplot(
        data=df_pra,
        x="A* path length / 4",
        y="PRA*(k) speed-up over A*",
        hue="algorithm",
        n_boot=10,
        linewidth=2,
    )
    handles, labels = plt.gca().get_legend_handles_labels()
    order = [1, 2, 4, 0, 3]
    plt.legend(
        [handles[idx] for idx in order], [labels[idx] for idx in order], title=None
    )
    sns_plot_duration_ratio.figure.savefig(
        os.path.join(OUTPUT_PATH, "plot_duration_ratio.png")
    )

    # PRA*(k) first move duration ratio to A*
    plt.clf()
    sns_plot_first_move_duration_ratio = sns.lineplot(
        data=df_pra,
        x="A* path length / 4",
        y="PRA*(k) speed-up over A* first move",
        hue="algorithm",
        n_boot=10,
        linewidth=2,
    )
    handles, labels = plt.gca().get_legend_handles_labels()
    order = [1, 2, 4, 0, 3]
    plt.legend(
        [handles[idx] for idx in order], [labels[idx] for idx in order], title=None
    )
    sns_plot_first_move_duration_ratio.figure.savefig(
        os.path.join(OUTPUT_PATH, "plot_first_move_duration_ratio.png")
    )

    plt.clf()
    sns_plot_expanded_ratio = sns.lineplot(
        data=df_pra,
        x="A* path length / 4",
        y="PRA*(k) speed-up over A* expansions",
        hue="algorithm",
        n_boot=10,
        linewidth=2,
    )
    handles, labels = plt.gca().get_legend_handles_labels()
    order = [1, 2, 4, 0, 3]
    plt.legend(
        [handles[idx] for idx in order], [labels[idx] for idx in order], title=None
    )
    sns_plot_expanded_ratio.figure.savefig(
        os.path.join(OUTPUT_PATH, "plot_expanded_ratio.png")
    )


def create_figures_combined(df: pd.DataFrame):
    df = df.rename(
        columns={
            "bucket": "A* path length / 4",
            "duration": "Time (s)",
            "cost_ratio": "PRA*(k) / A* path length",
            "optimal_cost": "A* path length",
            "duration_ratio": "PRA*(k) speed-up over A*",
            "first_move_duration": "Time (s) to issue first move",
            "first_move_duration_ratio": "PRA*(k) speed-up over A* first move",
            "expanded": "Expanded",
            "expanded_ratio": "PRA*(k) speed-up over A* expansions",
        }
    )
    df_pra = df[df["algorithm"] != "A*"]

    plt.clf()
    fig, ax = plt.subplots(1, 3, figsize=(30, 8))

    sns_plot_cost_ratio = sns.lineplot(
        data=df_pra,
        x="A* path length / 4",
        y="PRA*(k) / A* path length",
        hue="algorithm",
        n_boot=10,
        linewidth=2,
        ax=ax[0],
    )
    handles, labels = ax[0].get_legend_handles_labels()
    order = [1, 2, 4, 0, 3]
    ax[0].legend(
        [handles[idx] for idx in order], [labels[idx] for idx in order], title=None
    )

    sns_plot_duration_ratio = sns.lineplot(
        data=df_pra,
        x="A* path length / 4",
        y="PRA*(k) speed-up over A*",
        hue="algorithm",
        n_boot=10,
        linewidth=2,
        ax=ax[1],
    )
    handles, labels = ax[1].get_legend_handles_labels()
    order = [1, 2, 4, 0, 3]
    ax[1].legend(
        [handles[idx] for idx in order], [labels[idx] for idx in order], title=None
    )

    sns_plot_first_move_duration_ratio = sns.lineplot(
        data=df_pra,
        x="A* path length / 4",
        y="PRA*(k) speed-up over A* first move",
        hue="algorithm",
        n_boot=10,
        linewidth=2,
        ax=ax[2],
    )
    handles, labels = ax[2].get_legend_handles_labels()
    order = [1, 2, 4, 0, 3]
    ax[2].legend(
        [handles[idx] for idx in order], [labels[idx] for idx in order], title=None
    )

    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_PATH, "plot_combined.png"))


def main():
    df = load_dfs()
    create_figures_all(df)
    create_figures_combined(df)


if __name__ == "__main__":
    main()
