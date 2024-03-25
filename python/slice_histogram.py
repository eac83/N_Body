"""
Plot stars in 2d.
"""
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib.animation import FuncAnimation
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
    
SKIP = 1
BOX_SIZE = float(parameters["BoxSize"])
NUM_SNAPSHOTS = get_number_snapshots(SNAPSHOTS_DIR)
num = int(NUM_SNAPSHOTS/SKIP)
fg, ax = plt.subplots(1, 3)
fg.tight_layout()
#ax = fg.add_subplot(1, 3)
h = [0,0,0]
h[0] = ax[0].hist2d([1,2,3],[1,2,3])
h[1] = ax[1].hist2d([1,2,3],[1,2,3])
h[2] = ax[2].hist2d([1,2,3],[1,2,3])
cmap = plt.get_cmap('inferno')
vmin = 0
vmax = 0
RANGE = (-BOX_SIZE/2, BOX_SIZE/2)
def create_plot(snapshot):
    global vmin
    global vmax
    plt.cla()
    ax[0].clear()
    ax[1].clear()
    ax[2].clear()

    df = pd.read_csv(f"snapshots/snapshot{snapshot*SKIP}.csv")
    stars = [Star(row) for _, row in df.iterrows()]
    x=[]
    y=[]
    z=[]
    for i, star in enumerate(stars):
        #c = cmap((star.position[2] + BOX_SIZE/2) / BOX_SIZE)
        if abs(star.position[2]) < BOX_SIZE:
            x.append(star.position[0])
            y.append(star.position[1])
            z.append(star.position[2])
    bins = 75

    # Determine vmin and vmax
    h0, _, _ = np.histogram2d(x, y, bins=bins, range=[RANGE, RANGE])
    h1, _, _ = np.histogram2d(y, z, bins=bins, range=[RANGE, RANGE])
    h2, _, _ = np.histogram2d(x, z, bins=bins, range=[RANGE, RANGE])  
    vmin = min(np.min(h0), np.min(h1), np.min(h2))
    vmax = max(np.max(h0), np.max(h1), np.max(h2))
    h = ax[0].hist2d(x, y, bins=bins  , cmap='inferno', range=[(-BOX_SIZE/2,BOX_SIZE/2),(-BOX_SIZE/2,BOX_SIZE/2)], vmin=vmin, vmax=vmax)
    ax[1].hist2d(y, z, bins=bins  , cmap='inferno', range=[(-BOX_SIZE/2,BOX_SIZE/2),(-BOX_SIZE/2,BOX_SIZE/2)], vmin=vmin, vmax=vmax)
    ax[2].hist2d(x, z, bins=bins  , cmap='inferno', range=[(-BOX_SIZE/2,BOX_SIZE/2),(-BOX_SIZE/2,BOX_SIZE/2)], vmin=vmin, vmax=vmax)
    ax[0].set_aspect("equal")
    ax[1].set_aspect("equal")
    ax[2].set_aspect("equal")
    ax[0].set_xlim(-BOX_SIZE/2, BOX_SIZE/2)
    ax[0].set_ylim(-BOX_SIZE/2, BOX_SIZE/2)
    ax[1].set_xlim(-BOX_SIZE/2, BOX_SIZE/2)
    ax[1].set_ylim(-BOX_SIZE/2, BOX_SIZE/2)
    ax[2].set_xlim(-BOX_SIZE/2, BOX_SIZE/2)
    ax[2].set_ylim(-BOX_SIZE/2, BOX_SIZE/2)
    ax[0].set_xlabel("x / cMpc")
    ax[0].set_ylabel("y / cMpc")
    ax[1].set_xlabel("y / cMpc")
    ax[1].set_ylabel("z / cMpc")
    ax[2].set_xlabel("x / cMpc")
    ax[2].set_ylabel("z / cMpc")
    fg.suptitle(f"Snapshot: {snapshot}", y=0.8)

    # Determine vmin and vmax

    # Adding colorbar

ani = FuncAnimation(fg, create_plot, frames=num)
norm = mpl.colors.Normalize(vmin=vmin, vmax=vmax)
sm = plt.cm.ScalarMappable(cmap=cmap, norm=norm)
cbar = plt.colorbar(sm, ax=ax, orientation="horizontal", location="bottom")
cbar.set_label("Surface Density")
# Define a progress callback function with a progress bar
def save_progress(current_frame, total_frames):
    progress_percentage = (current_frame / total_frames) * 100
    return progress_percentage

# Save the animation as an MP4 file with progress tracking
with tqdm(total=num) as pbar:
    def update_progress(current_frame, total_frames):
        pbar.update(1)
        return save_progress(current_frame, num)

    ani.save('./plots/animation_2d_slice.mp4', fps=30, extra_args=['-vcodec', 'libx264', "-crf", "18"], progress_callback=update_progress)


