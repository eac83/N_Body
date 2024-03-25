"""
Plot Sun, Earth, Mars, Jupiter system and combine into a gif.
"""

from math import sin, cos, pi

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter
from tqdm import tqdm

from star import Star
from constants import AU, DISTANCE_MARS, DISTANCE_JUPITER
from utils import get_number_snapshots

SNAPSHOTS_DIR = "snapshots"
NUM_SNAPSHOTS = get_number_snapshots(SNAPSHOTS_DIR)
COLORS = ["orange", "blue", "red", "orange"]

def create_plot(snapshot):
    plt.cla()

    df = pd.read_csv(f"snapshots/snapshot{snapshot}.csv")
    
    #print(f"Creating stars")
    stars = [Star(row) for _, row in df.iterrows()]
    #print(f"Plotting stars")

    # Plot ideal circular orbit for each planet
    circle = plt.Circle((0, 0), radius=AU, edgecolor="black", facecolor="none")
    circle2 = plt.Circle((0, 0), radius=DISTANCE_MARS, edgecolor="black", facecolor="none")
    circle3 = plt.Circle((0, 0), radius=DISTANCE_JUPITER, edgecolor="black", facecolor="none")
    plt.gca().add_artist(circle)
    plt.gca().add_artist(circle2)
    plt.gca().add_artist(circle3)
 

    # Plot each star
    for i, star in enumerate(stars):
        plt.plot(star.position[0], star.position[1], c=COLORS[i], marker="o", markersize=5)
    
    plt.gca().set_aspect("equal")
    plt.xlim(-1e12, 1e12)
    plt.ylim(-1e12, 1e12)
    plt.title(f"Snapshot: {snapshot}")

fg, ax = plt.subplots()
ani = FuncAnimation(fg, create_plot, frames=NUM_SNAPSHOTS)

# Save the animation
print("Saving animation")
ani.save("./plots/animation_semj.mp4", writer="ffmpeg", fps=30)
print("Saved animation")

