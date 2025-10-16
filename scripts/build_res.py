import os.path
import shutil
import sys
from pathlib import Path
import shaders

print()
if len(sys.argv) < 2:
    print("Not enough arguments!")
    exit()

path = sys.argv[1]
shaders.process_shaders(path)