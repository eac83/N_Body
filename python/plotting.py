"""
Plot Sun, Earth, Mars, Jupiter system and combine into a gif.
"""

from math import sin, cos, pi

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import imageio.v2 as im

from star import Star
from constants import AU, DISTANCE_MARS, DISTANCE_JUPITER
from utils import get_number_snapshots

SNAPSHOTS_DIR = "../snapshots"
NUM_SNAPSHOTS = get_number_snapshots(SNAPSHOTS_DIR)
COLORS = ["orange", "blue", "red", "orange"]

# Read through snapshots
for snapshot in range(0, NUM_SNAPSHOTS):
    print("-"*20)
    print(f"Snapshot {snapshot}")
    print(f"Reading snapshot")
    df = pd.read_csv(f"../snapshots/snapshot{snapshot}.csv")
    
    print(f"Creating stars")
    stars = [Star(row) for _, row in df.iterrows()]

    print(f"Plotting stars")
    fg, ax = plt.subplots()

    # Plot ideal circular orbit for each planet
    circle = plt.Circle((0, 0), radius=AU, edgecolor="black", facecolor="none")
    circle2 = plt.Circle((0, 0), radius=DISTANCE_MARS, edgecolor="black", facecolor="none")
    circle3 = plt.Circle((0, 0), radius=DISTANCE_JUPITER, edgecolor="black", facecolor="none")
    ax.add_artist(circle)
    ax.add_artist(circle2)
    ax.add_artist(circle3)

    # Plot each star
    for i, star in enumerate(stars):
        ax.plot(star.position[0], star.position[1], c=COLORS[i], marker="o", markersize=5)
    
    ax.set_aspect("equal")
    ax.set_xlim(-1.2*DISTANCE_JUPITER, 1.2*DISTANCE_JUPITER)
    ax.set_ylim(-1.2*DISTANCE_JUPITER, 1.2*DISTANCE_JUPITER)
    ax.set_title(f"Snapshot: {snapshot}")

    print("Saving plot")
    fg.savefig(f"../plots/snapshot{snapshot}.png")

    print("Closing plot.")
    plt.close()

# Compile plots into gif
images = []
for snapshot in range(0, NUM_SNAPSHOTS):
    images.append(im.imread(f"../plots/snapshot{snapshot}.png"))
im.mimsave("../plots/plots.gif", images, fps=30)

