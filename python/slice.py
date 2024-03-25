import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import pandas as pd
from star import Star
from utils import get_number_snapshots
from tqdm import tqdm

SNAPSHOTS_DIR = "snapshots"
NUM_SNAPSHOTS = get_number_snapshots(SNAPSHOTS_DIR)
parameters = {}
with open("parameters.txt", "r") as f:
    for line in f:
        line=line.strip()
        if line and not line.startswith("#"):
            key, value = line.split("=")
            parameters[key.strip()] = value.strip()
BOX_SIZE = float(parameters["BoxSize"])
# Function to generate random data points
def generate_data(snapshot):
    with open(f"./snapshots/snapshot{snapshot}.csv") as f:
        df = pd.read_csv(f)
    x = df["x"].values
    y = df["y"].values
    return x, y

# Create a figure and axis
fig, ax = plt.subplots()
x_range = (-BOX_SIZE/2, BOX_SIZE/2)
y_range = (-BOX_SIZE/2, BOX_SIZE/2)
# Initialize empty histogram
hist = ax.hist2d([], [], bins=3000, range=[(-BOX_SIZE/2, BOX_SIZE/2), (-BOX_SIZE/2, BOX_SIZE/2)], cmap="inferno")

def update(snapshot):
    # Generate random data for this frame
    x_data, y_data = generate_data(snapshot)
    
    # Compute the new 2D histogram with the new data
    new_hist, x_edges, y_edges = np.histogram2d(x_data, y_data, bins=3000, range=[x_range, y_range])
    
    # Clear the axis
    ax.clear()
    
    # Create a new histogram with the new data
    hist = ax.hist2d(x_data, y_data, bins=3000, range=[x_range, y_range], cmap='inferno')
    
    return hist[3],


# Animation object
with tqdm(total=NUM_SNAPSHOTS) as pbar:
    ani = FuncAnimation(fig, update, frames=NUM_SNAPSHOTS, blit=True)
    # Define a callback function to update the progress bar
    def callback(frame, total_frames):
        pbar.update(1)


# Show the color bar
plt.colorbar(hist[3], ax=ax)

# Set axis labels and title
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_title('Animated 2D Histogram')

# Show the animation
ani.save("./plots/slice.mp4", writer="ffmpeg")
plt.show()
