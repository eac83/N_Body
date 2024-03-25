"""
Plot stars in 2d.
"""
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from tqdm import tqdm

from star import Star
from utils import get_number_snapshots

SNAPSHOTS_DIR = "snapshots"
parameters = {}
with open("parameters_hyperbolic.txt", "r") as f:
    for line in f:
        line=line.strip()
        if line and not line.startswith("#"):
            key, value = line.split("=")
            parameters[key.strip()] = value.strip()
    
SKIP = 1
BOX_SIZE = float(parameters["BoxSize"])
NUM_SNAPSHOTS = get_number_snapshots(SNAPSHOTS_DIR)

num = int(NUM_SNAPSHOTS/SKIP)
fg = plt.figure()
ax = fg.add_subplot()
cmap = plt.get_cmap('viridis')

def create_plot(snapshot):
    plt.cla()
    ax.clear()

    df = pd.read_csv(f"snapshots/snapshot{snapshot*SKIP}.csv")
    stars = [Star(row) for _, row in df.iterrows()]

    for i, star in enumerate(stars):
        c = cmap((star.position[2] + BOX_SIZE/2) / BOX_SIZE)
        if abs(star.position[2]) < BOX_SIZE:
            ax.plot(star.position[0], star.position[1], c=c, marker=".", markersize=5)

    ax.set_aspect("equal")
    ax.set_xlim(-BOX_SIZE/2, BOX_SIZE/2)
    ax.set_ylim(-BOX_SIZE/2, BOX_SIZE/2)
    ax.set_title(f"Snapshot: {snapshot}")

    # Adding colorbar

ani = FuncAnimation(fg, create_plot, frames=num)
norm = plt.Normalize(-BOX_SIZE/2, BOX_SIZE/2)
sm = plt.cm.ScalarMappable(cmap=cmap, norm=norm)
sm.set_array([])
cbar = plt.colorbar(sm, ax=ax)
cbar.set_label("z")
# Define a progress callback function with a progress bar
def save_progress(current_frame, total_frames):
    progress_percentage = (current_frame / total_frames) * 100
    return progress_percentage

# Save the animation as an MP4 file with progress tracking
with tqdm(total=num) as pbar:
    def update_progress(current_frame, total_frames):
        pbar.update(1)
        return save_progress(current_frame, num)

    ani.save('./plots/animation_2d.mp4', fps=30, extra_args=['-vcodec', 'libx264', "-crf", "18"], progress_callback=update_progress)


