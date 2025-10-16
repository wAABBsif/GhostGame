import os
from pathlib import Path

def process_shaders(res_path):
    path = res_path + "/shaders"
    os.chdir(path)

    for filename in os.listdir(path):
        shader = ""
        with open(filename, "r") as file:
            shader = file.read()
            start_index = 0
            for i in range(len(shader)):
                if ord(shader[i]) < 0x80:
                    start_index = i
                    break
            shader = _process_includes(shader[start_index : ])
        with open(filename, "w") as file:
            file.write(shader)
            print("Processed " + filename)

    for filename in os.listdir(path):
        if not filename.endswith(".frag.glsl") and not filename.endswith(".vert.glsl"):
            os.remove(filename)
            print("Removed " + filename)

def _process_includes(shader):
    include_str = "#include"
    for char in range(len(shader)):
        if shader[char] != "#":
            continue
        start_str = shader[char : char + len(include_str)]
        if start_str != include_str:
            continue
        start_part = shader[0 : char]
        end_index = shader[char : ].find("\n")
        end_part = shader[char + end_index :]

        include_part = shader[char : char + end_index]
        filename = include_part[include_part.find("\"") + 1 : include_part.rfind("\"")]

        n_shader = open(filename, "r").read()
        return _process_includes(start_part + _process_includes(n_shader) + end_part)
    return shader