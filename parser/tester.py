from pathlib import Path
import os
import ctypes
import time

project_root = Path.cwd()
caff_file_path = project_root.joinpath("caff_examples").joinpath("3.caff")
gif_out_path = project_root.joinpath("caff3.gif")
lib_path = project_root.joinpath("build").joinpath("caff_parser").joinpath("libcaff_parser_shared.dll")

# Load library
caff_parser = ctypes.CDLL(str(lib_path))

# Define the input types
caff_parser.parse.argtypes = [ctypes.c_char_p, ctypes.c_long, ctypes.c_char_p]

# Call the function
start_time = time.time()
caff_parser.parse(str(caff_file_path).encode("utf-8"), os.path.getsize(caff_file_path), str(gif_out_path).encode("utf-8"))
end_time = time.time()
print("GIF generated in:{} s".format(end_time-start_time))