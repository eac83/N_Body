"""
Plot distance of Earth from origin.
"""

from os.path import exists

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from star import Star
from constants import AU
from utils import get_number_snapshots
from tqdm import tqdm

SNAPSHOTS_DIR = "snapshots"
NUM_SNAPSHOTS = get_number_snapshots(SNAPSHOTS_DIR)

distances = []
snapshots = []

for snapshot in tqdm(range(0, 100)):
    df = pd.read_csv(f"snapshots/snapshot{snapshot}.csv")
    
    stars = [Star(row) for _, row in df.iterrows()]
    distance = np.sqrt(np.dot(stars[1].position, stars[1].position))
    distances.append(distance)
    snapshots.append(snapshot)

fg, ax = plt.subplots(2, 1)

ax[0].plot(snapshots, distances)  # absolute distance
ax[1].plot(snapshots, np.array(distances)/AU-1) # percent from 1 au
fg.savefig("plots/distance_moved.png")