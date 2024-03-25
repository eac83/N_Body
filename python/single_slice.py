"""
Plot stars in 2d.
"""
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib.animation import FuncAnimation
import matplotlib as mpl
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

h = ax[0].hist2d([1,2,3],[1,2,3])
h1 = ax[1].hist2d([1,2,3],[1,2,3])
h2 = ax[2].hist2d([1,2,3],[1,2,3])
cmap = plt.get_cmap('inferno')

def create_plot(snapshot):
    global vmin, vmax
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
    bins = 10
    h = ax[0].hist2d(x, y, bins=bins  , cmap='inferno', range=[(-BOX_SIZE/2,BOX_SIZE/2),(-BOX_SIZE/2,BOX_SIZE/2)])
    h1 = ax[1].hist2d(y, z, bins=bins  , cmap='inferno', range=[(-BOX_SIZE/2,BOX_SIZE/2),(-BOX_SIZE/2,BOX_SIZE/2)])
    h2 = ax[2].hist2d(x, z, bins=bins  , cmap='inferno', range=[(-BOX_SIZE/2,BOX_SIZE/2),(-BOX_SIZE/2,BOX_SIZE/2)])
    ax[0].imshow(h[0])
    ax[1].imshow(h1[0])
    ax[2].imshow(h2[0])
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
    fg.suptitle(f"Snapshot: {snapshot}")
    # Adding colorbar
vmin = min(np.min(h[0]), np.min(h1[0]), np.min(h2[0]))
print(np.where((h[0]>0)&(h[0]<1)))
vmax = max(np.max(h[0]), np.max(h1[0]), np.max(h2[0]))
sm = plt.cm.ScalarMappable(cmap=cmap)
norm = mpl.colors.Normalize(vmin=vmin, vmax=vmax)
sm.set_array([])
cbar = plt.colorbar(mpl.cm.ScalarMappable(norm=norm, cmap=cmap), ax=ax, orientation="horizontal", location="bottom")
cbar.set_label("Surface Density")
# Define a progress callback function with a progress bar

create_plot(150)
plt.show()
fg.savefig("./plots/single_slice.png")
