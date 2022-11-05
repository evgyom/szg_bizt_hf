from pathlib import Path
import ctypes

project_root = Path.cwd()
caff_file_path = project_root.joinpath("caff_examples").joinpath("1_credits_only_nonascii.caff")
lib_path = project_root.joinpath("build").joinpath("caff_parser").joinpath("libcaff_parser_shared.dll")

# Load library
caff_parser = ctypes.CDLL(str(lib_path))

# Define input type
caff_parser.parse.argtypes = [ctypes.c_char_p]

# Call a function
caff_parser.parse(str(caff_file_path).encode("utf-8"))