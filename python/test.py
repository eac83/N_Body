import matplotlib.pyplot as plt
import numpy as np

# Generating sample data (particle coordinates)
np.random.seed(0)
x = np.random.normal(loc=5, scale=2, size=1000)
y = np.random.normal(loc=3, scale=1, size=1000)

# Creating 2D histogram
plt.figure(figsize=(8, 6))
hb = plt.hist2d(x, y, bins=30, cmap='viridis')

# Adding colorbar
plt.colorbar(hb[3], label='Surface Density')

# Adding labels and title
plt.xlabel('X')
plt.ylabel('Y')
plt.title('2D Histogram of Particle Coordinates')

# Display the plot
plt.show()
