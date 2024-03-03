"""
Useful functions
"""

from glob import glob
import os

def get_number_snapshots(path) -> int:
    """
    Takes in path of snapshot directory, returns number of snapshots available.
    """
    return len(list(glob(os.path.join(path, "snapshot*.csv"))))

