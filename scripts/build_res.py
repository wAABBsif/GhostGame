import os.path
import shutil
from pathlib import Path
import shaders

print()

original_path = str(Path(__file__).parent.parent) + "\\res_master"
path = str(Path(__file__).parent.parent) + "\\res"
if os.path.isdir(path):
    shutil.rmtree(path)
shutil.copytree(original_path, path)

shaders.process_shaders()