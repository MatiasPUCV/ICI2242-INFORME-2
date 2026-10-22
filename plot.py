import sys
from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


def SavePlot(figure, path):
    figure.tight_layout()
    figure.savefig(path, dpi=150)
    plt.close(figure)


def PlotBalls(directory):
    data = pd.read_csv(directory / "scale_balls.csv")
    figure, axis = plt.subplots()
    axis.plot(data["ballCount"], data["medianMilliseconds"], marker="o")
    axis.set_xlabel("N (cantidad de bolas)")
    axis.set_ylabel("Tiempo (ms)")
    axis.set_title("Tiempo según la cantidad de bolas (q = 10)")
    axis.grid(True)
    SavePlot(figure, directory / "scale_balls.png")


def PlotQueries(directory):
    data = pd.read_csv(directory / "scale_queries.csv")
    figure, axis = plt.subplots()
    axis.plot(data["queryCount"], data["medianMilliseconds"], marker="o")
    axis.set_xlabel("q (cantidad de consultas)")
    axis.set_ylabel("Tiempo (ms)")
    axis.set_title("Tiempo según la cantidad de consultas (N = 20000)")
    axis.grid(True)
    SavePlot(figure, directory / "scale_queries.png")


def PlotComparison(directory):
    data = pd.read_csv(directory / "compare.csv")
    figure, axis = plt.subplots()
    axis.plot(data["ballCount"], data["optimizedMilliseconds"], marker="o", label="Optimizado O(N)")
    axis.plot(data["ballCount"], data["referenceMilliseconds"], marker="s", label="Referencia O(N²)")
    axis.set_xlabel("N (cantidad de bolas)")
    axis.set_ylabel("Tiempo (ms)")
    axis.set_title("Algoritmo optimizado vs referencia (q = 1)")
    axis.legend()
    axis.grid(True)
    SavePlot(figure, directory / "compare.png")


def Main():
    directory = Path(sys.argv[1] if len(sys.argv) > 1 else "results")
    PlotBalls(directory)
    PlotQueries(directory)
    PlotComparison(directory)


Main()
