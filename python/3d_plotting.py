"""
Plot stars in 3d.
"""

from math import sin, cos, pi

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter
from mpl_toolkits import mplot3d
from tqdm import tqdm

from star import Star
from utils import get_number_snapshots

SNAPSHOTS_DIR = "snapshots"

parameters = {}
with open("parameters.txt", "r") as f:
    for line in f:
        line=line.strip()
        if line and not line.startswith("#"):
            key, value = line.split("=")
            parameters[key.strip()] = value.strip()
    
BOX_SIZE = float(parameters["BoxSize"])
NUM_SNAPSHOTS = get_number_snapshots(SNAPSHOTS_DIR)

fg = plt.figure()
ax = fg.add_subplot(projection="3d")

def create_plot(snapshot):
    plt.cla()
    ax.clear()

    df = pd.read_csv(f"snapshots/snapshot{snapshot}.csv")
    
    #print(f"Creating stars")
    stars = [Star(row) for _, row in df.iterrows()]

    #print(f"Plotting stars")

    # Plot ideal circular orbit for each planet
    #lt.axes(projection="3d")
    # Plot each star
    for i, star in enumerate(stars):
        plt.plot(star.position[0], star.position[1], star.position[2], marker=".", c="orange", markersize=5)
    
  #  plt.gca().set_aspect("equal")
    plt.xlim(-BOX_SIZE/2, BOX_SIZE/2)
    plt.ylim(-BOX_SIZE/2, BOX_SIZE/2)
    plt.gca().set_zlim(-BOX_SIZE/2, BOX_SIZE/2)
    plt.title(f"Snapshot: {snapshot}")

fg = plt.figure()
ax = fg.add_subplot(projection="3d")
ani = FuncAnimation(fg, create_plot, frames=NUM_SNAPSHOTS)


# Define a progress callback function with a progress bar
def save_progress(current_frame, total_frames):
    progress_percentage = (current_frame / total_frames) * 100
    return progress_percentage

# Save the animation as an MP4 file with progress tracking
with tqdm(total=NUM_SNAPSHOTS) as pbar:
    def update_progress(current_frame, total_frames):
        pbar.update(1)
        return save_progress(current_frame, NUM_SNAPSHOTS)

    ani.save('plots/animation_3d.mp4', fps=30, extra_args=['-vcodec', 'libx264', "-crf", "18"], progress_callback=update_progress)
# Save the animation
#print("Saving animation")
#ani.save("animation.mp4", writer="ffmpeg", fps=30)
#print("Saved animation")

