#!/usr/bin/env python3

from math import sqrt, sin, cos, pi
import matplotlib.pyplot as plt
import numpy as np

def plot_riemann(title, f, low, high, partitions=20):
    """
    Uses 20 partitions by default.
    Special thanks to Patrick Walls for an in-depth guide to
    mathematical Python and an elegant way of computing midpoints.
    https://personal.math.ubc.ca/~pwalls/math-python/integration/riemann-sums/
    """
    fig = plt.figure()
    plt.title(title, pad='10')
    plt.xlabel("x", rotation=0) # Orient label upright.
    plt.ylabel("y", rotation=0) # Orient label upright.
    plt.axhline(y=0, color='lightgray', linestyle='dashed') # Origin x-axis.
    plt.axvline(x=0, color='lightgray', linestyle='dashed') # Origin y-axis.

    # Plot the function itself with a million points.
    # This should give enough points to make the line look smooth.
    x_vals = np.linspace(low, high, 100000)
    y_vals = [f(x) for x in x_vals]
    plt.plot(x_vals, y_vals, color="black")

    # Plot the partitions (+1 for 0-based indexing).
    x_vals = np.linspace(low, high, partitions + 1)
    x_boxes = (x_vals[:-1] + x_vals[1:]) / 2
    y_boxes = [f(x) for x in x_boxes]
    width = (high - low) / partitions
    plt.bar(x_boxes, y_boxes, width=width, alpha=0.3, edgecolor="black")

    return fig

def main():
    sinx = lambda x: sin(x**2)
    cosx = lambda x: cos(x**2)
    poly = lambda x: x**3 - 2 * x - 5

    plots = [
        ("sin.pdf", r"$\int_{-\pi}^{\pi}\,\,\sin(x^2)$", sinx, -pi, pi, 34),
        ("cos.pdf", r"$\int_{-\pi}^{\pi}\,\,\cos(x^2)$", cosx, -pi, pi, 50),
        ("poly.pdf", r"$\int_{-100}^{100}\,\, x^3 - 2x - 5$", poly, -100, 100, 50),
    ]

    for pdf, title, f, low, high, partitions in plots:
        fig = plot_riemann(title, f, low, high, partitions)
        fig.savefig(pdf)

if __name__ == "__main__":
    main()
