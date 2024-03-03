import numpy as np
import pandas


class Star:
    def __init__(self, row):
        """
        Takes in row from pandas dataframe, constructs star.
        """
        self.mass = row.loc["m"]
        self.position = np.array([row.loc["x"], row.loc["y"], row.loc["z"]])
        self.velocity = np.array([row.loc["vx"], row.loc["vy"], row.loc["vz"]])
        self.acceleration = np.array([row.loc["ax"], row.loc["ay"], row.loc["az"]])

    def __str__(self):
        return f"Mass: {self.mass} \n\
Position: {self.position} \n\
Velocity: {self.velocity} \n\
Acceleration: {self.acceleration}"